import serial
import micropyGPS
import numpy
import time
import datetime
import subprocess
import threading
import pigpio
from enum import Flag,auto


class Coordinate:
	timestamp = [0,0,0]
	latitude = 0
	longitude = 0
	def __init__(self,lat,lng,timestamp):
		self.latitude = lat
		self.longitude = lng
		self.timestamp = [0,0,0]

	def __eq__(self,other):
		if (self.latitude == other.latitude) and (self.longitude == other.longitude):
			return True
		else:
			return False

	def __ne__(self,other):
		if self == other:
			return False
		else:
			return True

	def toString(self):
		h = self.timestamp[0] if self.timestamp[0] < 24 else self.timestamp[0] - 24
		time_str = ('%02d:%02d:%02d' %(h, self.timestamp[1], self.timestamp[2]))
		return time_str + ':' + str(self.latitude) +','+ str(self.longitude) 

	def getDistanceFrom(self,another):
		lat0 = another.latitude
		lng0 = another.longitude
		lat1 = self.latitude
		lng1 = self.longitude
		#差異が無ければ0
		"""if numpy.isclose(lat0,lat1) and numpy.isclose(lng0,lng1):
			return {
				'distance':0,
				'courseS2G':0,
				'courseG2S':0
			}"""
	
		#WGS84
		a = 6378137.0#赤道半径
		f = 1/298.257223563#扁平率
		#短軸半径
		b = (1-f)*a
		#ラジアンへ変換
		rlat0 = numpy.deg2rad(lat0)
		rlng0 = numpy.deg2rad(lng0)
		rlat1 = numpy.deg2rad(lat1)
		rlng1 = numpy.deg2rad(lng1)
		#更成緯度
		U0 = numpy.arctan((1-f)*numpy.tan(rlat0))
		U1 = numpy.arctan((1-f)*numpy.tan(rlat1))
		#2点間の経度差  
		L = rlng1-rlng0
		#補助球上の経度(初期化)
		Lambda=L

		sinU0 = numpy.sin(U0)
		sinU1 = numpy.sin(U1)
		cosU0 = numpy.cos(U0)
		cosU1 = numpy.cos(U1)
	
		for _ in range(1000):
			sinLmd=numpy.sin(Lambda)
			cosLmd=numpy.cos(Lambda)
		
			sins=((cosU1*sinLmd)**2+(cosU0*sinU1-sinU0*cosU1*cosLmd)**2)**(1/2)
			coss=sinU0*sinU1+cosU0*cosU1*cosLmd

			sigma=numpy.arctan2(sins,coss)

			sina=cosU0*cosU1*sinLmd/sins
			cos2a=1-sina**2

			cos2dm=coss-(2*sinU0*sinU1/cos2a)

			C=f*(cos2a)*(4+f*(4-3*cos2a))/16
			Lambda_prev=Lambda
			Lambda=L+(1-C)*f*sina*(sigma+C*sins*(cos2dm+C*coss*(-1+2*coss**2)))
			#精度の指定
			if abs(Lambda-Lambda_prev)<=1e-12:
				break
	
		u2=cos2a*(a**2-b**2)/b**2
		A=1+u2*(4096+u2*(-768+u2*(320-175*u2)))/16384
		B=u2*(256+u2*(-128+u2*(74-47*u2)))/1024
	
		deltas=B*sins*(cos2dm+B*(coss*(-1+2*cos2dm**2)-B*cos2dm*(-3+4*sins**2)*(-3+4*cos2dm**2)/6)/4)

		distance=b*A*(sigma-deltas)
		course0to1=numpy.rad2deg(numpy.arctan2(cosU1*sinLmd,cosU0*sinU1-sinU0*cosU1*cosLmd))
		course1to0=numpy.rad2deg(numpy.arctan2(cosU0*sinLmd,-sinU0*cosU1+cosU0*sinU1*cosLmd) + numpy.pi)

		if course1to0 > 180:
			course1to0 -= 360
		elif course1to0 < -180:
			course1to0 += 360

		return {
			'distance':distance,
			'courseS2G':course0to1,
			'courseG2S':course1to0
		}

class dist(Flag):
	COUT = auto()
	FOUT = auto()
	SOUT = auto()
	CFOUT = COUT | FOUT
	CSOUT = COUT | SOUT
	FSOUT = FOUT | SOUT
	CFSOUT = COUT | FOUT | SOUT

class Stream:

	DEBUG = False
	RUNGPS = False
	
	string_buffer_tx = []
	string_buffer_rx = []
	string_buffer_crx = [] 

	DOUT = dist.CFOUT

	procrunning = False
	coordinate_now = Coordinate(0,0,0)
	coordinate_prev = Coordinate(0,0,0)

	UPDATE = False
	

	def __init__(self,pinBusy,ser='/dev/ttyAMA0',baudrate=19200,timeout=1,timezone=9,fmt='dd',log='./log.txt',gpslog='./gpslog.txt'):
		self.log = log
		self.gpslog = gpslog
		with open(log,mode='w'):
			pass
		with open(gpslog,mode='w'):
			pass
		self.pinbusy = pinBusy
		self.gpio = pigpio.pi()
		self.gpio.set_mode(pinBusy,pigpio.INPUT)
		self.gpio.set_pull_up_down(pinBusy,pigpio.PUD_DOWN)

		self.s = serial.Serial(ser,baudrate=baudrate,timeout=timeout)
		self.command('ECIO')
		self.gps = micropyGPS.MicropyGPS(timezone,fmt)
		self.gpslock = threading.Lock()

	def __lshift__(self,something):
		if isinstance(something,dist):
			self.out(something)
		else:
			self.buffer(something)
		return self

	def __rshift__(self,something):
		if isinstance(something,Coordinate):
			something = self.readGpsData()
		else:
			something = self.readSerialLine()
		return self

	def buffer(self,something):
		self.string_buffer_tx.append(str(something))
		return self

	def clear_tx(self):
		self.string_buffer_tx = []
		return self

	def clear_rx(self):
		self.string_buffer_rx = []
		return self

	def out(self,distination=None):
		for string in self.string_buffer_tx:
			if (distination & dist.COUT):
				self.stdout(string)
			if (distination & dist.FOUT):
				self.dump(string)
			if (distination & dist.SOUT):
				self.transmit(string)
		self.clear_tx()
		return self

	def dump(self,string):
		with open(self.log,'a') as f:
			f.write(str(string)+'\n')
		return self

	def command(self,string):
		while(self.gpio.read(self.pinbusy)):
			time.sleep(0.200)
		self.s.write((str(string)+'\r\n').encode('ascii'))
		return self

	def transmit(self,string):
		while(self.gpio.read(self.pinbusy)):
			time.sleep(0.200)
		self.s.write(('TXDA'+str(string)+'\r\n').encode('ascii'))
		time.sleep(0.1)
		return self

	def stdout(self,string):
		print(string)
		return self

	def startGps(self):
		self.s.readline()
		self.clear_rx()
		self.RUNGPS = True
		self.gpsprocess = threading.Thread(target=self.GpsWorker,args=())
		self.gpsprocess.setDaemon(True)
		self.gpsprocess.start()

	def stopGps(self):
		self.RUNGPS = False
		self.gpsprocess._stop()

	def GpsWorker(self):
		while self.RUNGPS:
			self.readGpsData()

	def readSerialLine(self,append=False):
		try:
			string = self.s.readline()
		except UnicodeDecodeError:
			return self.readSerialLine(append)

		if append:
			self.string_buffer_rx.append(string)
		return string

	"""def readSerialLines(self):
		self.clear_rx()
		self.string_buffer_rx = self.s.readlines()
		return self"""

	def stopSerial(self):
		self.s.close()
		return self

	def readGpsData(self):
		while self.s.in_waiting:
			with self.gpslock:
				string = self.readSerialLine()
				for char in string.decode('utf-8'):
					self.gps.update(char)
		return self

	def updateGps(self):
		new = Coordinate(self.gps.latitude[0],self.gps.longitude[0],self.gps.timestamp)
		if new != self.coordinate_now:
			self.coordinate_prev = self.coordinate_now
			self.coordinate_now = Coordinate(self.gps.latitude[0],self.gps.longitude[0],self.gps.timestamp)
			with open(self.gpslog,'a') as f:
				f.write(self.coordinate_now.toString()+'\n')
			self.UPDATE = True
		else:
			self.UPDATE = False
		time.sleep(0.5)
		return self

	def getGpsData(self):
		return [self.coordinate_prev,self.coordinate_now]
		
	def startImu(self):
		self.runSubprocess('./mpu9250_libraries/bin/imustreaming.exec')

	def readImu(self):
		if self.procrunning == False:
			self << "Process is not Running!" << self.DOUT
			return
		self.process.stdin.write('u'.encode())
		self.process.stdin.write('\n'.encode())
		self.process.stdin.flush()
		res =  self.process.stdout.readline().decode().splitlines()[0].split(',')
		resf = []
		for i in res:
			 resf.append(float(i))
		return resf

	def runSubprocess(self,command='echo Can You Hear Me?'):
		self.process = subprocess.Popen(command,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,shell=True)
		self.procrunning = True

	def readPipe(self):
		if (self.procrunning == False):
			return None
		if (self.process.stdout.readable()):
			return self.process.stdout.readline().decode()

	def terminateSubProcess(self):
		if (self.process.poll()):
			self.process.terminate()

		


		
	

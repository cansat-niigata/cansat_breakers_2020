from Stream import Stream,Coordinate,dist
from Gpio import Gpio,MotorControl,act,PID,HeatWire
import Camera
import time




class Runback:
	usecompass = False
	usepid = False
	usecamera = False
	debug = False
	stdc = 150
	turn_margin = 10

	coordinate_goal = Coordinate(0,0,[0,0,0])
	coordinates = [Coordinate(0,0,[0,0,0]),Coordinate(0,0,[0,0,0])]
	distance2goal = 0
	course2goal = 0

	north_angle = 0

	starttime = 0



	def __init__(self,goal_lat=37.8658,goal_lng=138.9383,usecompass=False,usepid=True,usecamera=False):
		self.starttime = time.time()

		self.initGpio()

		self.stream = Stream(17)
		self.stream << 'connection is established.' << 'Can you read me?' << dist.CFSOUT

		self.setGoal(goal_lat,goal_lng)
		self.stream << 'goal is ' + self.coordinate_goal.toString() << dist.CFSOUT

		self.stream.startImu()
		self.stream << 'imu is started.' << dist.CFSOUT
		if usecompass == False:
			self.usecompass = False
			self.stream << 'calibration start.' << 'please dont touch...'
			for _ in range(5):
				prevdatas =  self.stream.readImu()
			while (True):
				datas = self.stream.readImu()
				self.stream << abs(datas[0] - prevdatas[0]) << dist.CSOUT
				if (abs(datas[0] - prevdatas[0]) < 0.3):
					break
			self.stream << 'OK!' << dist.CFSOUT
		else:
			self.usecompass = True
			self.stream << 'digital compass mode on.' << dist.CFSOUT

		self.stream.startGps()
		self.stream << 'GPS process is enabled.' << dist.CFSOUT


		self.motors = MotorControl(20,26,5,7)
		self.motors.stopMotor()
		if (usepid):
			self.usepid = True
			self.pid = PID().setParam(0.4,0.1,0.2).setLimit(100,-100)
			self.stream << 'pid control mode on.' << dist.CFSOUT

		self.stream << 'Motors are initialzed.' << dist.CFSOUT
		self.wire = HeatWire(25)
		self.stream << 'HeatWire is ready.' << dist.CFSOUT

		if usecamera:
			self.usecamera = True
			self.cam = Camera.Camera(768,512)
			self.stream << 'Camera is initialized.' << dist.CFSOUT
		else:
			self.stream << 'Camera is disabled.' << dist.CFSOUT
		
		self.stream << r" ____                 _                                      "                
		self.stream << r"| __ ) _ __ ___  __ _| | _____ _ __ ___     _____     ____   "
		self.stream << r"|  _ \| '__/ _ \/ _` | |/ / _ \ '__/ __|   /\/\/\/\  |  o |  "
		self.stream << r"| |_) | | |  __/ (_| |   <  __/ |  \__ \  |\/\/\/\/|/ ___\|  "
		self.stream << r"|____/|_|  \___|\__,_|_|\_\___|_|  |___/  |/\/\/\/\/_/       "
		self.stream << r"                                          |_|_| |_|_|        "
		self.stream << r" ____            _                                  _   ___  "
		self.stream << r"/ ___| _   _ ___| |_ ___ _ __ ___    _ __ _____   _/ | ( _ ) "
		self.stream << r"\___ \| | | / __| __/ _ \ '_ ` _ \  | '__/ _ \ \ / / | / _ \ "
		self.stream << r" ___) | |_| \__ \ ||  __/ | | | | | | | |  __/\ V /| || (_) /"
		self.stream << r"|____/ \__, |___/\__\___|_| |_| |_| |_|  \___| \_(_)_(_)___/ "
		self.stream << r"       |___/                                                 " << dist.CFSOUT
		
		self.stream << "Welcome to Breakers' Runback Misson Program rev.1.87" << dist.CFSOUT

	def moveForward(self,timeout=5):
		self.stream << str(self.getTime()) + ":moveForward(timeout=" + str(timeout) + ")" << dist.CFSOUT

		start = time.time()
		dt = 0
		tag = self.stream.readImu()[0]
		self.motors.setSpeed(self.stdc,self.stdc).spinEither()
		if timeout != 0:
			while dt < timeout:
				dt = time.time() - start
				datas = self.stream.readImu()
				err = self.fixAngle(datas[0],tag)#err = datas[0] - tag
				if (self.usepid):
					c = self.pid.execute(err,datas[5],dt)
					if self.debug:
						self.stream << 'dt:'+str(dt) << 'err:'+str(err) << 'pid:'+str(c) << 'vect:'+str(datas[0]) << 'gyr:'+str(datas[5]) << dist.COUT
					self.motors.setSpeed(self.stdc + c,self.stdc - c)
			self.motors.stopMotor()
		self.pid.reset()
		return self

	def moveto(self,deg=0,timeout=8):
		self.stream << str(self.getTime()) + ":moveto(deg=" + str(deg) + ",timeout=" + str(timeout) +")" << dist.CFSOUT
		start = time.time()
		dt = 0
		tag = deg
		self.motors.setSpeed(self.stdc,self.stdc).spinEither()
		if timeout != 0:
			while dt < timeout:
				dt = time.time() - start
				datas = self.stream.readImu()
				err = self.fixAngle(datas[0],tag)#err = datas[0] - tag
				if (self.usepid):
					c = self.pid.execute(err,datas[5],dt)
					if self.debug:
						self.stream << 'dt:'+str(dt) << 'err:'+str(err) << 'pid:'+str(c) << 'vect:'+str(datas[0]) << 'gyr:'+str(datas[5]) << dist.COUT
					self.motors.setSpeed(self.stdc + c,self.stdc - c)
		self.pid.reset()
		return self

	def movetoGoal(self,timeout=8):
		self.moveto(self.course2goal)

	def stop(self):
		self.motors.stopMotor()
		return self
	
	def faceto(self,deg=0,timeout=5):
		self.stream << str(self.getTime()) + ":faceto(deg=" + str(deg) + ",timeout=" + str(timeout) + ")" << dist.CFSOUT

		if self.usecompass:
			ref_angle = self.stream.readImu()[6]
		else:
			ref_angle = self.north_angle
		tag = self.fixAngle(deg,ref_angle)

		if self.debug:
			self.stream << 'tag:'+str(tag) << dist.COUT

		start = time.time()
		dt = 0
		tag = self.fixAngle(deg,self.stream.readImu()[0],mode='m')
		self.motors.setSpeed(self.stdc,self.stdc).spinEither()
		if timeout != 0:
			while dt < timeout:
				dt = time.time() - start
				datas = self.stream.readImu()
				err = self.fixAngle(datas[0],tag)#err = datas[0] - tag
				if self.usepid:
					c = self.pid.execute(err,datas[5],dt)
					if self.debug:
						self.stream << 'dt:'+str(dt) << 'err:'+str(err) << 'pid:'+str(c) << 'vect:'+str(datas[0]) << 'gyr:'+str(datas[5]) << dist.COUT
					self.motors.setSpeed(self.stdc + c,self.stdc - c)
				if abs(err) <= self.turn_margin:
					break
			self.motors.brakeMotor()
		self.pid.reset()
		self.motors.brakeMotor()
		return self

	def facetoGoal(self,timeout=5):
		return self.faceto(self.getDistance_CoursetoGoal()['courseS2G'],timeout)

	def turnto(self,deg=0,timeout=5):
		start = time.time()
		dt = 0
		tag = self.fixAngle(deg,self.stream.readImu()[0],mode='p')
		self.motors.setSpeed(self.stdc,self.stdc).spinEither()
		if timeout != 0:
			while dt < timeout:
				dt = time.time() - start
				datas = self.stream.readImu()
				err = self.fixAngle(datas[0],tag)#err = datas[0] - tag
				if self.usepid:
					c = self.pid.execute(err,datas[5],dt)
					if self.debug:
						self.stream << 'dt:'+str(dt) << 'err:'+str(err) << 'pid:'+str(c) << 'vect:'+str(datas[0]) << 'gyr:'+str(datas[5]) << dist.COUT
					self.motors.setSpeed(self.stdc + c,self.stdc - c)
				if abs(err) <= self.turn_margin:
					break
			self.motors.brakeMotor()
		self.pid.reset()
		return self

	def setGoal(self,lat,lng):
		self.coordinate_goal = Coordinate(lat,lng,[0,0,0])
		self.stream << "Goal is updated. newer:" + self.coordinate_goal.toString() << dist.CFSOUT  
		return self

	def updateGPS(self):
		self.stream << str(self.getTime()) + ":updateGPS()" << dist.CFSOUT
		self.coordinates = self.stream.updateGps().getGpsData()
		if self.stream.UPDATE:
			self.stream << "GPS is updated. newer:" + self.coordinates[1].toString() << dist.CFSOUT
			dicts = self.getDistance_CoursetoGoal()
			self.distance2goal = dicts['distance']
			self.course2goal = dicts['courseS2G']
		else:
			self.stream << "GPS is not updated!" << dist.CFSOUT
			self.stop().updateGPS()
		return self

	def getGPSCoordinate(self):
		return self.coordinates

	def getDistance_Course(self,coordinate_start,coordinate_end):
		self.stream << str(self.getTime()) + ":getDistance_Course" << dist.CFSOUT
		result = coordinate_end.getDistanceFrom(coordinate_start)
		self.stream << "distance :" + str(result['distance']) + " course :" + str(result['courseS2G']) << dist.CFSOUT 
		return result

	def getDistance_CoursetoGoal(self):
		self.stream << "Processing Distance & Course to Goal..." << dist.CFSOUT
		return self.getDistance_Course(self.coordinates[1],self.coordinate_goal)

	def getDistance_CoursefromPrev(self):
		self.stream << "Processing Distance & Course from Previous Ref..." << dist.CFSOUT
		return self.getDistance_Course(self.coordinates[0],self.coordinates[1])
		
	def detectGoal(self):
		if self.usecamera:
			return self.cam.detectTargetv2()
		else:
			return 0

	def waitDrop(self,timeout=20):
		while True:
			datas = self.stream.readImu()
			if (pow(datas[3],2)+pow(datas[4],2)+pow(datas[5],2))**0.5 < 0.3:
				break
		start = time.time()
		while True:
			if time.time() - start > timeout:
				break
		return self
			
	def drill(self,elv1=6,elv2=13,head1=8,head2=11):
		MotorControl(elv1,elv2,head1,head2,80,255).spinEither().wait(2.8).spinMotor(act.RIGHT_STOP).wait(3.0).spinMotor(act.RIGHT_INVERT).wait(3.0).stopMotor().terminate()
		return self

	def getTime(self):
		return time.time() - self.starttime

	def setNorth(self):
		self.north_angle = self.stream.readImu()[0]
		self.stream << "North Angle is Updated. newer:" + str(self.north_angle) << dist.CFSOUT
		return self

	def debugOn(self):
		self.debug = True
		return self

	@staticmethod
	def fixAngle(angle,ref_angle,mode='m'):
		if (mode =='p'):
			agl = angle + ref_angle
		else:
			agl = angle - ref_angle
		if agl > 180:
			return agl - 360
		elif agl <= -180:
			return agl + 360
		return agl

	def initGpio(self):
		gpio = Gpio()
		gpio.toggleOff(20)
		gpio.toggleOff(26)
		gpio.toggleOff(5)
		gpio.toggleOff(7)
		gpio.toggleOff(8)
		gpio.toggleOff(11)
		gpio.toggleOff(6)
		gpio.toggleOff(13)
		gpio.toggleOff(25)
		gpio.toggleOn(12)
		gpio.toggleOn(21)
		gpio.terminate()
		return self

	def test(self):
		while self.updateGPS().distance2goal > 5:
			self.movetoGoal()
		self.stop().stream << "goal!" << dist.CFSOUT
	


if __name__ == '__main__':
	Runback(usecompass=True,usepid=True,usecamera=False).test()
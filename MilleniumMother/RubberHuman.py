from Stream import Stream,coordinate,dist
from Gpio import MotorControl,act,PID,HeatWire
import Camera
import time

class Runback:
	usecompass = False
	usepid = False
	usecamera = False
	debug = False
	stdc = 150
	turn_margin = 0

	coordinate_goal = coordinate(0,0,[0,0,0])
	coordinates = [coordinate(0,0,[0,0,0]),coordinate(0,0,[0,0,0])]

	def __init__(self,MR1=20,MR2=26,ML1=5,ML2=7,RTS=17,usecompass=False,usepid=True,usecamera=False,goal_lat=37.8658,goal_lng=138.9383):
		self.stream = Stream(RTS)
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
	
		self.motors = MotorControl(MR1,MR2,ML1,ML2)
		self.motors.stopMotor()
		if (usepid):
			self.usepid = True
			self.pid = PID().setParam(0.1,0.5,0).setLimit(100,-100)
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
		
		self.stream << "Welcome to Breaker's Runback Misson Program rev.1.85" << dist.CFSOUT

	def moveForward(self,timeout=5):
		start = time.time()
		dt = 0
		tag = self.stream.readImu()[0]
		self.motors.setSpeed(self.stdc,self.stdc).spinEither()
		if timeout != 0:
			while dt < timeout:
				dt = time.time() - start
				datas = self.stream.readImu()
				err = datas[0] - tag
				if (self.usepid):
					c = self.pid.execute(err,datas[5],dt)
					if (self.debug):
						self.stream << 'pid:'+str(c) << 'vect:'+str(datas[0]) << 'gyr:'+str(datas[5]) << dist.CSOUT
					self.motors.setSpeed(self.stdc - c,self.stdc + c)
			self.motors.stopMotor()
		return self
	
	def faceto(self,deg=0,timeout=5,):
		if self.usecompass:
			ref_angle = self.stream.readImu()[6]
		else:
			ref_angle = self.stream.readImu()[0]
		tag = self.fixAngle(deg,ref_angle)

		start = time.time()
		if tag > 0:
			self.motors.setSpeed(0,255).spinMotor(act.TURN_RIGHT)
			while time.time() - start < timeout:
				if self.usecompass:
					heading = self.fixAngle(self.stream.readImu()[6],ref_angle)
				else:
					heading = self.fixAngle(self.stream.readImu()[0],ref_angle)
				if heading > tag:
					break
		elif tag < 0:
			self.motors.setSpeed(255,0).spinMotor(act.TURN_LEFT)
			while time.time() - start < timeout:
				if self.usecompass:
					heading = self.fixAngle(self.stream.readImu()[6],ref_angle)
				else:
					heading = self.fixAngle(self.stream.readImu()[0],ref_angle)
				if heading < tag:
					break
		return self

	def facetoGoal(self,timeout=5):
		return self.faceto(self.getDistance_CoursetoGoal(),timeout)

	def turnto(self,deg,timeout=5):
		if self.usecompass:
			ref_angle = self.stream.readImu()[6]
		else:
			ref_angle = self.stream.readImu()[0]
		tag = deg

		start = time.time()
		if tag > 0:
			self.motors.setSpeed(0,255).spinMotor(act.TURN_RIGHT)
			while time.time() - start < timeout:
				if self.usecompass:
					heading = self.fixAngle(self.stream.readImu()[6],ref_angle)
				else:
					heading = self.fixAngle(self.stream.readImu()[0],ref_angle)
				if heading > tag:
					break
		elif tag < 0:
			self.motors.setSpeed(255,0).spinMotor(act.TURN_LEFT)
			while time.time() - start < timeout:
				if self.usecompass:
					heading = self.fixAngle(self.stream.readImu()[6],ref_angle)
				else:
					heading = self.fixAngle(self.stream.readImu()[0],ref_angle)
				if heading < tag:
					break
		return self

	def setGoal(self,lat,lng):
		self.coordinate_goal = coordinate(lat,lng,[0,0,0])
		return self

	def updateGPS(self):
		self.coordinates = self.stream.updateGps().getGpsData()
		return self

	def getGPSReference(self):
		return self.coordinates

	def getDistance_Course(self,coordinate_start,coordinate_end):
		return coordinate_end.getDistanceFrom(coordinate_start)

	def getDistance_CoursetoGoal(self):
		return self.coordinates[1].getDistanceFrom(self.coordinate_goal)

	def getDistance_CoursefromPrev(self):
		return self.coordinates[1].getDistanceFrom(self.coordinates[0])
		
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
			
	def drill(self):
		MotorControl(6,13,11,8,80,255).spinEither().wait(2.8).spinMotor(act.RIGHT_STOP).wait(5.0).spinMotor(act.RIGHT_INVERT).wait(3.0).stopMotor().terminate()
		return self

	@staticmethod
	def fixAngle(angle,ref_angle):
		agl = angle - ref_angle
		if agl > 180:
			return agl - 360
		elif agl <= -180:
			return agl + 360
		return agl


if __name__ == '__main__':
	machine = Runback(usecompass=True,usepid=False,usecamera=False)
	while machine.updateGPS().getDistance_CoursetoGoal()['distance'] > 3:
		machine.facetoGoal().moveForward()
	machine.stream << "goal!" << dist.CFSOUT
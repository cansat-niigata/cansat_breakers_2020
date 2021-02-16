import pigpio
import time
import threading

class Gpio:
	def __init__(self):
		self.gpio = pigpio.pi()

	def setPinOut(self,gpionum):
		self.gpio.set_mode(gpionum,pigpio.OUTPUT)

	def setPinIn(self,gpionum):
		self.gpio.set_mode(gpionum,pigpio.INPUT)
		self.gpio.set_pull_up_down(gpionum,pigpio.PUD_DOWN)

	def setServo(self,gpionum,pulse_width=500):#500~2500
		self.gpio.set_servo_pulsewidth(gpionum,pulse_width)

	def setPWM(self,gpionum,frequency=1000,range_=255,dutycycle=128):
		self.gpio.set_PWM_frequency(gpionum,frequency)
		self.gpio.set_PWM_range(gpionum,range_)
		self.gpio.set_PWM_dutycycle(gpionum,dutycycle)

	def changeDutycycle(self,gpionum,dutycycle):
		self.gpio.set_PWM_dutycycle(gpionum,dutycycle)

	def setInterrupt(self,gpionum,target,edge='FALLING'):
		self.setPinIn(gpionum)
		if edge == 'FALLING':
			self.gpio.set_pull_up_down(gpionum,pigpio.PUD_UP)
			var = pigpio.FALLING_EDGE
		elif edge == 'RISING':
			self.gpio.set_pull_up_down(gpionum,pigpio.PUD_DOWN)
			var = pigpio.RISING_EDGE
		elif edge == 'EITHER_UP':
			self.gpio.set_pull_up_down(gpionum,pigpio.PUD_UP)
			var = pigpio.EITHER_EDGE
		elif edge == 'EITHER_DOWN':
			self.gpio.set_pull_up_down(gpionum,pigpio.PUD_DOWN)
			var = pigpio.EITHER_EDGE
		else:
			raise Exception('Invalid EDGE Value')
		self.cb = self.gpio.callback(gpionum,var,target)
		return self.cb

	def toggleOn(self,gpionum):
		self.gpio.write(gpionum,1)

	def toggleOff(self,gpionum):
		self.gpio.write(gpionum,0)

	def readPin(self,gpionum):
		return self.gpio.read(gpionum)

	def terminate(self):
		self.gpio.stop()

class act:
	LEFT_DRIVE = 1
	LEFT_STOP = 2
	LEFT_INVERT = 4
	RIGHT_DRIVE = 8
	RIGHT_STOP = 16
	RIGHT_INVERT = 32
	DRIVE = LEFT_DRIVE | RIGHT_DRIVE
	STOP = LEFT_STOP | RIGHT_STOP
	INVERT = LEFT_INVERT | RIGHT_INVERT
	TURN_LEFT = LEFT_STOP | RIGHT_DRIVE
	TURN_RIGHT = LEFT_DRIVE | RIGHT_STOP
	TURN_LEFT_SPIN = LEFT_INVERT | RIGHT_DRIVE
	TURN_RIGHT_SPIN = LEFT_DRIVE | RIGHT_INVERT

class MotorControl:#for TB67H450
	def __init__(self,PinR1,PinR2,PinL1,PinL2,R_speed=128,L_speed=128):
		self.gpio = Gpio()
		self.R1 = PinR1
		self.R2 = PinR2
		self.L1 = PinL1
		self.L2 = PinL2
		self.R_speed = R_speed
		self.L_speed = L_speed
		self.gpio.setPinOut(PinR1)
		self.gpio.setPinOut(PinR2)
		self.gpio.setPinOut(PinL1)
		self.gpio.setPinOut(PinL2)
		self.status = act.STOP
		
	def spinMotor(self,val):
		if val & act.RIGHT_DRIVE:
			self.gpio.setPWM(self.R1,dutycycle=self.R_speed)
			self.gpio.toggleOff(self.R2)
		elif val & act.RIGHT_INVERT:
			self.gpio.toggleOff(self.R1)
			self.gpio.setPWM(self.R2,dutycycle=self.R_speed)
		elif val & act.RIGHT_STOP:
			self.gpio.toggleOff(self.R1)
			self.gpio.toggleOff(self.R2)
		if val & act.LEFT_DRIVE:
			self.gpio.setPWM(self.L1,dutycycle=self.L_speed)
			self.gpio.toggleOff(self.L2)
		elif val & act.LEFT_INVERT:
			self.gpio.toggleOff(self.L1)
			self.gpio.setPWM(self.L2,dutycycle=self.L_speed)
		elif val & act.LEFT_STOP:
			self.gpio.toggleOff(self.L1)
			self.gpio.toggleOff(self.L2)
		self.status = val
		return self

	def spinEither(self):
		self.gpio.setPWM(self.R1,dutycycle=self.R_speed)
		self.gpio.setPWM(self.L1,dutycycle=self.L_speed)
		self.gpio.toggleOff(self.R2)
		self.gpio.toggleOff(self.L2)
		self.status = act.DRIVE
		return self
	
	def stopMotor(self):
		self.gpio.toggleOff(self.R1)
		self.gpio.toggleOff(self.R2)
		self.gpio.toggleOff(self.L1)
		self.gpio.toggleOff(self.L2)
		self.status = act.STOP
		return self

	def setSpeed(self,speed_R=128,speed_L=128):
		self.R_speed = speed_R
		self.L_speed = speed_L
		if (self.status != act.DRIVE)and(self.status != act.STOP):
			self.spinMotor(self.status)
		else:
			if self.status&act.DRIVE:
				self.spinEither()
			elif self.status&act.STOP:
				self.stopMotor()
		return self

	def wait(self,sec=2):
		time.sleep(sec)
		return self

	def escapeStack(self):
		self.spinMotor(act.LEFT_INVERT | act.RIGHT_STOP)
		time.sleep(2)
		self.stopMotor()
		self.spinMotor(act.LEFT_STOP | act.RIGHT_DRIVE)
		time.sleep(2)
		self.stopMotor()
		return self

	def terminate(self):
		self.gpio.terminate()

class PID:
	Kp = 1.0
	Ki = 0
	Kd = 0
	LimMax = 255
	LimMin = 0
	Err_sum = 0
	Err_prev = 0

	def __init__(self,Kp=1.0,Ki=0,Kd=0,LimMax=255,LimMin=0):
		self.setParam(Kp,Ki,Kd).setLimit(LimMax,LimMin)
	
	def setParam(self,Kp=1,Ki=0,Kd=0):
		self.Kp = Kp
		self.Ki = Ki
		self.Kd = Kd
		return self

	def setLimit(self,LimMax=255,LimMin=0):
		self.LimMax = LimMax
		self.LimMin = LimMin
		return self

	def reset(self):
		self.Err_sum = 0
		self.Err_prev = 0
		return self
	
	def execute(self,Err_now,dErrdt,dt):
		Err_sum_abt = self.Err_sum
		self.Err_sum += (self.Err_prev + Err_now)*0.5*dt
		self.Err_prev = Err_now
		res = self.Kp*(Err_now + self.Ki*self.Err_sum + self.Kd*dErrdt)

		if (res > self.LimMax):
			res = self.LimMax
			self.Err_sum = Err_sum_abt
		elif (res < self.LimMin):
			res = self.LimMin
			self.Err_sum = Err_sum_abt

		return res

	def execute_nd(self,Err_now,dt):
		dErrdt = (Err_now - self.Err_prev)/dt
		return self.execute(Err_now,dErrdt,dt)
		
	
			
class HeatWire:
	def __init__(self,pinFET,time=5):
		self.gpio = Gpio()
		self.pin = pinFET
		self.gpio.setPinOut(pinFET)
		self.time = time

	def purge(self):
		self.gpio.toggleOn(self.pin)
		time.sleep(self.time)
		self.gpio.toggleOff(self.pin)



		
		






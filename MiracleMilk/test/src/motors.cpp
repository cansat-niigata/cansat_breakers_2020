#include "motors.h"

using namespace drv;

Motor_1wire::Motor_1wire(unsigned int PinNum,unsigned int Frequency,unsigned int Range,unsigned int Dutycycle)
:pin(PinNum),PWM_Frequency(Frequency),PWM_Range(Range),PWM_Dutycycle(Dutycycle),status(false){
	Gpio::toggleOff(PinNum);
	Gpio::setPWM(PinNum,Dutycycle,Range,Frequency);
}

Motor_1wire::~Motor_1wire(void){
	Gpio::clearPin(this->pin);
}

void Motor_1wire::spin(void){
	Gpio::setPWM(this->pin,this->PWM_Dutycycle,this->PWM_Range,this->PWM_Frequency);
	this->status = true;
}

void Motor_1wire::spin(unsigned int Speed){
	Gpio::setPWM(this->pin,Speed,this->PWM_Range,this->PWM_Frequency);
	this->status = true;
}

void Motor_1wire::spinDuring(double Time){
	Gpio::setPWM(this->pin,this->PWM_Dutycycle,this->PWM_Range,this->PWM_Frequency);
	this->status = true;
	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

}

void Motor_1wire::stop(void){
	Gpio::toggleOff(this->pin);
	this->status = false;
}

void Motor_1wire::setSpeed(unsigned int Speed){
	this->PWM_Dutycycle = Speed;
	if(this->status == true){
		this->spin();
	}
}

/*class Motor{
		private:
			const unsigned int pinA;
			const unsigned int pinB;
			unsigned int PWM_Frequency;
			unsigned int PWM_Range;
			unsigned int PWM_Dutycycle;
		public:
			Motor(unsigned int PinNum1,unsigned int PinNum2,unsigned int Frequency=1000,unsigned int Range=255,unsigned int Dutycycle=128);
			~Motor(void);

			void spin(unsigned int Speed=128,bool Invert=false);
			void spinDuring(double Time,unsigned int Speed=128);
			void stop(void);
			void setSpeed(unsigned int Speed=128);
	};*/

Motor::Motor(void)
:pinA(NULL),pinB(NULL),PWM_Frequency(NULL),PWM_Range(NULL),PWM_Dutycycle(NULL),sts(STS_MOTOR_S){
};

Motor::Motor(const char* name,unsigned int pinNum1,unsigned int PinNum2,unsigned int Frequency,unsigned int Range,unsigned int Dutycycle)
:pinA(pinNum1),pinB(PinNum2),PWM_Frequency(Frequency),PWM_Range(Range),PWM_Dutycycle(Dutycycle),sts(STS_MOTOR_S){
    Gpio::setPinOut(pinA);
    Gpio::setPinOut(pinB);
}

Motor::~Motor(void){
}

void Motor::spin(bool invert){
    int* res;
    int res2;
    if (invert == true){
        res = Gpio::setPWM(pinB,PWM_Dutycycle,PWM_Range,PWM_Frequency);
        res2 = Gpio::toggleOff(pinA);
		sts = STS_MOTOR_B;
    }else{
        res = Gpio::setPWM(pinA,PWM_Dutycycle,PWM_Range,PWM_Frequency);
        res2 = Gpio::toggleOff(pinB);
		sts = STS_MOTOR_F;
    }
}

void Motor::stop(void){
	Gpio::toggleOff(pinA);
	Gpio::toggleOff(pinB);
	sts = STS_MOTOR_S;
}

void Motor::spinDuring(double Time,unsigned int Speed,bool invert=false){
	if (invert){
		sts = STS_MOTOR_B;
	}else{
		sts = STS_MOTOR_F;
	}
    spin(invert);
	Log().waitFor(Time);
	stop();
	sts = STS_MOTOR_S;
}

void Motor::setSpeed(unsigned int Speed){
	PWM_Dutycycle = Speed;
	if (sts == STS_MOTOR_F){
		spin();
	}else if (sts == STS_MOTOR_B){
		spin(true);
	}
}

uint8_t Motor::getStatus(void){
	return sts;
}

Motors::Motors(unsigned int pinR1,unsigned int pinR2,unsigned int pinL1,unsigned int pinL2,unsigned int R_SPEED=128,unsigned int L_SPEED=128){
	left = new Motor("MOTOR_L",pinL1,pinL2,1000,255,L_SPEED);
	right = new Motor("MOTOR_R",pinR1,pinR2,1000,255,R_SPEED);
}

void Motors::drive(uint8_t rsts,uint8_t lsts){
	if (rsts==STS_MOTOR_F){
		right->spin();
	}else if (rsts==STS_MOTOR_B){
		right->spin(true);
	}else{
		right->stop();
	}
	
	if (lsts==STS_MOTOR_F){
		left->spin();
	}else if (lsts==STS_MOTOR_B){
		left->spin(true);
	}else{
		left->stop();
	}
}

void Motors::drive(uint8_t rsts,uint8_t lsts,unsigned int R_speed,unsigned int L_speed){
	right->setSpeed(R_speed);
	left->setSpeed(L_speed);
	this->drive(rsts,lsts);
}

void Motors::setSpeedR(unsigned int speed){
	right->setSpeed(speed);
}

void Motors::setSpeedL(unsigned int speed){
	left->setSpeed(speed);
}

void Motors::setSpeed(unsigned int R_speed,unsigned int L_speed){
	right->setSpeed(R_speed);
	left->setSpeed(L_speed);
}

void Motors::stop(void){
	right->stop();
	left->stop();
}

uint8_t Motors::getStatusR(void){
	return right->getStatus();
}

uint8_t Motors::getStatusL(void){
	return left->getStatus();
}
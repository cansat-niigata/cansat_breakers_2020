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

Motor::Motor(unsigned int pinNum1,unsigned int PinNum2,unsigned int Frequency,unsigned int Range,unsigned int Dutycycle,const char* file="./log/motors.txt")
:pinA(pinNum1),pinB(PinNum2),PWM_Frequency(Frequency),PWM_Range(Range),PWM_Dutycycle(Dutycycle){
    notes = Notes(file);
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
    }else{
        res = Gpio::setPWM(pinA,PWM_Dutycycle,PWM_Range,PWM_Frequency);
        res2 = Gpio::toggleOff(pinB);
    }

    notes.append(Note(res[0]));
    notes.append(Note(res[1]));
    notes.append(Note(res[2]));
    notes.append(Note(res2));
}

void Motor::stop(void){
	notes.append(Note(Gpio::toggleOff(pinA)));
	notes.append(Note(Gpio::toggleOff(pinB)));
}

void Motor::spinDuring(double Time,unsigned int Speed,bool invert=false){
    spin(invert);
	Log().waitFor(Time);
	stop();
}

void Motor::setSpeed(unsigned int Speed){
	
}

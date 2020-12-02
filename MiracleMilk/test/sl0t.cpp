#include "sl0t.h"
#include <chrono>

//constructer
DRV::GPIO::GPIO(void)
:initValue(gpioInitialise()),pinStatus(nullptr){
}
//destructer
DRV::GPIO::~GPIO(void){
	gpioTerminate();
}

bool DRV::GPIO::checkInitValue(void){
	if(initValue < 0){
		return false;
	}
	else{
		return true;
	}
}

int DRV::GPIO::getInitValue(void){
	return initValue;
}

void DRV::GPIO::initializeAllGPIO::clearPin(unsigned int PinNum){
	this->toggleOff(PinNum);
	this->setPinIn(PinNum);
	this->setPinPullDown(PinNum);
}

int DRV::GPIO::getPinStatus(unsigned int PinNum){
	int i = gpioGetMode(PinNum);
	pinStatus[PinNum-1] = i;
	return i;
}

int* DRV::GPIO::getPinStatus(void){
	pinStatus = nullptr;
	pinStatus = new int[59];
	for(unsigned int i;i<59;i++){
		this->getPinStatus(i+1);
	}
	return pinStatus;
}

void DRV::GPIO::setPinIn(unsigned int PinNum){
	gpioSetMode(PinNum,PI_INPUT);
}

void DRV::GPIO::setPinOut(unsigned int PinNum){
	gpioSetMode(PinNum,PI_OUTPUT);
}

void DRV::GPIO::setPinPullUp(unsigned int PinNum){
	gpioSetPullUpDown(PinNum,PI_PUD_UP);
}

void DRV::GPIO::setPinPullDown(unsigned int PinNum){
	gpioSetPullUpDown(PinNum,PI_PUD_DOWN);
}

void DRV::GPIO::clearPinPullUpDown(unsigned int PinNum){
	gpioSetPullUpDown(PinNum,PI_PUD_OFF);
}

void DRV::GPIO::toggleOn(unsigned int PinNum){
	this->setPinOut(PinNum);
	gpioWrite(PinNum,PI_HIGH);
}

void DRV::GPIO::toggleOff(unsigned int PinNum){
	this->setPinOut(PinNum);
	gpioWrite(PinNum,PI_LOW);
}

bool DRV::GPIO::readPin(unsigned int PinNum){
	if (gpioRead(PinNum) == 1){
		return true;
	}else
	{
		return false;
	}	
}

void DRV::GPIO::setPWM(unsigned int PinNum,unsigned int DutyCycle,unsigned int Range,unsigned int Frequency){
	gpioSetPWMfrequency(PinNum,Frequency);
	gpioSetPWMrange(PinNum,Range);
	gpioPWM(PinNum,DutyCycle);
}

void DRV::GPIO::setInterrupt(unsigned int PinNum,gpioAlertFunc_t Function){
	gpioSetAlertFunc(PinNum,Function);
}



DRV::Motor_1wire::Motor_1wire(unsigned int PinNum,unsigned int Frequency,unsigned int Range,unsigned int Dutycycle)
:pin(PinNum),PWM_Frequency(Frequency),PWM_Range(Range),PWM_Dutycycle(Dutycycle),status(false){
	DRV::GPIO::toggleOff(PinNum);
	//DRV::GPIO::setPWM(PinNum,Dutycycle,Range,Frequency);
}

DRV::Motor_1wire::~Motor_1wire(void){
	DRV::GPIO::clearPin(this->pin);
}

void DRV::Motor_1wire::spin(void){
	DRV::GPIO::setPWM(this->pin,this->PWM_Dutycycle,this->PWM_Range,this->PWM_Frequency);
	this->status = true;
}

void DRV::Motor_1wire::spin(unsigned int Speed){
	DRV::GPIO::setPWM(this->pin,Speed,this->PWM_Range,this->PWM_Frequency);
	this->status = true;
}

void DRV::Motor_1wire::spinDuring(double Time){
	DRV::GPIO::setPWM(this->pin,this->PWM_Dutycycle,this->PWM_Range,this->PWM_Frequency);
	this->status = true;
	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	for (;;)
	{
		if(std::chrono::duration_cast<std::chrono::miliseconds>(std::chrono::system_clock::now()-start) > Time){
			break;
		} 
	}
}

void DRV::Motor_1wire::stop(void){
	DRV::GPIO::toggleOff(this->pin);
	this->status = false;
}

void DRV::Motor_1wire::setSpeed(unsigned int Speed){
	this->PWM_Dutycycle = Speed;
	if(this->status == true){
		this->spin();
	}
}

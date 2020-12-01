#include "DRV_GPIO.h"
#include "pigpio.h"

//constructer
DRV_GPIO::DRV_GPIO(void) : initValue(gpioInitialise()),pinStatus(nullptr){
}
//destructer
DRV_GPIO::~DRV_GPIO(void){
	gpioTerminate();
}

bool DRV_GPIO::checkInitValue(void){
	if(initValue < 0){
		return false;
	}
	else{
		return true;
	}
}

int DRV_GPIO::getInitValue(void){
	return initValue;
}

void DRV_GPIO::initializeAllPin(void){
	int list[22] = {4,17,27,22,10,9,11,5,6,13,19,26,18,23,24,25,8,7,12,16,20,21};
	for(unsigned int i;i<22;i++){
		this->clearPin(list[i]);
	} 
}

void DRV_GPIO::clearPin(unsigned int PinNum){
	this->toggleOff(PinNum);
	this->setPinIn(PinNum);
	this->setPinPullDown(PinNum);
}

int DRV_GPIO::getPinStatus(unsigned int PinNum){
	int i = gpioGetMode(PinNum);
	pinStatus[PinNum-1] = i;
	return i;
}

int* DRV_GPIO::getPinStatus(void){
	pinStatus = nullptr;
	pinStatus = new int[59];
	for(unsigned int i;i<59;i++){
		this->getPinStatus(i+1);
	}
	return pinStatus;
}

void DRV_GPIO::setPinIn(unsigned int PinNum){
	gpioSetMode(PinNum,PI_INPUT);
}

void DRV_GPIO::setPinOut(unsigned int PinNum){
	gpioSetMode(PinNum,PI_OUTPUT);
}

void DRV_GPIO::setPinPullUp(unsigned int PinNum){
	gpioSetPullUpDown(PinNum,PI_PUD_UP);
}

void DRV_GPIO::setPinPullDown(unsigned int PinNum){
	gpioSetPullUpDown(PinNum,PI_PUD_DOWN);
}

void DRV_GPIO::clearPinPullUpDown(unsigned int PinNum){
	gpioSetPullUpDown(PinNum,PI_PUD_OFF);
}

void DRV_GPIO::toggleOn(unsigned int PinNum){
	gpioWrite(PinNum,PI_HIGH);
}

void DRV_GPIO::toggleOff(unsigned int PinNum){
	gpioWrite(PinNum,PI_LOW);
}

bool DRV_GPIO::readPin(unsigned int PinNum){
	if (gpioRead(PinNum) == 1){
		return true;
	}else
	{
		return false;
	}	
}

void DRV_GPIO::setPWM(unsigned int PinNum,unsigned int DutyCycle,unsigned int Range,unsigned int Frequency){
	gpioSetPWMfrequency(PinNum,Frequency);
	gpioSetPWMrange(PinNum,Range);
	gpioPWM(PinNum,DutyCycle);
}

void DRV_GPIO::setInterrupt(unsigned int PinNum,gpioAlertFunc_t Function){
	gpioSetAlertFunc(PinNum,Function);
}



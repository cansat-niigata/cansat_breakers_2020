#ifndef GPIO_H
	#define GPIO_H
#endif

#include "log.h"
#include <string>
#include <string.h>
#include <pigpio.h>

namespace drv{
    class Gpio{

		public:
			static int startGPIO(void);
			static void terminate(void);
		//GPIO Control
		//  GPIO setting
			static void initializeAllPin(void);
			static void clearPin(unsigned int gpio);

			static int getPinStatus(unsigned int gpio);
		//	static int* getAllPinStatus(void);

			static int setPinIn(unsigned int gpio);
			static int setPinOut(unsigned int gpio);

			static int setPinPullUp(unsigned int gpio);
			static int setPinPullDown(unsigned int gpio);
			static int clearPinPullUpDown(unsigned int gpio);
	
			static int toggleOn(unsigned int gpio);
			static int toggleOff(unsigned int gpio);
			static int setOffTimer(unsigned int gpio,double miliseconds);

			static bool readPin(unsigned int gpio);

			static int* setPWM(unsigned int gpio,unsigned int dutycycle=128,unsigned int range=255,unsigned int frequency=1000);

			static int setInterrupt(unsigned int gpio,gpioAlertFunc_t Function,const char* edge="FALLING");
			static int setISRInterrupt(unsigned int gpio,unsigned int edge,gpioAlertFunc_t Function,unsigned int timeout=1000);

			static int openSerial(const char* device,unsigned int baudrate);
			static int closeSerial(unsigned int openedSerial);
			static int getDataAvailable(unsigned int openedSerial);
			static std::string readSerial(unsigned int openedSerial);
			static std::string readSerial(unsigned int openedSerial,unsigned int count=1);
			static int sendSerial(unsigned int openedSerial,const char* message);
			static int sendSerial(unsigned int openedSerial,std::string message);

			static int openSoftwareSerial2read(unsigned int gpio,unsigned int baudrate,unsigned int datebits,bool invert=false);
			static int closeSoftwareSerial(unsigned int gpio);
			static std::string readSoftwareSerial(unsigned int gpio);

			static int openI2C(unsigned int i2cbus,unsigned int address);
			static int closeI2C(unsigned int openedbus);
			static int writeI2CByte(unsigned int openedbus,unsigned int sendbyte);
			static int readI2CByte(unsigned int openedbus,unsigned int readbyte);
			static int writeI2CBlockData(unsigned int openedbus,unsigned int _register,char* buffer);
			static int* readI2CBlockData(unsigned int openedbus,unsigned int _register);


	};
}
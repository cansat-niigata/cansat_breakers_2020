#ifndef SL0T_H
	#define SL0T_H
#endif

#include <string>
#include <iostream>

#include "pigpio.h"

class DRV{

	class Stream{
        private:
            const unsigned int pinBusy;

            const bool terminal_mode;
            const char* device;
            const unsigned int baudrate;
            int handle;

            const char* Buffer2transmit;
            const char* Buffer2recieve;

        public:
            Stream(bool terminal);
            Stream(char* port,unsigned int baud=19200);
            ~Stream(void);

            bool print(char* message);
            char* read();
    };

	class GPIO{
		private:
			const int initValue;
			int* pinStatus;


		public:
		//constructer&destructer
			GPIO(void);
			~GPIO(void);
		//getValue of handle
			bool checkInitValue(void);
			int getInitValue(void);
		//GPIO Control
		//  GPIO setting
			void initializeAllPin(void);
			void clearPin(unsigned int PinNum);

			int getPinStatus(unsigned int PinNum);
			int* getPinStatus(void);

			void setPinIn(unsigned int PinNum);
			void setPinOut(unsigned int PinNum);

			void setPinPullUp(unsigned int PinNum);
			void setPinPullDown(unsigned int PinNum);
			void clearPinPullUpDown(unsigned int PinNum);
	
			void toggleOn(unsigned int PinNum);
			void toggleOff(unsigned int PinNum);

			bool readPin(unsigned int PinNum);

			void setPWM(unsigned int PinNum,unsigned int DutyCycle=128,unsigned int Range=255,unsigned int Frequency=1000);

			void setInterrupt(unsigned int PinNum,gpioAlertFunc_t Function);
	};

	class Motor_1wire{
		private:
			const unsigned int pin;
			unsigned int PWM_Frequency;
			unsigned int PWM_Range;
			unsigned int PWM_Dutycycle;
            bool status;
		public:
			Motor_1wire(unsigned int PinNum,unsigned int Frequency=1000,unsigned int Range=255,unsigned int Dutycycle=128);
			~Motor_1wire(void);

            void spin(void);
			void spin(unsigned int Speed);
			void spinDuring(double Time);
			void stop(void);
			void setSpeed(unsigned int Speed=128);
	};

	class Motor{
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
	};

};
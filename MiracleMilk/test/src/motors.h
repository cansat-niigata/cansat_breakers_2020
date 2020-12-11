#ifndef MOTORS_H
#define MOTORS_H
#endif

#include "log.h"
#include "gpio.h"

namespace drv{
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
            Notes notes;
			const unsigned int pinA;
			const unsigned int pinB;
			unsigned int PWM_Frequency;
			unsigned int PWM_Range;
			unsigned int PWM_Dutycycle;
		public:
			Motor(unsigned int PinNum1,unsigned int PinNum2,unsigned int Frequency=1000,unsigned int Range=255,unsigned int Dutycycle=128,const char* file="./log/motors.txt");
			~Motor(void);

            void spin(bool invert=false);
			void spin(unsigned int Speed=128,bool Invert=false);
			void spinDuring(double Time,unsigned int Speed=128,bool invert=false);
			void stop(void);
			void setSpeed(unsigned int Speed=128);
	};
}
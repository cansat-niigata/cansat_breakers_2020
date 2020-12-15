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

#define STS_MOTOR_S 0x00
#define STS_MOTOR_F 0x01
#define STS_MOTOR_B 0x02

	class Motor{
		private:
            Notes notes;
			const char* name;
			const unsigned int pinA;
			const unsigned int pinB;
			unsigned int PWM_Frequency;
			unsigned int PWM_Range;
			unsigned int PWM_Dutycycle;
			uint8_t sts;

		public:
			Motor(const char* name,unsigned int PinNum1,unsigned int PinNum2,unsigned int Frequency=1000,unsigned int Range=255,unsigned int Dutycycle=128,const char* file="./log/motors.txt");
			~Motor(void);

            void spin(bool invert=false);
			void spin(unsigned int Speed,bool Invert=false);
			void spinDuring(double Time,unsigned int Speed=128,bool invert=false);
			void stop(void);
			void setSpeed(unsigned int Speed=128);
			Note checkStatus(int res);
			void dumpFile(void);
	};

	class Motors{
		private:
			Motor LEFT;
			Motor RIGHT;
			uint8_t stsL;
			uint8_t stsR;
		public:
			Motors(unsigned int pinR1,unsigned int pinR2,unsigned int pinL1,unsigned int pinL2,unsigned int R_SPEED=128,unsigned int L_SPEED=128);
			~Motors(void);

			void drive(uint8_t RIGHT,uint8_t LEFT);
			void drive(uint8_t RIGHT,uint8_t LEFT,unsigned int R_speed,unsigned int L_speed);
			void stop(void);
	};
}


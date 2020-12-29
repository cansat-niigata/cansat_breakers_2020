#ifndef MOTORS_H
#define MOTORS_H
#endif

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
			const unsigned int pinA;
			const unsigned int pinB;
			unsigned int PWM_Frequency;
			unsigned int PWM_Range;
			unsigned int PWM_Dutycycle;
			uint8_t sts;

		public:
			Motor(void);
			Motor(const char* name,unsigned int PinNum1,unsigned int PinNum2,unsigned int Frequency=1000,unsigned int Range=255,unsigned int Dutycycle=128);
			~Motor(void);

            void spin(bool invert=false);
			void spin(unsigned int Speed,bool Invert=false);
			void spinDuring(double Time,unsigned int Speed=128,bool invert=false);
			void stop(void);
			void setSpeed(unsigned int Speed=128);;
			uint8_t getStatus(void);
			void dumpFile(void);
	};

	class Motors{
		private:
			Motor* left;
			Motor* right;
		public:
			Motors(unsigned int pinR1,unsigned int pinR2,unsigned int pinL1,unsigned int pinL2,unsigned int R_SPEED=128,unsigned int L_SPEED=128);
			~Motors(void);

			void drive(uint8_t rsts,uint8_t lsts);
			void drive(uint8_t rsts,uint8_t lsts,unsigned int R_speed,unsigned int L_speed);
			void setSpeedL(unsigned int speed);
			void setSpeedR(unsigned int speed);
			void setSpeed(unsigned int R_speed,unsigned int L_speed);
			void stop(void);
			uint8_t getStatusL(void);
			uint8_t getStatusR(void);
	};
}


#ifndef IMU_H
#define IMU_H
#endif 

#include "MPU9250-driver/inv_mpu.h"
#include "MPU9250-driver/inv_mpu_dmp_motion_driver.h"
#include "MPU9250-driver/dmpKey.h"
#include "MPU9250-driver/dmpmap.h"

#include "../log/log.h"
#include "./vectors/vector.h"
#include "./vectors/quaternion.h"
#include <iostream>

#define MPU9250
#define AK8963_SECONDARY
#define COMPASS_ENABLED

typedef int inv_error_t;
#define INV_SUCCESS 0
#define INV_ERROR 0x20

#define DIM 3

namespace drv{
	class imu9250{
		private:
			uint8_t stat;

			int isready = 0;
			int initialized = 0;

			int respond;
			
			uint8_t asence;
			uint16_t lfp_freq;
			uint16_t gsence;
			//unsigned int msence;

			int16_t raw_acc[3];
			int16_t raw_gyr[3];
			int16_t raw_mgn[3];
			int64_t raw_quat[4];
			int16_t sensors;
			uint8_t counter_fifo;
			uint8_t bbuffer_fifo[256];

			Quaternion quat;
			Vector grav;

			double rpy[3];

			float acc[3];
			float gyr[3];
			float mgn[3];

			unsigned int comm_rate;

			static void delay_ms(unsigned int ms);

			static double fixAngle(double angle);

			static void updateGrav(Vector* v,Quaternion* q);

			static void updateRollPitchYaw(double buf[3],Quaternion* q,Vector* v);

		public:
			imu9250(void);
			imu9250(uint8_t acc_range,uint16_t gyro_range,uint16_t lfp_cfreq,unsigned int commrate);
			~imu9250(void);

			int start(void);
			int update(void);

			double* getQuaternion(void);
			double* getRollPitchYaw(void);

			float* getAccel(void);
			float* getGyro(void);
			float* getCompass(void);
		
	};
};
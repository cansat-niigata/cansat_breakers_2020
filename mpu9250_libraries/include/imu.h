#pragma once

#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "dmpKey.h"
#include "dmpmap.h"

#include "vector.h"
#include "quaternion.h"
#include <iostream>
#include <unistd.h>

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
			unsigned char stat;

			int isready = 0;
			int initialized = 0;

			int respond;
			
			unsigned short asence;
			unsigned short gsence;
			unsigned short lpf_freq;
			//unsigned int msence;

			short raw_acc[3];
			short raw_gyr[3];
			short raw_mgn[3];
			long raw_quat[4];
			short sensors;
			unsigned char counter_fifo;
			unsigned char buffer_fifo[256];

			Quaternion quat;
			Vector grav;

			float rpy[3];

			float acc[3];
			float gyr[3];
			float mgn[3];

			unsigned short comm_rate;

			static void delay_ms(unsigned int ms);

			static float fixAngle(float angle);

			static void updateGrav(Vector* v,Quaternion* q);

			static void updateRollPitchYaw(float* buf,Quaternion* q,Vector* v);

		public:
			imu9250(void);
			imu9250(unsigned short acc_range,unsigned short gyro_range,unsigned short lfp_cfreq,unsigned int commrate);
			~imu9250(void);

			int start(void);
			int update(void);

			void getQuaternion(float* buf);
			void getRollPitchYaw(float* buf);

			float* getAccel(void);
			float* getGyro(void);
			float* getCompass(void);
		
			
	};
};
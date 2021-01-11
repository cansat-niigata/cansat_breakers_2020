#pragma once

#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "dmpKey.h"
#include "dmpmap.h"

#include "vector.h"
#include "quaternion.h"
#include "others.h"
#include <iostream>
#include <string>
#include <mutex>
#include <thread>
#include <math.h>


#define MPU9250
#define AK8963_SECONDARY
#define COMPASS_ENABLED

namespace drv{
	class imu9250{
		private:
			std::mutex mtx;
			std::thread th;			

			int stat = 0;

			unsigned char asense_conf;
			unsigned short gsense_conf;
			unsigned char lpf_freq;
			
			float asense;
			float gsense;
			float msense = 6.665f; 
			

			short raw_acc[3];
			short raw_gyr[3];
			short raw_mgn[3];
			long raw_quat[4];

			long bias_acc[3];
			long bias_gyr[3];
			float bias_mgn[3] = {10.4225,-7.5992,-32.2036};

			short sensors;
			unsigned char counter_fifo;

			Quaternion quat;

			unsigned short comm_rate;
			unsigned int interval; 
			
			static float processRawData(short raw,float sense);
			static std::string to_binString(unsigned int val);

		public:
			imu9250(void);
			imu9250(unsigned char acc_range,unsigned char gyro_range,unsigned char lfp_cfreq,unsigned int commrate);
			~imu9250(void);

			float getAccelSense(void);
			float getGyroSense(void);
			int setAccelSense(unsigned char fsr);
			int setGyroSense(unsigned short fsr);

			int start(void);
			int update(void); 

			void loop(unsigned int _interval);

			void run(void);
			void terminate(void);
			bool isRunning(void);

			Vector getAccel(void);
			Vector getGyro(void);
			Vector getCompass(void);
		
			Quaternion getQuaternion(void);

			float getHeading(void);

			
	};
};
#pragma once

#include "gpio.h"
#include "log.h"
#include "coordinate.h"
#include "../mpu9250_libraries/include/imu.h"
#include "../TinyGPSPlus-1.0.2b/src/TinyGPS++.h"

#include <unistd.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

namespace drv{

	enum dump2{
		COUT,
		SOUT,
		FOUT,
		CSOUT,
		CFOUT,
		SFOUT,
		CSFOUT
	};

	struct _gendl{
		char dum;
	}gendl;

	struct _gflush{
		char dum;
	}gflush;

	struct Navigation{
		Quaternion attitude;
		Vector accel;
		Vector gyro;
		Vector compass;
		Coordinate pos_now;
		Coordinate pos_prev;
		float direction_imu;
		float direction_gps;
		float distance_gps;
	}; 

	class gStream{
		private:
			bool gpioinit = false;
			bool readyimu = false;
			bool readygps = false;
			int handle = -1;

			unsigned int def_RTS = 17;
			unsigned int RTS = 0;

			const char* def_sdev = "/dev/ttyAMA0";
			const char* def_logf = "./log.txt";
			

			std::stringstream gss_rx;
			std::stringstream gss_tx;
			std::ofstream logfs;

			imu9250 imu;
			TinyGPSPlus ggps;
			
			bool waitRTS(void);
			int readSerial(void);
			int getSerReaded(void);

		public:
			dump2 DOUT = CFOUT;
			
			gStream(void);
			gStream(const char* logfile="./log.txt");
			gStream(bool initGPIO,bool useIMU,bool useGPS,const char* logfile="./log.txt");
			~gStream(void);

			gStream& setRTS(void);
			gStream& setRTS(unsigned int pin);

			int openSerial(const char* devname,unsigned int baudrate);
			int openSerial(unsigned int baudrate);
			int closeSerial(void);

			template <typename Type>
			gStream& operator << (const Type& something);
			gStream& operator << (const _gendl& endl);
			gStream& operator << (const _gflush& flush);
			gStream& operator << (const Note& n);
			gStream& operator << (const dump2& dest);
			gStream& operator >> (std::string& str);	
		
			gStream& updateGps(void);
			Coordinate getGpsData(void);

			Vector getAccel(void);
			Vector getGyro(void);
			Vector getCompass(void);
			Quaternion getQuaternion(void);
			float getDirection(void);
			Navigation getNavigation(void);
			
	};
}
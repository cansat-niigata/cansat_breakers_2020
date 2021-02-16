#include "../include/imu.h"

using namespace drv;

imu9250::imu9250(void):
asense_conf(2),gsense_conf(2000),lpf_freq(5),comm_rate(16){
	asense = 0;
	gsense = 0.0f;
}

/*imu9250::imu9250(unsigned char acc_range,unsigned char gyro_range,unsigned char lfp_cfreq,unsigned int commrate):
asense_conf(acc_range),gsense_conf(gyro_range),lpf_freq(lfp_cfreq),comm_rate(commrate){
	asense = 0;
	gsense = 0.0f;
}*/

imu9250::~imu9250(void){
	terminate();
}

imu9250& imu9250::setCommrate(int commrate){
	if (commrate > 200 || commrate < 1){
		std::cout << "invalid argument." << std::endl;
	}else{
		this->comm_rate = commrate;
	}
	return *this;
}

float imu9250::getAccelSense(void){
	if (mpu_get_accel_fsr(&asense_conf) != 0){
		std::cout << "Failed to get Accel fsr.\n" << std::endl;
		return -1;
	}
	unsigned short sens;
	if (mpu_get_accel_sens(&sens) != 0){
		std::cout << "Failed to get Accel Sens.\n" << std::endl;
		return -1;
	}
	return (float)sens;
}

float imu9250::getGyroSense(void){
	if (mpu_get_gyro_fsr(&gsense_conf) != 0){
		std::cout << "Failed to get Accel fsr.\n" << std::endl;
		return -1;
	}
	float sens;
	if (mpu_get_gyro_sens(&sens) != 0){
		std::cout << "Failed to get Accel Sens.\n" << std::endl;
		return -1;
	}
	return sens;
}

int imu9250::setAccelSense(unsigned char fsr){
	if (mpu_set_accel_fsr(fsr) != 0){
		std::cout << "Failed to set Accel fsr.\n" << std::endl;
		return -1;
	}
	asense = getAccelSense();
	return 0;
}

int imu9250::setGyroSense(unsigned short fsr){
	if (mpu_set_gyro_fsr(fsr) != 0){
		std::cout << "Failed to set Gyro fsr.\n" << std::endl;
		return -1;
	}
	gsense = getGyroSense();
	return 0;
}

int imu9250::start(bool quiet){
	
	if (!quiet) std::cout << "Initializing...MPU9250\n" << std::endl;
	if (mpu_init(NULL) != 0){
		std::cout << "failed to initialize MPU9250.\n" << std::endl;
		return -1;
	}

	/*if (!quiet) std::cout << "Running self test...\n" << std::endl;
	int res = mpu_run_6500_self_test(bias_gyr,bias_acc,true);
	if (!quiet) std::cout << "Result is:" << to_binString(res) << "\n" << bias_acc << ',' << bias_gyr << std::endl;*/


	if (!quiet) std::cout << "Setting MPU sensors...\n" << std::endl;
	if (mpu_set_sensors(INV_XYZ_GYRO|INV_XYZ_ACCEL|INV_XYZ_COMPASS) != 0){
		std::cout << "Failed to set sensors!\n" << std::endl;
		return -1;
	} 

	if (!quiet) std::cout << "Setting ACCEL sensitivity...\n" << std::endl;
	if (setAccelSense(asense_conf) != 0){
		std::cout << "Failed to set GYRO sensitivity...\n" << std::endl;
		return -1;
	}

	if (!quiet) std::cout << "Setting GYRO sensitivity...\n" << std::endl;
	if (setGyroSense(gsense_conf) != 0){
		std::cout << "Failed to set GYRO sensitivity...\n" << std::endl;
		return -1;
	}

	if (!quiet) std::cout << "Setting LPF...\n" << std::endl;
	if (mpu_set_lpf(lpf_freq) != 0){
		std::cout << "Failed to set LPF\n" << std::endl;
		return -1;
	}

	if (!quiet) std::cout << "Setting FIFO...\n" << std::endl;
	if (mpu_configure_fifo(INV_XYZ_GYRO|INV_XYZ_ACCEL|INV_XYZ_COMPASS) != 0){
		std::cout << "Failed to set FIFO\n" << std::endl;
		return -1;
	}

	if (!quiet) std::cout << "Loading DMP firmware...\n" << std::endl;
	if (dmp_load_motion_driver_firmware() != 0){
		std::cout << "Failed to load DMP.\n" << std::endl;
		return -1;
	}

	if (!quiet) std::cout << "Activating DMP..." << std::endl;
	if (mpu_set_dmp_state(1) != 0){
		std::cout << "Failed.\n" << std::endl;
		return -1;
	}else{
		if (!quiet) std::cout << "Success.\n" << std::endl;
	}

	if (!quiet) std::cout << "Configuring...\n" << std::endl;
	if (dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT|DMP_FEATURE_SEND_RAW_ACCEL|DMP_FEATURE_SEND_CAL_GYRO|DMP_FEATURE_GYRO_CAL) != 0){
		std::cout <<"Failed to enable DMP features\n" << std::endl;
		return -1;
	}

	if (dmp_set_fifo_rate(comm_rate) != 0){
		std::cout << "Failed to set DMP FIFO rate.\n" << std::endl;
		return -1;
	}

	if (mpu_reset_fifo() != 0){
		std::cout << "Failed to reset FIFO.\n" << std::endl;
		return -1;
	}

	interval = (int)(1000/comm_rate);
	stat = 1;

	return 0;
}

int imu9250::update(void){
	if (stat == 0){
		std::cout << "Error:DMP not ready.\n" << std::endl;
		return -1;
	}

	while (dmp_read_fifo(raw_gyr,raw_acc,raw_quat,nullptr,&sensors,&counter_fifo));
	mpu_get_compass_reg(raw_mgn,NULL);
	if (raw_acc[0] != raw_acc_x_prev){
		isupdated = true;
	}else{
		isupdated = false;
	}
	raw_acc_x_prev = raw_quat[0];

	//quat = Quaternion(raw_quat);
	
	/*float dt = log.getDurationfrom()*0.001*0.5;
    Vector tmp1 = absAccel;
    Vector tmp2 = absVeloc;
    absAccel = Vector(processRawData(raw_acc[0],asense),processRawData(raw_acc[1],asense),processRawData(raw_acc[2],asense)).rotate(quat);
	absAccel.z -= 1;
    absVeloc += (tmp1 + absAccel)*dt;
    absLengt += (tmp2 + absVeloc)*dt;*/
	return 0;
}

void imu9250::loop(unsigned int interval){
	while (isRunning()){
		mtx.lock();
		update();
		mtx.unlock();
		delay_ms(interval);
	}
}

void imu9250::run(void){
	if (stat == 0){
		start(true);
	}
	stat = 2;
	log = Log();
	std::thread th(&imu9250::loop,this,interval);
	th.detach();
}

void imu9250::terminate(void){
	if (isRunning()){
		std::lock_guard<std::mutex> lock(mtx);
		stat = 1;
	}
}

bool imu9250::isRunning(void){
	bool res;
	std::lock_guard<std::mutex> lock(mtx);
	if (stat == 2){
		res = true;
	}else{
		res = false;
	}
	return res;
}

bool imu9250::isUpdated(void){
	std::lock_guard<std::mutex> lock(mtx);
	return isupdated;
}

Vector imu9250::getAccel(void){
	std::lock_guard<std::mutex> lock(mtx);
	return Vector(processRawData(raw_acc[0],asense),processRawData(raw_acc[1],asense),processRawData(raw_acc[2],asense));
}

Vector imu9250::getGyro(void){
	std::lock_guard<std::mutex> lock(mtx);
	return Vector(processRawData(raw_gyr[0],gsense),processRawData(raw_gyr[1],gsense),processRawData(raw_gyr[2],gsense));
}

Vector imu9250::getCompass(bool calib){
	std::lock_guard<std::mutex> lock(mtx);
	if (calib){
		float mgn[3];
		mgn[0] = processRawData(raw_mgn[0],msense) + bias_mgn[0];
		mgn[1] = processRawData(raw_mgn[1],msense) + bias_mgn[1];
		mgn[2] = processRawData(raw_mgn[2],msense) + bias_mgn[2];

		return Vector(mgn[0],mgn[1],mgn[2]);
	}
	return Vector(processRawData(raw_mgn[0],msense),processRawData(raw_mgn[1],msense),processRawData(raw_mgn[2],msense));
}

/*Vector imu9250::getAbsAccel(void){
	std::lock_guard<std::mutex> lock(mtx);
	return absAccel;
}

Vector imu9250::getAbsVelocity(void){
	std::lock_guard<std::mutex> lock(mtx);
	return absVeloc;
}

Vector imu9250::getAbsLength(void){
	std::lock_guard<std::mutex> lock(mtx);
	return absLengt;
}*/

Quaternion imu9250::getQuaternion(void){
	std::lock_guard<std::mutex> lock(mtx);
	return Quaternion(raw_quat);
}

float imu9250::getHeading(float macro){
	//std::lock_guard<std::mutex> lock(mtx);
	float heading;
	Vector comp = getCompass(true);
	if (comp.y == 0){
		if (comp.x < 0){
			heading = M_PI;
		}else{
			heading = 0;
		}
	}else{
		heading = atan2(comp.y,comp.x);
	}

	heading *= 180/M_PI;
	if (macro != 0){
		heading += macro;
		if (heading > 180){
			heading -= 360;
		}else if (heading <= -180){
			heading += 360;
		}
	}

	return heading;
}

float imu9250::processRawData(short raw,float sense){
	return raw/sense;
}

std::string imu9250::to_binString(unsigned int val)
{
    if( !val )
        return std::string("0");
    std::string str;
    while( val != 0 ) {
        if( (val & 1) == 0 )  // val は偶数か？
            str.insert(str.begin(), '0');  //  偶数の場合
        else
            str.insert(str.begin(), '1');  //  奇数の場合
        val >>= 1;
    }
    return str;
}




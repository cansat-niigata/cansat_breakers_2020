#include "../include/imu.h"

using namespace drv;

imu9250::imu9250(void):
asense_conf(2),gsense_conf(2000),lpf_freq(5),comm_rate(8){
	asense = 0;
	gsense = 0.0f;
}

imu9250::imu9250(unsigned char acc_range,unsigned char gyro_range,unsigned char lfp_cfreq,unsigned int commrate):
asense_conf(acc_range),gsense_conf(gyro_range),lpf_freq(lfp_cfreq),comm_rate(commrate){
	asense = 0;
	gsense = 0.0f;
}

imu9250::~imu9250(void){
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

int imu9250::start(void){

	std::cout << "Initializing...MPU9250\n" << std::endl;
	if (mpu_init(NULL) != 0){
		std::cout << "failed to initialize MPU9250.\n" << std::endl;
		return -1;
	}

	std::cout << "Running self test...\n" << std::endl;
	int res = mpu_run_6500_self_test(bias_gyr,bias_acc,true);
	std::cout << "Result is:" << to_binString(res) << "\n" << std::endl;
	//mpu_set_bypass(1);

	std::cout << "Setting MPU sensors...\n" << std::endl;
	if (mpu_set_sensors(INV_XYZ_GYRO|INV_XYZ_ACCEL|INV_XYZ_COMPASS) != 0){
		std::cout << "Failed to set sensors!\n" << std::endl;
		return -1;
	} 

	std::cout << "Setting ACCEL sensitivity...\n" << std::endl;
	if (setAccelSense(asense_conf) != 0){
		std::cout << "Failed to set GYRO sensitivity...\n" << std::endl;
		return -1;
	}

	std::cout << "Setting GYRO sensitivity...\n" << std::endl;
	if (setGyroSense(gsense_conf) != 0){
		std::cout << "Failed to set GYRO sensitivity...\n" << std::endl;
		return -1;
	}

	std::cout << "Setting LPF...\n" << std::endl;
	if (mpu_set_lpf(lpf_freq) != 0){
		std::cout << "Failed to set LPF\n" << std::endl;
		return -1;
	}

	std::cout << "Setting FIFO...\n" << std::endl;
	if (mpu_configure_fifo(INV_XYZ_GYRO|INV_XYZ_ACCEL|INV_XYZ_COMPASS) != 0){
		std::cout << "Failed to set FIFO\n" << std::endl;
		return -1;
	}

	std::cout << "Loading DMP firmware...\n" << std::endl;
	if (dmp_load_motion_driver_firmware() != 0){
		std::cout << "Failed to load DMP.\n" << std::endl;
		return -1;
	}

	std::cout << "Activating DMP..." << std::endl;
	if (mpu_set_dmp_state(1) != 0){
		std::cout << "Failed.\n" << std::endl;
		return -1;
	}else{
		std::cout << "Success.\n" << std::endl;
	}

	std::cout << "Configuring...\n" << std::endl;
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
	quat = Quaternion(raw_quat);
	mpu_get_compass_reg(raw_mgn,NULL);
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
		start();
	}
	stat = 2;
	th = std::thread(loop,interval);
}

void imu9250::terminate(void){
	if (isRunning()){
		std::lock_guard<std::mutex>lock(mtx);
		stat = 1;
		th.join();
	}
}

bool imu9250::isRunning(void){
	bool res;
	std::lock_guard<std::mutex>lock(mtx);
	if (stat == 2){
		res = true;
	}else{
		res = false;
	}
	return res;
}

Vector imu9250::getAccel(void){
	std::lock_guard<std::mutex>lock(mtx);
	return Vector(processRawData(raw_acc[0],asense),processRawData(raw_acc[1],asense),processRawData(raw_acc[2],asense));
}

Vector imu9250::getGyro(void){
	std::lock_guard<std::mutex>lock(mtx);
	return Vector(processRawData(raw_gyr[0],gsense),processRawData(raw_gyr[1],gsense),processRawData(raw_gyr[2],gsense));
}

Vector imu9250::getCompass(void){
	std::lock_guard<std::mutex>lock(mtx);
	return Vector(processRawData(raw_mgn[0],msense),processRawData(raw_mgn[1],msense),processRawData(raw_mgn[2],msense));
}

Quaternion imu9250::getQuaternion(void){
	std::lock_guard<std::mutex>lock(mtx);
	Quaternion ret = quat;
	return ret;
}

float imu9250::getHeading(void){
	std::lock_guard<std::mutex>lock(mtx);
	float heading;
	Vector comp = getCompass();
	if (comp.y + bias_mgn[1] == 0){
		if (comp.x + bias_mgn[0] < 0){
			heading = M_PI;
		}else{
			heading = 0;
		}
	}else{
		heading = atan2(comp.x + bias_mgn[0],comp.y + bias_mgn[1]);
	}

	if (heading > M_PI){
		heading -= M_PI*2;
	}else if (heading < -M_PI){
		heading += M_PI*2;
	}

	return heading *= 180/M_PI;
}

float imu9250::processRawData(short raw,float sense){
	return (float)(raw)/sense;
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


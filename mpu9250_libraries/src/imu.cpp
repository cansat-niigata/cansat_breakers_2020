#include "../include/imu.h"

using namespace drv;

imu9250::imu9250(void):
asence(2),gsence(2000),lpf_freq(5),comm_rate(8){
}

imu9250::imu9250(unsigned short acc_range,unsigned short gyro_range,unsigned short lfp_cfreq,unsigned int commrate):
asence(acc_range),gsence(gyro_range),lpf_freq(lfp_cfreq),comm_rate(commrate){
}

imu9250::~imu9250(void){
}

int imu9250::start(void){
	initialized = 0;

	std::cout << "Initializing...MPU9250\n" << std::endl;
	if (mpu_init(NULL) != 0){
		std::cout << "failed to initialize MPU9250.\n" << std::endl;
		return -1;
	}

	std::cout << "Setting MPU sensors...\n" << std::endl;
	if (mpu_set_sensors(INV_XYZ_GYRO|INV_XYZ_ACCEL|INV_XYZ_COMPASS) != 0){
		std::cout << "Failed to set sensors!\n" << std::endl;
		return -1;
	} 

	std::cout << "Setting ACCEL sensitivity...\n" << std::endl;
	if (mpu_set_accel_fsr(asence) != 0){
		std::cout << "Failed to set GYRO sensitivity...\n" << std::endl;
		return -1;
	}

	std::cout << "Setting GYRO sensitivity...\n" << std::endl;
	if (mpu_set_gyro_fsr(gsence) != 0){
		std::cout << "Failed to set GYRO sensitivity...\n" << std::endl;
		return -1;
	}

	std::cout << "Activating..." << std::endl;
	mpu_get_power_state(&stat);
	if (stat){
		std::cout << "Success.\n" << std::endl;
	}else{
		std::cout << "Failed. " << stat << '\n' << std::endl;
	}

	std::cout << "Setting LPF...\n" << std::endl;
	if (mpu_set_lpf(lpf_freq) < 0){
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

	std::cout << "Varifing...\n" << std::endl;
	interval = (unsigned long)(1000/comm_rate);
	std::cout << "Running self test...\n" << std::endl;
	int res = mpu_run_6500_self_test(bias_gyr,bias_acc,true);
	std::cout << "Result is:" << to_binString(res) << "\n" << std::endl;

	initialized = 1;
	isready = 1;

	return 0;
}

int imu9250::update(void){
	if (isready == 0){
		std::cout << "Error:DMP not ready.\n" << std::endl;
		return -1;
	}

	while (dmp_read_fifo(raw_gyr,raw_acc,raw_quat,nullptr,&sensors,&counter_fifo));
	quat = Quaternion(raw_quat);
	quat.toEulerAngle().toArray(ypr);

	mpu_get_compass_reg(raw_mgn,nullptr);

	for (unsigned int i = 0; i < 3; i++){
		acc[i] = (float)(raw_acc[i]);
		gyr[i] = (float)(raw_gyr[i]);
		mgn[i] = (float)(raw_mgn[i]);
	}

	return 0;
}

float* imu9250::getAccel(void){
	return acc;
}

float* imu9250::getGyro(void){
	return gyr;
}

float* imu9250::getCompass(void){
	return mgn;
}

Quaternion imu9250::getQuaternion(void){
	Quaternion ret = quat;
	return ret;
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
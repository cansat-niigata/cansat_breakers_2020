#include "imu.h"

using namespace drv;

imu9250::imu9250(void):
asence(2),gsence(250),lfp_freq(5),comm_rate(8){
}

imu9250::imu9250(uint8_t acc_range,uint16_t gyro_range,uint16_t lfp_cfreq,unsigned int commrate):
asence(acc_range),gsence(gyro_range),lfp_freq(lfp_cfreq),comm_rate(commrate){
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
	do{
		delay_ms((unsigned int)(1000/comm_rate));
		respond = dmp_read_fifo(raw_gyr,raw_acc,raw_quat,nullptr,&sensors,&counter_fifo);
	}while (respond != 0 || counter_fifo < 5);

	initialized = 1;
	isready = 1;

	return 0;
}

int imu9250::update(void){
	if (!isready){
		std::cout << "Error:DMP not ready.\n" << std::endl;
		return -1;
	}

	while (dmp_read_fifo(raw_gyr,raw_acc,raw_quat,nullptr,&sensors,&counter_fifo));
	quat = Quaternion(raw_quat[0],raw_quat[1],raw_quat[2],raw_quat[3]).normalize();
	updateGrav(&grav,&quat);
	updateRollPitchYaw(rpy,&quat,&grav);
	
	mpu_get_compass_reg(raw_mgn,nullptr);

	for (unsigned int i = 0; i < 3; i++){
		rpy[i] = rpy[i]*180/M_PI;
	}

	rpy[2] = fixAngle(rpy[2]);
	rpy[1] = -rpy[1];

	for (unsigned int i = 0; i < 3; i++){
		acc[i] = (float)(raw_acc[i]);
		gyr[i] = (float)(raw_gyr[i]);
		mgn[i] = (float)(raw_mgn[i]);
	}

	return 0;
}

void imu9250::delay_ms(unsigned int ms){
	usleep(ms);
}

float imu9250::fixAngle(float angle){
	if (angle <= -180){
		return angle + 360;
	}else if(angle > 180){
		return angle - 360;
	}
}

void imu9250::updateGrav(Vector* v,Quaternion* q){
	v->x = 2*(q->x*q->z - q->w*q->y);
	v->y = 2*(q->w*q->x + q->y*q->z);
	v->z = q->w*q->w - q->x*q->x - q->y*q->y +q->z*q->z;
}

void imu9250::updateRollPitchYaw(float* buf,Quaternion* q,Vector* v){
	buf[0] = atan(v->y/sqrt(v->x*v->x + v->z*v->z));
	buf[1] = atan(v->x/sqrt(v->y*v->y + v->z*v->z));
	buf[2] = atan2(2*(q->x*q->y - q->w*q->z),2*(q->w*q->w + q->x*q->x) - 1);
}

float* imu9250::getQuaternion(void){
	float q[4];
	q[0] = quat.w;
	q[1] = quat.x;
	q[2] = quat.y;
	q[3] = quat.z;
	return q;
}

float* imu9250::getRollPitchYaw(void){
	return rpy;
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
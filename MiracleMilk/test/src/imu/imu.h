#ifndef IMU_H
#define IMU_H
#endif 

#include "MPU9250-driver/inv_mpu.h"
#include "MPU9250-driver/inv_mpu_dmp_motion_driver.h"
#include "MPU9250-driver/dmpKey.h"
#include "MPU9250-driver/dmpmap.h"

#include "log.h"
#include "vectors.h"
#include <thread>
#include <mutex>

#define MPU9250
#define AK8963_SECONDARY
#define COMPASS_ENABLED

typedef int inv_error_t;
#define INV_SUCCESS 0
#define INV_ERROR 0x20



namespace drv{
    class imu9250{
        private:
            uint8_t status;
            uint8_t fifocount;
            uint8_t fifobuffer[64];
            std::mutex mtx;
            bool readydmp = false;

            int16_t accel_v[3] = {0,0,0};
            int16_t gravity_v[3] = {0,0,0};
            int32_t quat[4];
            int32_t t;
            int16_t c[3];

            Vector gravity;
            Quaternion rotate;

            float temp;
            double accel[3];
            double gyro[3];
            double magnet[3];

            uint8_t fifo_rate = 40;

        public:
            imu9250(void);
            imu9250(int acc_range,int gyro_range,int magnet_range);
        
    };
};
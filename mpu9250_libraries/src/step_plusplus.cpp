#include <iostream>
#include "../include/imu.h"
#include "../include/others.h"


int long2float(long val){
    return (float)val;
}

int main(void){
    drv::imu9250 mpu925 = drv::imu9250();
    std::cout << "initialize..." << "\n" << std::endl;
    if (mpu925.start() < 0){
        std::cout << "initialize failed." << "\n" << std::endl;
        return 0;
    }
    std::cout << "dmp start." << "\n" << std::endl;
    float quat[3];
    float rpy[3];
    while (true){
        if (mpu925.update()){
            std::cout << "update failed." << "\n" << std::endl;
        }
        mpu925.getQuaternion(quat);
        mpu925.getRollPitchYaw(rpy);
        std::cout << "Quaternion:" << std::endl;
        std::cout << "   w:" << quat[0] << "   x:" << quat[1] << "   y:" << quat[2] << "   z:" << quat[3] << "\n" << std::endl;
        std::cout << "Roll Pitch Yaw:" << std::endl;
        std::cout << "   Roll:" << rpy[0] << "   Pitch:" << rpy[1] << "   Yaw:" << rpy[2] << "\n" << std::endl;
        delay_ms(2);
    }
    
    return 0;
}
#include <iostream>
#include "../include/imu.h"
#include "../include/quaternion.h"
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
    Quaternion quat;
    float ypr[3];
    while (true){
        if (mpu925.update()){
            std::cout << "update failed." << "\n" << std::endl;
        }
        quat = mpu925.getQuaternion(); 
        quat.toEulerAngle().toArray(ypr);
        std::cout << "Quaternion:" << std::endl;
        std::cout << "   w:" << quat.w << "   x:" << quat.x << "   y:" << quat.y << "   z:" << quat.z << "\n" << std::endl;
        std::cout << "Yaw Pitch Roll:" << std::endl;
        std::cout << "   Yaw:" << ypr[0] << "   Pitch:" << -ypr[2] << "   Roll:" << ypr[1] << "\n" << std::endl;
        std::cout << "Heading:" << std::endl;
        std::cout << "  " << mpu925.getHeading() << std::endl;
        delay_ms(2);
    }
    
    return 0;
}
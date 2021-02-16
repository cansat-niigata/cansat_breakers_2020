#include <iostream>
#include "../include/imu.h"
#include "../include/quaternion.h"
#include "../include/others.h"


int long2float(long val){
    return (float)val;
}

using namespace drv;

int main(void){
    imu9250 mpu925;
    mpu925.run();
    std::cout << "dmp start." << "\n" << std::endl;
    Quaternion quat;
    Vector m;
    float ypr[3];
    while (true){
        quat = mpu925.getQuaternion(); 
        quat.toEulerAngle().toArray(ypr);
        m = mpu925.getCompass(true);
        std::cout << "Quaternion:" << std::endl;
        std::cout << quat << std::endl;
        std::cout << "   Yaw:" << ypr[0] << "   Pitch:" << -ypr[2] << "   Roll:" << ypr[1] << "\n" << std::endl;
        std::cout << "Heading:" << std::endl;
        std::cout << "  " << -mpu925.getHeading(26) << std::endl;
        std::cout << "Raw magnet:" << std::endl;
        std::cout << m << std::endl;
        delay_ms(200);
    }
    
    return 0;
}
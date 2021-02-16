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
    mpu925.setCommrate(100).start();
    mpu925.run();
    std::cout << "dmp start." << "\n" << std::endl;
    Quaternion quat;
    Vector a;
    Vector aa;
    Vector m;
    float ypr[3];
    bool update;
    while (true){
        update = mpu925.isUpdated();
        std::cout << update << std::endl;
        if (update){
            quat = mpu925.getQuaternion(); 
            quat.toEulerAngle().toArray(ypr);
            a = mpu925.getAccel();
            aa = a.rotate(quat);
            m = mpu925.getCompass(true);
            std::cout << "Quaternion:" << std::endl;
            std::cout << quat << std::endl;
            std::cout << "   Yaw:" << ypr[0] << "   Pitch:" << -ypr[2] << "   Roll:" << ypr[1] << "\n" << std::endl;
            std::cout << "Accel raw:" << std::endl;
            std::cout << "   X:" << a.x << "   Y:" << a.y << "   Z:" << a.z << "\n" << std::endl;
            std::cout << "Accel world:" << std::endl;
            std::cout << "   X:" << aa.x << "   Y:" << aa.y << "   Z:" << aa.z << "\n" << std::endl;
            std::cout << "Heading:" << std::endl;
            std::cout << "  " << -mpu925.getHeading(26) << std::endl;
            std::cout << "Raw magnet:" << std::endl;
            std::cout << m << std::endl;
            usleep(100*1000);
        }
    }
    
    return 0;
}
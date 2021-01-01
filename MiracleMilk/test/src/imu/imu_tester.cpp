#include "imu.h"
#include <iostream>

using namespace drv;

int main(void){
    imu9250 imu = imu9250();
    if (imu.start() < 0){
        std::cout << "abort.\n" << std::endl;
    }
    
    while (true){
        imu.update();

        float q[3];
        imu.getQuaternion(q);
        float rpy[3];
        imu.getRollPitchYaw(rpy);
        float* acc = imu.getAccel();
        float* gyr = imu.getGyro();
        float* mgn = imu.getCompass();

        std::cout << "Quaternion: " << q[0] << ',' << q[1] << ',' << q[2] << ',' << q[3] << '\n' << std::endl;
        std::cout << "Roll,Pitch,Yaw: " << rpy[0] << ',' << rpy[1] << ',' << rpy[2] << '\n' << std::endl;
        std::cout << "Accel(x,y,x): " << acc[0] << ',' << acc[1] << ',' << acc[2] << '\n' << std::endl;
        std::cout << "Gyro(x,y,z): " << gyr[0] << ',' << gyr[1] << ',' << gyr[2] << '\n' << std::endl;
        std::cout << "Compass(x,y,z): " << mgn[0] << ',' << mgn[1] << ',' << mgn[2] << '\n' << std::endl; 
    }
    
}
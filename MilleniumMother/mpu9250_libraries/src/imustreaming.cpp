#include "../include/imu.h"
#include <iostream>
using namespace drv;

int main(void){
    Vector accel;
    Vector velocity;
    Vector length;
    Vector tmp1;
    Vector tmp2;
    Quaternion quat;
    imu9250 imu;
    imu.setCommrate(50).start(true);
    imu.run();
    char m;
    //std::cout << "EulerY,EulerP,EulerR,GyroX,GyroY,GyroZ,heading,AccX,AccY,AccZ" << std::endl;
    while (true){
        std::cin >> m;
        if (m == 'u'){
            quat = imu.getQuaternion();
            std::cout << quat.toEulerAngle() << ',' << imu.getGyro() << ',' << imu.getHeading() << ',' << imu.getAccel() << std::endl;
        }else if (m == 's'){
            imu.terminate();
            break;
        }
    }
}
#include "../include/imu.h"
#include <fstream>

using namespace drv;

int main(void){
    imu9250 mpu;
    Vector mgn;
    std::ofstream f("./MagData.csv");
    if (!f){
        return 0;
    }
    mpu.run();
    for (int i ; i < 1000 ; i++){
        std::cout << i << std::endl;
        mgn = mpu.getCompass();
        f << mgn.x << ',' << mgn.y << ',' << mgn.z << std::endl;
        delay_ms(100);
    }
    return 0;
}
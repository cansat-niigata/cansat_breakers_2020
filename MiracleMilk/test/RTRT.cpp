#include <iostream>
#include <pigpio.h>

int main(void){
    std::cout << "Can you hear me?" << std::endl;

    std::cout << "gpio:" << gpioInitialise() << std::endl;
    return 0;
}
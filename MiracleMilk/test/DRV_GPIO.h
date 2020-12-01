#ifndef DRV_GPIO_H
    #define DRV_GPIO_H
#endif

#include "pigpio.h"

class DRV_GPIO{
private:
    /* data */
    const int initValue;
    int* pinStatus;

public:
//constructer&destructer
    DRV_GPIO(void);
    ~DRV_GPIO(void);
//getValue of handle
    bool checkInitValue(void);
    int getInitValue(void);
//GPIO Control
//  GPIO setting

    void initializeAllPin(void);
    void clearPin(unsigned int PinNum);

    int getPinStatus(unsigned int PinNum);
    int* getPinStatus(void);

    void setPinIn(unsigned int PinNum);
    void setPinOut(unsigned int PinNum);

    void setPinPullUp(unsigned int PinNum);
    void setPinPullDown(unsigned int PinNum);
    void clearPinPullUpDown(unsigned int PinNum);
    
    void toggleOn(unsigned int PinNum);
    void toggleOff(unsigned int PinNum);

    bool readPin(unsigned int PinNum);

    void setPWM(unsigned int PinNum,unsigned int DutyCycle=128,unsigned int Range=255,unsigned int Frequency=1000);

    void setInterrupt(unsigned int PinNum,gpioAlertFunc_t Function);

};



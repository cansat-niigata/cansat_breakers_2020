#include "gpio.h"
using namespace drv;

/*namespace drv{
    class Gpio{*/

//		public:
//			static int startGPIO(void);
int Gpio::startGPIO(void){
    return gpioInitialise();
}
//			static void terminate(void);
void Gpio::terminate(void){
    gpioTerminate();
}
//			static void initializeAllPin(void);
void Gpio::initializeAllPin(void){
    gpioInitialise();
}
//			static void clearPin(unsigned int gpio);
void clearPin(unsigned int gpio){
    
}
//			static int getPinStatus(unsigned int gpio);
int Gpio::getPinStatus(unsigned int gpio){
    return gpioGetMode(gpio);
}
//			static int setPinIn(unsigned int gpio);
int Gpio::setPinIn(unsigned int gpio){
    return gpioSetMode(gpio,PI_INPUT);
}
//			static int setPinOut(unsigned int gpio);
int Gpio::setPinOut(unsigned int gpio){
    return gpioSetMode(gpio,PI_OUTPUT);
}
//			static int setPinPullUp(unsigned int gpio);
int Gpio::setPinPullUp(unsigned int gpio){
    return gpioSetPullUpDown(gpio,PI_PUD_UP);
}
//			static int setPinPullDown(unsigned int gpio);
int Gpio::setPinPullDown(unsigned int gpio){
    return gpioSetPullUpDown(gpio,PI_PUD_DOWN);
}
//			static int clearPinPullUpDown(unsigned int gpio);
int Gpio::clearPinPullUpDown(unsigned int gpio){
    return gpioSetPullUpDown(gpio,PI_PUD_OFF);
}
//			static int toggleOn(unsigned int gpio);
int Gpio::toggleOn(unsigned int gpio){
    return gpioWrite(gpio,PI_HIGH);
}
//			static int toggleOff(unsigned int gpio);
int Gpio::toggleOff(unsigned int gpio){
    return gpioWrite(gpio,PI_LOW);
}
//			static int setOffTimer(unsigned int gpio,unsigned int miliseconds);
int Gpio::setOffTimer(unsigned int gpio,double miliseconds){
    toggleOn(gpio);
    Log().waitFor(miliseconds);
    toggleOff(gpio);
}
//			static bool readPin(unsigned int gpio);
bool Gpio::readPin(unsigned int gpio){
    return gpioRead(gpio);
}
//			static int setPWM(unsigned int gpio,unsigned int dutycycle=128,unsigned int range=255,unsigned int frequency=1000);
int* Gpio::setPWM(unsigned int gpio,unsigned int dutycycle,unsigned int range,unsigned int frequency){
    int res[3] = {0,0,0};
    res[0] = gpioSetPWMfrequency(gpio,frequency);
    res[1] = gpioSetPWMrange(gpio,range);
    res[2] = gpioPWM(gpio,dutycycle);
    return res; 
}
//			static int setInterrupt(unsigned int gpio,unsigned int edge,gpioAlertFunc_t Function);
int Gpio::setInterrupt(unsigned int gpio,gpioAlertFunc_t Function,const char* edge){
    if (edge == nullptr){
        setPinPullUp(gpio);
    }else{
        if (edge == "FALLING"){
            setPinPullUp(gpio);
        }else if (edge == "RISING"){
            setPinPullDown(gpio);
        }else{
           setPinPullUp(gpio); 
        }
    }
    return gpioSetAlertFunc(gpio,Function);
}
//			static int setISRInterrupt(unsigned int gpio,unsigned int edge,gpioAlertFunc_t Function,unsigned int timeout=1000);
int Gpio::setISRInterrupt(unsigned int gpio,unsigned int edge,gpioAlertFunc_t Function,unsigned int timeout){
    return gpioSetISRFunc(gpio,edge,timeout,Function);
}
//			static int openSerial(const char* device,unsigned int baudrate);
int Gpio::openSerial(const char* device,unsigned int baudrate){
    return serOpen(const_cast<char*>(device),baudrate,0);//おゆるし～
}
//			static int closeSerial(int openedSerial);
int Gpio::closeSerial(unsigned int openedSerial){
    return serClose(openedSerial);
}
//			static int getDataAvailable(unsigned int openedSerial);
int Gpio::getDataAvailable(unsigned int openedSerial){
    return serDataAvailable(openedSerial);
}
//			static std::string readSerial(unsigned int openedSerial);
std::string Gpio::readSerial(unsigned int openedSerial){
    char* buf;
    unsigned int count = getDataAvailable(openedSerial);
    serRead(openedSerial,buf,count);
    return std::string(buf);
}
//          static std::string readSerial(unsigned int openedSerial,unsigned int count=1);
std::string Gpio::readSerial(unsigned int openedSerial,unsigned int count){
    char* buf;
    serRead(openedSerial,buf,count);
    return std::string(buf);
}
//			static int sendSerial(unsigned int openedSerial,const char* message);
int Gpio::sendSerial(unsigned int openedSerial,const char* message){
    return serWrite(openedSerial,const_cast<char*>(message),strlen(message)+1);
}
//			static int sendSerial(unsigned int openedSerial,std::string message);

//			static int openSoftwareSerial2read(unsigned int gpio,unsigned int baudrate,unsigned int datebits,bool invert=false);
//			static int closeSoftwareSerial(unsigned int gpio);
//			static std::string readSoftwareSerial(unsigned int gpio);

//			static int openI2C(unsigned int i2cbus,unsigned int address);
//			static int closeI2C(unsigned int openedbus);
//			static int writeI2CByte(unsigned int openedbus,unsigned int sendbyte);
//			static int readI2CByte(unsigned int openedbus,unsigned int readbyte);
//			static int writeI2CBlockData(unsigned int openedbus,unsigned int _register,char* buffer);
//			static int* readI2CBlockData(unsigned int openedbus,unsigned int _register);


//	};
//}
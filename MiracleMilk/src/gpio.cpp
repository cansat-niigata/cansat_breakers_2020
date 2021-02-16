#include "../include/gpio.h"
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
int Gpio::setOffTimer(unsigned int gpio,double milliseconds){
    toggleOn(gpio);
    usleep(milliseconds*1000);
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

int Gpio::changeDutyCycle(unsigned int gpio,unsigned int dutycycle){
    return gpioPWM(gpio,dutycycle);
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
    buf = new char[count];
    serRead(openedSerial,buf,count);
    std::string str = buf;
    delete[] buf;
    return str;
}
//          static std::string readSerial(unsigned int openedSerial,unsigned int count=1);
char Gpio::readSerial_1byte(unsigned int openedSerial){
    const char res = serReadByte(openedSerial);
    if (res < 0){
        return -1;
    }
    return res;
}
//			static int sendSerial(unsigned int openedSerial,const char* message);
int Gpio::sendSerial(unsigned int openedSerial,const char* message){
    return serWrite(openedSerial,const_cast<char*>(message),strlen(message)+1);
}
//			static int sendSerial(unsigned int openedSerial,std::string message);
int Gpio::sendSerial(unsigned int openedSerial,std::string message){
    const char* msg = message.c_str();
    return serWrite(openedSerial,const_cast<char*>(msg),strlen(msg)+1);
}
//			static int openSoftwareSerial2read(unsigned int gpio,unsigned int baudrate,unsigned int datebits,bool invert=false);
int Gpio::openSoftwareSerial2read(unsigned int gpio,unsigned int baudrate,unsigned int databits,bool invert=false){
    int res;
    res = gpioSerialReadOpen(gpio,baudrate,databits);
    if (invert){
        gpioSerialReadInvert(gpio,PI_BB_SER_INVERT);
    }

    return res; 
}
//			static int closeSoftwareSerial(unsigned int gpio);
int Gpio::closeSoftwareSerial(unsigned int gpio){
    return gpioSerialReadClose(gpio);
}

int invertSoftwareSerial(unsigned int gpio){
    return gpioSerialReadInvert(gpio,PI_BB_SER_INVERT);
}

int normalSoftwareSerial(unsigned int gpio){
    return gpioSerialReadInvert(gpio,PI_BB_SER_NORMAL);
}

//			static std::string readSoftwareSerial(unsigned int gpio);
std::string Gpio::readSoftwareSerial(unsigned int gpio){
    char* buf;
    gpioSerialRead(gpio,buf,8);
    return std::string(buf);
}
//			static int openI2C(unsigned int i2cbus,unsigned int address);
int Gpio::openI2C(unsigned int i2cbus,unsigned int address){
    return i2cOpen(i2cbus,address,0);
}
//			static int closeI2C(unsigned int openedbus);
int Gpio::closeI2C(unsigned int openedbus){
    return i2cClose(openedbus);
}
//			static int writeI2CByte(unsigned int openedbus,unsigned int sendbyte);
int Gpio::writeI2CByte(unsigned int openedbus,unsigned int sendbyte){
    return i2cWriteByte(openedbus,sendbyte);
}
//			static int readI2CByte(unsigned int openedbus,unsigned int readbyte);
int Gpio::readI2CByte(unsigned int openedbus){
    return i2cReadByte(openedbus);
}
//			static int writeI2CBlockData(unsigned int openedbus,unsigned int _register,char* buffer);
int Gpio::writeI2CBlockData(unsigned int openedbus,unsigned int _register,char* buffer,unsigned int count){
    return i2cWriteBlockData(openedbus,_register,buffer,count); 
}
//			static int* readI2CBlockData(unsigned int openedbus,unsigned int _register);
const char* Gpio::readI2CBlockData(unsigned int openedbus,unsigned int _register,unsigned int count){
    char* buf;
    i2cReadI2CBlockData(openedbus,_register,buf,count);
    return const_cast<char*>(buf);
}

std::string Gpio::checkThis(int result){
    const char* errmsg;
    switch (result){
        case PI_INIT_FAILED:
            errmsg = "GPIO Initialize Failed!";
            break;
        case PI_BAD_USER_GPIO:
            errmsg = "GPIO not 0-31";
            break;
        case PI_BAD_GPIO:
            errmsg = "GPIO not 0-53";
            break;
        case PI_BAD_MODE:
            errmsg = "GPIO Mode not 0-7";
            break;
        case PI_BAD_PUD:
            errmsg = "GPIO PUD not 0-2";
            break;
        case PI_BAD_PULSEWIDTH:
            errmsg = "PWM PulseWidth not 0 or 500-2500";
            break;
        case PI_BAD_DUTYCYCLE:
            errmsg = "PWM DutyCycle outside set Range";
            break;
        case PI_GPIO_IN_USE:
            errmsg = "This Gpio already using";
            break;

        default:
            if (result < 0){
                return std::string("Something Wrong!!:[ErrorCode]>>") + std::to_string(result);
            }
    }

    return std::string(errmsg);
    
}

//	};
//}
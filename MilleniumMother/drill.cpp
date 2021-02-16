#include <pigpio.h>
#include <unistd.h>
#include <iostream>
#include <chrono>

#define PIN_UPDOWN_IN1 6
#define PIN_UPDOWN_IN2 13
#define PIN_ROTATE_IN1 11
#define PIN_ROTATE_IN2 8
#define PIN_ENCODER_OUT1 16
#define PIN_ENCODER_OUT2 19

#define RATIO 150.58*(1/12)
#define MAX 12
#define MIN 0

#define P 5
#define I 3
#define D 0.2

volatile int rot = 0;
volatile unsigned char val = 0b0000;
volatile unsigned char val_prev = 0b0000;

//順回転
//初期値   HL     HH     LH     LL     HL     HH
//0000 → 0010 → 1011 → 1101 → 0100 → 0010 → 1011...
//.... ← 1110 ← 0111 ← 0001 ← 1000 ← 1110 ← 
//逆回転
//初期値   LH     HH     HL     LL     LH     HH
//0000 → 0001 → 0111 → 1110 → 1000 → 0001 → 0111...

inline void increment(void){
    switch (val){
        case 0b0010:
            rot++;
            break;
        case 0b1011:
            rot++;
            break;
        case 0b1101:
            rot++;
            break;
        case 0b0100:
            rot++;
            break;
        case 0b0001:
            rot--;
            break;
        case 0b0111:
            rot--;
            break;
        case 0b1110:
            rot--;
            break;
        case 0b1000:
            rot--;
            break;
        default:
            val = val_prev;
            break;
    }
}

void update1(int gpio,int level,uint32_t tick){
    if (level == 2){
        return;
    }
    val_prev = val;
    val = ((val << 2) & 0b1100) + ((val & 0b0001) + (level << 1));
    increment();
}

void update2(int gpio,int level,uint32_t tick){
    if (level == 2){
        return;
    }
    val_prev = val;
    val = ((val << 2) & 0b1100) + ((val & 0b0010) + level);
    increment();
}

class pid{
    private:
        double fix(double val){
            if (val > max){
                return max;
            }else if (val < min){
                return min;
            }else{
                return val;
            } 
        }
    public:
        double Kp;
        double Ki;
        double Kd;
        double diff = 0;
        double diff_prev = 0;
        double integral;
        unsigned int max;
        unsigned int min;
        pid (double Kp,double Ti,double Td,unsigned int max,unsigned int min){
            this->max = max;
            this->min = min;
            this->Kp = Kp;
            this->Kd = Kp*Td;
            if (Ti == 0){
                this->Ki = 0;
            }else{
                this->Ki = Kp/Ti;
            }
        }
        double exec(double diff,double dt){
            integral += (diff + diff_prev)*dt*0.5;
            double derivative = (diff - diff_prev)/dt;
            diff_prev = diff;
            return fix(diff_prev*Kp + integral*Ki + derivative*Kd);
        }
};

int main(int argc,char* argv[]){
    opterr = 0;

    if (argc < 2){
        std::cout << "Usage: "<< argv[0] << " [-u or -d] [-r] [-v speed] [-t sec] LENGTH\n"
            << "-u:up head (default)\n"
            << "-d:down head\n"
            << "-r:rotate head (default not)\n"
            << "-v value:change speed for up or down 0-255(default 64))\n"
            << "-t value:change timeout for up or down. -1 will disable timeout (default 5)\n"
            << std::endl;
        return -1;
    }

    int opt;
    bool up = true;
    bool rotate = false;
    int speed = 64;
    int timeout = 5;
    bool dopid = false;
    bool debug = false;
    while ((opt = getopt(argc,argv,"udrv:t:pb")) != -1){
        switch (opt){
            case 'u':
                up = true;
                break;
            case 'd':
                up = false;
                break;
            case 'r':
                rotate = true;
                break;
            case 'v':
                speed = atoi(optarg);
                if (speed > 255 || speed < 0){
                    std::cerr << "invalid option." << std::endl;
                    return -1;
                }
                break;
            case 't':
                timeout = atoi(optarg);
                if (timeout < -1){
                    std::cerr << "invalid option." << std::endl;
                    return -1;
                }
                if (timeout == -1){
                    timeout = 9999;
                }
                break;
            case 'p':
                dopid = true;
            case 'b':
                std::cout << up << ',' << rotate << ',' << speed << ',' << timeout  << ',' << optind << std::endl;
                break;
            default:
                std::cout << "Usage: "<< argv[0] << " [-u or -d] [-r] [-v speed] [-t sec] LENGTH\n"
                << "-u:up head (default)\n"
                << "-d:down head\n"
                << "-r:rotate head (default not)\n"
                << "-v value:change speed for up or down 0-255(default 64))\n"
                << "-t value:change timeout for up or down. -1 will disable timeout (default 5)\n"
                << std::endl;
                return -1;
        }
    }

    if (optind > argc){
        std::cerr << "invalid option." << std::endl;
        return -1;
    }

    int length = atoi(argv[optind]);
        std::cout << argv[optind] << length << std::endl;

    if (length > MAX){
        length = MAX;
    }else if (length < MIN){
        length = MIN;
    }

    if (gpioInitialise() < 0){
        std::cerr << "Failed to initialize GPIO Library." << std::endl;
        return -1;
    }

    gpioSetMode(PIN_UPDOWN_IN1,PI_OUTPUT);
    gpioSetMode(PIN_UPDOWN_IN2,PI_OUTPUT);
    if(rotate){
        gpioSetMode(PIN_ROTATE_IN1,PI_OUTPUT);
        gpioSetMode(PIN_ROTATE_IN2,PI_OUTPUT);
        gpioWrite(PIN_ROTATE_IN1,PI_HIGH);
        gpioWrite(PIN_ROTATE_IN2,PI_LOW);
    }

    gpioSetMode(PIN_ENCODER_OUT1,PI_INPUT);
    gpioSetMode(PIN_ENCODER_OUT2,PI_INPUT);
    gpioSetPullUpDown(PIN_ENCODER_OUT1,PI_PUD_UP);
    gpioSetPullUpDown(PIN_ENCODER_OUT2,PI_PUD_UP);

    gpioSetAlertFunc(PIN_ENCODER_OUT1,update1);
    gpioSetAlertFunc(PIN_ENCODER_OUT2,update2);

    pid param(P,I,D,255,0);
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
    double dt = 0;
    double t = 0;

    if (up){
        gpioSetPWMfrequency(PIN_UPDOWN_IN1,1000);
        gpioSetPWMrange(PIN_UPDOWN_IN1,255);
        gpioPWM(PIN_UPDOWN_IN1,speed);
        gpioWrite(PIN_UPDOWN_IN2,PI_LOW);
    }else{
        gpioSetPWMfrequency(PIN_UPDOWN_IN2,1000);
        gpioSetPWMrange(PIN_UPDOWN_IN2,255);
        gpioPWM(PIN_UPDOWN_IN2,speed);
        gpioWrite(PIN_UPDOWN_IN1,PI_LOW);
    }
    
    do{
        t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count();
        if (dopid){
            dt = t - dt;
            if (up){
                gpioPWM(PIN_UPDOWN_IN1,param.exec(length - rot*RATIO,dt));
            }else{
                gpioPWM(PIN_UPDOWN_IN1,param.exec(length + rot*RATIO,dt));
            }
            dt = t;
        }
        if (abs(rot*RATIO) >= length){
            break;
        }
    } while (t <= timeout);
    

    return rot*RATIO;
    
}
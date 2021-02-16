#pragma once

#include "gstream.h"
#include "gpio.h"
#include "motors.h"
#include "log.h"

using namespace drv;

class TurtleCore{
    private:
        
    public:
        Motor right()
        TurtleCore(/* args */);
        ~TurtleCore();
};

TurtleCore::TurtleCore(/* args */){
    Gpio::startGPIO();
}

TurtleCore::~TurtleCore()
{
}

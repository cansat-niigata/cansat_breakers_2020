#pragma once

#include <math.h>

namespace drv{
    class Pid{
        private:
            double Kp = 1;
            double Ki = 0;
            double Kd = 0;
            
            double err_prev = 0;
            double err_sum = 0;
            double derr_dt = 0;
            double dt = 0;
            double val = 0;

            unsigned int e_max = 1023;
            unsigned int e_min = 0;
            unsigned int c_max = 1023;
            unsigned int c_min = 0; 
        

        public:
            Pid(void);
            Pid(double p,double i,double d,unsigned int c_max=1023,unsigned int c_min=0,unsigned int e_max=1023,unsigned int e_min=0);
            ~Pid();

            Pid& setParam(double p,double i,double d);
            Pid& setLimit(unsigned int c_max,unsigned int c_min,unsigned int e_max,unsigned int e_min);
            Pid& setControlLim(unsigned int c_max,unsigned int c_min);
            Pid& setErrorLim(unsigned int e_max,unsigned int e_min);

            Pid& update(double err,double dt);
            Pid& update(double err,double derr_dt,double dt);
            Pid& reset(void);

            double execute(void);
            double getConstrolValue(void);    
    };  
} 

























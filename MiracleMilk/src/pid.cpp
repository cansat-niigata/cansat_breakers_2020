    #include "../include/pid.h"
    using namespace drv;
    
    Pid::Pid(void){}
    Pid::Pid(double p,double i,double d,unsigned int c_max,unsigned int c_min,unsigned int e_max,unsigned int e_min){
        setParam(p,i,d);
        setLimit(c_max,c_min,e_max,e_min);
    }
    Pid::~Pid(void){}

    Pid& Pid::setParam(double p,double i,double d){
        Kp = p;
        Ki = i;
        Kd = d;
        return *this;
    }

    Pid& Pid::setLimit(unsigned int c_max,unsigned int c_min,unsigned int e_max,unsigned int e_min){
        setControlLim(c_max,c_min);
        setErrorLim(e_max,e_min);
        return *this; 
    }

    Pid& Pid::setControlLim(unsigned int c_max,unsigned int c_min){
        this->c_max = c_max;
        this->c_min = c_min;
        return *this;
    }

    Pid& Pid::setErrorLim(unsigned int e_max,unsigned int e_min){
        this->e_max = e_max;
        this->e_min = e_min;
        return *this;
    }

    Pid& Pid::reset(void){
        err_prev = 0;
        err_sum = 0;
        return *this;
    }

    Pid& Pid::update(double err,double dt){
        double de_dt = (err - err_prev)/dt;
        return update(err,de_dt,dt);
    }

    Pid& Pid::update(double err,double derr_dt,double dt){
        double tmp = err_sum + (err + err_prev)*0.5*dt;
        double tmp_abs = abs(tmp);
        if (tmp_abs > e_max){
            tmp = e_max;
        }else if (tmp_abs < e_min){
            tmp = e_min;
        }
        err_sum = tmp;
        err_prev = err;
        this->derr_dt = derr_dt; 
        this->dt = dt;
        execute();
        return *this;
    }

    double Pid::execute(void){
        val = Kp*(err_prev + Ki*err_sum + Kd* derr_dt);
        double val_abs = abs(val);
        if (val_abs > c_max){
            val = c_max;
        }else if (val_abs < c_min){
            val = c_min;
        }
        return val;
    }

    double Pid::getConstrolValue(void){
        return val;
    }

int main(void){
    Pid myPid;
    myPid.setParam(1,0,0).setLimit(255,40,179,1).update(39,0.003).getConstrolValue();
}
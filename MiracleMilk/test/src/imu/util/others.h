#ifndef OTHERS_H
#define OTHERS_H
#endif

#include <unistd.h>
#include <time.h>

void* do_nothing(void*){}

void delay_ms(unsigned long ms){
    usleep(ms*1000);
}

void get_ms(unsigned long* count){
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME,&ts);
    *count = ts.tv_nsec/1.0e6;
}

void log_i(int priority,const char* tag,const char* fmt,...){}

void log_e(int priority,const char* tag,const char* fmt,...){}

void reg_int_cb(void* int_param_s){}

int min(int a,int b){
    if (a > b){
        return a;
    }else{
        return b;
    }
}


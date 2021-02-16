#include "../include/others.h"

void* do_nothing(void* somthing){}

void delay_ms(unsigned long ms){
    usleep(ms*1000);
}

void get_ms(unsigned long* count){
    if (count == NULL){
        return;
    }
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME,&ts);
    *count = (long)(ts.tv_nsec/1000000);
}

//void log_i(int priority,const char* tag,const char* fmt,...){}

//void log_e(int priority,const char* tag,const char* fmt,...){}

void reg_int_cb(void* int_param_s){}

int min(int a,int b){
    if (a > b){
        return b;
    }else{
        return a;
    }
}
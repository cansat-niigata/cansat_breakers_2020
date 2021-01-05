#pragma once

#include <unistd.h>
#include <time.h>

void* do_nothing(void* somthing);

void delay_ms(unsigned long ms);

void get_ms(unsigned long* count);

//void log_i(int priority,const char* tag,const char* fmt,...){}

//void log_e(int priority,const char* tag,const char* fmt,...){}

void reg_int_cb(void* int_param_s);

int min(int a,int b);


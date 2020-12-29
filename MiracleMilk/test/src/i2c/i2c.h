#ifndef _I2CDEV_H_
#define _I2CDEV_H_
#endif

#define I2C_OK 0
#define I2C_ERR -1

#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

int i2c_write(unsigned char slave_addr,unsigned char reg_addr,unsigned char length,unsigned char* const data);

int8_t i2c_read(unsigned char slave_addr,unsigned char reg_addr,unsigned char length,unsigned char* data);
#ifndef I2C_H
#define I2C_H
#endif

#include "i2c.h"

uint16_t readtimeout = 0;

int i2c_write(unsigned char slave_addr,unsigned char reg_addr,unsigned char length,unsigned char* const data){
    int8_t count = 0;
    uint8_t buffer[128];
    int fd;

#ifdef DEBUG
    printf("write %#x %#x\n",slave_addr,reg_addr);
#endif

    if (length > 127){
        fprintf(stderr,"Byte write count(%d) > 127\n",length);
        return 0;
    }

    fd = open("/dev/i2c-1",O_RDWR);
    if (fd < 0){
        fprintf(stderr,"Failed to open device: %s\n",strerror(errno));
        return 0;
    } 

    if (ioctl(fd,I2C_SLAVE,slave_addr) < 0){
        fprintf(stderr, "Failed to select device: %s\n", strerror(errno));
        close(fd);
        return 0;
    }

    buffer[0] = reg_addr;
    memcpy(buffer+1,data,length);
    count = write(fd,buffer,length+1);
    if (count < 0){
        fprintf(stderr, "Failed to write device(%d): %s\n", count, strerror(errno));
        close(fd);
        return 0;
    } else if (count != length+1) {
        fprintf(stderr, "Short write to device, expected %d, got %d\n", length+1, count);
        close(fd);
        return 0;
    }
    close(fd);
    
    return 1;
}

int i2c_read(unsigned char slave_addr,unsigned char reg_addr,unsigned char length,unsigned char* data) {
    int8_t count = 0;
#ifdef DEBUG
    printf("read %#x %#x %u\n",slave_addr,reg_addr,length);
#endif
    int fd = open("/dev/i2c-1",O_RDWR);

    if (fd < 0){
        fprintf(stderr,"Failed to open device: %s\n",strerror(errno));
        return(0);
    }
    if (ioctl(fd,I2C_SLAVE,slave_addr) < 0){
        fprintf(stderr,"Failed to select device: %s\n",strerror(errno));
        close(fd);
        return(0);
    }
    if (write(fd,&reg_addr,1) != 1){
        fprintf(stderr,"Failed to write reg: %s\n",strerror(errno));
        close(fd);
        return(0);
    }
    count = read(fd,data,length);
    if (count < 0){
        fprintf(stderr,"Failed to read device(%d): %s\n",count,strerror(errno));
        close(fd);
        return(0);
    } else if (count != length){
        fprintf(stderr,"Short read  from device, expected %d, got %d\n",length,count);
        close(fd);
        return(0);
    }
    close(fd);

    //return count;
    return 1;
}
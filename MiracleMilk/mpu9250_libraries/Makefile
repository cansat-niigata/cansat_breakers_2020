#Makefile

PRG = step_plusplus
CXXFLAGS = -Wall -g -lpthread

dir_inc = ./include
dir_src = ./src
dir_obj = ./obj
dir_lib = ./lib
dir_bin = ./bin

inv_mpu_h = $(dir_inc)/inv_mpu.h
inv_mpu_c = $(dir_src)/inv_mpu.c
inv_mpu_o = $(dir_obj)/inv_mpu.o
inv_dmp_h = $(dir_inc)/inv_mpu_dmp_motion_driver.h
inv_dmp_c = $(dir_src)/inv_mpu_dmp_motion_driver.c
inv_dmp_o = $(dir_obj)/inv_mpu_dmp_motion_driver.o

i2c_h = $(dir_inc)/i2c.h
i2c_c = $(dir_src)/i2c.c
i2c_o = $(dir_obj)/i2c.o

others_h = $(dir_inc)/others.h
others_c = $(dir_src)/others.c
others_o = $(dir_obj)/others.o

vector_h = $(dir_inc)/vector.h
vector_c = $(dir_src)/vector.cpp
vector_o = $(dir_obj)/vector.o

quat_h = $(dir_inc)/quaternion.h
quat_c = $(dir_src)/quaternion.cpp
quat_o = $(dir_obj)/quaternion.o

imu_h = $(dir_inc)/imu.h
imu_c = $(dir_src)/imu.cpp
imu_o = $(dir_obj)/imu.o

#$(dir_bin)/magnet_calibrater.exec:$(dir_src)/magnet_calibrater.cpp $(dir_lib)/libMPU920.a
#	g++ $(CXXFLAGS) $(dir_src)/magnet_calibrater.cpp -I$(dir_inc) -L$(dir_lib) -lMPU920 -o $(dir_bin)/magnet_calibrater.exec 
$(dir_bin)/$(PRG).exec:$(dir_src)/$(PRG).cpp $(dir_lib)/libMPU920.a
	g++ $(CXXFLAGS) $(dir_src)/$(PRG).cpp -I$(dir_inc) -L$(dir_lib) -lMPU920 -o $(dir_bin)/$(PRG).exec 
$(dir_lib)/libMPU920.a:$(i2c_o) $(others_o) $(inv_mpu_o) $(inv_dmp_o) $(imu_o) $(vector_o) $(quat_o)
	ar rcs $(dir_lib)/libMPU920.a $(i2c_o) $(others_o) $(inv_mpu_o) $(inv_dmp_o) $(imu_o) $(vector_o) $(quat_o)
$(imu_o):$(imu_c)
	g++ $(CXXFLAGS) -c $(imu_c) -o $(imu_o)
$(vector_o):$(vector_c)
	g++ $(CXXFLAGS) -c $(vector_c) -o $(vector_o)
$(quat_o):$(quat_c)
	g++ $(CXXFLAGS) -c $(quat_c) -o $(quat_o)
$(inv_dmp_o):$(inv_dmp_c)
	g++ $(CXXFLAGS) -c $(inv_dmp_c) -o $(inv_dmp_o)
$(inv_mpu_o):$(inv_mpu_c)
	g++ $(CXXFLAGS) -c $(inv_mpu_c) -o $(inv_mpu_o)
#$(dir_lib)/libUtil.a:$(i2c_o) $(others_o)
#	ar rcs $(dir_lib)/libUtil.a $(i2c_o) $(others_o)
$(i2c_o):$(i2c_c)
	g++ $(CXXFLAGS) -c $(i2c_c) -o $(i2c_o)
$(others_o):$(others_c)
	g++ $(CXXFLAGS) -c $(others_c) -o $(others_o)

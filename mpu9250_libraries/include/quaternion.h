#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

struct Vector;

struct EulerAngle{
	float yaw;
	float pitch;
	float roll;

	EulerAngle(void);
	EulerAngle(float _yaw,float _pitch,float _roll);
	void toArray(float* ypr,bool deg = true);
	private:
		static float rad2deg(float rad);
};

struct Quaternion{
	public:
		float w;
		float x;
		float y;
		float z;

		Quaternion(void);

		Quaternion(float _w,float _x,float _y,float _z);

		Quaternion(long* wxyz);

		~Quaternion(void);

		Quaternion multiply(Quaternion another);

		Quaternion fromAnglerVelocity(float avx,float avy,float avz,float time);

		Quaternion invert(void);

		float getNorm(void);

		Quaternion normalize(void);

		Vector toVector(void);

		EulerAngle toEulerAngle(void);
		//void getEulerAngle(float* rpy);
	private:
		static float long2float(long val);
		
};

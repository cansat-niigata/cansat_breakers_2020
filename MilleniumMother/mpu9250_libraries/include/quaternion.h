#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <ostream>

struct Vector;

struct EulerAngle{
	float yaw;
	float pitch;
	float roll;

	EulerAngle(void);
	EulerAngle(float _yaw,float _pitch,float _roll);
	void toArray(float* ypr,bool deg = true)const;

	friend std::ostream& operator << (std::ostream& os,const EulerAngle& e);

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

		Quaternion multiply(const Quaternion& another) const;

		Quaternion fromAnglerVelocity(float avx,float avy,float avz,float time) const;

		Quaternion invert(void) const;

		float getNorm(void) const;

		Quaternion normalize(void);

		Vector toVector(void) const;

		EulerAngle toEulerAngle(void) const;
		//void getEulerAngle(float* rpy);

		friend std::ostream& operator << (std::ostream& os,const Quaternion& q);

	private:
		static float long2float(long val);
		
};


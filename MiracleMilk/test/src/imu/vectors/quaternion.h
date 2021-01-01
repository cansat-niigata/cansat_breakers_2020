#ifndef QUATERNION_H
#define QUATERNION_H
#endif

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

struct Vector;

struct Quaternion{
	public:
		float w;
		float x;
		float y;
		float z;

		Quaternion(void);

		Quaternion(float _w,float _x,float _y,float _z);

		Quaternion(int32_t* wxyz);

		~Quaternion(void);

		Quaternion multiply(Quaternion another);

		Quaternion fromAnglerVelocity(float avx,float avy,float avz,float time);

		Quaternion invert(void);

		float getNorm(void);

		Quaternion normalize(void);

		Vector toVector(void);
};
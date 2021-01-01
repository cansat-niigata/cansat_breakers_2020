#ifndef QUATERNION_H
#define QUATERNION_H
#endif

#include <stdlib.h>
#include <math.h>

struct Vector;

struct Quaternion{
	public:
		double w;
		double x;
		double y;
		double z;

		Quaternion(void);

		Quaternion(double _w,double _x,double _y,double _z);

		~Quaternion(void);

		Quaternion multiply(Quaternion another);

		Quaternion fromAnglerVelocity(double avx,double avy,double avz,double time);

		Quaternion invert(void);

		double getNorm(void);

		Quaternion normalize(void);

		Vector toVector(void);
};
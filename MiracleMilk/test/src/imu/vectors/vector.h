#ifndef VECTORS_H
#define VECTORS_H
#endif

#include <stdlib.h>
#include <math.h>

struct Quaternion;

struct Vector{
	public:
		double x;
		double y;
		double z;

		Vector(void);

		Vector(double _x,double _y,double _z);

		~Vector();

		Vector multiply(Vector another);

		double getNorm(void);

		Vector normalize(void);

		Quaternion toQuaternion(void);

		Vector rotate(Quaternion &Q);
		
};
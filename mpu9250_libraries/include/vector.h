#pragma once

#include <stdlib.h>
#include <math.h>

struct Quaternion;

struct Vector{
	public:
		float x;
		float y;
		float z;

		Vector(void);

		Vector(float _x,float _y,float _z);

		~Vector();

		Vector multiply(Vector another);

		float getNorm(void);

		Vector normalize(void);

		Quaternion toQuaternion(void);

		Vector rotate(Quaternion &Q);
		
};
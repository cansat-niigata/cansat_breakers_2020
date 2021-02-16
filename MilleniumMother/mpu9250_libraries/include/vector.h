#pragma once

#include <stdlib.h>
#include <math.h>
#include <ostream>

struct Quaternion;

struct Vector{
	public:
		float x;
		float y;
		float z;

		Vector(void);

		Vector(float _x,float _y,float _z);

		~Vector();

		Vector multiply(const Vector& another) const;

		float getNorm(void);

		Vector normalize(void);

		Quaternion toQuaternion(void) const;

		Vector rotate(const Quaternion &Q) const;

		template <typename Type>
		Vector operator * (const Type& something_val) const{
			return Vector(x*something_val,y*something_val,z*something_val);
		}

		Vector operator + (const Vector& another) const;
		Vector operator - (const Vector& another) const;
		Vector operator - (void) const;
		Vector& operator += (const Vector& another);
		Vector& operator -= (const Vector& another);


		friend std::ostream& operator << (std::ostream& os,const Vector& v);
};

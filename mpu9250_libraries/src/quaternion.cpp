#include "../include/quaternion.h"
#include "../include/vector.h"

Quaternion::Quaternion(void):
w(1.0),x(0.0),y(0.0),z(0.0){
}

Quaternion::Quaternion(float _w,float _x,float _y,float _z):
w(_w),x(_x),y(_y),z(_z){
}

Quaternion::Quaternion(long* wxyz){
	if (wxyz == nullptr){
		w = 1;
		x = 0;
		y = 0;
		z = 0;
	}

	w = (float)wxyz[0] / 1073741824.0f;
	x = (float)wxyz[1] / 1073741824.0f;
	y = (float)wxyz[2] / 1073741824.0f;
	z = (float)wxyz[3] / 1073741824.0f;
	
}

Quaternion::~Quaternion(void){
}

Quaternion Quaternion::multiply(Quaternion another){
	float nw = w*another.w - x*another.x - y*another.y - z*another.z;
	float nx = w*another.x + x*another.w + y*another.z - z*another.y;
	float ny = w*another.y - x*another.z + y*another.w + z*another.x;
	float nz = w*another.z + x*another.y - y*another.x + z*another.w;
	return Quaternion(nw,nx,ny,nz);
}

Quaternion Quaternion::fromAnglerVelocity(float avx,float avy,float avz,float time){
	return this->multiply(Quaternion(0,avx,avy,avz));
}

Quaternion Quaternion::invert(void){
	return Quaternion(w,-x,-y,-z);
}

float Quaternion::getNorm(void){
	return sqrt(w*w + x*x + y*y + z*z);
}

Quaternion Quaternion::normalize(void){
	float norm = getNorm();
	return Quaternion(w/norm,x/norm,y/norm,z/norm);
}

Vector Quaternion::toVector(void){//vector型からもどすとき専用!
	return Vector(x,y,z);
}
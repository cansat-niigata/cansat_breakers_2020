#include "quaternion.h"
#include "vector.h"

Quaternion::Quaternion(void):
w(1.0),x(0.0),y(0.0),z(0.0){
}

Quaternion::Quaternion(double _w,double _x,double _y,double _z):
w(_w),x(_x),y(_y),z(_z){
}

Quaternion::~Quaternion(void){
}

Quaternion Quaternion::multiply(Quaternion another){
	double nw = w*another.w - x*another.x - y*another.y - z*another.z;
	double nx = w*another.x + x*another.w + y*another.z - z*another.y;
	double ny = w*another.y - x*another.z + y*another.w + z*another.x;
	double nz = w*another.z + x*another.y - y*another.x + z*another.w;
	return Quaternion(nw,nx,ny,nz);
}

Quaternion Quaternion::fromAnglerVelocity(double avx,double avy,double avz,double time){
	return this->multiply(Quaternion(0,avx,avy,avz));
}

Quaternion Quaternion::invert(void){
	return Quaternion(w,-x,-y,-z);
}

double Quaternion::getNorm(void){
	return sqrt(w*w + x*x + y*y + z*z);
}

Quaternion Quaternion::normalize(void){
	double norm = getNorm();
	return Quaternion(w/norm,x/norm,y/norm,z/norm);
}

Vector Quaternion::toVector(void){//vector型からもどすとき専用!
	return Vector(x,y,z);
}
#include "vector.h"
#include "quaternion.h"

Vector::Vector(void):
x(0.0),y(0.0),z(0.0){
}

Vector::Vector(double _x,double _y,double _z):
x(_x),y(_y),z(_z){
}

Vector::~Vector(){
}

Vector Vector::multiply(Vector another){
	double nx = y*another.z - z*another.y;
	double ny = z*another.x - x*another.z;
	double nz = x*another.y - y*another.x;
	return Vector(nx,ny,nz);
}

double Vector::getNorm(void){
	return sqrt(x*x + y*y + z*z);
}

Vector Vector::normalize(void){
	double norm = getNorm();
	return Vector(x/norm,y/norm,z/norm);
}

Quaternion Vector::toQuaternion(void){
	return Quaternion(0,x,y,z);
}

Vector Vector::rotate(Quaternion &Q){
	return Q.multiply(this->toQuaternion()).multiply(Q.invert()).toVector();
}
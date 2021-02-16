#include "../include/vector.h"
#include "../include/quaternion.h"

Vector::Vector(void):
x(0.0),y(0.0),z(0.0){
}

Vector::Vector(float _x,float _y,float _z):
x(_x),y(_y),z(_z){
}

Vector::~Vector(){
}

Vector Vector::multiply(const Vector& another) const{
	float nx = y*another.z - z*another.y;
	float ny = z*another.x - x*another.z;
	float nz = x*another.y - y*another.x;
	return Vector(nx,ny,nz);
}

float Vector::getNorm(void){
	return sqrt(x*x + y*y + z*z);
}

Vector Vector::normalize(void){
	float norm = getNorm();
	return Vector(x/norm,y/norm,z/norm);
}

Quaternion Vector::toQuaternion(void) const{
	return Quaternion(0,x,y,z);
}

Vector Vector::rotate(const Quaternion& Q) const{
	return Q.multiply(this->toQuaternion()).multiply(Q.invert()).toVector();
}

template <typename Type>
Vector Vector::operator * (const Type& something_val) const{
	return Vector(x*something_val,y*something_val,z*something_val);
}

Vector Vector::operator + (const Vector& another) const{
	return Vector(x+another.x,y+another.y,z+another.z);
}

Vector Vector::operator - (const Vector& another) const{
	return Vector(x-another.x,y-another.y,z-another.z);
}

Vector Vector::operator - (void) const{
	return Vector(-x,-y,-z);
}

Vector& Vector::operator += (const Vector& another){
	x += another.x;
	y += another.y;
	z += another.z;
	return *this;
}

Vector& Vector::operator-= (const Vector& another){
	x -= another.x;
	y -= another.y;
	z -= another.z;
	return *this;
}

std::ostream& operator << (std::ostream& os,const Vector& v){
	os << ',' << v.x << ',' << v.y << ',' << v.z;
	return os;
}
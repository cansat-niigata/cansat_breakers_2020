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

	w = long2float(wxyz[0]);
	x = long2float(wxyz[1]);
	y = long2float(wxyz[2]);
	z = long2float(wxyz[3]);
	
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

EulerAngle Quaternion::toEulerAngle(void){
	float yaw = atan2(2*(x*y - w*z),2*(w*w + x*x) - 1);
	float pitch = -asin(2*(x*z + w*y));
	float roll = atan2(2*(y*z - w*x),2*(w*w + z*z) -1);
	return EulerAngle(yaw,pitch,roll);
}

float Quaternion::long2float(long val){
	unsigned long mask = 0;
	unsigned char q = 30;
	for (unsigned int i = 0; i < q; i++){
		mask |= (1 << i);
	}
	return (val >> q) + ((val&mask))/(float)(2 << (q - 1));
}

EulerAngle::EulerAngle(void):
yaw(0),pitch(0),roll(0){	
}

EulerAngle::EulerAngle(float _yaw,float _pitch,float _roll):
yaw(_yaw),pitch(_pitch),roll(_roll){
}

void EulerAngle::toArray(float* ypr,bool deg){
	if (deg == true){
		ypr[0] = rad2deg(yaw);
		ypr[1] = rad2deg(pitch);
		ypr[2] = rad2deg(roll);
	}else{
		ypr[0] = yaw;
		ypr[1] = pitch;
		ypr[2] = roll;
	}	
}

float EulerAngle::rad2deg(float rad){
	float ret = (float)(rad*(180.0/M_PI));
	if (ret > 180){
		ret -= 360;
	}else if (ret <= -180){
		ret += 360;
	}
	return ret;
}
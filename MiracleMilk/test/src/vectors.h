#ifndef VECTORS_H
#define VECTORS_H
#endif

#include <stdlib.h>
#include <math.h>


typedef struct Quaternion{
	public:
		double w;
		double x;
		double y;
		double z;

		Quaternion(void):
		w(1.0),x(0.0),y(0.0),z(0.0){
		}

		/*Quaternion(int32_t* data_array){
			w = (double)data_array[0] / 16384.0;
			x = (double)data_array[1] / 16384.0;
			y = (double)data_array[2] / 16384.0;
			z = (double)data_array[3] / 16384.0;
		}*/

		Quaternion(double _w,double _x,double _y,double _z):
		w(_w),x(_x),y(_y),z(_z){
		}

		~Quaternion(void){
		}

		Quaternion multiply(Quaternion another){
			double nw = w*another.w - x*another.x - y*another.y - z*another.z;
			double nx = w*another.x + x*another.w + y*another.z - z*another.y;
			double ny = w*another.y - x*another.z + y*another.w + z*another.x;
			double nz = w*another.z + x*another.y - y*another.x + z*another.w;
			return Quaternion(nw,nx,ny,nz);
		}

		Quaternion invert(void){
			return Quaternion(w,-x,-y,-z);
		}

		double getNorm(void){
			return sqrt(w*w + x*x + y*y + z*z);
		}

		Quaternion normalize(void){
			double norm = getNorm();
			return Quaternion(w/norm,x/norm,y/norm,z/norm);
		}

		Vector toVector(void){//vector型からもどすとき専用!
			return Vector(x,y,z);
		}

} Quaternion,*pQuaternion;

typedef struct Vector{
	public:
		double x;
		double y;
		double z;

		Vector(void){
			x = 0.0;
			y = 0.0;
			z = 0.0;
		}

		Vector(double _x,double _y,double _z){
			x = _x;
			y = _y;
			z = _z;
		}

		~Vector(){
		}

		Vector multiply(Vector another){
			double nx = y*another.z - z*another.y;
			double ny = z*another.x - x*another.z;
			double nz = x*another.y - y*another.x;
			return Vector(nx,ny,nz);
		}

		double getNorm(void){
			return sqrt(x*x + y*y + z*z);
		}

		Vector normalize(void){
			double norm = getNorm();
			return Vector(x/norm,y/norm,z/norm);
		}

		Quaternion toQuaternion(void){
			return Quaternion(0,x,y,z);
		}

		Vector rotate(Quaternion &Q){
			return Q.multiply(this->toQuaternion()).multiply(Q.invert()).toVector();
		}

} Vector,*pVector;
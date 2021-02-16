#pragma once

#include "log.h"
#include <math.h>
#include <ostream>


namespace drv{

	struct Coordinate{
		float latitude = 0;
		float longitude = 0;
		float course = 0;
		float distance = 0;
		Note info;

		Coordinate(void){}
		Coordinate(float lat,float lng){
			latitude = lat;
			longitude = lng;
			info = Note();
			info << "latitude:" << latitude << ",longitude:" << longitude;
		}

		float getCourse(Coordinate& another){
			//WGS84
			float a = 6378137.0;//#赤道半径
			float f = 1/298.257223563;//#扁平率
			//短軸半径
			float b = (1-f)*a;
			//ラジアンへ変換
		
			float rlat0 = deg2rad(another.latitude);
			float rlng0 = deg2rad(another.longitude);
			float rlat1 = deg2rad(latitude);
			float rlng1 = deg2rad(longitude);
			//更成緯度
			float U0 = atan((1-f)*tan(rlat0));
			float U1 = atan((1-f)*tan(rlat1));
			//2点間の経度差  
			float L = rlng0-rlng1;
			//補助球上の経度(初期化)
			float Lambda = L;

			float sinU0 = sin(U0);
			float sinU1 = sin(U1);
			float cosU0 = cos(U0);
			float cosU1 = cos(U1);

			float sinLmd;
			float cosLmd;
			float sins;
			float coss;
			float cos2a;
			float cos2dm;
			float sigma;
			for (int i = 0; i < 1000; i++){
				sinLmd = sin(Lambda);
				cosLmd = cos(Lambda);
		
				sins = pow(pow(cosU1*sinLmd,2)+pow(cosU0*sinU1-sinU0*cosU1*cosLmd,2),2);
				coss = sinU0*sinU1+cosU0*cosU1*cosLmd;

				sigma = atan2(sins,coss);

				float sina = cosU0*cosU1*sinLmd/sins;
				cos2a = 1-pow(sina,2);

				cos2dm = coss-(2*sinU0*sinU1/cos2a);

				float C = f*(cos2a)*(4+f*(4-3*cos2a))/16;
				float Lambda_prev = Lambda;
				float Lambda = L+(1-C)*f*sina*(sigma+C*sins*(cos2dm+C*coss*(-1+2*pow(coss,2))));
				//精度の指定
				if (abs(Lambda-Lambda_prev) <= 1e-12){
					break;
				}
			}

			float u2 = cos2a*(pow(a,2) - pow(b,2))/pow(b,2);
			float A = 1 + u2*(4096 + u2*(-768 + u2*(320 - 175*u2)))/16384;
			float B = u2*(256+u2*(-128+u2*(74-47*u2)))/1024;
	
			float deltas = B*sins*(cos2dm+B*(coss*(-1+2*pow(cos2dm,2))-B*cos2dm*(-3+4*pow(sins,2))*(-3+4*pow(cos2dm,2))/6)/4);

			float distance = b*A*(sigma - deltas);
			float course0to1 = rad2deg(atan2(cosU1*sinLmd,cosU0*sinU1-sinU0*cosU1*cosLmd));
			float course1to0 = rad2deg(atan2(cosU0*sinLmd,-sinU0*cosU1+cosU0*sinU1*cosLmd));

			course = course0to1;
			this->distance = distance;

			return course;
		}

		float getDistance(Coordinate& another){
			getCourse(another);
			return distance;
		}

		private:
			static double deg2rad(double deg){
				return M_PI*deg/180;
			}
			static double rad2deg(double rad){
				return 180*rad/M_PI;
			}

	};

}


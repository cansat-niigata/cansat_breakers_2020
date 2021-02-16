#include "include/gstream.h"
#include "include/log.h"
#include <sstream>
#include <iostream>

using namespace drv;

int main(void){
    Vector accel;
    Vector velocity;
    Vector length;
    Vector tmp1;
    Vector tmp2;
    Quaternion quat;
    Note note;
    float dt;
    std::stringstream ss;
    std::string str;

    gStream stream(true,true,false,"./imulog.txt");
    
    ss << "QuatW,QuatX,QuatY,QuatZ,EulerY,EulerP,EulerR,AccX,AccY,AccZ,VelcX,VelocY,VelocZ,LengX,LengY,LengZ" << std::endl;
    while (true){
        dt = note.getDurationfrom()*0.5;
        quat = stream.getQuaternion();
        tmp1 = accel;
        tmp2 = velocity;
        accel = stream.getAccel().rotate(quat);
        velocity += (tmp1 + accel)*dt;
        length += (tmp2 + velocity)*dt;
        ss << quat << quat.toEulerAngle() << accel << velocity << length << std::flush;
        note << ss.str();
        ss.str("");
        stream << note << gendl << COUT;
    }
}
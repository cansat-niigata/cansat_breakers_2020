#include "../include/log.h"
#include "../include/imu.h"
#include <fstream>

using namespace drv;

int main(void){
    Quaternion quat;
    float ypr[3];
    Vector as;
    Vector aw;
    Log start;
    Log now;
    float dt;
    std::ofstream f("./fall.csv");//毎回上書きされるので注意
    imu9250 imu;

    imu.setCommrate(100).start();//更新レートを100Hzへ、DMPモードを起動
    imu.run();//通信用プロセスを起動
    start = Log();//開始時刻を記録
    f << start.toUnixTime() << std::endl;
    f << "time(milliseconds),QuatW,QuatX,QuatY,QuatZ,EulerY,EulerP,EulerR,AccX(Sensor),AccY(Sensor),AccZ(Sensor),AccX(World),AccY(World),AccZ(World)" << std::endl;
    while (true){
        now = Log();//現在時刻を記録
        dt = now.getDurationfrom(start,cast_milli);//開始時刻との差分を算出
        std::cout << dt << std::endl; 
        if (imu.isUpdated()){//ここ信頼性が若干怪しい
            quat = imu.getQuaternion();//姿勢クォータニオンを取得
            quat.toEulerAngle().toArray(ypr);//オイラー角へ変換
            as = imu.getAccel();//加速度データ取得
            aw = as.rotate(quat);//加速度をワールド座標系へ変換
            f << dt << ',' << quat << ',' << ypr[0] << ',' << ypr[1] << ',' << ypr[2] << ',' << as << ',' << aw << std::endl;
        }
        if (dt > 60*1000){//60秒以上であれば終了
            f.close();
            std::cout << "end." << std::endl;
            return 0;
        }
    }
}

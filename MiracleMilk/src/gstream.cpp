#include "../include/gstream.h"

using namespace drv;

bool gStream::waitRTS(void){
	if (RTS == NULL){
		return true;
	}
	while(true){
		if (Gpio::readPin(RTS) != true){
			break;
		}
		usleep(500*1000);
	}
	return true;
}

int gStream::readSerial(void){
	char c;
	bool avail = true;
	gss_rx.str("");
	while (avail){
		c = Gpio::readSerial_1byte(handle);
		if (c >= 0){
			gss_rx << c;
		}else{
			gss_rx << std::flush;
			avail = false;
		}
	}
	return getSerReaded();
}

int gStream::getSerReaded(void){
	gss_rx.seekg(0, std::ios::end);
	int size = (int)gss_rx.tellg();
	gss_rx.seekg(0, std::ios::beg);
	return size;
}

gStream::gStream(void){
	gStream(true,true,true,def_logf);
}

gStream::gStream(const char* logfile){
	gStream(true,true,true,logfile);
}

gStream::gStream(bool initGPIO,bool useIMU,bool useGPS,const char* logfile){
	if (initGPIO){
		Gpio::startGPIO();
		gpioinit = true;
	}
	if (useIMU){
		imu.run();
		readyimu = true;
	}
	if (useGPS){
		readygps = true;
	}
	setRTS();
	gss_tx.str("");
	gss_rx.str("");
	logfs.open(logfile);
}

gStream::~gStream(void){
	if (gpioinit){
		Gpio::terminate();
	}
	if (readyimu){
		imu.terminate();
	}
}

gStream& gStream::setRTS(void){
	return setRTS(def_RTS);
}

gStream& gStream::setRTS(unsigned int pin){
	if (pin != RTS){
		RTS = pin;
	}
	Gpio::setPinIn(pin);
	Gpio::setPinPullDown(pin);
		return *this;
}

int gStream::openSerial(const char* devname,unsigned int baudrate){
	handle = Gpio::openSerial(devname,baudrate); 
	if (handle < 0){
		std::cerr << "Failed to open Serial device." << std::endl;
		return -1;
	}
	return handle; 
}

int gStream::openSerial(unsigned int baudrate){
	return openSerial(def_sdev,baudrate);
}

int gStream::closeSerial(void){
	return Gpio::closeSerial(handle);
}

template <typename Type>
gStream& gStream::operator << (const Type& something){
	gss_tx << something;
return *this;
}

gStream& gStream::operator << (const _gendl& endl){
	gss_tx << std::endl;
	return *this;
}

gStream& gStream::operator << (const _gflush& flush){
	gss_tx << std::flush;
	return *this;
}

gStream& gStream::operator << (const Note& n){
	gss_tx << n;
	return *this;
}

gStream& gStream::operator << (const dump2& dest){
	switch (dest){
		case COUT:
			std::cout << gss_tx.str() << std::endl;
			break;
		case SOUT:
			if (handle < 0){
				*this << "Serial Port is not available." << gendl << DOUT;
				return *this;
			}
			waitRTS();
			Gpio::sendSerial(handle,gss_tx.str().c_str());
			break;
		case FOUT:
			logfs << gss_tx.str() << std::endl;
			break;
		case CSOUT:
			*this << COUT;
			*this << SOUT;
			break;
		case CFOUT:
			*this << COUT;
			*this << FOUT;
			break;
		case SFOUT:
			*this << SOUT;
			*this << FOUT;
			break;
		case CSFOUT:
			*this << COUT;
			*this << SOUT;
			*this << FOUT;
		default:
			*this << DOUT;
			break;
	}
	gss_tx.str("");
	return *this;
} 

gStream& gStream::operator >> (std::string& str){
	str = gss_rx.str();
	return *this;
}		
			
gStream& gStream::updateGps(void){
	if (!readygps){
		*this << "GPS is not ready." << DOUT;
		return *this;
	}
	while (Gpio::getDataAvailable(handle) > 0){
		ggps.encode(Gpio::readSerial_1byte(handle));
	}
	return *this;
}

Coordinate gStream::getGpsData(void){
	if (!readygps){
		*this << "GPS is not ready." << DOUT;
		Coordinate();
	}
	return Coordinate(ggps.location.lat(),ggps.location.lng());
}

Vector gStream::getAccel(void){
	if (!readyimu){
		*this << "IMU is not ready." << DOUT;
		return Vector();
	}
	return imu.getAccel();
}

Vector gStream::getGyro(void){
	if (!readyimu){
		*this << "IMU is not ready." << DOUT;
		return Vector();
	}
	return imu.getGyro();
}

Vector gStream::getCompass(void){
	if (!readyimu){
		*this << "IMU is not ready." << DOUT;
		return Vector();
	}
	return imu.getCompass();
}

Quaternion gStream::getQuaternion(void){
	if (!readyimu){
		*this << "IMU is not ready." << DOUT;
		return Quaternion();
	}
	return imu.getQuaternion();
}

float gStream::getDirection(void){
	if (!readyimu){
		*this << "IMU is not ready." << DOUT;
		return 0;
	}
	return imu.getHeading();
}

Navigation gStream::getNavigation(void){
	Navigation navi;
	navi.attitude = imu.getQuaternion();
	navi.accel = imu.getAccel();
	navi.gyro = imu.getGyro();
	navi.compass = imu.getCompass();
	navi.direction_imu = imu.getHeading();
	navi.pos_prev = navi.pos_now;
	navi.pos_now = getGpsData();
	navi.direction_gps = navi.pos_now.getCourse(navi.pos_prev);
	navi.distance_gps = navi.pos_now.distance;
	return navi;
}

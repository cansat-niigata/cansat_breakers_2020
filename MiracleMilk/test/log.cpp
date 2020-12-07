#include "log.h"
using namespace DRV;
/*namespace DRV{
	class Log{
		private:
			std::chrono::system_clock::time_point log_chrono;
			std::string note;
		public:
			Log(void);
			Log(const std::string &note);
			~Log();

			double getDurationfrom(Log anotherLog);
			static double getDurationbetween(Log Log1,Log Log2);
	};
}*/

Log::Log(void):log_chrono(std::chrono::system_clock::now()){
	note = std::string("null.");
}

Log::Log(const std::string &note,bool noteDate){
	
	if (noteDate == true){
		std::time_t now = std::chrono::system_clock::to_time_t(log_chrono);
		std::tm* date = localtime(&now);
		now = std::mktime(date);
		this->note = std::string(std::ctime(&now)) +  note;
	}else{
		this->note = note;
	}
	
}

Log::Log(const char* note,bool noteDate){
	char* _note;
	if (note == nullptr){
		_note = "null.";
	}else{
		*_note = *note;
	}

	if (noteDate == true){
		std::time_t now = std::chrono::system_clock::to_time_t(log_chrono);
		std::tm* date = localtime(&now);
		now = std::mktime(date);
		this->note = std::string(std::ctime(&now)) +  std::string(_note);
	}else{
		this->note = std::string(_note);
	}
	
	this->note = std::string(note);
}

std::time_t Log::toUnixTime(void){
	return std::chrono::system_clock::to_time_t(log_chrono);
}

double Log::getDurationfrom(Log anotherLog){
	return std::chrono::duration_cast<std::chrono::milliseconds>(this->log_chrono - anotherLog.log_chrono).count();
}

double Log::getDurationbetween(Log Log1,Log Log2){
	return std::chrono::duration_cast<std::chrono::milliseconds>(Log2.log_chrono - Log1.log_chrono).count();
}

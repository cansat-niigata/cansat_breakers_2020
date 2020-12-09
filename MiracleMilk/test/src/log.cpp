#include "log.h"
using namespace drv;
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

	class Note:public Log{
		private:
			std::chrono::system_clock::time_point log_chrono;
			std::string note;
		public:
			Note(void);
			Note(const std::string &note,bool noteDate = false);
			Note(const char* note,bool noteDate = false);
			~Note(void);

			std::string getNote(void);
	};
}*/

//class log
Log::Log(void):log_chrono(std::chrono::system_clock::now()){
}

Log::~Log(void){
}

Log Log::waitFor(double miliseconds){
	usleep(miliseconds*1000);
	return Log();
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


//class note
Note::Note(void):log_chrono(std::chrono::system_clock::now()){
	note = std::string("null.");
}

Note::Note(const std::string &note,bool noteDate):log_chrono(std::chrono::system_clock::now()){
	
	if (noteDate == true){
		char date[30];
		std::time_t now = std::chrono::system_clock::to_time_t(log_chrono);
		std::strftime(date,sizeof(date),"%Y/%m/%d %a %H:%M:%S : ",std::localtime(&now));
		this->note = std::string(date) + note;
	}else{
		this->note = note;
	}
}

Note::Note(const char* note,bool noteDate):log_chrono(std::chrono::system_clock::now()){
	const char* _note;
	if (note == nullptr){
		_note = "null.";
	}else{
		_note = note;
	}

	if (noteDate == true){
		char date[30];
		std::time_t now = std::chrono::system_clock::to_time_t(log_chrono);
		std::strftime(date,sizeof(date),"%Y/%m/%d %a %H:%M:%S : ",std::localtime(&now));
		this->note = std::string(date) + std::string(_note);
	}else{
		this->note = std::string(_note);
	}
}

Note::~Note(void){
}

Note Note::waitFor(double miliseconds){
	usleep(miliseconds*1000);
	return Note("sleeped.");
}

std::string Note::getNote(void){
	return this->note;
}
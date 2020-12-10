#ifndef LOG_H
	#define LOG_H
#endif
	
#include <chrono>
#include <string>
#include <unistd.h>

#define cast_nano 0 
#define cast_micro 1
#define cast_milli 2
#define cast_sec 3
#define cast_min 4
#define cast_hours 5

namespace drv{
	class Log{
		private:
			std::chrono::system_clock::time_point log_chrono;
		public:
			Log(void);
			~Log();

			Log waitFor(unsigned int milliseconds);

			std::time_t toUnixTime(void);
			double getDurationfrom(Log anotherLog);
			double getDurationfrom(Log anotherLog,unsigned int mode);
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

			Note waitFor(double milliseconds);
			std::string getNote(void);
	};
}
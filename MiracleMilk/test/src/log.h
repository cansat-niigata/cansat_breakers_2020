#ifndef LOG_H
	#define LOG_H
#endif
	
#include <chrono>
#include <string>
#include <unistd.h>

namespace drv{
	class Log{
		private:
			std::chrono::system_clock::time_point log_chrono;
		public:
			Log(void);
			~Log();

			Log waitFor(double miliseconds);

			std::time_t toUnixTime(void);
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

			Note waitFor(double miliseconds);
			std::string getNote(void);
	};
}
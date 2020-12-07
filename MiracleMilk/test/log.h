#ifndef LOG_H
	#define LOG_H
#endif
    
#include <chrono>
#include <string>

namespace DRV{
    class Log{
	    private:
			std::chrono::system_clock::time_point log_chrono;
			std::string note;
		public:
			Log(void);
			Log(const std::string &note,bool noteDate = false);
			Log(const char* note,bool noteDate = false);
			~Log();

			std::time_t toUnixTime(void);
            double getDurationfrom(Log anotherLog);
            static double getDurationbetween(Log Log1,Log Log2);
	};
}
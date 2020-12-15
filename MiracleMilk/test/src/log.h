#ifndef LOG_H
#define LOG_H
#endif
	
#include <chrono>
#include <string>
#include <ostream>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>

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
			Note(const std::string &note,bool noteDate = true);
			Note(const char* note,bool noteDate = true);
			Note(const int note,bool noteDate = true);
			~Note(void);

			Note waitFor(double milliseconds);
			std::string getNote(void);
			void modifyNote(const char* new_note);
			void modifyNote(std::string new_note);
	};

	class Notes{
		private:
			const char* name;
			unsigned int length = 0;
			unsigned int update = 0;
			//unsigned int autodump = 0;
			Note* notes = nullptr;
			const char* logfile;
		public:
			Notes(void);
			Notes(const std::string &logfile,const char* name=nullptr);
			Notes(const char* logfile,const char* name=nullptr);

			//void setAutoDump(unsigned int autodump);

			void setLogFile(const std::string &logfile);
			void setLogFile(const char* logfile);

			bool dumpThis(void);
			bool dumpThis(unsigned int to);
			bool dumpUpdated(void);
			void append(const Note &note);
			void append(const std::string &note,bool noteDate = true);
			void append(const char* note,bool noteDate = true);
			void append(const int note,bool noteDate = true);
			const Note& getLastNote(void);
			unsigned int isUpdated(void);
		
	};
}
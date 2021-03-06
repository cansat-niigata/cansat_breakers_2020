#include "../include/log.h"
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

Log Log::waitFor(unsigned int milliseconds){
	usleep(milliseconds*1000);
	return Log();
}

std::time_t Log::toUnixTime(void){
	return std::chrono::system_clock::to_time_t(log_chrono);
}

double Log::getDurationfrom(void){
	Log tmp = Log();
	double duration = getDurationbetween(*this,tmp);
	this->log_chrono = tmp.log_chrono;
	return duration;
}

double Log::getDurationfrom(Log anotherLog){
	return std::chrono::duration_cast<std::chrono::milliseconds>(this->log_chrono - anotherLog.log_chrono).count();
}

double Log::getDurationfrom(Log anotherLog,unsigned int mode){
	if (mode == cast_micro){
		return std::chrono::duration_cast<std::chrono::microseconds>(this->log_chrono - anotherLog.log_chrono).count();
	}else if (mode == cast_micro){
		return std::chrono::duration_cast<std::chrono::microseconds>(this->log_chrono - anotherLog.log_chrono).count();
	}else if (mode == cast_milli){
		return std::chrono::duration_cast<std::chrono::milliseconds>(this->log_chrono - anotherLog.log_chrono).count();
	}else if (mode == cast_sec){
		return std::chrono::duration_cast<std::chrono::seconds>(this->log_chrono - anotherLog.log_chrono).count();
	}else if (mode == cast_min){
		return std::chrono::duration_cast<std::chrono::minutes>(this->log_chrono - anotherLog.log_chrono).count();
	}else if (mode == cast_hours){
		return std::chrono::duration_cast<std::chrono::hours>(this->log_chrono - anotherLog.log_chrono).count();
	}else{
		return std::chrono::duration_cast<std::chrono::milliseconds>(this->log_chrono - anotherLog.log_chrono).count();
	}
}

double Log::getDurationfromStart(void){
	return std::chrono::duration_cast<std::chrono::milliseconds>(this->log_chrono.time_since_epoch()).count();
}

double Log::getDurationbetween(Log Log1,Log Log2){
	return std::chrono::duration_cast<std::chrono::milliseconds>(Log2.log_chrono - Log1.log_chrono).count();
}

double Log::getMillis(void){
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}


//class note
Note::Note(void):log_chrono(std::chrono::system_clock::now()){
	note = std::string("");
}

Note::Note(const std::string &note):log_chrono(std::chrono::system_clock::now()){
	this->note = note;
}

Note::Note(const char* note):log_chrono(std::chrono::system_clock::now()){
	const char* _note;
	if (note == nullptr){
		_note = "";
	}else{
		_note = note;
	}
	this->note = std::string(_note);
}

Note& Note::operator << (const std::string& something){
	this->modifyNote(something);
	return *this;
}

Note& Note::operator << (const char* something){
	this->modifyNote(something);
	return *this;
}

Note Note::waitFor(double milliseconds){
	usleep(milliseconds*1000);
	return Note("sleeped.");
}

std::string Note::getNote(void) const{
	char date[32];
	std::time_t now = std::chrono::system_clock::to_time_t(this->log_chrono);
	std::strftime(date,sizeof(date),"%Y/%m/%d %a %H:%M:%S,",std::localtime(&now));
	return std::string(date) += this->note;
}

Note& Note::modifyNote(const char* new_note){
	if (new_note!=nullptr){
		note = std::string(new_note);
	}
	return *this;
}

Note& Note::modifyNote(std::string new_note){
	note = new_note;
	return *this;
}

std::ostream& operator << (std::ostream& os,const Note& n){
	os << n.getNote();
	return os;
}



/*Notes::Notes(void):logfile(nullptr){
}

Notes::Notes(const std::string &logfile,const char* name):logfile(logfile.c_str()){
	if(name != nullptr){
		this->name = name;
	}else{
		this->name = nullptr;
	}
}

Notes::Notes(const char* logfile,const char* name):name(name),logfile(logfile){
	if(name != nullptr){
		this->name = name;
	}else{
		this->name = nullptr;
	}
}

Notes::~Notes(void){
	dumpThis();
}

void Notes::setAutoDump(unsigned int autodump){
	this->autodump = autodump;
	length = autodump;
}

void Notes::setLogFile(const std::string &logfile){
	this->logfile = logfile.c_str();
}

void Notes::setLogFile(const char* logfile){
	this->logfile = logfile;
}

void Notes::append(const Note &note){
	length++;
	update++;
	if (autodump != 0){
		if (update >= autodump){
			dumpUpdated();
		}
	}

	Note* tmp = notes;
	Note* notes_ = new Note[length];

	if (length == 1){
		notes_[0] = note;
	}else{
		for (int i=0;i < length - 1 ;i++){
			notes_[i] = this->notes[i];
		}
		notes_[length-1] = note;
	}
	this->notes = notes_;
	delete tmp;
	
}

void Notes::append(const std::string &note,bool noteDate = true){
	length++;
	update++;
	Note* notes_ = new Note[length];
	Note note(note,noteDate);
	if (length == 1){
		notes_[0] = note;
	}else{
		for (int i=0;i < length - 1 ;i++){
			notes_[i] = this->notes[i];
		}
		notes_[length-1] = note;
	}
	this->notes = notes_;
}

void Notes::append(const char* note,bool noteDate = true){
	length++;
	update++;
	Note* notes_ = new Note[length];
	Note note(note,noteDate);
	if (length == 1){
		notes_[0] = note;
	}else{
		for (int i=0;i < length - 1 ;i++){
			notes_[i] = this->notes[i];
		}
		notes_[length-1] = note;
	}
	this->notes = notes_;
}

void Notes::append(const int note,bool noteDate = true){
	length++;
	update++;
	Note* notes_ = new Note[length];
	Note note_(note,noteDate);
	if (length == 1){
		notes_[0] = note;
	}else{
		for (int i=0;i < length - 1 ;i++){
			notes_[i] = this->notes[i];
		}
		notes_[length-1] = note_;
	}
	this->notes = notes_;
}

const Note& Notes::getLastNote(void){
	return notes[length-1];
}

unsigned int Notes::isUpdated(void){
	return update;
}

bool Notes::dumpThis(void){
	std::ofstream file(logfile,std::ios::app);
	if (!file){
		return false;
	}else{
		for (int i;i==length;i++){
			file << notes[i+1].getNote() << std::endl;
		}
		return true;
	}
}

bool Notes::dumpThis(unsigned int from){
	std::ofstream file(logfile,std::ios::app);
	if (!file){
		return false;
	}else{
		for (int i=0;i==from;i++){
			file << notes[i+1].getNote() << std::endl;
		}
		return true;
	}
}

bool Notes::dumpUpdated(void){
	std::ofstream file(logfile,std::ios::app);
	if (!file){
		return false;
	}else{
		for (int i=0;i==update;i++){
			file << notes[i+1].getNote() << std::endl;
		}
		update = 0;
		return true;
	}
}*/
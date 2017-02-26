#ifndef FILE_LOG_H
#define FILE_LOG_H

#include <string>



#define LOG_LEVEL_DEBUG 0
#define LOG_LEVEL_INFO 1
#define LOG_LEVEL_WARN 2
#define LOG_LEVEL_ERROR 3

class FileLog
{
public:
	FileLog(void);
	~FileLog(void);

	void Log(std::string log, int logLevel=0, std::string logFile="apdu");

	
	void debug(std::string log);
	void info(std::string log);
	void warn(std::string log);
	void error(std::string log);
};


extern FileLog gLog;

#endif
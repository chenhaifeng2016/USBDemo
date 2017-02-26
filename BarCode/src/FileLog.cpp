#include <fstream>
#include <iostream>
#include <sstream>


#include "FileLog.h"


FileLog gLog;

FileLog::FileLog(void)
{
}


FileLog::~FileLog(void)
{
}


void FileLog::Log(std::string log, int logLevel, std::string logFile)
{
	std::fstream f;
	f.open("d:\\bar_code.log", std::ios::app);

	if (f.is_open())
	{
		

		std::stringstream ss;

		ss << "ÄÚÈÝ£º" << log << "\n";
		ss << "\n";
		
		f << ss.str();
		f.close();
	}
}


void FileLog::debug(std::string log)
{
	Log(log, LOG_LEVEL_DEBUG);
}

void FileLog::info(std::string log)
{
	Log(log, LOG_LEVEL_INFO);
}

void FileLog::warn(std::string log)
{
	Log(log, LOG_LEVEL_WARN);
}

void FileLog::error(std::string log)
{
	Log(log, LOG_LEVEL_ERROR);
}
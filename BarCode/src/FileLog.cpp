#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

#include <windows.h>

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

	SYSTEMTIME st;

	GetLocalTime(&st);
	//GetSystemTime();

	//目录不存在就创建
	CreateDirectory("d:\\barcode_log", NULL);

	std::stringstream filename;
	filename << "d:\\barcode_log\\barcode_" << st.wYear << "_" << st.wMonth << "_" << st.wDay << ".log";

	f.open(filename.str(), std::ios::app);

	

	if (f.is_open())
	{
		

		std::stringstream ss;
		ss << "时间：" << std::setfill('0') << std::setw(2) << st.wHour << ":" << std::setfill('0') << std::setw(2) << st.wMinute << ":" << std::setfill('0') << std::setw(2) << st.wSecond << "\n";

		ss << "内容：" << log << "\n";
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
#include <afx.h>
//#include <windows.h>

#include "Config.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "FileLog.h"



Config gConfig;

Config::Config()
{
	log = 0;
	timeout = 4000;
}


Config::~Config()
{
}

std::string GetExecPath()
{
	char szPath[255];
	std::string execPath;
	std::string rootPath;

	memset(szPath, 0x00, sizeof(szPath));

	GetModuleFileName(NULL, szPath, sizeof(szPath));

	std::string path = szPath;

	size_t last = path.rfind('\\');
	if (last != std::string::npos)
	{
		execPath = path.substr(0, last + 1);

		last = path.rfind('\\', last - 1);
		if (last != std::string::npos)
			rootPath = path.substr(0, last + 1);
	}

	return execPath;
	
}

void Config::ReadConfigFile()
{
	std::string path = GetExecPath();
	std::string config_txt = path + "config.ini";
	
	

	std::ifstream infile;
	infile.open(config_txt);
	if (!infile.is_open())
	{
		log = 1;
		gLog.error("读取配置文件信息失败 " + config_txt);
	}
		

	timeout = ::GetPrivateProfileInt("config", "timeout", 4000, path.c_str());
	log = GetPrivateProfileInt("config", "log", 0, path.c_str());

	
	infile.close();

	
}
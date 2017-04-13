#include "Config.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "FileLog.h"

Config gConfig;

Config::Config()
{
}


Config::~Config()
{
}

void Config::ReadConfigFile()
{
	std::ifstream infile;
	infile.open("D:\\barcode_log\\config.txt");
	if (!infile.is_open())
		gLog.error("读取配置文件D:\\barcode_log\\config.txt信息失败");

	std::string s;
	getline(infile, s);
	try
	{
		timeout = atoi(s.c_str());
	}
	catch (...)
	{
		timeout = 5000;
	}

	gLog.debug("读取配置文件信息" + s);
	infile.close();

	
}
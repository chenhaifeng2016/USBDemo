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
		gLog.error("��ȡ�����ļ�D:\\barcode_log\\config.txt��Ϣʧ��");

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

	gLog.debug("��ȡ�����ļ���Ϣ" + s);
	infile.close();

	
}
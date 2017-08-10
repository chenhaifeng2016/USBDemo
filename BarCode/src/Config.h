#pragma once

class Config
{
public:
	Config();
	~Config();

	void ReadConfigFile();

	int timeout;
	int log;
};

extern Config gConfig;
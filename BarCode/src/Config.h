#pragma once

class Config
{
public:
	Config();
	~Config();

	void ReadConfigFile();

	int timeout;
};

extern Config gConfig;
#pragma once
#ifndef INC_CONFIG_H
#define INC_CONFIG_H

#include "inih/INIReader.h"

#include <string>

class Config
{
public:
	Config(const std::string &fileName);

	double getAlpha();
	double getLambda();
	unsigned int getNumEpisodes();
	double getResolutionScale();
	unsigned int getNumTilings();
	double getShapingWeight();

	std::string getExperimentName();
	unsigned int getNumTrials();

	std::string getOutputPath();
	bool getEnableLogging();

private:
	INIReader m_reader;
};

#endif // INC_CONFIG_H

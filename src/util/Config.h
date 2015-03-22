#pragma once
#ifndef INC_CONFIG_H
#define INC_CONFIG_H

#include "inih/INIReader.h"

#include <string>
#include <vector>

/**
 * Utility class reading the configuration of the AIModule, experiment and trials from an ini file.
 */
class Config
{
public:
	Config(const std::string &fileName);

	double getAlpha();
	double getLambda();
	unsigned int getNumEpisodes();
	double getResolutionScale();
	unsigned int getNumTilings();
	std::vector<std::string> getShapingPotentials();
	std::string getLoadInitialWeights();
	bool getHumanAdvice();
	unsigned int getHumanAdviceEpisodes();
	double getHumanAdviceGamma();

	std::string getExperimentName();
	unsigned int getNumTrials();

	std::string getOutputPath();
	bool getEnableLogging();
	bool getSaveWeights();

private:
	INIReader m_reader;
};

#endif // INC_CONFIG_H

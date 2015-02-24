#include "Config.h"

Config::Config(const std::string &fileName)
	: m_reader(fileName)
{
}

double Config::getAlpha()
{
	return m_reader.GetReal("TRIAL", "alpha", 0.4);
}

double Config::getLambda()
{
	return m_reader.GetReal("TRIAL", "lambda", 0.9);
}

unsigned int Config::getNumEpisodes()
{
	return static_cast<unsigned int>(m_reader.GetInteger("TRIAL", "numEpisodes", 10));
}

double Config::getResolutionScale()
{
	return m_reader.GetReal("TRIAL", "resolutionScale", 1);
}

unsigned int Config::getNumTilings()
{
	return static_cast<unsigned int>(m_reader.GetInteger("TRIAL", "numTilings", 1));
}

double Config::getShapingWeight()
{
	return m_reader.GetReal("TRIAL", "shapingWeight", 25);
}

std::string Config::getExperimentName()
{
	return m_reader.Get("EXPERIMENT", "name", "Trials");
}

unsigned int Config::getNumTrials()
{
	return static_cast<unsigned int>(m_reader.GetInteger("EXPERIMENT", "numTrials", 1));
}

std::string Config::getOutputPath()
{
	return m_reader.Get("GENERAL", "outputPath", "C:/");
}

bool Config::getEnableLogging()
{
	return m_reader.GetBoolean("GENERAL", "enableLogging", false);
}
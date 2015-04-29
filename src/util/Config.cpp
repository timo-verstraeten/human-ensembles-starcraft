#include "Config.h"

#include <sstream>

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

std::vector<std::string> Config::getShapingPotentials()
{
	std::istringstream potentialsStream(m_reader.Get("TRIAL", "shapingPotentials", ""));
	return std::vector<std::string>(std::istream_iterator<std::string>(potentialsStream), std::istream_iterator<std::string>());
}

std::string Config::getLoadInitialWeights()
{
	return m_reader.Get("TRIAL", "loadInitialWeights", "");
}

bool Config::getHumanAdvice()
{
	return m_reader.GetBoolean("TRIAL", "humanAdvice", false);
}

unsigned int Config::getHumanAdviceEpisodes()
{
	return static_cast<unsigned int>(m_reader.GetInteger("TRIAL", "humanAdviceEpisodes", 5));
}

double Config::getHumanAdviceAlpha()
{
	return m_reader.GetReal("TRIAL", "humanAdviceAlpha", 0.5);
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

bool Config::getSaveWeights()
{
	return m_reader.GetBoolean("GENERAL", "saveWeights", false);
}

bool Config::getShowDebug() {
	return m_reader.GetBoolean("GENERAL", "showDebug", true);
}

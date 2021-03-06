#include "TrialLogger.h"

TrialLogger::TrialLogger(const std::string &fileName)
	: m_outputFile(fileName.c_str(), std::ios::out)
{
}

TrialLogger::~TrialLogger()
{
	if (m_outputFile.is_open()) {
		m_outputFile.close();
	}
}

void TrialLogger::writeHeader(unsigned int numHumans)
{
	m_outputFile << "episode" << ","
	             << "step" << ","
	             << "state.x" << ","
	             << "state.y" << ","
	             << "state.enemyDistance" << ","
	             << "state.hitPointDifference" << ","
	             << "state.enemyActive" << ","
	             << "state.enemyAngle" << ","
	             << "action" << ","
	             << "episodeReward";

	for (unsigned int i = 1; i <= numHumans; ++i) {
		m_outputFile << "," << "human" << i;
	}
	m_outputFile << std::endl;
}


void TrialLogger::writeLine(unsigned int episode, unsigned int step, const State &state, Action action, double episodeReward, const std::vector<bool*> &humanAdvice)
{
	m_outputFile << episode << ","
	             << step << ","
	             << state.x << ","
	             << state.y << ","
	             << state.enemyDistance << ","
	             << state.hitPointDifference << ","
	             << state.enemyActive << ","
	             << state.enemyAngle << ","
	             << action << ","
	             <<	episodeReward;

	for (unsigned int i = 0; i < humanAdvice.size(); ++i) {
		m_outputFile << "," << *humanAdvice[i];
	}
	m_outputFile << std::endl;
}

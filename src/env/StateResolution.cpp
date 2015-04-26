#include "StateResolution.h"

#include "State.h"

#include "../util/ErrorLogger.h"

StateResolution::StateResolution(const std::vector<double> &resolutions)
	: m_resolutions(resolutions)
{
	ErrorLogger::instance().assert(m_resolutions.size() == 6, "Number of given resolutions is not right");
}

std::vector<double> StateResolution::scaleState(const State &state)
{
	std::vector<double> scaled;
	scaled.push_back(state.x / m_resolutions[0]);
	scaled.push_back(state.y / m_resolutions[1]);
	scaled.push_back(state.enemyDistance / m_resolutions[2]);
	scaled.push_back(state.hitPointDifference / m_resolutions[3]);
	scaled.push_back(static_cast<double>(state.enemyActive) / m_resolutions[4]);
	scaled.push_back(state.enemyAngle / m_resolutions[5]);

	return scaled;
}
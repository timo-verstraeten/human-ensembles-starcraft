#pragma once
#ifndef INC_STATE_RESOLUTION_H
#define INC_STATE_RESOLUTION_H

#include <vector>

struct State;

class StateResolution
{
public:
	StateResolution(const std::vector<double> &featureResolutions);

	std::vector<double> scaleState(const State &state);

private:
	std::vector<double> m_resolutions;
};

#endif // INC_STATE_RESOLUTION_H

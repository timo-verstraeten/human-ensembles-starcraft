#pragma once
#ifndef INC_STATE_RESOLUTION_H
#define INC_STATE_RESOLUTION_H

#include <vector>

struct State;

/**
 * Class scaling and converting the State struct to a vector of doubles
 */
class StateResolution
{
public:
	/**
	 * Constructor.
	 *
	 * @param	featureResolutions	A vector containing the resolution for each state feature.
	 */
	StateResolution(const std::vector<double> &featureResolutions);

	/**
	 * Converts the state to a vector of doubles, and scales that vector with the resolutions.
	 * Each feature value is divided by its respective resolution, such that an interval of length 'resolution' is scaled to length 1.
	 */
	std::vector<double> scaleState(const State &state);

private:
	std::vector<double> m_resolutions;
};

#endif // INC_STATE_RESOLUTION_H

#include "PreferenceBasedEnsembleAgent.h"

#include "../policies/Policy.h"

#include <algorithm>
#include <cmath>
#include <numeric>
#include <sstream>

PreferenceBasedEnsembleAgent::PreferenceBasedEnsembleAgent(const std::vector<SMDPAgent*> &agents, double epsilon)
	: EnsembleAgent(agents), m_epsilon(epsilon)
{
}

PreferenceBasedEnsembleAgent::~PreferenceBasedEnsembleAgent()
{
}

Action PreferenceBasedEnsembleAgent::nextAction(const State &state, std::ostream &output)
{
	std::stringstream dummy;
	for (unsigned int i = 0; i < m_agents.size(); ++i) {
		m_agents[i]->nextAction(state, dummy); // Don't care about the chosen action, we just want to make sure the agent has the right state set
	}

	//Compute the preference values per action
	std::vector<double> preferenceValues(NUMBER_OF_ACTIONS);
	for (unsigned int a = 0; a < NUMBER_OF_ACTIONS; ++a) {
		preferenceValues[a] = preferenceValue(state, static_cast<Action>(a));
	}

	//Select action for the ensemble agent
	output << "Epsilon-Greedy policy - ";
	if (Policy::random() < m_epsilon)	{
		output << "Random" << std::endl;
		return static_cast<Action>(Policy::randomInt(NUMBER_OF_ACTIONS));
	}
	else {
		output << "Greedy policy" << std::endl;

		unsigned int ties = 0;
		unsigned int maxAction = 0;
		double maxValue = preferenceValues[maxAction];

		//TODO: output? 

		for (unsigned int i = 1; i < NUMBER_OF_ACTIONS; ++i) {
			double v = preferenceValues[i];
			//TODO: output? 
			
			if (v > maxValue) {
				ties = 0;
				maxAction = i;
				maxValue = v;
			}
			else if (v == maxValue) {
				++ties;
				if (Policy::randomInt(ties + 1)) {
					maxAction = i;
					maxValue = v;
				}
			}
		}
		return static_cast<Action>(maxAction);
	}
}
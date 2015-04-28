#include "PreferenceBasedEnsembleAgent.h"

#include <algorithm>
#include <cmath>
#include <numeric>
#include <sstream>

PreferenceBasedEnsembleAgent::PreferenceBasedEnsembleAgent(const std::vector<SMDPAgent*> &agents, double epsilon)
	: EnsembleAgent(agents), m_epsilonGreedyPolicy(epsilon)
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
	Action action = m_epsilonGreedyPolicy.selectAction(preferenceValues, output);
	
	for (unsigned int a = 0; a < NUMBER_OF_ACTIONS; ++a) {
		output << "preference[" << a << "] = " << preferenceValues[a] << std::endl;
	}

	return action;
}
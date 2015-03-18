#include "BoltzmannEnsembleAgent.h"
#include "Policy.h"
#include "QValuesAgent.h"

#include <algorithm>
#include <cmath>
#include <numeric>

BoltzmannEnsembleAgent::BoltzmannEnsembleAgent(const std::vector<QValuesAgent*> &agents, double temperature)
	: EnsembleAgent(std::vector<SMDPAgent*>(agents.begin(), agents.end())), m_temperature(temperature)
{
}

BoltzmannEnsembleAgent::~BoltzmannEnsembleAgent()
{
}

Action BoltzmannEnsembleAgent::nextAction(const State &state, std::ostream &output)
{
	//Compute the action selection probabilities per agent
	std::vector<std::vector<double>> probabilities(m_agents.size());
	for (unsigned int i = 0; i < m_agents.size(); ++i) {
		QValuesAgent* agent = static_cast<QValuesAgent*>(m_agents[i]);
		probabilities[i] = agent->policy().selectionProbabilities(agent->functionApproximator());
	}

	//Compute the preference values per action
	std::vector<double> preferenceValues(NUMBER_OF_ACTIONS);
	for (unsigned int a = 0; a < NUMBER_OF_ACTIONS; ++a) {
		std::vector<double> weights(m_agents.size());
		for (unsigned int i = 0; i < m_agents.size(); ++i) {
			weights[i] = probabilities[i][a];
		}
		preferenceValues[a] = aggregateWeights(weights);
	}

	//Compute the action probability distribution for the ensemble agent
	std::vector<double> distribution(NUMBER_OF_ACTIONS);
	for (unsigned int a = 0; a < NUMBER_OF_ACTIONS; ++a) {
		distribution[a] = pow(preferenceValues[a], 1 / m_temperature);
	}

	//Pick an action
	return static_cast<Action>(Policy::randomChoice(distribution));
}
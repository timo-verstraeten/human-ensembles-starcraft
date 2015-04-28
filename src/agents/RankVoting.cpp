#include "RankVoting.h"

#include "QValuesAgent.h"

#include "../env/FunctionApproximator.h"

#include <numeric>
#include <sstream>

RankVoting::RankVoting(const std::vector<QValuesAgent*> &agents, double epsilon)
	: PreferenceBasedEnsembleAgent(std::vector<SMDPAgent*>(agents.begin(), agents.end()), epsilon)
{
}

RankVoting::~RankVoting()
{
}

double RankVoting::preferenceValue(const State &state, Action action) const
{
	unsigned int rank = 0;

	for (unsigned int i = 0; i < m_agents.size(); ++i) {
		QValuesAgent* agent = static_cast<QValuesAgent*>(m_agents[i]);

		std::vector<double> probabilities = agent->policy().selectionProbabilities(agent->functionApproximator());
		for (unsigned int a = 0; a < NUMBER_OF_ACTIONS; ++a) {
			if (probabilities[action] >= probabilities[a]) {
				++rank;
			}
		}
	}

	return rank;
}
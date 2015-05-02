#include "MajorityVoting.h"
#include "QValuesAgent.h"

#include "../env/FunctionApproximator.h"

#include <algorithm>
#include <numeric>
#include <sstream>
#include <vector>

MajorityVoting::MajorityVoting(const std::vector<QValuesAgent*> &agents, double epsilon)
	: PreferenceBasedEnsembleAgent(std::vector<SMDPAgent*>(agents.begin(), agents.end()), epsilon)
{
}

MajorityVoting::~MajorityVoting()
{
}

double MajorityVoting::preferenceValue(const State &state, Action action) const
{
	std::stringstream dummy;
	unsigned int count = 0;

	for (unsigned int i = 0; i < m_agents.size(); ++i) {

		QValuesAgent* agent = static_cast<QValuesAgent*>(m_agents[i]);
		std::vector<double> Qvalues(NUMBER_OF_ACTIONS);

		for (unsigned int a = 0; a < NUMBER_OF_ACTIONS; ++a) {
			Qvalues[a] = agent->functionApproximator().computeQ(static_cast<Action>(a));
		}

		double maxQ = *std::max_element(Qvalues.begin(), Qvalues.end());
		double q = agent->functionApproximator().computeQ(static_cast<Action>(action));

		if (q == maxQ) {
			++count;
		}
	}

	return count;
}
#include "MajorityVoting.h"
#include "QValuesAgent.h"

#include <numeric>
#include <sstream>

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
		if (action == m_agents[i]->nextAction(state, dummy)) {
			++count;
		}
	}

	return count;
}
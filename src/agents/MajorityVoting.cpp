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
		QValuesAgent* agent = static_cast<QValuesAgent*>(m_agents[i]);

		Action maxAction = m_policy.selectQAction(agent->functionApproximator(), dummy);

		if (action == maxAction) {
			++count;
		}
	}

	return count;
}
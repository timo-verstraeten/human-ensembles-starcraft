#include "AdaptiveObjectiveSelection.h"

#include "../env/FunctionApproximator.h"
#include "QValuesAgent.h"

#include <sstream>

AdaptiveObjectiveSelection::AdaptiveObjectiveSelection(const std::vector<QValuesAgent*> &agents, double epsilon)
	: EnsembleAgent(std::vector<SMDPAgent*>(agents.begin(), agents.end())), m_epsilonGreedyPolicy(epsilon)
{
}

AdaptiveObjectiveSelection::~AdaptiveObjectiveSelection()
{
}

Action AdaptiveObjectiveSelection::nextAction(const State &state, std::ostream &output)
{
	std::stringstream dummy;
	for (unsigned int i = 0; i < m_agents.size(); ++i) {
		m_agents[i]->nextAction(state, dummy); // Don't care about the chosen action, we just want to make sure the agent has the right state set
	}

	return m_epsilonGreedyPolicy.selectAction(selectFunctionApproximator(output), output);
}

FunctionApproximator &AdaptiveObjectiveSelection::selectFunctionApproximator(std::ostream &output)
{
	if (m_agents.size() == 1) {
		return static_cast<QValuesAgent*>(m_agents[0])->functionApproximator();
	}

	output << "Adaptive objective selection" << std::endl;

	double bestConfidence = static_cast<QValuesAgent*>(m_agents[0])->functionApproximator().computeConfidence();
	int best = 0;
	int numTies = 0;
	output << "Confidence " << best << " = " << bestConfidence << std::endl;

	for (unsigned int i = 1; i < m_agents.size(); ++i) {
		double confidence = static_cast<QValuesAgent*>(m_agents[i])->functionApproximator().computeConfidence();
		output << "Confidence " << i << " = " << confidence << std::endl;

		if (confidence > bestConfidence) {
			numTies = 0;
			bestConfidence = confidence;
			best = i;
		}
		else if (confidence == bestConfidence) {
			numTies++;
			if (Policy::randomInt(numTies + 1) == 0) {
				bestConfidence = confidence;
				best = i;
			}
		}
	}

	return static_cast<QValuesAgent*>(m_agents[best])->functionApproximator();
}

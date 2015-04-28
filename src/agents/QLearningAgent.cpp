#include "QLearningAgent.h"

#include "../env/FunctionApproximator.h"

#include <cmath>

QLearningAgent::QLearningAgent(double alpha, double lambda, double gamma, Policy *policy, FunctionApproximator *functionApproximator, Potential *potential)
	: QValuesAgent(alpha, lambda, gamma, policy, functionApproximator, potential), m_lastProposedQ(0)
{
}

QLearningAgent::~QLearningAgent()
{
}

void QLearningAgent::applyAction(Action action, std::ostream &output)
{
	QValuesAgent::applyAction(action, output);
	if (m_lastQ != m_lastProposedQ) {
		m_functionApproximator->decayTraces(0);
		m_functionApproximator->updateTraces(action);
	}
}

double QLearningAgent::nextQ(Action)
{
	// The previously selected action is of no relevance for default Q-Learning, we just need the max

	double maxQ = m_functionApproximator->computeQ(static_cast<Action>(0));
	for (unsigned int i = 1; i < NUMBER_OF_ACTIONS; ++i) {
		Action a = static_cast<Action>(i);
		maxQ = std::max(maxQ, m_functionApproximator->computeQ(a));
	}
	m_lastProposedQ = maxQ;
	return maxQ;
}

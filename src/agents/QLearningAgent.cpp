#include "QLearningAgent.h"

#include "../env/FunctionApproximator.h"

#include <cmath>

QLearningAgent::QLearningAgent(double alpha, double lambda, double gamma, Policy *policy, FunctionApproximator *functionApproximator, Potential *potential)
	: QValuesAgent(alpha, lambda, gamma, policy, functionApproximator, potential)
{
}

QLearningAgent::~QLearningAgent()
{
}

double QLearningAgent::nextQ(Action selected)
{
	(void)selected; // The previously selected action is of no relevance for default Q-Learning, we just need the max
	
	double maxQ = m_functionApproximator->computeQ(static_cast<Action>(0));
	for (unsigned int i = 1; i < NUMBER_OF_ACTIONS; ++i) {
		Action a = static_cast<Action>(i);
		maxQ = std::max(maxQ, m_functionApproximator->computeQ(a));
	}
	return maxQ;
}

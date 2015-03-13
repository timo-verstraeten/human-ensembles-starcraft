#include "QLearningAgent.h"

#include "FunctionApproximator.h"

#include <cmath>

QLearningAgent::QLearningAgent(double alpha, double lambda, double gamma, ActionSelector *actionSelector, const std::vector<FunctionApproximator*> &functionApproximators, const std::vector<Potential*> &potentials)
	: QValuesAgent(alpha, lambda, gamma, actionSelector, functionApproximators, potentials)
{
}

QLearningAgent::~QLearningAgent()
{
}

double QLearningAgent::nextQ(FunctionApproximator *functionApproximator, Action selected)
{
	double maxQ = functionApproximator->computeQ(static_cast<Action>(0));
	for (unsigned int i = 1; i < NUMBER_OF_ACTIONS; ++i) {
		Action a = static_cast<Action>(i);
		maxQ = std::max(maxQ, functionApproximator->computeQ(a));
	}
	return maxQ;
}

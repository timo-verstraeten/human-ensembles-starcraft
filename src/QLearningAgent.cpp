#include "QLearningAgent.h"

#include "FunctionApproximator.h"

#include <cmath>

QLearningAgent::QLearningAgent(double alpha, double lambda, double gamma, ActionSelector *actionSelector, FunctionApproximator *functionApproximator, Potential *potential)
	: QValuesAgent(alpha, lambda, gamma, actionSelector, functionApproximator, potential)
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

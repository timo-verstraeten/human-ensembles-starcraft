#include "SarsaAgent.h"

#include "FunctionApproximator.h"

SarsaAgent::SarsaAgent(double alpha, double lambda, double gamma, ActionSelector *actionSelector, const std::vector<FunctionApproximator*> &functionApproximators, const std::vector<Potential*> &potentials)
	: QValuesAgent(alpha, lambda, gamma, actionSelector, functionApproximators, potentials)
{
}

SarsaAgent::~SarsaAgent()
{
}

double SarsaAgent::nextQ(FunctionApproximator *functionApproximator, Action selected)
{
	return functionApproximator->computeQ(selected);
}

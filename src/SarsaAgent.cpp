#include "SarsaAgent.h"

#include "FunctionApproximator.h"

SarsaAgent::SarsaAgent(double alpha, double lambda, double gamma, ActionSelector *actionSelector, FunctionApproximator *functionApproximator, Potential *potential)
	: QValuesAgent(alpha, lambda, gamma, actionSelector, functionApproximator, potential)
{
}

SarsaAgent::~SarsaAgent()
{
}

double SarsaAgent::nextQ(FunctionApproximator *functionApproximator, Action selected)
{
	return functionApproximator->computeQ(selected);
}

#include "SarsaAgent.h"

#include "../env/FunctionApproximator.h"

SarsaAgent::SarsaAgent(double alpha, double lambda, double gamma, Policy *policy, FunctionApproximator *functionApproximator, Potential *potential)
	: QValuesAgent(alpha, lambda, gamma, policy, functionApproximator, potential)
{
}

SarsaAgent::~SarsaAgent()
{
}

double SarsaAgent::nextQ(Action selected)
{
	return m_functionApproximator->computeQ(selected);
}

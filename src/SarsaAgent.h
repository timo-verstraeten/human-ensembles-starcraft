#pragma once
#ifndef INC_SARSA_AGENT_H
#define INC_SARSA_AGENT_H

#include "QValuesAgent.h"

class SarsaAgent : public QValuesAgent
{
public:
	SarsaAgent(double alpha, double lambda, double gamma, ActionSelector *actionSelector, FunctionApproximator *functionApproximator, Potential *potential);
	virtual ~SarsaAgent();

protected:
	virtual double nextQ(FunctionApproximator *functionApproximator, Action selected);
};

#endif // INC_SARSA_AGENT_H

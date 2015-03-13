#pragma once
#ifndef INC_SARSA_AGENT_H
#define INC_SARSA_AGENT_H

#include "QValuesAgent.h"

class SarsaAgent : public QValuesAgent
{
public:
	SarsaAgent(double alpha, double lambda, double gamma, ActionSelector *actionSelector, const std::vector<FunctionApproximator*> &functionApproximators, const std::vector<Potential*> &potentials);
	virtual ~SarsaAgent();

protected:
	virtual double nextQ(FunctionApproximator *functionApproximator, Action selected);
};

#endif // INC_SARSA_AGENT_H

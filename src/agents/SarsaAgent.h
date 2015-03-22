#pragma once
#ifndef INC_SARSA_AGENT_H
#define INC_SARSA_AGENT_H

#include "QValuesAgent.h"

/**
 * Agent implementing the SARSA algorithm.
 */
class SarsaAgent : public QValuesAgent
{
public:
	SarsaAgent(double alpha, double lambda, double gamma, Policy *policy, FunctionApproximator *functionApproximator, Potential *potential);
	virtual ~SarsaAgent();

private:
	// --- QValuesAgent ---
	virtual double nextQ(Action selected);
};

#endif // INC_SARSA_AGENT_H

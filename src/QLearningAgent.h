#pragma once
#ifndef INC_Q_LEARNING_AGENT_H
#define INC_Q_LEARNING_AGENT_H

#include "QValuesAgent.h"

class QLearningAgent : public QValuesAgent
{
public:
	QLearningAgent(double alpha, double lambda, double gamma, Policy *policy, FunctionApproximator *functionApproximator, Potential *potential);
	virtual ~QLearningAgent();

protected:
	virtual double nextQ(FunctionApproximator *functionApproximator, Action selected);
};

#endif // INC_Q_LEARNING_AGENT_H

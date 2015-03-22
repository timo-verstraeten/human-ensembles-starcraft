#pragma once
#ifndef INC_Q_LEARNING_AGENT_H
#define INC_Q_LEARNING_AGENT_H

#include "QValuesAgent.h"

/**
 * Agent implementing the Q-learning algorithm.
 */
class QLearningAgent : public QValuesAgent
{
public:
	QLearningAgent(double alpha, double lambda, double gamma, Policy *policy, FunctionApproximator *functionApproximator, Potential *potential);
	virtual ~QLearningAgent();

private:
	// --- QValuesAgent ---
	virtual double nextQ(Action selected);
};

#endif // INC_Q_LEARNING_AGENT_H

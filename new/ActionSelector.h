#pragma once
#ifndef INC_ACTION_SELECTOR_H
#define INC_ACTION_SELECTOR_H

#include "Action.h"

#include <ostream>
#include <vector>

class FunctionApproximator;

class ActionSelector
{
public:
	virtual ~ActionSelector() {}

	virtual Action select(const std::vector<FunctionApproximator*> &functionApproximators, std::ostream &output) = 0;

protected:
	double random();
	unsigned int randomInt(unsigned int maxRange);

	Action epsilonGreedy(FunctionApproximator &functionApproximator, double epsilon, std::ostream &output);
	Action argmaxQ(FunctionApproximator &functionApproximator, std::ostream &output);
};

#endif // INC_ACTION_SELECTOR_H

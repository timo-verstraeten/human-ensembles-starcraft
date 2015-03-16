#pragma once
#ifndef INC_POLICY_H
#define INC_POLICY_H

#include "Action.h"

#include <ostream>

class FunctionApproximator;

class Policy
{
public:
	virtual ~Policy() {}

	virtual Action selectAction(FunctionApproximator &functionApproximator, std::ostream &output) = 0;

	static double random() { return static_cast<double>(rand()) / RAND_MAX; }
	static unsigned int randomInt(unsigned int maxRange) { return static_cast<unsigned int>(random() * maxRange); }
};

#endif // INC_POLICY_H

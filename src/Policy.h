#pragma once
#ifndef INC_POLICY_H
#define INC_POLICY_H

#include "Action.h"

#include <numeric>
#include <ostream>
#include <vector>

class FunctionApproximator;

class Policy
{
public:
	virtual ~Policy() {}

	virtual Action selectAction(FunctionApproximator &functionApproximator, std::ostream &output) const = 0;
	virtual std::vector<double> selectionProbabilities(FunctionApproximator &functionApproximator) const = 0;

	static double random() { return static_cast<double>(rand()) / RAND_MAX; }
	static unsigned int randomInt(unsigned int maxRange) { return static_cast<unsigned int>(random() * maxRange); }
	static unsigned int randomChoice(const std::vector<double> &weights) {
		double total = std::accumulate(weights.begin(), weights.end(), 0.0);
		double r = total*Policy::random();
		unsigned int i = 0;
		while (weights[i] < r) {
			r -= weights[i++];
		}
		return i;
	}
};

#endif // INC_POLICY_H

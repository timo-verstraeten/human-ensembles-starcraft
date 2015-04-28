#pragma once
#ifndef INC_GREEDY_POLICY_H
#define INC_GREEDY_POLICY_H

#include "Policy.h"

/**
 * Class implementing the greedy policy
 */
class GreedyPolicy : public Policy
{
public:
	GreedyPolicy();
	virtual ~GreedyPolicy();

	// --- Policy ---
	virtual Action selectAction(const std::vector<double>& preferenceValues, std::ostream &output) const;
	virtual std::vector<double> selectionProbabilities(FunctionApproximator &functionApproximator) const;
};

#endif // INC_GREEDY_POLICY_H

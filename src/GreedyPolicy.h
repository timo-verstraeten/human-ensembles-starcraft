#pragma once
#ifndef INC_GREEDY_POLICY_H
#define INC_GREEDY_POLICY_H

#include "Policy.h"

class GreedyPolicy : public Policy
{
public:
	GreedyPolicy();
	virtual ~GreedyPolicy();

	virtual Action selectAction(FunctionApproximator &functionApproximator, std::ostream &output) const;
	virtual std::vector<double> selectionProbabilities(FunctionApproximator &functionApproximator) const;
};

#endif // INC_GREEDY_POLICY_H

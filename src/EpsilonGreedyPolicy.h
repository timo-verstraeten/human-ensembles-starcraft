#pragma once
#ifndef INC_EPSILON_GREEDY_POLICY_H
#define INC_EPSILON_GREEDY_POLICY_H

#include "Policy.h"

#include "GreedyPolicy.h"

class EpsilonGreedyPolicy : public Policy
{
public:
	EpsilonGreedyPolicy(double epsilon);
	virtual ~EpsilonGreedyPolicy();

	virtual Action selectAction(FunctionApproximator &functionApproximator, std::ostream &output) const;
	virtual std::vector<double> selectionProbabilities(FunctionApproximator &functionApproximator) const;

private:
	const double m_epsilon;
	
	GreedyPolicy m_greedyPolicy;
};

#endif // INC_EPSILON_GREEDY_POLICY_H

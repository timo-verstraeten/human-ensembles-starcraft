#pragma once
#ifndef INC_ADAPTIVE_OBJECTIVE_SELECTOR_H
#define INC_ADAPTIVE_OBJECTIVE_SELECTOR_H

#include "ActionSelector.h"

class AdaptiveObjectiveSelector : public ActionSelector
{
public:
	AdaptiveObjectiveSelector(double epsilon);
	virtual ~AdaptiveObjectiveSelector();

	virtual Action select(const std::vector<FunctionApproximator*> &functionApproximators, std::ostream &output);

private:
	FunctionApproximator &selectFunctionApproximator(const std::vector<FunctionApproximator*> &functionApproximators, std::ostream &output);

	const double m_epsilon;
};

#endif // INC_ADAPTIVE_OBJECTIVE_SELECTOR_H

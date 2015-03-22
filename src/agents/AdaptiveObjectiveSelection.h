#pragma once
#ifndef INC_ADAPTIVE_OBJECTIVE_SELECTION_H
#define INC_ADAPTIVE_OBJECTIVE_SELECTION_H

#include "EnsembleAgent.h"

#include "../policies/EpsilonGreedyPolicy.h"

class QValuesAgent;

/**
 * Ensemble agent implementing the Adaptive Objective Selection action selection.
 */
class AdaptiveObjectiveSelection : public EnsembleAgent
{
public:
	AdaptiveObjectiveSelection(const std::vector<QValuesAgent*> &agents, double epsilon);
	virtual ~AdaptiveObjectiveSelection();

	virtual Action nextAction(const State &state, std::ostream &output);

private:
	FunctionApproximator &selectFunctionApproximator(std::ostream &output);

	EpsilonGreedyPolicy m_epsilonGreedyPolicy;
};

#endif // INC_ADAPTIVE_OBJECTIVE_SELECTION_H

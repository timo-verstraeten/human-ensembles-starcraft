#pragma once
#ifndef INC_MAJORITY_VOTING_H
#define INC_MAJORITY_VOTING_H

#include "PreferenceBasedEnsembleAgent.h"

#include <vector>

class QValuesAgent;

/**
 * Ensemble agent implementing the MajorityVoting action selection.
 */
class MajorityVoting : public PreferenceBasedEnsembleAgent
{
public:
	MajorityVoting(const std::vector<QValuesAgent*> &agents, double epsilon);
	virtual ~MajorityVoting();

private:
	virtual double preferenceValue(const State &state, Action action) const;
};

#endif // INC_MAJORITY_VOTING_H
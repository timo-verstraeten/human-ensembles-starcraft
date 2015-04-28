#pragma once
#ifndef INC_RANK_VOTING_H
#define INC_RANK_VOTING_H

#include "PreferenceBasedEnsembleAgent.h"

#include <vector>

class QValuesAgent;

/**
 * Ensemble agent implementing the RankVoting action selection.
 */
class RankVoting : public PreferenceBasedEnsembleAgent
{
public:
	RankVoting(const std::vector<QValuesAgent*> &agents, double epsilon);
	virtual ~RankVoting();

private:
	virtual double preferenceValue(const State &state, Action action) const;
};

#endif // INC_RANK_VOTING_H
#pragma once
#ifndef INC_PREFERENCE_BASED_ENSEMBLE_AGENT_H
#define INC_PREFERENCE_BASED_ENSEMBLE_AGENT_H

#include "EnsembleAgent.h"

#include <vector>

/**
 * Abstract base class for ensemble agents with action selection based on preference values (MajorityVoting).
 */
class PreferenceBasedEnsembleAgent : public EnsembleAgent
{
public:
	PreferenceBasedEnsembleAgent(const std::vector<SMDPAgent*> &agents, double epsilon);
	virtual ~PreferenceBasedEnsembleAgent();

	virtual Action nextAction(const State &state, std::ostream &output);

protected:
	virtual double preferenceValue(const State &state, Action action) const = 0;

private:
	double m_epsilon;
};

#endif // INC_PREFERENCE_BASED_ENSEMBLE_AGENT_H
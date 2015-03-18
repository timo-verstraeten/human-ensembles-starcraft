#pragma once
#ifndef INC_SMDP_AGENT_H
#define INC_SMDP_AGENT_H

#include "Action.h"

#include <istream>
#include <ostream>

struct State;

class SMDPAgent
{
public:
	virtual ~SMDPAgent() {}

	virtual Action startEpisode(const State &state, std::ostream &output);
	virtual Action step(double reward, const State &state, std::ostream &output);
	virtual void endEpisode(double reward, std::ostream &output);

	virtual void saveWeights(std::ostream &output) = 0;
	virtual void loadWeights(std::istream &input) = 0;

	// These methods should only be used through the 'startEpisode', 'step' and 'endEpisode' methods from above.
	// ONLY to be directly called if you know what they do (they are public, such that an ensemble agent can call these methods for the agents in the ensemble)
	virtual void initialize(std::ostream &output) = 0;
	virtual Action nextAction(const State &state, std::ostream &output) = 0;
	virtual void applyAction(Action action, std::ostream &output) = 0;
	virtual void giveReward(double reward, std::ostream &output) = 0;
	virtual void finalize(std::ostream &output) = 0;
};

#endif // INC_SMDP_AGENT_H

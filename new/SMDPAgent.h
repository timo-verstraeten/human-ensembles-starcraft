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

	virtual Action startEpisode(const State &state, std::ostream &output) = 0;
	virtual Action step(double reward, const State &state, std::ostream &output) = 0;
	virtual void endEpisode(double reward) = 0;

	virtual void saveWeights(std::ostream &output) = 0;
	virtual void loadWeights(std::istream &input) = 0;
};

#endif // INC_SMDP_AGENT_H

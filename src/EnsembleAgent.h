#pragma once
#ifndef INC_ENSEMBLE_AGENT_H
#define INC_ENSEMBLE_AGENT_H

#include "SMDPAgent.h"

#include <vector>

class EnsembleAgent : public SMDPAgent
{
public:
	EnsembleAgent(const std::vector<SMDPAgent*> &agents);
	virtual ~EnsembleAgent();

protected:
	virtual void initialize(std::ostream &output);
	virtual void applyAction(Action action, std::ostream &output);
	virtual void giveReward(double reward, std::ostream &output);
	virtual void finalize(std::ostream &output);

	std::vector<SMDPAgent*> m_agents;
};

#endif // INC_ENSEMBLE_AGENT_H

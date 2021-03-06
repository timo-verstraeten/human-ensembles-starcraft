#pragma once
#ifndef INC_ENSEMBLE_AGENT_H
#define INC_ENSEMBLE_AGENT_H

#include "SMDPAgent.h"

#include <vector>

/**
 * Abstract base class for an agent.
 */
class EnsembleAgent : public SMDPAgent
{
public:
	/**
	 * Constructor
	 *
	 * @param	agents	The subagents assembled in this ensemble agent.
	 */
	EnsembleAgent(const std::vector<SMDPAgent*> &agents);
	virtual ~EnsembleAgent();

	// --- SMDPAgent ---
	virtual void saveWeights(std::ostream &output);
	virtual void loadWeights(std::istream &input);

	virtual void initialize(std::ostream &output);
	virtual void applyAction(Action action, std::ostream &output);
	virtual void giveReward(double reward, std::ostream &output);
	virtual void finalize(std::ostream &output);

protected:
	const std::vector<SMDPAgent*> m_agents;
};

#endif // INC_ENSEMBLE_AGENT_H

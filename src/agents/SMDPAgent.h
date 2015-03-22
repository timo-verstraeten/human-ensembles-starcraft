#pragma once
#ifndef INC_SMDP_AGENT_H
#define INC_SMDP_AGENT_H

#include "../env/Action.h"

#include <istream>
#include <ostream>

struct State;

/**
 * Interface for a learning agent.
 */
class SMDPAgent
{
public:
	virtual ~SMDPAgent() {}

	/**
	 * Starts a new episode for the agent.
	 * 
	 * @param	state	The starting state in which the agent is placed.
	 * @return	Action	The first action the agent selects.
	 */
	virtual Action startEpisode(const State &state, std::ostream &output);

	/**
	 * Notifies the agent of the last reward and its new state.
	 * 
	 * @param	reward	The reward from the last transition.
	 * @param	state	The new state the agent finds itself in.
	 * @return	Action	The action the agent selects in this state.
	 */
	virtual Action step(double reward, const State &state, std::ostream &output);

	/**
	 * Notifies an agent that the episode is finished.
	 * 
	 * @param	reward	The extra final reward.
	 */
	virtual void endEpisode(double reward, std::ostream &output);


	/**
	 * Saves the weights the agent learned to a (binary) output stream.
	 */
	virtual void saveWeights(std::ostream &output) = 0;
	/**
	 * Loads initial weigths from a (binary) input stream.
	 */
	virtual void loadWeights(std::istream &input) = 0;


	// These methods should only be used through the 'startEpisode', 'step' and 'endEpisode' methods from above.
	// ONLY to be directly called if you know what they do (they are public, such that an ensemble agent can call these methods for the agents in the ensemble)

	/// Initializes the agent's state
	virtual void initialize(std::ostream &output) = 0;
	/// Selects the next action in the current state
	virtual Action nextAction(const State &state, std::ostream &output) = 0;
	/// Notifies the agent of the actual chosen action
	virtual void applyAction(Action action, std::ostream &output) = 0;
	/// Notifies the agent of the received reward
	virtual void giveReward(double reward, std::ostream &output) = 0;
	/// Finalizes the agent's state at the end of an episode
	virtual void finalize(std::ostream &output) = 0;
};

#endif // INC_SMDP_AGENT_H

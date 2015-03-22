#pragma once
#ifndef INC_TRIAL_H
#define INC_TRIAL_H

#include "env/Action.h"
#include "env/State.h"

#include <ostream>
#include <string>
#include <vector>

/**
 * Interface for a single trial of an experiment.
 */
class Trial
{
public:
	/**
	 * Constructor
	 * 
	 * @param	number	The sequence number of the trial.
	 */
	Trial(unsigned int number) : m_number(number) {}
	virtual ~Trial() {}

	unsigned int number() const { return m_number; }

	/**
	 * Advances the trial one step.
	 * 
	 * @param	state	The current state of the environment.
	 * @return	Action	The action chosen at the current step of the trial to execute.
	 */
	virtual Action step(const State &state, std::ostream &output) = 0;

	/**
	 * Notifies the Trial that an episode of the trial has ended.
	 * 
	 * @param	state	The current state of the environment.
	 * @return	bool	Whether there is a next episode in the trial.
	 */
	virtual bool nextEpisode(const State &state, std::ostream &output) = 0;

	/**
	 * Returns the vector of human advice that can be given during the current step of the trial.
	 * 
	 * @return	vector<bool*>	The human advice, an empty vector by default, unless overridden by a subclass.
	 */
	virtual const std::vector<bool*> &humanAdvice() { static std::vector<bool*> vector; return vector; }

private:
	const unsigned int m_number;
};

#endif // INC_TRIAL_H

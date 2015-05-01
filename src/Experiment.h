#pragma once
#ifndef INC_EXPERIMENT_H
#define INC_EXPERIMENT_H

#include <ostream>

class Trial;

/**
 * Interface for a full experiment, consisting of (possibly) multiple trials.
 * Plays factory for the Trial hierarchy.
 */
class Experiment
{
public:
	virtual ~Experiment() {}
	
	/**
	 * Returns the next trial.
	 *
	 * @return	Trial*	The next trial of the experiment. A null pointer to end the experiment.
	 */
	virtual Trial *nextTrial(std::ostream &output) = 0;
};

#endif // INC_EXPERIMENT_H

#pragma once
#ifndef INC_MALS_EXPERIMENT_H
#define INC_MALS_EXPERIMENT_H

#include "Experiment.h"

#include "SeminarTrial.h"

#include <istream>
#include <vector>

class Config;

/**
 * The main experiment of the Multi-Agent Learning Seminar, combining multiple human advice potential-based reward shapings.
 */
class SeminarExperiment : public Experiment
{
public:
	/**
	 * Constructor.
	 *
	 * @param	config		The configuration of the experiment.
	 */
	SeminarExperiment(Config &config);

	// --- Experiment ---
	virtual Trial *nextTrial(std::ostream &output);

private:
	Config &m_config;
	const unsigned int m_trials;

	unsigned int m_nextTrial;
};

#endif // INC_MALS_EXPERIMENT_H

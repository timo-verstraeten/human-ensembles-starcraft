#pragma once
#ifndef INC_MALS_EXPERIMENT_H
#define INC_MALS_EXPERIMENT_H

#include "Experiment.h"

#include "SeminarTrial.h"

#include <istream>
#include <vector>

class Config;

class SeminarExperiment : public Experiment
{
public:
	SeminarExperiment(Config &config);

	virtual Trial *nextTrial();

private:
	Config &m_config;
	const unsigned int m_trials;

	unsigned int m_nextTrial;
};

#endif // INC_MALS_EXPERIMENT_H

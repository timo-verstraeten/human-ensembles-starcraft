#pragma once
#ifndef INC_MALS_EXPERIMENT_H
#define INC_MALS_EXPERIMENT_H

#include "Experiment.h"
#include "Config.h"

#include "SeminarTrial.h"

#include <istream>
#include <vector>

class SeminarExperiment : public Experiment
{
public:
	SeminarExperiment(Config &config);

	virtual Trial *nextTrial();

private:
	void loadParameters(Config &config);

	std::vector<SeminarTrial::Parameters> m_trials;
	unsigned int m_nextTrial;
};

#endif // INC_MALS_EXPERIMENT_H

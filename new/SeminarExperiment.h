#pragma once
#ifndef INC_MALS_EXPERIMENT_H
#define INC_MALS_EXPERIMENT_H

#include "Experiment.h"

#include "SeminarTrial.h"

#include <istream>
#include <vector>

class SeminarExperiment : public Experiment
{
public:
	SeminarExperiment(std::istream &description);

	virtual Trial *nextTrial();

private:
	void readDescription(std::istream &description);

	std::vector<SeminarTrial::Parameters> m_trials;
	unsigned int m_nextTrial;
};

#endif // INC_MALS_EXPERIMENT_H

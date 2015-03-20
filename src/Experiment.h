#pragma once
#ifndef INC_EXPERIMENT_H
#define INC_EXPERIMENT_H

#include "Trial.h"

#include <istream>
#include <vector>

class Config;

class Experiment
{
public:
	Experiment(Config &config);

	virtual ~Experiment() {}

	Trial *nextTrial();

private:
	Config &m_config;
	const unsigned int m_trials;

	unsigned int m_nextTrial;
};

#endif // INC_EXPERIMENT_H

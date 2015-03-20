#include "Experiment.h"

#include "util/Config.h"

Experiment::Experiment(Config &config)
	: m_config(config), m_trials(config.getNumTrials()), m_nextTrial(0)
{
}

Trial *Experiment::nextTrial()
{
	if (m_nextTrial < m_trials) {
		Trial *trial = new Trial(m_nextTrial, m_config);
		++m_nextTrial;
		return trial;
	}
	else {
		return 0;
	}
}

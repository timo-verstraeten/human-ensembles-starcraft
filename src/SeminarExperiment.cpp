#include "SeminarExperiment.h"

#include "util/Config.h"

SeminarExperiment::SeminarExperiment(Config &config)
	: m_config(config), m_trials(config.getNumTrials()), m_nextTrial(0)
{
}

Trial *SeminarExperiment::nextTrial()
{
	if (m_nextTrial < m_trials) {
		Trial *trial = new SeminarTrial(m_nextTrial, m_config);
		++m_nextTrial;
		return trial;
	}
	else {
		return 0;
	}
}

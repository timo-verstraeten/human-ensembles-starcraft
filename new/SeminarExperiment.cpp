#include "SeminarExperiment.h"

#include <sstream>

SeminarExperiment::SeminarExperiment(Config &config)
	: m_nextTrial(0)
{
	loadParameters(config);
}

Trial *SeminarExperiment::nextTrial()
{
	if (m_nextTrial < m_trials.size()) {
		Trial *trial = new SeminarTrial(m_nextTrial, m_trials[m_nextTrial]);
		++m_nextTrial;
		return trial;
	}
	else {
		return 0;
	}
}

void SeminarExperiment::loadParameters(Config &config)
{
	int numTrials = config.getNumTrials();
	for (int i=0; i < numTrials; i++) {
		SeminarTrial::Parameters parameters;

		parameters.alpha = config.getAlpha();
		parameters.lambda = config.getLambda();
		parameters.episodes = config.getNumEpisodes();

		m_trials.push_back(parameters);
	}
}

#include "SeminarExperiment.h"

#include <sstream>

SeminarExperiment::SeminarExperiment(std::istream &description)
	: m_nextTrial(0)
{
	readDescription(description);
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

void SeminarExperiment::readDescription(std::istream &description)
{
	std::string line;
	while (std::getline(description, line)) {
		std::stringstream lineStream(line);
		SeminarTrial::Parameters parameters;
		char c;

		if (!(lineStream >> parameters.alpha))
			continue;

		if (!(lineStream >> c) || c != ',')
			continue;

		if (!(lineStream >> parameters.lambda))
			continue;

		if (!(lineStream >> c) || c != ',')
			continue;

		if (!(lineStream >> parameters.episodes))
			continue;

		m_trials.push_back(parameters);
	}
}

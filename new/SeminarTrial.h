#pragma once
#ifndef INC_MALS_TRIAL_H
#define INC_MALS_TRIAL_H

#include "Trial.h"

class SeminarTrial : public Trial
{
public:
	struct Parameters
	{
		double alpha;
		double lambda;
		int episodes;
	};

	SeminarTrial(unsigned int number, Parameters parameters);
	virtual ~SeminarTrial();

	virtual Action step(const State &state, std::ostream &output);
	virtual bool nextEpisode(const State &state, std::ostream &output);

private:
	struct EpisodeOutput
	{
		double reward;
		unsigned int steps;
	};

	void writeOutput();

	static const double STEP_REWARD;

	const Parameters m_parameters;

	unsigned int m_episode;
	double m_episodeReward;
	unsigned int m_step;

	std::vector<EpisodeOutput> m_output;
};

#endif // INC_MALS_TRIAL_H

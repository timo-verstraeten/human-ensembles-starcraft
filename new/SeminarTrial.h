#pragma once
#ifndef INC_MALS_TRIAL_H
#define INC_MALS_TRIAL_H

#include "Trial.h"

class SMDPAgent;
class FunctionApproximator;

class SeminarTrial : public Trial
{
public:
	struct Parameters
	{
		double alpha;
		double lambda;
		unsigned int episodes;
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

	static const double GAMMA;
	static const double EPSILON;

	static const unsigned int CUTOFF_EPISODE_LIMIT;

	static const double DISTANCE_RESOLUTION;
	static const double HEALTH_RESOLUTION;
	static const double ANGLE_RESOLUTION;
	static const std::vector<double> RESOLUTIONS;
	static const unsigned int TILINGS_PER_GROUP;

	static std::vector<double> makeResolutionsVector();

	const Parameters m_parameters;

	SMDPAgent *m_agent;

	unsigned int m_episode;
	double m_episodeReward;
	unsigned int m_step;

	unsigned int m_killed;
	unsigned int m_died;

	std::vector<EpisodeOutput> m_output;
};

#endif // INC_MALS_TRIAL_H

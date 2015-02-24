#pragma once
#ifndef INC_MALS_TRIAL_H
#define INC_MALS_TRIAL_H

#include "Trial.h"

class Config;
class FunctionApproximator;
class SMDPAgent;

class SeminarTrial : public Trial
{
public:
	SeminarTrial(unsigned int number, Config &config);
	virtual ~SeminarTrial();

	virtual Action step(const State &state, std::ostream &output);
	virtual bool nextEpisode(const State &state, std::ostream &output);

private:
	struct EpisodeOutput
	{
		double reward;
		unsigned int steps;
	};

	std::vector<double> makeResolutionsVector(double scale);
	void readWeights(const std::string &fileName);
	void writeOutput();
	void writeWeights();

	const double m_alpha;
	const double m_lambda;
	const unsigned int m_episodes;
	const std::string m_outputPath;
	const bool m_saveWeights;

	SMDPAgent *m_agent;

	unsigned int m_episode;
	double m_episodeReward;
	unsigned int m_step;

	unsigned int m_killed;
	unsigned int m_died;

	std::vector<EpisodeOutput> m_output;


	static const double STEP_REWARD;

	static const double GAMMA;
	static const double EPSILON;

	static const unsigned int CUTOFF_EPISODE_LIMIT;

	static const double DISTANCE_RESOLUTION;
	static const double HEALTH_RESOLUTION;
	static const double ANGLE_RESOLUTION;
	static const std::vector<double> RESOLUTIONS;
	static const unsigned int TILINGS_PER_GROUP;
};

#endif // INC_MALS_TRIAL_H

#pragma once
#ifndef INC_TRIAL_H
#define INC_TRIAL_H

#include "env/Action.h"
#include "env/State.h"

class Config;
class FunctionApproximator;
class HumanAdvicePotential;
class Potential;
class SMDPAgent;

#include <ostream>
#include <string>
#include <vector>

class Trial
{
public:
	Trial(unsigned int number, Config &config);

	virtual ~Trial();

	unsigned int number() const { return m_number; }

	Action step(const State &state, std::ostream &output);
	bool nextEpisode(const State &state, std::ostream &output);

	const std::vector<bool*> &humanAdvice();


private:
	const unsigned int m_number;

	struct Parameters
	{
	public:
		Parameters(Config &config);

		double alpha;
		double lambda;
		unsigned int episodes;
		std::string outputPath;
		bool saveWeights;
		unsigned int humanAdviceEpisodes;
	};

	struct EpisodeOutput
	{
		double reward;
		unsigned int steps;
	};

	Potential *createPotential(const std::string &description, Config &config);
	std::vector<double> makeResolutionsVector(double scale);
	void readWeights(const std::string &fileName);
	void writeOutput();
	void writeWeights();

	const Parameters m_parameters;

	std::vector<bool*> m_humanAdvice;
	std::vector<HumanAdvicePotential*> m_humanAdvicePotentials;
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

#endif // INC_TRIAL_H

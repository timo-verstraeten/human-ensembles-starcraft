#include "SeminarTrial.h"

#include "CMAC.h"
#include "SarsaAgent.h"

#include <fstream>
#include <sstream>

std::vector<double> SeminarTrial::makeResolutionsVector()
{
	std::vector<double> resolutions;
	resolutions.push_back(SeminarTrial::DISTANCE_RESOLUTION);
	resolutions.push_back(SeminarTrial::DISTANCE_RESOLUTION);
	resolutions.push_back(SeminarTrial::DISTANCE_RESOLUTION);
	resolutions.push_back(SeminarTrial::HEALTH_RESOLUTION);
	resolutions.push_back(0.9);
	resolutions.push_back(SeminarTrial::ANGLE_RESOLUTION);
	return resolutions;
}

const double SeminarTrial::STEP_REWARD = -0.3;

const double SeminarTrial::GAMMA = 0.1;
const double SeminarTrial::EPSILON = 0.1;

const unsigned int SeminarTrial::CUTOFF_EPISODE_LIMIT = 1000;

const double SeminarTrial::DISTANCE_RESOLUTION = 30;
const double SeminarTrial::HEALTH_RESOLUTION = 10;
const double SeminarTrial::ANGLE_RESOLUTION = 0.7;
const std::vector<double> SeminarTrial::RESOLUTIONS = makeResolutionsVector();
const unsigned int SeminarTrial::TILINGS_PER_GROUP = 1;

SeminarTrial::SeminarTrial(unsigned int number, Parameters parameters)
	: Trial(number), m_parameters(parameters), m_episode(0), m_episodeReward(0.0), m_step(0), m_killed(0), m_died(0)
{
	m_functionApproximator = new CMAC(StateResolution(RESOLUTIONS), TILINGS_PER_GROUP);
	m_agent = new SarsaAgent(m_parameters.alpha, m_parameters.lambda, GAMMA, EPSILON, m_functionApproximator);
}

SeminarTrial::~SeminarTrial()
{
	writeOutput();
}

Action SeminarTrial::step(const State &state, std::ostream &output)
{

	Action action;
	if (m_step > CUTOFF_EPISODE_LIMIT) {
		output << "Cutoff episode: Trying to die" << std::endl;
		action = STOP;
	}
	else if (m_step == 0) {
		action = m_agent->startEpisode(state, output);
	}
	else {
		action = m_agent->step(STEP_REWARD, state, output);
		m_episodeReward += STEP_REWARD;
	}

	output << "Action: " << action << std::endl;
	output << "MAL SEMINAR 2014-2015" << std::endl;
	output << "Alpha: " << m_parameters.alpha << ", lambda: " << m_parameters.lambda << std::endl;
	output << "Trial: " << number() << ", episode: " << m_episode << ", step: " << m_step << ", reward: " << m_episodeReward << std::endl;
	
	++m_step;

	return action;
}

bool SeminarTrial::nextEpisode(const State &state, std::ostream &output)
{
	if (state.hitPointDifference > 0) {
		++m_killed;
	}
	else {
		++m_died;
	}

	m_agent->endEpisode(state.hitPointDifference);

	m_episodeReward += state.hitPointDifference;
	output << "Trial: " << number() << ", episode: " << m_episode << ", steps: " << m_step << ", reward: " << m_episodeReward << " (" << m_killed << " to " << m_died << ")" << std::endl;
	
	EpisodeOutput episodeOutput;
	episodeOutput.reward = m_episodeReward;
	episodeOutput.steps = m_step;
	m_output.push_back(episodeOutput);

	m_step = 0;
	m_episodeReward = 0;
	return ++m_episode < m_parameters.episodes;
}

void SeminarTrial::writeOutput()
{
	std::stringstream ss;
	ss << "trial" << number() << "_out.txt";

	std::ofstream file(ss.str().c_str());
	for (unsigned int i = 0; i < m_output.size(); ++i) {
		file << i << ',' << m_output[i].reward << ',' << m_output[i].steps << std::endl;
	}
}

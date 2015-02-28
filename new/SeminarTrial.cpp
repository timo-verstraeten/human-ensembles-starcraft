#include "SeminarTrial.h"

#include "AdaptiveObjectiveSelector.h"
#include "CMAC.h"
#include "Config.h"
#include "HumanAdvice.h"
#include "HumanAdvicePotential.h"
#include "Potentials.h"
#include "SarsaAgent.h"

#include <fstream>
#include <sstream>

const double SeminarTrial::STEP_REWARD = -0.3;

const double SeminarTrial::GAMMA = 1.0;
const double SeminarTrial::EPSILON = 0.1;

const unsigned int SeminarTrial::CUTOFF_EPISODE_LIMIT = 1000;

const double SeminarTrial::DISTANCE_RESOLUTION = 30;
const double SeminarTrial::HEALTH_RESOLUTION = 10;
const double SeminarTrial::ANGLE_RESOLUTION = 0.7;

SeminarTrial::SeminarTrial(unsigned int number, Config &config)
	: Trial(number), m_parameters(config), m_humanAdvice(0), m_humanAdvicePotential(0), m_agent(0), m_episode(0), m_episodeReward(0.0), m_step(0), m_killed(0), m_died(0)
{
	if (config.getHumanAdvice()) {
		m_humanAdvice = new HumanAdvice(1);
		FunctionApproximator *functionApproximator = new CMAC(StateResolution(makeResolutionsVector(config.getResolutionScale())), config.getNumTilings());
		m_humanAdvicePotential = new HumanAdvicePotential(config.getShapingWeight(), *m_humanAdvice, 0, functionApproximator, m_parameters.alpha, m_parameters.lambda, config.getHumanAdviceGamma());
	}

	ActionSelector *actionSelector = new AdaptiveObjectiveSelector(EPSILON);
	FunctionApproximator *functionApproximator = new CMAC(StateResolution(makeResolutionsVector(config.getResolutionScale())), config.getNumTilings());
	m_agent = new SarsaAgent(m_parameters.alpha, m_parameters.lambda, GAMMA, actionSelector, functionApproximator, m_humanAdvicePotential);

	std::string initialWeights = config.getLoadInitialWeights();
	if (initialWeights != "") {
		readWeights(initialWeights);
	}
}

SeminarTrial::~SeminarTrial()
{
	writeOutput();
	if (m_parameters.saveWeights) {
		writeWeights();
	}
	delete m_agent;
	delete m_humanAdvice;
	// m_humanAdvicePotential is owned by m_agent, like any other potential, and should not be deleted here
}

Action SeminarTrial::step(const State &state, std::ostream &output)
{
	Action action;
	if (m_step > CUTOFF_EPISODE_LIMIT) {
		output << "Cutoff episode: Trying to die" << std::endl;
		action = MOVE_TOWARDS_ENEMY;
	}
	else if (m_step == 0) {
		action = m_agent->startEpisode(state, output);
		if (m_humanAdvice) {
			m_humanAdvice->reset();
		}
	}
	else {
		action = m_agent->step(STEP_REWARD, state, output);
		if (m_humanAdvicePotential && m_episode < m_parameters.humanAdviceEpisodes) {
			m_humanAdvicePotential->step(state, action);
			m_humanAdvice->reset();
		}
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

HumanAdvice *SeminarTrial::humanAdvice()
{
	return m_humanAdvice;
}

std::vector<double> SeminarTrial::makeResolutionsVector(double scale)
{
	std::vector<double> resolutions;
	resolutions.push_back(scale * SeminarTrial::DISTANCE_RESOLUTION);
	resolutions.push_back(scale * SeminarTrial::DISTANCE_RESOLUTION);
	resolutions.push_back(scale * SeminarTrial::DISTANCE_RESOLUTION);
	resolutions.push_back(scale * SeminarTrial::HEALTH_RESOLUTION);
	resolutions.push_back(0.9);
	resolutions.push_back(scale * SeminarTrial::ANGLE_RESOLUTION);
	return resolutions;
}

void SeminarTrial::readWeights(const std::string &fileName)
{
	std::ifstream file(fileName.c_str(), std::ios::in | std::ios::binary);
	m_agent->loadWeights(file);
}

void SeminarTrial::writeOutput()
{
	std::stringstream ss;
	ss << m_parameters.outputPath << "/trial" << number() << "_out.txt";

	std::ofstream file(ss.str().c_str());
	for (unsigned int i = 0; i < m_output.size(); ++i) {
		file << i << ',' << m_output[i].reward << ',' << m_output[i].steps << std::endl;
	}
}

void SeminarTrial::writeWeights()
{
	std::stringstream ss;
	ss << m_parameters.outputPath << "/trial" << number() << "_weights";

	std::ofstream file(ss.str().c_str(), std::ios::out | std::ios::binary);
	m_agent->saveWeights(file);
}

SeminarTrial::Parameters::Parameters(Config &config)
{
	alpha = config.getAlpha();
	lambda = config.getLambda();
	episodes = config.getNumEpisodes();
	outputPath = config.getOutputPath() + "/" + config.getExperimentName();
	saveWeights = config.getSaveWeights();
	humanAdviceEpisodes = config.getHumanAdviceEpisodes();
}

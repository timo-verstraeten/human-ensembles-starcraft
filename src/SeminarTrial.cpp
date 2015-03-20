#include "SeminarTrial.h"

#include "agents/AdaptiveObjectiveSelection.h"
#include "env/CMAC.h"
#include "util/Config.h"
#include "policies/EpsilonGreedyPolicy.h"
#include "util/ErrorLogger.h"
#include "potentials/HumanAdvicePotential.h"
#include "potentials/Potentials.h"
#include "agents/QLearningAgent.h"
#include "agents/SarsaAgent.h"
#include "policies/EpsilonGreedyPolicy.h"
#include "env/CMAC.h"
#include "agents/QLearningAgent.h"
#include "agents/AdaptiveObjectiveSelection.h"

#include <algorithm>
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
	: Trial(number), m_parameters(config), m_humanAdvice(0), m_agent(0), m_episode(0), m_episodeReward(0.0), m_step(0), m_killed(0), m_died(0)
{
	std::vector<std::string> potentialStrings = config.getShapingPotentials();
	if (potentialStrings.size() == 0) {
		potentialStrings.push_back("");
	}

	std::vector<QValuesAgent*> agents;
	for (unsigned int i = 0; i < potentialStrings.size(); ++i) {
		Policy *policy = new EpsilonGreedyPolicy(EPSILON);
		FunctionApproximator *functionApproximator = new CMAC(StateResolution(makeResolutionsVector(config.getResolutionScale())), config.getNumTilings());
		Potential *potential = createPotential(potentialStrings[i], config);
		agents.push_back(new QLearningAgent(m_parameters.alpha, m_parameters.lambda, GAMMA, policy, functionApproximator, potential));
	}
	
	m_agent = new AdaptiveObjectiveSelection(agents, EPSILON);

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
	m_agent = 0;
	for (unsigned int i = 0; i < m_humanAdvice.size(); ++i) {
		delete m_humanAdvice[i];
		m_humanAdvice[i] = 0;
	}
	// m_humanAdvicePotentials are owned by m_agent, like any other potential, and should not be deleted here
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
		for (unsigned int i = 0; i < m_humanAdvice.size(); ++i) {
			m_humanAdvicePotentials[i]->start(state, action);
			*m_humanAdvice[i] = false;
		}
	}
	else {
		action = m_agent->step(STEP_REWARD, state, output);
		if (m_humanAdvicePotentials.size() > 0 && m_episode < m_parameters.humanAdviceEpisodes) {
			for (unsigned int i = 0; i < m_humanAdvicePotentials.size(); ++i) {
				m_humanAdvicePotentials[i]->step(state, action);
				*m_humanAdvice[i] = false;
			}
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

	double finalReward = (m_step > CUTOFF_EPISODE_LIMIT) ? 0 : state.hitPointDifference;
	m_agent->endEpisode(finalReward, output);

	m_episodeReward += finalReward;
	output << "Trial: " << number() << ", episode: " << m_episode << ", steps: " << m_step << ", reward: " << m_episodeReward << " (" << m_killed << " to " << m_died << ")" << std::endl;

	EpisodeOutput episodeOutput;
	episodeOutput.reward = m_episodeReward;
	episodeOutput.steps = m_step;
	m_output.push_back(episodeOutput);

	m_step = 0;
	m_episodeReward = 0;
	return ++m_episode < m_parameters.episodes;
}

const std::vector<bool*> &SeminarTrial::humanAdvice()
{
	if (m_episode < m_parameters.humanAdviceEpisodes) {
		return m_humanAdvice;
	}
	else {
		return Trial::humanAdvice();
	}
}

Potential *SeminarTrial::createPotential(const std::string &description, Config &config)
{
	size_t pos = description.find(":");
	std::string potentialString = description.substr(0, pos);
	std::transform(potentialString.begin(), potentialString.end(), potentialString.begin(), tolower);

	if (potentialString == "") {
		return 0;
	}
	else if (potentialString == "zero") {
		return new ZeroPotential();
	}
	else if (pos != std::string::npos) {
		std::string parameters = description.substr(pos + 1);
		std::replace(parameters.begin(), parameters.end(), ',', ' ');
		std::istringstream parametersStream(parameters);

		double scaling;
		parametersStream >> scaling;

		if (potentialString == "health") {
			return new HealthPotential(scaling);
		}
		else if (potentialString == "distance") {
			return new DistancePotential(scaling);
		}
		else if (potentialString == "healthdistance") {
			return new HealthDistancePotential(scaling);
		}
		else if (potentialString == "human" || potentialString == "humanadvice") {
			bool *humanAdvice = new bool(false);
			m_humanAdvice.push_back(humanAdvice); // Can't just push back a bool and get a reference to it, since std::vector can reallocate its internal array
			FunctionApproximator *functionApproximator = new CMAC(StateResolution(makeResolutionsVector(config.getResolutionScale())), config.getNumTilings());
			HumanAdvicePotential *humanAdvicePotential = new HumanAdvicePotential(scaling, *humanAdvice, functionApproximator, m_parameters.alpha, m_parameters.lambda, config.getHumanAdviceGamma());
			m_humanAdvicePotentials.push_back(humanAdvicePotential);
			return humanAdvicePotential;
		}
	}
	
	ErrorLogger::instance().assert(false, "Unknown potential string!");
	return 0;
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
	{
		std::stringstream ss;
		ss << m_parameters.outputPath << "/trial" << number() << "_weights";

		std::ofstream file(ss.str().c_str(), std::ios::out | std::ios::binary);
		m_agent->saveWeights(file);
	}

	for (unsigned int i = 0; i < m_humanAdvicePotentials.size(); ++i) {
		std::stringstream ss;
		ss <<  m_parameters.outputPath << "/trial" << number() << "_human_advice_" << i;

		std::ofstream file(ss.str().c_str(), std::ios::out | std::ios::binary);
		m_humanAdvicePotentials[i]->saveWeights(file);
	}
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

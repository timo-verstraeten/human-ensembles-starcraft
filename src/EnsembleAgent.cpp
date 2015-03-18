#include "EnsembleAgent.h"

#include "ErrorLogger.h"

EnsembleAgent::EnsembleAgent(const std::vector<SMDPAgent*> &agents)
	: m_agents(agents)
{
	ErrorLogger::instance().assert(m_agents.size() > 0, "No agents were given to the ensemble agent!");
	for (unsigned int i = 0; i < m_agents.size(); ++i) {
		ErrorLogger::instance().assert(m_agents[i] != 0, "Agent is a null pointer!");
	}
}

EnsembleAgent::~EnsembleAgent()
{
	for (unsigned int i = 0; i < m_agents.size(); ++i) {
		delete m_agents[i];
	}
}

void EnsembleAgent::saveWeights(std::ostream &output)
{
	for (unsigned int i = 0; i < m_agents.size(); ++i) {
		m_agents[i]->saveWeights(output);
	}
}

void EnsembleAgent::loadWeights(std::istream &input)
{
	for (unsigned int i = 0; i < m_agents.size(); ++i) {
		m_agents[i]->loadWeights(input);
	}
}

void EnsembleAgent::initialize(std::ostream &output)
{
	for (unsigned int i = 0; i < m_agents.size(); ++i) {
		m_agents[i]->initialize(output);
	}
}

void EnsembleAgent::applyAction(Action action, std::ostream &output)
{
	for (unsigned int i = 0; i < m_agents.size(); ++i) {
		m_agents[i]->applyAction(action, output);
	}
}

void EnsembleAgent::giveReward(double reward, std::ostream &output)
{
	for (unsigned int i = 0; i < m_agents.size(); ++i) {
		m_agents[i]->giveReward(reward, output);
	}
}

void EnsembleAgent::finalize(std::ostream &output)
{
	for (unsigned int i = 0; i < m_agents.size(); ++i) {
		m_agents[i]->finalize(output);
	}
}

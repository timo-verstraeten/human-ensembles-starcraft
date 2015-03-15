#include "EnsembleAgent.h"

#include "ErrorLogger.h"

EnsembleAgent::EnsembleAgent(const std::vector<SMDPAgent*> &agents)
	: m_agents(agents)
{
	for (unsigned int i = 0; i < m_agents.size(); ++i) {
		ErrorLogger::instance().assert(m_agents[i] != 0, "Agent is a null pointer!");
	}
}

EnsembleAgent::~EnsembleAgent()
{
	for (unsigned int i = 0; i < m_agents.size(); ++i) {
		delete m_agents[i];
		m_agents[i] = 0;
	}
}

void initialize(std::ostream &output)
{
	for (unsigned int i = 0; i < m_agents.size(); ++i) {
		m_agents[i]->initialize(output);
	}
}

void applyAction(Action action, std::ostream &output)
{
	for (unsigned int i = 0; i < m_agents.size(); ++i) {
		m_agents[i]->applyAction(action, output);
	}
}

void giveReward(double reward, std::ostream &output)
{
	for (unsigned int i = 0; i < m_agents.size(); ++i) {
		m_agents[i]->giveReward(reward, output);
	}
}

void finalize(std::ostream &output)
{
	for (unsigned int i = 0; i < m_agents.size(); ++i) {
		m_agents[i]->finalize(output);
	}
}

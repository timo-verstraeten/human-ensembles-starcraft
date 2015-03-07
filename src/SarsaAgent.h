#pragma once
#ifndef INC_SARSA_AGENT_H
#define INC_SARSA_AGENT_H

#include "SMDPAgent.h"

#include <vector>

class ActionSelector;
class FunctionApproximator;
class Potential;

class SarsaAgent : public SMDPAgent
{
public:
	SarsaAgent(double alpha, double lambda, double gamma, ActionSelector *actionSelector, const std::vector<FunctionApproximator*> &functionApproximators, const std::vector<Potential*> &potentials);
	virtual ~SarsaAgent();

	virtual Action startEpisode(const State &state, std::ostream &output);
	virtual Action step(double reward, const State &state, std::ostream &output);
	virtual void endEpisode(double reward);

	virtual void saveWeights(std::ostream &output);
	virtual void loadWeights(std::istream &input);

private:
	Action selectAction(std::ostream &output);
	Action argmaxQ(std::ostream &output);

	const double m_alpha;
	const double m_lambda;
	const double m_gamma;
	ActionSelector *m_actionSelector;
	std::vector<FunctionApproximator*> m_functionApproximators;
	std::vector<Potential*> m_potentials;

	std::vector<double> m_lastQs;
	std::vector<double> m_lastPotentials;
};

#endif // INC_SARSA_AGENT_H

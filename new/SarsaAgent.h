#pragma once
#ifndef INC_SARSA_AGENT_H
#define INC_SARSA_AGENT_H

#include "SMDPAgent.h"

class ActionSelector;
class FunctionApproximator;
class Potential;

class SarsaAgent : public SMDPAgent
{
public:
	SarsaAgent(double alpha, double lambda, double gamma, ActionSelector *actionSelector, FunctionApproximator *functionApproximator, Potential *potential);
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
	FunctionApproximator *m_functionApproximator;
	Potential *m_potential;

	double m_lastQ;
	double m_lastPotential;
};

#endif // INC_SARSA_AGENT_H

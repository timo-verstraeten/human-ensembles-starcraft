#pragma once
#ifndef INC_SARSA_AGENT_H
#define INC_SARSA_AGENT_H

#include "SMDPAgent.h"

class FunctionApproximator;

class SarsaAgent : public SMDPAgent
{
public:
	SarsaAgent(double alpha, double lambda, double gamma, double epsilon, FunctionApproximator *functionApproximator);
	virtual ~SarsaAgent();

	virtual Action startEpisode(const State &state, std::ostream &output);
	virtual Action step(double reward, const State &state, std::ostream &output);
	virtual void endEpisode(double reward);

private:
	Action selectAction(std::ostream &output);
	Action argmaxQ(std::ostream &output);

	const double m_alpha;
	const double m_lambda;
	const double m_gamma;
	const double m_epsilon;
	FunctionApproximator *m_functionAppoximator;

	double m_lastQ;
};

#endif // INC_SARSA_AGENT_H

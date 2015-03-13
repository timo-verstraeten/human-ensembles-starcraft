#pragma once
#ifndef INC_Q_VALUES_AGENT_H
#define INC_Q_VALUES_AGENT_H

#include "SMDPAgent.h"

#include <vector>

class ActionSelector;
class FunctionApproximator;
class Potential;

class QValuesAgent : public SMDPAgent
{
public:
	QValuesAgent(double alpha, double lambda, double gamma, ActionSelector *actionSelector, const std::vector<FunctionApproximator*> &functionApproximators, const std::vector<Potential*> &potentials);
	virtual ~QValuesAgent();

	virtual Action startEpisode(const State &state, std::ostream &output);
	virtual Action step(double reward, const State &state, std::ostream &output);
	virtual void endEpisode(double reward);

	virtual void saveWeights(std::ostream &output);
	virtual void loadWeights(std::istream &input);

protected:
	virtual double nextQ(FunctionApproximator *functionApproximator, Action selected) = 0;

private:
	const double m_alpha;
	const double m_lambda;
	const double m_gamma;
	ActionSelector *m_actionSelector;
	std::vector<FunctionApproximator*> m_functionApproximators;
	std::vector<Potential*> m_potentials;

	std::vector<double> m_lastQs;
	std::vector<double> m_lastPotentials;
};

#endif // INC_Q_VALUES_AGENT_H

#pragma once
#ifndef INC_Q_VALUES_AGENT_H
#define INC_Q_VALUES_AGENT_H

#include "SMDPAgent.h"

#include <utility>

class ActionSelector;
class FunctionApproximator;
class Potential;

class QValuesAgent : public SMDPAgent
{
public:
	QValuesAgent(double alpha, double lambda, double gamma, ActionSelector *actionSelector, FunctionApproximator *functionApproximator, Potential *potential);
	virtual ~QValuesAgent();

	virtual void saveWeights(std::ostream &output);
	virtual void loadWeights(std::istream &input);

	FunctionApproximator &functionApproximator();

protected:
	virtual void initialize(std::ostream &output);
	virtual Action nextAction(const State &state, std::ostream &output);
	virtual void applyAction(Action action, std::ostream &output);
	virtual void giveReward(double reward, std::ostream &output);
	virtual void finalize(std::ostream &output);

	virtual double nextQ(FunctionApproximator *functionApproximator, Action selected) = 0; // TODO Make m_functionApproximator protected?

private:
	const double m_alpha;
	const double m_lambda;
	const double m_gamma;
	ActionSelector *m_actionSelector; // TODO Change to some kind of Policy interface
	FunctionApproximator *m_functionApproximator;
	Potential *m_potential;

	std::pair<bool, double> m_pendingReward;
	double m_lastQ;
	double m_lastPotential;
};

#endif // INC_Q_VALUES_AGENT_H

#pragma once
#ifndef INC_HUMAN_ADVICE_POTENTIAL_H
#define INC_HUMAN_ADVICE_POTENTIAL_H

#include "Potential.h"

#include "Action.h"

class FunctionApproximator;
class HumanAdvice;

struct State;

class HumanAdvicePotential : public Potential
{
public:
	HumanAdvicePotential(double scaling, const bool &humanAdvice, FunctionApproximator *functionApproximator, double alpha, double lambda, double gamma);
	virtual ~HumanAdvicePotential();

	virtual double getUnscaled(const State &state, Action action);

	void step(const State &state, Action action);

private:
	const bool &m_humanAdvice;

	FunctionApproximator *m_functionApproximator;

	const double m_alpha;
	const double m_lambda;
	const double m_gamma;
};

#endif // HUMAN_ADVICE_POTENTIAL_H
#include "HumanAdvicePotential.h"

#include "FunctionApproximator.h"
#include "HumanAdvice.h"

HumanAdvicePotential::HumanAdvicePotential(double scaling, const bool &humanAdvice, FunctionApproximator *functionApproximator, double alpha, double lambda, double gamma)
	: Potential(scaling), m_humanAdvice(humanAdvice), m_functionApproximator(functionApproximator), m_alpha(alpha), m_lambda(lambda), m_gamma(gamma)
{
}

HumanAdvicePotential::~HumanAdvicePotential()
{
	delete m_functionApproximator;
	m_functionApproximator = 0;
}

double HumanAdvicePotential::getUnscaled(const State &state, Action action)
{
	m_functionApproximator->setState(state);
	return m_functionApproximator->computeQ(action);
}

void HumanAdvicePotential::step(const State &state, Action action)
{
	if (m_humanAdvice) {
		m_functionApproximator->updateWeights(-1, m_alpha);
	}

	m_functionApproximator->setState(state);
	m_functionApproximator->decayTraces(m_gamma * m_lambda);
	m_functionApproximator->updateTraces(action);
}
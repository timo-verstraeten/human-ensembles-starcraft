#include "HumanAdvicePotential.h"

#include "../env/FunctionApproximator.h"

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

void HumanAdvicePotential::start(const State &state, Action action)
{
	m_functionApproximator->decayTraces(0);
	m_functionApproximator->setState(state);
	m_lastQ = m_functionApproximator->computeQ(action);
	m_functionApproximator->updateTraces(action);
}

void HumanAdvicePotential::step(const State &state, Action action)
{
	m_functionApproximator->setState(state);

	double reward = m_humanAdvice ? -1 : 0;
	double q = m_functionApproximator->computeQ(action);
	m_functionApproximator->updateWeights(reward + m_gamma * q - m_lastQ, m_alpha);
	m_lastQ = m_functionApproximator->computeQ(action);

	m_functionApproximator->decayTraces(m_gamma * m_lambda);

	for (unsigned int j = 0; j < NUMBER_OF_ACTIONS; ++j) { // clear other than F[a]
		Action a = static_cast<Action>(j);
		if (a != action) {
			m_functionApproximator->clearTraces(a);
		}
	}
	m_functionApproximator->updateTraces(action);
}

void HumanAdvicePotential::saveWeights(std::ostream &output)
{
	m_functionApproximator->saveWeights(output);
}

void HumanAdvicePotential::loadWeights(std::istream &input)
{
	m_functionApproximator->loadWeights(input);
}

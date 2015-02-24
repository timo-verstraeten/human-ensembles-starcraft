#include "SarsaAgent.h"

#include "FunctionApproximator.h"
#include "Potential.h"

#include <cassert>

namespace
{

double random()
{
	return static_cast<double>(rand()) / RAND_MAX;
}

int random_int(int maxRange)
{
	return static_cast<int>(random() * maxRange);
}

}

SarsaAgent::SarsaAgent(double alpha, double lambda, double gamma, double epsilon, FunctionApproximator *functionApproximator, Potential *potential)
	: m_alpha(alpha), m_lambda(lambda), m_gamma(gamma), m_epsilon(epsilon), m_functionApproximator(functionApproximator), m_potential(potential)
{
	assert(m_functionApproximator && "FunctionApproximator is a null pointer!");
	assert(m_gamma == 1.0 && "Gamma is assumed to be equal to 1.0!");
}

SarsaAgent::~SarsaAgent()
{
	delete m_functionApproximator;
	m_functionApproximator = 0;

	delete m_potential;
	m_potential = 0;
}

Action SarsaAgent::startEpisode(const State &state, std::ostream &output)
{
	if (m_gamma != 1.0) {
		output << "ERROR: Gamma is not equal to 1!" << std::endl;
	}

	m_functionApproximator->decayTraces(0);

	m_functionApproximator->setState(state);

	Action action = selectAction(output);
	m_lastQ = m_functionApproximator->computeQ(action);
	m_functionApproximator->updateTraces(action);

	m_lastPotential = 0;
	if (m_potential) {
		m_lastPotential = m_potential->get(state, action);
	}

	return action;
}

Action SarsaAgent::step(double reward, const State &state, std::ostream &output)
{
	m_functionApproximator->setState(state);

	Action action = selectAction(output);
	double q = m_functionApproximator->computeQ(action);
	double potential = 0;
	if (m_potential) {
		potential = m_potential->get(state, action);
	}

	m_functionApproximator->updateWeights(reward + m_gamma * q - m_lastQ + m_gamma * potential - m_lastPotential, m_alpha);

	m_lastQ = m_functionApproximator->computeQ(action); // need to redo because weights changed
	m_lastPotential = potential;
	
	m_functionApproximator->decayTraces(m_gamma * m_lambda);
	for (unsigned int i = 0; i < NUMBER_OF_ACTIONS; ++i) { // clear other than F[a]
		Action a = static_cast<Action>(i);
		if (a != action) {
			m_functionApproximator->clearTraces(a);
		}
	}
	m_functionApproximator->updateTraces(action);

	return action;
}

void SarsaAgent::endEpisode(double reward)
{
	m_functionApproximator->updateWeights(reward - m_lastQ, m_alpha);
}

void SarsaAgent::saveWeights(std::ostream &output)
{
	m_functionApproximator->saveWeights(output);
}

void SarsaAgent::loadWeights(std::istream &input)
{
	m_functionApproximator->loadWeights(input);
}

Action SarsaAgent::selectAction(std::ostream &output)
{
	output << " In Select";
	if (random() < m_epsilon)	{
		output << " RandA" << std::endl;
		return static_cast<Action>(random_int(NUMBER_OF_ACTIONS));
	}
	else {
		output << " ArgMax" << std::endl;
		return argmaxQ(output);
	}
}

Action SarsaAgent::argmaxQ(std::ostream &output)
{
	unsigned int ties = 0;

	Action maxAction = static_cast<Action>(0);
	double maxQ = m_functionApproximator->computeQ(maxAction);

	output << "Q[" << maxAction << "] = " << maxQ << std::endl;

	for (unsigned int i = 1; i < NUMBER_OF_ACTIONS; ++i) {
		Action action = static_cast<Action>(i);
		double q = m_functionApproximator->computeQ(action);
		output << "Q[" << action << "] = " << q << std::endl;

		if (q > maxQ) {
			ties = 0;
			maxAction = action;
			maxQ = q;
		}
		else if (q == maxQ) {
			++ties;
			if (random_int(ties + 1)) {
				maxAction = action;
				maxQ = q;
			}
		}
	}

	return maxAction;
}

#include "SarsaAgent.h"

#include "ActionSelector.h"
#include "FunctionApproximator.h"
#include "Potential.h"

#include <cassert>

SarsaAgent::SarsaAgent(double alpha, double lambda, double gamma, ActionSelector *actionSelector, FunctionApproximator *functionApproximator, Potential *potential)
	: m_alpha(alpha), m_lambda(lambda), m_gamma(gamma), m_actionSelector(actionSelector), m_functionApproximator(functionApproximator), m_potential(potential)
{
	assert(m_functionApproximator && "FunctionApproximator is a null pointer!");
	assert(m_gamma == 1.0 && "Gamma is assumed to be equal to 1.0!");
}

SarsaAgent::~SarsaAgent()
{
	delete m_potential;
	m_potential = 0;

	delete m_functionApproximator;
	m_functionApproximator = 0;

	delete m_actionSelector;
	m_actionSelector = 0;
}

Action SarsaAgent::startEpisode(const State &state, std::ostream &output)
{
	if (m_gamma != 1.0) {
		output << "ERROR: Gamma is not equal to 1!" << std::endl;
	}

	m_functionApproximator->decayTraces(0);

	m_functionApproximator->setState(state);

	std::vector<FunctionApproximator*> functionApproximators;
	functionApproximators.push_back(m_functionApproximator);
	Action action = m_actionSelector->select(functionApproximators, output);

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

	std::vector<FunctionApproximator*> functionApproximators;
	functionApproximators.push_back(m_functionApproximator);
	Action action = m_actionSelector->select(functionApproximators, output);

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

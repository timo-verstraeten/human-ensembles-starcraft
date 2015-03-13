#include "QValuesAgent.h"

#include "ActionSelector.h"
#include "ErrorLogger.h"
#include "FunctionApproximator.h"
#include "Potential.h"

QValuesAgent::QValuesAgent(double alpha, double lambda, double gamma, ActionSelector *actionSelector, const std::vector<FunctionApproximator*> &functionApproximators, const std::vector<Potential*> &potentials)
	: m_alpha(alpha), m_lambda(lambda), m_gamma(gamma), m_actionSelector(actionSelector), m_functionApproximators(functionApproximators), m_potentials(potentials), m_lastQs(potentials.size()), m_lastPotentials(potentials.size())
{
	ErrorLogger::instance().assert(m_functionApproximators.size() == m_potentials.size(), "Different number of function approximators than potentials given!");
	for (unsigned int i = 0; i < m_functionApproximators.size(); ++i) {
		ErrorLogger::instance().assert(m_functionApproximators[i] != 0, "FunctionApproximator is a null pointer!");
	}
	ErrorLogger::instance().assert(m_gamma == 1.0, "Gamma is assumed to be equal to 1.0!");
}

QValuesAgent::~QValuesAgent()
{
	for (unsigned int i = 0; i < m_potentials.size(); ++i) {
		delete m_potentials[i];
		m_potentials[i] = 0;

		delete m_functionApproximators[i];
		m_functionApproximators[i] = 0;
	}

	delete m_actionSelector;
	m_actionSelector = 0;
}

Action QValuesAgent::startEpisode(const State &state, std::ostream &output)
{
	if (m_gamma != 1.0) {
		output << "ERROR: Gamma is not equal to 1!" << std::endl;
	}

	for (unsigned int i = 0; i < m_potentials.size(); ++i) {
		m_functionApproximators[i]->decayTraces(0);
		m_functionApproximators[i]->setState(state);
	}

	Action action = m_actionSelector->select(m_functionApproximators, output);

	for (unsigned int i = 0; i < m_potentials.size(); ++i) {
		m_lastQs[i] = m_functionApproximators[i]->computeQ(action);
		m_functionApproximators[i]->updateTraces(action);

		m_lastPotentials[i] = 0;
		if (m_potentials[i]) {
			m_lastPotentials[i] = m_potentials[i]->get(state, action);
		}
	}

	return action;
}

Action QValuesAgent::step(double reward, const State &state, std::ostream &output)
{
	for (unsigned int i = 0; i < m_potentials.size(); ++i) {
		m_functionApproximators[i]->setState(state);
	}

	Action action = m_actionSelector->select(m_functionApproximators, output);

	for (unsigned int i = 0; i < m_potentials.size(); ++i) {
		double q = nextQ(m_functionApproximators[i], action);//m_functionApproximators[i]->computeQ(action);
		double potential = 0;
		if (m_potentials[i]) {
			potential = m_potentials[i]->get(state, action);
		}

		m_functionApproximators[i]->updateWeights(reward + m_gamma * q - m_lastQs[i] + m_gamma * potential - m_lastPotentials[i], m_alpha);

		m_lastQs[i] = m_functionApproximators[i]->computeQ(action); // need to redo because weights changed
		m_lastPotentials[i] = potential;
		
		m_functionApproximators[i]->decayTraces(m_gamma * m_lambda);
		for (unsigned int j = 0; j < NUMBER_OF_ACTIONS; ++j) { // clear other than F[a]
			Action a = static_cast<Action>(j);
			if (a != action) {
				m_functionApproximators[i]->clearTraces(a);
			}
		}
		m_functionApproximators[i]->updateTraces(action);
	}

	return action;
}

void QValuesAgent::endEpisode(double reward)
{
	for (unsigned int i = 0; i < m_potentials.size(); ++i) {
		m_functionApproximators[i]->updateWeights(reward - m_lastQs[i], m_alpha);
	}
}

void QValuesAgent::saveWeights(std::ostream &output)
{
	for (unsigned int i = 0; i < m_potentials.size(); ++i) {
		m_functionApproximators[i]->saveWeights(output);
	}
}

void QValuesAgent::loadWeights(std::istream &input)
{
	for (unsigned int i = 0; i < m_potentials.size(); ++i) {
		m_functionApproximators[i]->loadWeights(input);
	}
}

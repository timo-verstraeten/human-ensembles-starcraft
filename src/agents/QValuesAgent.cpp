#include "QValuesAgent.h"

#include "../util/ErrorLogger.h"
#include "../env/FunctionApproximator.h"
#include "../policies/Policy.h"
#include "../potentials/Potential.h"

QValuesAgent::QValuesAgent(double alpha, double lambda, double gamma, Policy *policy, FunctionApproximator *functionApproximator, Potential *potential)
	: m_alpha(alpha), m_lambda(lambda), m_gamma(gamma), m_policy(policy), m_functionApproximator(functionApproximator), m_potential(potential), m_pendingReward(false, 0)
{
	ErrorLogger::instance().assert(m_policy != 0, "Policy is a null pointer!");
	ErrorLogger::instance().assert(m_functionApproximator != 0, "FunctionApproximator is a null pointer!");
	ErrorLogger::instance().assert(m_gamma == 1.0, "Gamma is assumed to be equal to 1.0!");
}

QValuesAgent::~QValuesAgent()
{
	delete m_potential;
	m_potential = 0;

	delete m_functionApproximator;
	m_functionApproximator = 0;

	delete m_policy;
	m_policy = 0;
}

void QValuesAgent::saveWeights(std::ostream &output)
{
	m_functionApproximator->saveWeights(output);
}

void QValuesAgent::loadWeights(std::istream &input)
{
	m_functionApproximator->loadWeights(input);
}

FunctionApproximator &QValuesAgent::functionApproximator()
{
	return *m_functionApproximator;
}

const Policy &QValuesAgent::policy() const {
	return *m_policy;
}

void QValuesAgent::initialize(std::ostream &output)
{
	if (m_gamma != 1.0) {
		output << "ERROR: Gamma is not equal to 1!" << std::endl;
	}
	
	m_functionApproximator->decayTraces(0);
}

Action QValuesAgent::nextAction(const State &state, std::ostream &output)
{
	m_functionApproximator->setState(state);
	return m_policy->selectAction(*m_functionApproximator, output);
}

void QValuesAgent::applyAction(Action action, std::ostream &output)
{
	double q = nextQ(action);
	double potential = 0;
	if (m_potential) {
		potential = m_potential->get(m_functionApproximator->state(), action); // TODO Is this right? Or should this be the resulting action of the nextQ call?
	}

	if (m_pendingReward.first) {
		m_functionApproximator->updateWeights(m_pendingReward.second + m_gamma * q - m_lastQ + m_gamma * potential - m_lastPotential, m_alpha);
		m_pendingReward.first = false;
	}

	m_lastQ = m_functionApproximator->computeQ(action); // need to redo because weights changed
	m_lastPotential = potential;
	
	m_functionApproximator->decayTraces(m_gamma * m_lambda);
	for (unsigned int j = 0; j < NUMBER_OF_ACTIONS; ++j) { // clear other than F[a]
		Action a = static_cast<Action>(j);
		if (a != action) {
			m_functionApproximator->clearTraces(a);
		}
	}
	m_functionApproximator->updateTraces(action);
}

void QValuesAgent::giveReward(double reward, std::ostream &output)
{
	m_pendingReward.first = true;
	m_pendingReward.second = reward;
}

void QValuesAgent::finalize(std::ostream &output)
{
	if (m_pendingReward.first) {
		m_functionApproximator->updateWeights(m_pendingReward.second - m_lastQ, m_alpha);	
		m_pendingReward.first = false;
	}
}

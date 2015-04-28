#include "EpsilonGreedyPolicy.h"

#include <cassert>

EpsilonGreedyPolicy::EpsilonGreedyPolicy(double epsilon)
	: m_epsilon(epsilon)
{
}

EpsilonGreedyPolicy::~EpsilonGreedyPolicy()
{
}

Action EpsilonGreedyPolicy::selectAction(const std::vector<double>& preferenceValues, std::ostream &output) const
{
	assert(preferenceValues.size() == NUMBER_OF_ACTIONS && "The size of the preference vector should be equal to the number of actions.");

	output << "Epsilon-Greedy policy - ";
	if (random() < m_epsilon)	{
		output << "Random" << std::endl;
		return static_cast<Action>(randomInt(NUMBER_OF_ACTIONS));
	}
	else {
		return m_greedyPolicy.selectAction(preferenceValues, output);
	}
}

std::vector<double> EpsilonGreedyPolicy::selectionProbabilities(FunctionApproximator &functionApproximator) const
{
	std::vector<double> probabilities = m_greedyPolicy.selectionProbabilities(functionApproximator);
	for (unsigned int i = 0; i < probabilities.size(); ++i) {
		probabilities[i] = m_epsilon*(1 / static_cast<double>(NUMBER_OF_ACTIONS)) + (1 - m_epsilon)*probabilities[i];
	}
	return probabilities;
}
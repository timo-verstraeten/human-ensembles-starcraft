#include "EpsilonGreedyPolicy.h"

EpsilonGreedyPolicy::EpsilonGreedyPolicy(double epsilon)
	: m_epsilon(epsilon)
{
}

EpsilonGreedyPolicy::~EpsilonGreedyPolicy()
{
}

Action EpsilonGreedyPolicy::selectAction(FunctionApproximator &functionApproximator, std::ostream &output) const
{
	output << "Epsilon-Greedy policy - ";
	if (random() < m_epsilon)	{
		output << "Random" << std::endl;
		return static_cast<Action>(randomInt(NUMBER_OF_ACTIONS));
	}
	else {
		return m_greedyPolicy.selectAction(functionApproximator, output);
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
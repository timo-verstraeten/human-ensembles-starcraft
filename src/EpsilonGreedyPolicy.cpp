#include "EpsilonGreedyPolicy.h"

EpsilonGreedyPolicy::EpsilonGreedyPolicy(double epsilon)
	: m_epsilon(epsilon)
{
}

EpsilonGreedyPolicy::~EpsilonGreedyPolicy()
{
}

Action EpsilonGreedyPolicy::selectAction(FunctionApproximator &functionApproximator, std::ostream &output)
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

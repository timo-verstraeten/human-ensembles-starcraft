#include "ActionSelector.h"

#include "FunctionApproximator.h"

double ActionSelector::random()
{
	return static_cast<double>(rand()) / RAND_MAX;
}

unsigned int ActionSelector::randomInt(unsigned int maxRange)
{
	return static_cast<unsigned int>(random() * maxRange);
}

Action ActionSelector::epsilonGreedy(FunctionApproximator &functionApproximator, double epsilon, std::ostream &output)
{
	output << "In Select - ";
	if (random() < epsilon)	{
		output << "RandA" << std::endl;
		return static_cast<Action>(randomInt(NUMBER_OF_ACTIONS));
	}
	else {
		output << "ArgMax" << std::endl;
		return argmaxQ(functionApproximator, output);
	}
}

Action ActionSelector::argmaxQ(FunctionApproximator &functionApproximator, std::ostream &output)
{
	unsigned int ties = 0;

	Action maxAction = static_cast<Action>(0);
	double maxQ = functionApproximator.computeQ(maxAction);

	output << "Q[" << maxAction << "] = " << maxQ << std::endl;

	for (unsigned int i = 1; i < NUMBER_OF_ACTIONS; ++i) {
		Action action = static_cast<Action>(i);
		double q = functionApproximator.computeQ(action);
		output << "Q[" << action << "] = " << q << std::endl;

		if (q > maxQ) {
			ties = 0;
			maxAction = action;
			maxQ = q;
		}
		else if (q == maxQ) {
			++ties;
			if (randomInt(ties + 1)) {
				maxAction = action;
				maxQ = q;
			}
		}
	}

	return maxAction;
}

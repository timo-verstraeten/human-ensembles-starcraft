#include "GreedyPolicy.h"

#include "FunctionApproximator.h"

GreedyPolicy::GreedyPolicy()
{
}

GreedyPolicy::~GreedyPolicy()
{
}

Action GreedyPolicy::selectAction(FunctionApproximator &functionApproximator, std::ostream &output)
{
	output << "Greedy policy" << std::endl;

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

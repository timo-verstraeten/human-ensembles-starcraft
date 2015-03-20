#include "GreedyPolicy.h"

#include "../env/FunctionApproximator.h"

GreedyPolicy::GreedyPolicy()
{
}

GreedyPolicy::~GreedyPolicy()
{
}

Action GreedyPolicy::selectAction(FunctionApproximator &functionApproximator, std::ostream &output) const
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

std::vector<double> GreedyPolicy::selectionProbabilities(FunctionApproximator &functionApproximator) const
{
	std::vector<double> qValues(NUMBER_OF_ACTIONS);
	qValues[0] = functionApproximator.computeQ(static_cast<Action>(0));
	double maxQ = qValues[0];
	unsigned int count = 1;
	for (unsigned int i = 1; i < NUMBER_OF_ACTIONS; ++i) {
		qValues[i] = functionApproximator.computeQ(static_cast<Action>(i));
		if (qValues[i] > maxQ) {
			maxQ = qValues[i];
			count = 1;
		} else if (qValues[i] == maxQ) {
			++count;
		}
	}

	std::vector<double> probabilities(NUMBER_OF_ACTIONS);
	for (unsigned int i = 0; i < NUMBER_OF_ACTIONS; ++i) {
		if (maxQ == qValues[i]) {
			probabilities[i] = 1 / static_cast<double>(count);
		}
	}

	return probabilities;
}
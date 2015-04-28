#include "GreedyPolicy.h"

#include "../env/FunctionApproximator.h"

#include <cassert>

GreedyPolicy::GreedyPolicy()
{
}

GreedyPolicy::~GreedyPolicy()
{
}


Action GreedyPolicy::selectAction(const std::vector<double>& preferenceValues, std::ostream &output) const
{
	assert(preferenceValues.size() == NUMBER_OF_ACTIONS && "The size of the preference vector should be equal to the number of actions.");

	output << "Greedy policy" << std::endl;

	unsigned int ties = 0;
	Action maxAction = static_cast<Action>(0);
	double maxValue = preferenceValues[0];
	for (unsigned int i = 1; i < NUMBER_OF_ACTIONS; ++i) {
		Action action = static_cast<Action>(i);
		if (preferenceValues[i] > maxValue) {
			ties = 0;
			maxAction = action;
			maxValue = preferenceValues[i];
		}
		else if (preferenceValues[i] == maxValue) {
			++ties;
			if (randomInt(ties + 1)) {
				maxAction = action;
				maxValue = preferenceValues[i];
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
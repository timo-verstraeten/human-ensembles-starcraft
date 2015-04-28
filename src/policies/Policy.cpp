#include "Policy.h"

#include "../env/FunctionApproximator.h"

Action Policy::selectQAction(FunctionApproximator &functionApproximator, std::ostream &output) const
{
	std::vector<double> preferenceValues(NUMBER_OF_ACTIONS);
	for (unsigned int i = 0; i < NUMBER_OF_ACTIONS; ++i) {
		Action action = static_cast<Action>(i);
		preferenceValues[i] = functionApproximator.computeQ(action);
	}

	Action action = selectAction(preferenceValues, output);

	for (unsigned int i = 0; i < NUMBER_OF_ACTIONS; ++i) {
		output << "Q[" << i << "] = " << preferenceValues[i] << std::endl;
	}

	return action;
}
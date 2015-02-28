#include "AdaptiveObjectiveSelector.h"

#include "FunctionApproximator.h"

AdaptiveObjectiveSelector::AdaptiveObjectiveSelector(double epsilon)
	: m_epsilon(epsilon)
{
}

AdaptiveObjectiveSelector::~AdaptiveObjectiveSelector()
{
}

Action AdaptiveObjectiveSelector::select(const std::vector<FunctionApproximator*> &functionApproximators, std::ostream &output)
{
	return epsilonGreedy(selectFunctionApproximator(functionApproximators, output), m_epsilon, output);
}

FunctionApproximator &AdaptiveObjectiveSelector::selectFunctionApproximator(const std::vector<FunctionApproximator*> &functionApproximators, std::ostream &output)
{
	if (functionApproximators.size() == 1) {
		return *functionApproximators[0];
	}

	output << "Adaptive objective selection" << std::endl;

	double bestConfidence = functionApproximators[0]->computeConfidence();
	int best = 0;
	int numTies = 0;
	output << "Confidence " << best << " = " << bestConfidence << std::endl;

	for (unsigned int i = 1; i < functionApproximators.size(); ++i) {
		double confidence = functionApproximators[i]->computeConfidence();
		output << "Confidence " << i << " = " << confidence << std::endl;

		if (confidence > bestConfidence) {
			numTies = 0;
			bestConfidence = confidence;
			best = i;
		}
		else if (confidence == bestConfidence) {
			numTies++;
			if (randomInt(numTies + 1) == 0) {
				bestConfidence = confidence;
				best = i;
			}
		}
	}

	return *functionApproximators[best];
}

#pragma once
#ifndef INC_POLICY_H
#define INC_POLICY_H

#include "../env/Action.h"

#include <numeric>
#include <ostream>
#include <vector>

class FunctionApproximator;

/**
 * Interface for an action selection policy of a single agent.
 */
class Policy
{
public:
	virtual ~Policy() {}

	/**
	 * Selects an action based on a given function approximator.
	 */
	virtual Action selectAction(FunctionApproximator &functionApproximator, std::ostream &output) const = 0;

	/**
	 * Returns a vector of the probability that each respective action is selected.
	 */
	virtual std::vector<double> selectionProbabilities(FunctionApproximator &functionApproximator) const = 0;


	/**
	 * Returns a random double in [0, 1].
	 */
	static double random() { return static_cast<double>(rand()) / RAND_MAX; }

	/**
	 * Returns a random integer in [0, maxRange].
	 */
	static unsigned int randomInt(unsigned int maxRange) { return static_cast<unsigned int>(random() * maxRange); }

	/**
	 * Returns the random index of the given vector, weighted by the contents of the vector.
	 */
	static unsigned int randomChoice(const std::vector<double> &weights) {
		double total = std::accumulate(weights.begin(), weights.end(), 0.0);
		double r = total*random();
		unsigned int i = 0;
		while (weights[i] < r) {
			r -= weights[i++];
		}
		return i;
	}
};

#endif // INC_POLICY_H

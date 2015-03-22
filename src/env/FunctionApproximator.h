#pragma once
#ifndef INC_FUNCTION_APPROXIMATOR_H
#define INC_FUNCTION_APPROXIMATOR_H

#include "Action.h"
#include "State.h"
#include "StateResolution.h"

#include <istream>
#include <ostream>

/**
 * Abstract base class for a function approximator, keeping track of Q-values over a continuous state-action space.
 */
class FunctionApproximator
{
public:
	FunctionApproximator(const StateResolution &stateResolution);
	virtual ~FunctionApproximator() {}

	/**
	 * Sets the current state of the function approximator. To be called before calling any other function operating on this state.
	 */
	virtual void setState(const State &state);
	const State &state();

	/**
	 * Computes the Q-value of an action.
	 */
	virtual double computeQ(Action action) = 0;

	/**
	 * Computes a confidence metric of the current function approximator in the current state.
	 */
	virtual double computeConfidence() = 0;

	/**
	 * Updates the weights of the currently set traces, by a certain delta, at a certain learning rate.
	 */
	virtual void updateWeights(double delta, double alpha) = 0;


	/**
	 * Clears the eligibility traces of the given action in the currently set state.
	 */
	virtual void clearTraces(Action action) = 0;

	/**
	 * Decays all eligibility traces in the current state by a certain rate.
	 */
	virtual void decayTraces(double decayRate) = 0;

	/**
	 * Refreshes the eligibility traces of the action in the currently set state.
	 */
	virtual void updateTraces(Action action) = 0;


	/**
	 * Saves the current weights of the function approximator to a (binary) output stream.
	 */
	virtual void saveWeights(std::ostream &output) = 0;

	/**
	 * Loads previously saved weights from a (binary) output stream.
	 */
	virtual void loadWeights(std::istream &input) = 0;

protected:
	StateResolution m_stateResolution;
	State m_state;
};

#endif // INC_FUNCTION_APPROXIMATOR_H

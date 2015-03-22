#pragma once
#ifndef INC_HUMAN_ADVICE_POTENTIAL_H
#define INC_HUMAN_ADVICE_POTENTIAL_H

#include "Potential.h"

#include "../env/Action.h"

#include <istream>
#include <ostream>

class FunctionApproximator;

struct State;

/**
 * Class keeping and updating the secondary value function for the human advice, and acting as a state-action potential to the agents.
 */
class HumanAdvicePotential : public Potential
{
public:
	HumanAdvicePotential(double scaling, const bool &humanAdvice, FunctionApproximator *functionApproximator, double alpha, double lambda, double gamma);
	virtual ~HumanAdvicePotential();

	// --- Potential ---
	virtual double getUnscaled(const State &state, Action action);


	/**
	 * Updates the human advice potential in the start state, with a given state-action pair
	 */
	void start(const State &state, Action action);

	/**
	 * Updates the human advice potential with a given state-action pair
	 */
	void step(const State &state, Action action);


	/**
	 * Saves the learned weights of the secondary value function to a (binary) output stream
	 */
	void saveWeights(std::ostream &output);

	/**
	 * Loads previously saved weights from a (binary) output stream
	 */
	void loadWeights(std::istream &input);

private:
	const bool &m_humanAdvice;

	FunctionApproximator *m_functionApproximator;

	const double m_alpha;
	const double m_lambda;
	const double m_gamma;

	double m_lastQ;
};

#endif // HUMAN_ADVICE_POTENTIAL_H

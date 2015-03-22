#pragma once
#ifndef INC_POTENTIAL_H
#define INC_POTENTIAL_H

#include "../env/Action.h"

struct State;

/**
 * Abstract base class for a potential over the state-action space.
 */
class Potential
{
public:
	Potential(double scaling) : m_scaling(scaling) {}
	virtual ~Potential() {}

	/**
	 * Gets the unscaled potential, and scales it by the given scaling factor.
	 */
	virtual double get(const State &state, Action action) { return m_scaling * getUnscaled(state, action); }

protected:
	/**
	 * Retuns the unscaled potential for the given state and action.
	 * This unscaled potential should preferably be between 0 and 1, such that the scaling factor has a more or less equal effect on different potentials.
	 */
	virtual double getUnscaled(const State &state, Action action) = 0;

private:
	double m_scaling;
};

#endif // INC_POTENTIAL_H

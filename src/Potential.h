#pragma once
#ifndef INC_POTENTIAL_H
#define INC_POTENTIAL_H

#include "Action.h"

struct State;

class Potential
{
public:
	Potential(double scaling) : m_scaling(scaling) {}
	virtual ~Potential() {}

	virtual double get(const State &state, Action action) { return m_scaling * getUnscaled(state, action); }

protected:
	virtual double getUnscaled(const State &state, Action action) = 0;

private:
	double m_scaling;
};

#endif // INC_POTENTIAL_H

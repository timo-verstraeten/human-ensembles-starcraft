#pragma once
#ifndef INC_FUNCTION_APPROXIMATOR_H
#define INC_FUNCTION_APPROXIMATOR_H

#include "Action.h"
#include "State.h"
#include "StateResolution.h"

#include <istream>
#include <ostream>

class FunctionApproximator
{
public:
	FunctionApproximator(const StateResolution &stateResolution);
	virtual ~FunctionApproximator() {}

	virtual void setState(const State &state);

	virtual double computeQ(Action action) = 0;
	virtual void updateWeights(double delta, double alpha) = 0;

	virtual void clearTraces(Action action) = 0;
	virtual void decayTraces(double decayRate) = 0;
	virtual void updateTraces(Action action) = 0;

	virtual void saveWeights(std::ostream &output) = 0;
	virtual void loadWeights(std::istream &input) = 0;

protected:
	StateResolution m_stateResolution;
	State m_state;
};

#endif // INC_FUNCTION_APPROXIMATOR_H

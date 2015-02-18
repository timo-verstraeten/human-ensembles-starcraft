#pragma once
#ifndef INC_FUNCTION_APPROXIMATOR_H
#define INC_FUNCTION_APPROXIMATOR_H

#include "SMDPAgent.h"

class FunctionApproximator
{
public:
	FunctionApproximator(const std::vector<double> &minValues, const std::vector<double> &ranges, const std::vector<double> &resolutions);
	virtual ~FunctionApproximator() {}

	virtual void setState(const State &state);

	virtual double computeQ(int action) = 0;
	virtual void updateWeights(double delta, double alpha) = 0;

	virtual void clearTraces(int action ) = 0;
	virtual void decayTraces(double decayRate) = 0;
	virtual void updateTraces(int action) = 0;

protected:
	std::vector<double> m_state;
	std::vector<double> m_minValues;
	std::vector<double> m_ranges;
	std::vector<double> m_resolutions;
};

#endif // INC_FUNCTION_APPROXIMATOR_H

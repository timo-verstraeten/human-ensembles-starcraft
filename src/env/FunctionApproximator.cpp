#include "FunctionApproximator.h"

FunctionApproximator::FunctionApproximator(const StateResolution &stateResolution)
	: m_stateResolution(stateResolution)
{
}

void FunctionApproximator::setState(const State &state)
{
	m_state = state;
}

const State &FunctionApproximator::state()
{
	return m_state;
}

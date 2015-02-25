#pragma once
#ifndef INC_TRIAL_H
#define INC_TRIAL_H

#include "Action.h"
#include "State.h"

#include <ostream>
#include <string>

class HumanAdvice;

class Trial
{
public:
	Trial(unsigned int number) : m_number(number) {}
	virtual ~Trial() {}

	unsigned int number() const { return m_number; }

	virtual Action step(const State &state, std::ostream &output) = 0;
	virtual bool nextEpisode(const State &state, std::ostream &output) = 0;

	virtual HumanAdvice *humanAdvice() { return 0; }

private:
	const unsigned int m_number;
};

#endif // INC_TRIAL_H

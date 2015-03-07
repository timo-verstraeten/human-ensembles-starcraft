#pragma once
#ifndef INC_HUMAN_ADVICE_H
#define INC_HUMAN_ADVICE_H

#include <vector>

class HumanAdvice
{
public:
	HumanAdvice(unsigned int advisors);

	void reward(unsigned int advisor);
	bool rewarded(unsigned int advisor) const;
	void reset();

private:
	std::vector<bool> m_rewards;
};

#endif // INC_HUMAN_ADVICE_H

#include "HumanAdvice.h"
#include "ErrorLogger.h"

HumanAdvice::HumanAdvice(unsigned int advisors)
	: m_rewards(advisors, false)
{
}

void HumanAdvice::reward(unsigned int advisor)
{
	ErrorLogger::instance()->assert(advisor < m_rewards.size(), "Advisor doesn't exist!");
	m_rewards[advisor] = true;
}

bool HumanAdvice::rewarded(unsigned int advisor) const
{
	ErrorLogger::instance()->assert(advisor < m_rewards.size(), "Advisor doesn't exist!");
	return m_rewards[advisor];
}

void HumanAdvice::reset()
{
	for (unsigned int i = 0; i < m_rewards.size(); ++i) {
		m_rewards[i] = false;
	}
}

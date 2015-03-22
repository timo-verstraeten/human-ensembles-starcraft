#pragma once
#ifndef INC_BOLTZMANN_ENSEMBLE_AGENT_H
#define INC_BOLTZMANN_ENSEMBLE_AGENT_H

#include "EnsembleAgent.h"

#include <vector>

class QValuesAgent;

/**
 * Abstract base class for ensemble agents that select actions based on Boltzmann distributions (BoltzmannMultiplication and BoltzmannAddition).
 */
class BoltzmannEnsembleAgent : public EnsembleAgent
{
public:
	BoltzmannEnsembleAgent(const std::vector<QValuesAgent*> &agents, double temperature);
	virtual ~BoltzmannEnsembleAgent();

	virtual Action nextAction(const State &state, std::ostream &output);

protected:
	virtual double aggregateWeights(const std::vector<double> &weights) const = 0;

private:
	double m_temperature;
};

#endif // INC_BOLTZMANN_ENSEMBLE_AGENT_H
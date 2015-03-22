#pragma once
#ifndef INC_BOLTZMANN_ADDITION_H
#define INC_BOLTZMANN_ADDITION_H

#include "BoltzmannEnsembleAgent.h"

#include <vector>

class QValuesAgent;

/**
 * Ensemble agent implementing the Boltzmann Addition action selection.
 */
class BoltzmannAddition : public BoltzmannEnsembleAgent
{
public:
	BoltzmannAddition(const std::vector<QValuesAgent*> &agents, double temperature);
	virtual ~BoltzmannAddition();

private:
	virtual double aggregateWeights(const std::vector<double> &weights) const;
};

#endif // INC_BOLTZMANN_ADDITION_H
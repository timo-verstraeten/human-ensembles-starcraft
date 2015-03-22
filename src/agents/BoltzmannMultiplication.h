#pragma once
#ifndef INC_BOLTZMANN_MULTIPLICATION_H
#define INC_BOLTZMANN_MULTIPLICATION_H

#include "BoltzmannEnsembleAgent.h"

#include <vector>

class QValuesAgent;

/**
 * Ensemble agent implementing the Boltzmann Multiplication action selection.
 */
class BoltzmannMultiplication : public BoltzmannEnsembleAgent
{
public:
	BoltzmannMultiplication(const std::vector<QValuesAgent*> &agents, double temperature);
	virtual ~BoltzmannMultiplication();

private:
	virtual double aggregateWeights(const std::vector<double> &weights) const;
};

#endif // INC_BOLTZMANN_MULTIPLICATION_H
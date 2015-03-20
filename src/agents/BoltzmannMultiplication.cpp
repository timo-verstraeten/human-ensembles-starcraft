#include "BoltzmannMultiplication.h"

#include "QValuesAgent.h"

#include <functional>
#include <numeric>

BoltzmannMultiplication::BoltzmannMultiplication(const std::vector<QValuesAgent*> &agents, double temperature)
	: BoltzmannEnsembleAgent(agents, temperature)
{
}

BoltzmannMultiplication::~BoltzmannMultiplication()
{
}

double BoltzmannMultiplication::aggregateWeights(const std::vector<double> &weights) const
{
	return std::accumulate(weights.begin(), weights.end(), 1.0, std::multiplies<double>());
}
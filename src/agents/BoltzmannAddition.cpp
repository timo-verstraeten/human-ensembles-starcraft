#include "BoltzmannAddition.h"

#include "QValuesAgent.h"

#include <numeric>

BoltzmannAddition::BoltzmannAddition(const std::vector<QValuesAgent*> &agents, double temperature)
	: BoltzmannEnsembleAgent(agents, temperature)
{
}

BoltzmannAddition::~BoltzmannAddition()
{
}

double BoltzmannAddition::aggregateWeights(const std::vector<double> &weights) const
{
	return std::accumulate(weights.begin(), weights.end(), 0.0);
}
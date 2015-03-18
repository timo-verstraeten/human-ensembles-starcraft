#include "BoltzmannMultiplication.h"
#include "QValuesAgent.h"

BoltzmannMultiplication::BoltzmannMultiplication(const std::vector<QValuesAgent*> &agents, double temperature)
	: BoltzmannEnsembleAgent(agents, temperature)
{
}

BoltzmannMultiplication::~BoltzmannMultiplication()
{
}

double BoltzmannMultiplication::aggregateWeights(const std::vector<double> &weights) const
{
	double aggregated = 1;
	for (unsigned int i = 0; i < weights.size(); ++i) {
		aggregated *= weights[i];
	}
	return aggregated;
}
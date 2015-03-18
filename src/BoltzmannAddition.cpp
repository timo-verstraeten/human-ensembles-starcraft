#include "BoltzmannAddition.h"
#include "QValuesAgent.h"

BoltzmannAddition::BoltzmannAddition(const std::vector<QValuesAgent*> &agents, double temperature)
	: BoltzmannEnsembleAgent(agents, temperature)
{
}

BoltzmannAddition::~BoltzmannAddition()
{
}

double BoltzmannAddition::aggregateWeights(const std::vector<double> &weights) const
{
	double aggregated = 0;
	for (unsigned int i = 0; i < weights.size(); ++i) {
		aggregated += weights[i];
	}
	return aggregated;
}
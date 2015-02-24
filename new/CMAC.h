#pragma once
#ifndef INC_CMAC_H
#define INC_CMAC_H

#include "FunctionApproximator.h"

class collision_table;


#define RL_MEMORY_SIZE 1048576
#define RL_MAX_NONZERO_TRACES 100000
#define RL_MAX_NUM_TILINGS 6000


class CMAC : public FunctionApproximator
{
public:
	CMAC(const StateResolution &stateResolution, unsigned int tilingsPerGroup);
	virtual ~CMAC();

	virtual void setState(const State &state);

	virtual double computeQ(Action action);
	virtual void updateWeights(double delta, double alpha);

	virtual void clearTraces(Action action);
	virtual void decayTraces(double decayRate);
	virtual void updateTraces(Action action);

	virtual void saveWeights(std::ostream &output);
	virtual void loadWeights(std::istream &input);

private:
	void loadTiles();

	void clearTrace(int f);
	void clearExistentTrace(int f, int loc);
	void setTrace(int f, double newTraceValue);
	void updateTrace(int f, double deltaTraceValue);
	void increaseMinTrace();

	const unsigned int m_tilingsPerGroup;

	int m_tiles[NUMBER_OF_ACTIONS][RL_MAX_NUM_TILINGS];
	double m_weights[RL_MEMORY_SIZE];
	double m_traces[RL_MEMORY_SIZE];
	unsigned int m_numberOfTilings;

	double m_minimumTrace;
	int m_nonzeroTraces[RL_MAX_NONZERO_TRACES];
	int m_numNonzeroTraces;
	int m_nonzeroTracesInverse[RL_MEMORY_SIZE];

	collision_table *m_collisionTable;
};

#endif // INC_CMAC_H

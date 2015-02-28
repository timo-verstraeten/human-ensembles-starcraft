#include "CMAC.h"

#include "tiles2.h"

#include <cmath>
#include <iostream>
#include <utility>

CMAC::CMAC(const StateResolution &stateResolution, unsigned int tilingsPerGroup)
	: FunctionApproximator(stateResolution), m_tilingsPerGroup(tilingsPerGroup)
{
	m_minimumTrace = 0.01;

	m_numNonzeroTraces = 0;
	for (int i = 0; i < RL_MEMORY_SIZE; i++) {
		m_weights[i] = 0;
		m_traces[i] = 0;
	}

	srand(0U);
	int tmp[2];
	float tmpf[2];
	m_collisionTable = new collision_table(RL_MEMORY_SIZE, 1);

	GetTiles(tmp, 1, 1, tmpf, 0); // A dummy call to set the hashing table
}

CMAC::~CMAC()
{
	delete m_collisionTable;
	m_collisionTable = 0;
}

void CMAC::setState(const State &state)
{
	FunctionApproximator::setState(state);

	loadTiles();
}

double CMAC::computeQ(Action action)
{
	double q = 0;
	for (unsigned int i = 0; i < m_numberOfTilings; i++) {
		q += m_weights[m_tiles[action][i]];
	}
	return q;
}

double CMAC::computeConfidence()
{
	std::pair<unsigned int, double> min = std::make_pair(0, computeQ(static_cast<Action>(0)));
	std::pair<unsigned int, double> max = std::make_pair(0, computeQ(static_cast<Action>(0)));
	for (unsigned int i = 1; i < NUMBER_OF_ACTIONS; ++i) {
		double q = computeQ(static_cast<Action>(i));
		if (q < min.second) {
			min.first = i;
			min.second = q;
		}
		else if (q > max.second) {
			max.first = i;
			max.second = q;
		}
	}

	// Paired t-test t-value calculation
	double S_x = 0;
	double S_x2 = 0;
	for (unsigned int i = 0; i < m_numberOfTilings; i++) {
		double x = m_weights[m_tiles[max.first][i]] - m_weights[m_tiles[min.first][i]];
		S_x += x;
		S_x += x * x;
	}
	double t = S_x / sqrt((m_numberOfTilings * S_x2 - S_x * S_x) / (m_numberOfTilings - 1));
	return t;
}

void CMAC::updateWeights(double delta, double alpha)
{
	double update = delta * alpha / m_numberOfTilings;

	for (int i = 0; i < m_numNonzeroTraces; i++) {
		int f = m_nonzeroTraces[i];
		
		if (f > RL_MEMORY_SIZE || f < 0)
			std::cerr << "f is too big or too small!!" << f << std::endl;
		
		m_weights[f] += update * m_traces[f];
	}
}

void CMAC::clearTraces(Action action)
{
	for (unsigned int i = 0; i < m_numberOfTilings; i++) {
		clearTrace(m_tiles[action][i]);
	}
}

void CMAC::decayTraces(double decayRate)
{
	for (int loc = m_numNonzeroTraces - 1; loc >= 0; loc--) {
		int f = m_nonzeroTraces[loc];

		if (f > RL_MEMORY_SIZE || f < 0)
			std::cerr << "DecayTraces: f out of range " << f << std::endl;

		m_traces[f] *= decayRate;
		if (m_traces[f] < m_minimumTrace)
			clearExistentTrace(f, loc);
	}
}

void CMAC::updateTraces(Action action)
{
	for (unsigned int i = 0; i < m_numberOfTilings; i++) {
		setTrace(m_tiles[action][i], 1.0); 
	} 
}

void CMAC::saveWeights(std::ostream &output)
{
	output.write(reinterpret_cast<char*>(&m_weights), RL_MEMORY_SIZE * sizeof(double));

	output.write(reinterpret_cast<char*>(&m_collisionTable->m), sizeof(long));
	output.write(reinterpret_cast<char*>(m_collisionTable->data), m_collisionTable->m * sizeof(long));
	output.write(reinterpret_cast<char*>(&m_collisionTable->safe), sizeof(int));
	output.write(reinterpret_cast<char*>(&m_collisionTable->calls), sizeof(long));
	output.write(reinterpret_cast<char*>(&m_collisionTable->clearhits), sizeof(long));
	output.write(reinterpret_cast<char*>(&m_collisionTable->collisions), sizeof(long));
}

void CMAC::loadWeights(std::istream &input)
{
	input.read(reinterpret_cast<char*>(&m_weights), RL_MEMORY_SIZE * sizeof(double));

	input.read(reinterpret_cast<char*>(&m_collisionTable->m), sizeof(long));
	input.read(reinterpret_cast<char*>(m_collisionTable->data), m_collisionTable->m * sizeof(long));
	input.read(reinterpret_cast<char*>(&m_collisionTable->safe), sizeof(int));
	input.read(reinterpret_cast<char*>(&m_collisionTable->calls), sizeof(long));
	input.read(reinterpret_cast<char*>(&m_collisionTable->clearhits), sizeof(long));
	input.read(reinterpret_cast<char*>(&m_collisionTable->collisions), sizeof(long));
}

void CMAC::loadTiles()
{
	unsigned int numTilings = 0;

	std::vector<double> features = m_stateResolution.scaleState(m_state);

	/* These are the 'tiling groups'  --  play here with representations */
	/* One tiling for each state variable */
	for (unsigned int v = 0; v < features.size(); ++v) {
		for (int a = 0; a < NUMBER_OF_ACTIONS; ++a) {
			GetTiles1(&(m_tiles[a][numTilings]), m_tilingsPerGroup, m_collisionTable, static_cast<float>(features[v]), a, v);
		}  
		numTilings += m_tilingsPerGroup;
	}

	if (numTilings > RL_MAX_NUM_TILINGS)
		std::cerr << "TOO MANY TILINGS! " << numTilings << std::endl;

	m_numberOfTilings = numTilings;
}

// Clear any trace for feature f
void CMAC::clearTrace(int f)
{
	if (f > RL_MEMORY_SIZE || f < 0)
		std::cerr << "ClearTrace: f out of range " << f << std::endl;

	if (m_traces[f] != 0)
		clearExistentTrace(f, m_nonzeroTracesInverse[f]);
}

// Clear the trace for feature f at location loc in the list of nonzero traces 
void CMAC::clearExistentTrace(int f, int loc)
{
	if (f > RL_MEMORY_SIZE || f < 0)
		std::cerr << "ClearExistentTrace: f out of range " << f << std::endl;

	m_traces[f] = 0.0;
	m_numNonzeroTraces--;
	m_nonzeroTraces[loc] = m_nonzeroTraces[m_numNonzeroTraces];
	m_nonzeroTracesInverse[m_nonzeroTraces[loc]] = loc;
}

// Set the trace for feature f to the given value, which must be positive   
void CMAC::setTrace(int f, double newTraceValue)
{
	if (f > RL_MEMORY_SIZE || f < 0)
		std::cerr << "SetTraces: f out of range " << f << std::endl;

	if (m_traces[f] >= m_minimumTrace)
		m_traces[f] = newTraceValue;         // trace already exists              
	else {
		while (m_numNonzeroTraces >= RL_MAX_NONZERO_TRACES)
			increaseMinTrace(); // ensure room for new trace              
		m_traces[f] = newTraceValue;
		m_nonzeroTraces[m_numNonzeroTraces] = f;
		m_nonzeroTracesInverse[f] = m_numNonzeroTraces;
		m_numNonzeroTraces++;
	}
}

// Try to make room for more traces by incrementing minimum_trace by 10%,
// culling any traces that fall below the new minimum                      
void CMAC::increaseMinTrace()
{
	m_minimumTrace *= 1.1;
	std::cerr << "Changing minimum_trace to " << m_minimumTrace << std::endl;

	for (int loc = m_numNonzeroTraces - 1; loc >= 0; loc--) { // necessary to loop downwards    
		int f = m_nonzeroTraces[loc];
		if (m_traces[f] < m_minimumTrace)
			clearExistentTrace(f, loc);
	}
}

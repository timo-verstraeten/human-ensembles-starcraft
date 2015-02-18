#include "SeminarTrial.h"

#include <fstream>
#include <sstream>

SeminarTrial::SeminarTrial(unsigned int number, Parameters parameters)
	: Trial(number), m_parameters(parameters), m_episode(0), m_episodeReward(0.0), m_step(0)
{
}

SeminarTrial::~SeminarTrial()
{
	writeOutput();
}

Action SeminarTrial::step(const State &state, std::ostream &output)
{
	return STOP;
}

bool SeminarTrial::nextEpisode(const State &state, std::ostream &output)
{
	return false;
}

void SeminarTrial::writeOutput()
{
	std::stringstream ss;
	ss << "trial" << number() << "_out.txt";

	std::ofstream file(ss.str().c_str());
	for (unsigned int i = 0; i < m_output.size(); ++i) {
		file << i << ',' << m_output[i].reward << ',' << m_output[i].steps << std::endl;
	}
}

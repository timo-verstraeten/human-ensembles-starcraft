#ifndef INC_TRIALLOGGER_H
#define INC_TRIALLOGGER_H

#include "../env/Action.h"
#include "../env/State.h"

#include <fstream>

class TrialLogger {
public:
    TrialLogger(const std::string &fileName);
    virtual ~TrialLogger();

    void writeHeader(unsigned int numHumans);

    void writeLine(unsigned int episode, unsigned int step, const State &state, Action action, double episodeReward,
            unsigned int killed, unsigned int died, const std::vector<bool*> &humanAdvice);

private:
    std::ofstream m_outputFile;
};


#endif // INC_TRIALLOGGER_H

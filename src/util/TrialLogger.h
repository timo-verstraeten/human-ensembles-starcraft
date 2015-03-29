#ifndef INC_TRIALLOGGER_H
#define INC_TRIALLOGGER_H


#include "../env/State.h"
#include "../env/Action.h"
#include <fstream>

class TrialLogger {
public:
    TrialLogger(const std::string &fileName);
    virtual ~TrialLogger();

    void writeHeader();

    void writeLine(unsigned int episode, unsigned int step, const State &state, Action action, double episodeReward,
            unsigned int killed, unsigned int died);

private:
    std::ofstream m_outputFile;

};


#endif // INC_TRIALLOGGER_H

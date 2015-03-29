#include "TrialLogger.h"


TrialLogger::TrialLogger(const std::string &fileName) {
    if (!m_outputFile.is_open()) {
        m_outputFile.open(fileName.c_str(), std::ios::out | std::ios::app);
    }
}

TrialLogger::~TrialLogger() {
    if (m_outputFile.is_open()) {
        m_outputFile.close();
    }
}

void TrialLogger::writeHeader() {
    m_outputFile << "episode" << "," << "step" << "," <<
            "state.x" << "," << "state.y" << "," << "state.enemyDistance" <<
            "," << "state.hitPointDifference" << "," <<
            "state.enemyActive" << "," << "state.enemyAngle" << "," <<
            "action" << "," <<
            "killed" << "," << "died" << "," <<
            "episodeReward" << std::endl;
}


void TrialLogger::writeLine(unsigned int episode, unsigned int step, const State &state, Action action, double episodeReward,
        unsigned int killed, unsigned int died) {
    m_outputFile << episode << "," << step << "," <<
            state.x << "," << state.y << "," << state.enemyDistance <<
            "," << state.hitPointDifference << "," <<
            state.enemyActive << "," << state.enemyAngle << "," <<
            action << "," <<
            killed << "," << died << "," <<
            episodeReward << std::endl;
}

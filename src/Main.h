#pragma once
#ifndef INC_MAIN_H
#define INC_MAIN_H

#include "env/Action.h"
#include "env/State.h"
#include "util/Config.h"

#include <BWAPI.h>

#include <fstream>
#include <ostream>
#include <string>
#include <vector>

class Experiment;
class Trial;

class Main : public BWAPI::AIModule
{
public:
	Main();
	virtual ~Main();

	virtual void onStart();
	virtual void onFrame();
	virtual void onUnitDestroy(BWAPI::Unit* unit);

	virtual void onSendText(std::string text);

private:
	BWAPI::Unit *getOwnUnit();
	BWAPI::Unit *getEnemyUnit();

	State getState();
	void executeAction(Action action);
	void updateHumanAdvice(const std::vector<bool*> &humanAdvice);

	Config m_config;
	std::ofstream m_log_file;

	Experiment *m_experiment;
	Trial *m_trial;

	static const std::string CONFIG_FILE_NAME;
	static const std::vector<BWAPI::Key> INPUT_KEYS;
};

#endif // INC_MAIN_H

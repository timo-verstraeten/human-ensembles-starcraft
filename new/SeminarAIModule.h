#pragma once
#ifndef INC_SEMINAR_AI_MODULE_H
#define INC_SEMINAR_AI_MODULE_H

#include "Action.h"
#include "Config.h"
#include "State.h"

#include <BWAPI.h>

#include <fstream>
#include <ostream>
#include <string>

class Experiment;
class Trial;

class SeminarAIModule : public BWAPI::AIModule
{
public:
	SeminarAIModule();
	virtual ~SeminarAIModule();

	virtual void onStart();
	virtual void onFrame();
	virtual void onUnitDestroy(BWAPI::Unit* unit);

	virtual void onSendText(std::string text);

private:
	BWAPI::Unit *getOwnUnit();
	BWAPI::Unit *getEnemyUnit();

	State getState();
	void executeAction(Action action);

	Config m_config;
	std::ofstream m_log_file;

	Experiment *m_experiment;
	Trial *m_trial;

	static const std::string CONFIG_FILE_NAME;
};

#endif // INC_SEMINAR_AI_MODULE_H

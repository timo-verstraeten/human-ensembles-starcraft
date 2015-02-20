#pragma once
#ifndef INC_SEMINAR_AI_MODULE_H
#define INC_SEMINAR_AI_MODULE_H

#include "Action.h"
#include "State.h"

#include <BWAPI.h>

#include <fstream>
#include <ostream>
#include <string>
#include <windows.h>

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

	static std::string LOG_NAME;

	Experiment *m_experiment;
	Trial *m_trial;
	std::ofstream m_log_file;
};

#endif // INC_SEMINAR_AI_MODULE_H

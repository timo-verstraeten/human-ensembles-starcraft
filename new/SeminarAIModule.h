#pragma once
#ifndef INC_SEMINAR_AI_MODULE_H
#define INC_SEMINAR_AI_MODULE_H

#include "Action.h"
#include "State.h"

#include <BWAPI.h>

#include <ostream>
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

	Experiment *m_experiment;
	Trial *m_trial;
};

#endif // INC_SEMINAR_AI_MODULE_H

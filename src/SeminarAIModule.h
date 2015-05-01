#pragma once
#ifndef INC_SEMINAR_AI_MODULE_H
#define INC_SEMINAR_AI_MODULE_H

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

/**
 * BroodWar AIModule implementing the necessary hooks that are called in Starcraft.
 * Takes care of all interaction with the BWAPI and the Starcraft game, and every frame passes on the state of the game to the Experiment/Trial.
 */
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
	/**
	 * Returns a pointer to the unit controlled in the experiment. Returns a null pointer if that unit doesn't exist.
	 */
	BWAPI::Unit *getOwnUnit();
	/**
	 * Returns a pointer to the enemy unit. Returns a null pointer if that unit doesn't exist.
	 */
	BWAPI::Unit *getEnemyUnit();

	/**
	 * Fills a State struct with the current state of the game episode.
	 */
	State getState();
	/**
	 * Translates the action chosen by the agent in the experiment to the BWAPI command to execute it.
	 */
	void executeAction(Action action);
	/**
	 * Updates the human advice based on the state of the dedicated keyboard keys.
	 */
	void updateHumanAdvice(const std::vector<bool*> &humanAdvice);

	Config m_config;
	std::ofstream m_log_file;

	Experiment *m_experiment;
	Trial *m_trial;

	static const std::string CONFIG_FILE_NAME;
	static const std::vector<BWAPI::Key> INPUT_KEYS;
	static const int HUMAN_BASE_SPEED;
	static const BWAPI::Key HUMAN_SPEED_ACCELERATION_KEY;
};

#endif // INC_SEMINAR_AI_MODULE_H

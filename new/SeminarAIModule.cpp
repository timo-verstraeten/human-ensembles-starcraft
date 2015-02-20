#include "SeminarAIModule.h"

#include "Experiment.h"
#include "SeminarExperiment.h"
#include "Trial.h"

#include <cassert>
#include <cmath>
#include <fstream>
#include <sstream>

using namespace BWAPI;

std::string SeminarAIModule::LOG_NAME = ""; // "log.txt";

SeminarAIModule::SeminarAIModule()
	: m_experiment(0), m_trial(0)
{
	if (LOG_NAME != "") {
		m_log_file.open(LOG_NAME.c_str());
	}
}

SeminarAIModule::~SeminarAIModule()
{
	delete m_experiment;
}


void SeminarAIModule::onStart()
{
	//Broodwar->setLocalSpeed(-1); // TODO
	Broodwar->setLocalSpeed(0);
	//Broodwar->setGUI(false);

	Broodwar->printf("The map is %s, a %d player map", Broodwar->mapName().c_str(), Broodwar->getStartLocations().size());
	Broodwar->enableFlag(Flag::UserInput); // Enable some cheat flags
	Broodwar->enableFlag(Flag::CompleteMapInformation); // Uncomment to enable complete map information
	Broodwar->sendText("black sheep wall"); // cheat code to disable fog of war

	std::ifstream configFile("trials.cfg");
	m_experiment = new SeminarExperiment(configFile);
}

void SeminarAIModule::onFrame()
{
	if (m_experiment && Broodwar->getFrameCount() % 30 == 0) {
		if (!m_trial) {
			m_trial = m_experiment->nextTrial();
			if (!m_trial) {
				delete m_experiment;
				m_experiment = 0;
				Broodwar->leaveGame();
				return;
			}
		}

		std::stringstream output;
		
		if (getOwnUnit() && getEnemyUnit()) {
			Action nextAction = m_trial->step(getState(), output);
			executeAction(nextAction);

			std::string outputString = output.str();
			if (outputString.length() > 0) {
				Broodwar->printf("%s", outputString.c_str());
				if (m_log_file.is_open()) {
					m_log_file << outputString << std::endl;
				}
			}
		}
	}
}

void SeminarAIModule::onUnitDestroy(Unit *unit)
{
	if (m_trial) {
		std::stringstream output;

		bool finishedTrial = !m_trial->nextEpisode(getState(), output);
		if (finishedTrial) {
			delete m_trial;
			m_trial = 0;
		}

		std::string outputString = output.str();
		if (outputString.length() > 0) {
			Broodwar->printf("%s", outputString.c_str());
		}
	}
}

void SeminarAIModule::onSendText(std::string text)
{
	if (text=="/speedUp") {
		Broodwar->setLocalSpeed(0);
	}
	else if (text=="/speedDown") {
		Broodwar->setLocalSpeed(-1);
	}
}

Unit *SeminarAIModule::getOwnUnit()
{
	if (Broodwar->self()->getUnits().size() < 1)
		return 0;

	Unit *unit = *Broodwar->self()->getUnits().begin();
	return unit;
}

Unit *SeminarAIModule::getEnemyUnit()
{
	if (Broodwar->enemy()->getUnits().size() < 1)
		return 0;

	Unit *unit = *Broodwar->enemy()->getUnits().begin();
	return unit;
}

State SeminarAIModule::getState()
{
	State state;

	Unit *ownUnit = getOwnUnit();
	Unit *enemyUnit = getEnemyUnit();

	Position ownPosition = ownUnit ? ownUnit->getPosition() : Position();
	Position enemyPosition = enemyUnit ? enemyUnit->getPosition() : Position();

	state.x = ownPosition.x();
	state.y = ownPosition.y();
	state.enemyDistance = (ownUnit && enemyUnit) ? ownUnit->getDistance(enemyUnit) : 0;
	state.hitPointDifference = (ownUnit ? ownUnit->getHitPoints() : 0) - (enemyUnit ? enemyUnit->getHitPoints() : 0);
	state.enemyActive = enemyUnit && (enemyUnit->isAttacking() || enemyUnit->isMoving() || enemyUnit->isAttackFrame() || enemyUnit->isStartingAttack());
	state.enemyAngle = (ownUnit && enemyUnit) ? atan2(static_cast<double>(ownPosition.y() - enemyPosition.y()), static_cast<double>(ownPosition.x() - enemyPosition.y())) : 0; // TODO WTF!?

	return state;
}

void SeminarAIModule::executeAction(Action action)
{
	Unit *ownUnit = getOwnUnit();
	Unit *enemyUnit = getEnemyUnit();

	switch (action) {
	case STOP:
		ownUnit->stop();
		break;
	case ATTACK:
		if (enemyUnit->isVisible()) {
			ownUnit->attack(enemyUnit->getPosition());
		}
		break;
	case MOVE_TOWARDS_ENEMY:
		if (enemyUnit->isVisible()) {
			ownUnit->move(enemyUnit->getPosition());
		}
		break;
	case MOVE_SOUTH:
		ownUnit->move(Position(ownUnit->getPosition().x(), ownUnit->getPosition().y() + 30));
		break;
	case MOVE_NORTH:
		ownUnit->move(Position(ownUnit->getPosition().x(), ownUnit->getPosition().y() - 30));
		break;
	case MOVE_EAST:
		ownUnit->move(Position(ownUnit->getPosition().x() + 30, ownUnit->getPosition().y()));
		break;
	case MOVE_WEST:
		ownUnit->move(Position(ownUnit->getPosition().x() - 30, ownUnit->getPosition().y()));
		break;
	default:
		assert(false && "Unknown action encountered!");
	}
}

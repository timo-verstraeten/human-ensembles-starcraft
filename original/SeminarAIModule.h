#pragma once
#include <BWAPI.h>
#include <BWTA.h>
//#include "BWTA.h"
#include <windows.h>
#include <cstdlib>
#include "RLConst.h"
#include "SMDPAgent.h"
#include "CMAC.h"
#include "SarsaAgent.h"
#include "FuncApprox.h"
#include <iostream>
#include <fstream>
#include <cstring>

extern bool analyzed;
extern bool analysis_just_finished;
extern BWTA::Region* home;
extern BWTA::Region* enemy_base;
DWORD WINAPI AnalyzeThread();

class SeminarAIModule : public BWAPI::AIModule
{

public:
	virtual ~SeminarAIModule() {
	  delete sa;
		sa = NULL;};
  virtual void onStart();
  virtual void onEnd(bool isWinner);
  virtual void onFrame();
  virtual void onSendText(std::string text);
  virtual void onReceiveText(BWAPI::Player* player, std::string text);
  virtual void onPlayerLeft(BWAPI::Player* player);
  virtual void onNukeDetect(BWAPI::Position target);
  virtual void onUnitDiscover(BWAPI::Unit* unit);
  virtual void onUnitEvade(BWAPI::Unit* unit);
  virtual void onUnitShow(BWAPI::Unit* unit);
  virtual void onUnitHide(BWAPI::Unit* unit);
  virtual void onUnitCreate(BWAPI::Unit* unit);
  virtual void onUnitDestroy(BWAPI::Unit* unit);
  virtual void onUnitMorph(BWAPI::Unit* unit);
  virtual void onUnitRenegade(BWAPI::Unit* unit);
  virtual void onSaveGame(std::string gameName);
  virtual void onUnitComplete(BWAPI::Unit *unit);
  void drawStats(); //not part of BWAPI::AIModule
  void drawBullets();
  void drawVisibilityData();
  void drawTerrainData();
  void showPlayers();
  void showForces();
  bool show_bullets;
  bool show_visibility_data;

  //void episodeStart();
  //void episodeStep();
  //void episodeEnd(int iMyHealth, int iEnemyHealth);


private:
  double m_State [10];
  SMDPAgent* sa;

  int UID;

  bool isLearning;
  bool justEnded;
  int trialNumber;
  int episodeNumber;
  int numSteps;         // number of steps in the current episode
  double episodeReward;
  bool humanOn;
  int adviceCount;      // amount of advice given this episode
  bool cutOffEpisode;   // true if the episode went on too many steps and we shouldn't learn from the result

  // Read from config file before every trial
  double alpha;
  double lambda;
  int numEpisodes;      // number of episodes in a trial
  char adviceUnit[10];  // episode or step
  int numAdvice;        // number of advice units to give advice for
  int adviceFreq;       // number of pieces of advice to give at a time
  int exploreFreq;      // number of episodes/steps to explore between advice
  bool showLearning;    // if true turn off learning and teacher every other episode to observe agent learning
  int trials;           // Number of trials to run
  double adviceQThreshold; //Q value threshold above which the teacher gives advice (Q value range, state importance Teacher)
  int Qindex; //Index showing with what value the best value will be compared , e.g. if Qindex = 1 the best value will compred to the second one, if it is 9 it will be compared to the worst

  void restartExperiment();
  void initQ();
  void getState();
  int selectAction();
  void executeAction(int iActionNum);
  double getAngle(Position myPos, Position enemyPos);
  int getMyHealth();
  int getEnemyHealth();
  int readConfigLine();

  std::ofstream myFile;
  std::ifstream configFile;

};

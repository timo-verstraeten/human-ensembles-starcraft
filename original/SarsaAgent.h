#ifndef SARSA_AGENT
#define SARSA_AGENT

#include "SMDPAgent.h"
#include "FuncApprox.h"
#include <cstdlib>
#include <sstream>
#include <string.h>
#include <stdio.h>
#include <ostream>
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <fcntl.h>
#include <io.h>
#include "CMAC.h"
//#include <fildes.h>

class SarsaAgent:public SMDPAgent
{
  char weightsFile[256];
  bool bLearning;
  bool bSaveWeights;

  bool bFollowTeacher;

  int epochNum;
  int lastAction;
  long int steps;          // total steps in the trial
  long int totalAdvice;    // total amount of advice given
  int adviceCount;         // advice given in this cycle
  int exploreCount;        // exploreations in this cycle

  double alpha;
  double gamma;
  double lambda;
  double epsilon;

  FunctionApproximator * FA;
  CMAC * TeacherFA;

  double Q[ MAX_ACTIONS ];

  // Load / Save weights from/to disk
  bool loadWeights( char *filename );
  bool saveWeights( char *filename );

  // Value function methods for CMACs
  int  selectAction(std::string* returnNote);
  int  argmaxQ(std::string* returnNote);

 public:
  SarsaAgent        ( double alph,
					  double lambd,
					  int    numFeatures,
				      int    numActions,
				      bool   bLearn,
				      // the function approximator should be created 
				      // with ranges, resolutions, numbers of features and actions
				      FunctionApproximator *anFA,
				      std::string* returnNote,
					  char   *loadWeightsFile = ""
					  );
  virtual ~SarsaAgent() {
	  delete FA;
	  FA=NULL;};



  // SMDP Sarsa implementation
  int  startEpisode( double state[], std::string* returnNote );
  int  step( double reward, double state[], std::string* returnNote, int episodeNumber, bool showLearning, bool cutOffEpisode);//,double adviceQThreshold,int Qindex );
  void endEpisode( double reward, int episodeNum, bool showLearning, bool cutOffEpisode );
} ;

#endif

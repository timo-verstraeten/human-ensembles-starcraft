#include "SeminarAIModule.h"
using namespace BWAPI;

BWTA::Region* home;
BWTA::Region* enemy_base;
int numKilled, numDied;
bool advicegiven;

void SeminarAIModule::onStart()
{
 //Broodwar->setLocalSpeed(-1);
 Broodwar->setLocalSpeed(0);//temp
 //Broodwar->setGUI(false);

  Broodwar->printf("The map is %s, a %d player map",Broodwar->mapName().c_str(),Broodwar->getStartLocations().size());
  // Enable some cheat flags
  Broodwar->enableFlag(Flag::UserInput);
  // Uncomment to enable complete map information
  Broodwar->enableFlag(Flag::CompleteMapInformation);
  // cheat code to disable fog of war
  Broodwar->sendText("black sheep wall");

  //read map information into BWTA so terrain analysis can be done in another thread
  BWTA::readMap();

  show_bullets=false;
  show_visibility_data=false;

  humanOn = false;

  if (Broodwar->isReplay())
  {
    Broodwar->printf("The following players are in this replay:");
    for(std::set<Player*>::iterator p=Broodwar->getPlayers().begin();p!=Broodwar->getPlayers().end();p++)
    {
      if (!(*p)->getUnits().empty() && !(*p)->isNeutral())
      {
        Broodwar->printf("%s, playing as a %s",(*p)->getName().c_str(),(*p)->getRace().getName().c_str());
      }
    }
  }
  else
  {
    Broodwar->printf("The match up is %s v %s",
      Broodwar->self()->getRace().getName().c_str(),
      Broodwar->enemy()->getRace().getName().c_str());

  }

  UID = 1;
  trialNumber = -1;
  //char name[100];
  //sprintf_s(name, "%d_%f_%f.txt", UID, dALPHA, dEPSILON);
  //myFile.open(name);

  configFile.open("trials.cfg");
  char wasteline[1000]; //first get rid of the format line
  configFile.getline(wasteline, 1000);
  char tmp[5];
  configFile.getline(tmp, 5); //number of trials
  trials = atoi(tmp);
  Broodwar->printf("Trials: %d",trials);

  restartExperiment(); 
  getState();
  std::string result;
  executeAction(sa->startEpisode(m_State, &result));
  numSteps = 0;
  episodeReward = 0.0;
  justEnded = false;
  cutOffEpisode = false;
  

}

int SeminarAIModule::readConfigLine(){
	char line[50];
	configFile.getline(line, 50);

	char* token;

/*	if((token = strtok(line, ",")) == NULL)
		return -1;

	if (strcmp(token, "episode") == 0 || strcmp(token, "step") == 0 || strcmp(token, "importance") == 0 || strcmp(token, "mistake") == 0  )
		strcpy(adviceUnit, token); 
	else
		return -1;

	if((token = strtok(NULL, ",")) == NULL)
		return -1;
	numAdvice = atoi(token);*/

	if((token = strtok(line, ",")) == NULL)
		return -1;
	alpha = atof(token);

	if((token = strtok(NULL, ",")) == NULL)
		return -1;
	lambda = atof(token);

	if((token = strtok(NULL, ",")) == NULL)
		return -1;
	numEpisodes = atoi(token);

	if((token = strtok(NULL, ",")) == NULL)
		return -1;
	if(atoi(token) == 0)
		showLearning = false;
	else
		showLearning = true;

	

/*	if((token = strtok(NULL, ",")) == NULL)
		return -1;
	adviceQThreshold = atof(token);

	if((token = strtok(NULL, ",")) == NULL)
		return -1;
	Qindex = atoi(token) - 1; //with who the best value will be compared, also substract 1 to go to normal array indexing
*/


	//Broodwar->printf("%f advice threshold",adviceQThreshold);

	return 0;


}

void SeminarAIModule::restartExperiment(){
	if (trialNumber != -1) {
		char name[100];
		//TO SAVE Q WEIGHTS IN A FILE
		sprintf_s(name, "weights_%d_%d.wts", episodeNumber, trialNumber);
		//sa->saveWeights(name); //TEMP
		myFile << "just saved weights in " << name << "\r\n";
		myFile.close();
		if (trialNumber == trials - 1)
			Broodwar->leaveGame();
	}
	Broodwar->printf("restartExperiment()");
	if(readConfigLine() == -1)
		Broodwar->leaveGame();
	trialNumber++;
	char name[100];
	sprintf_s(name, "trial%d_out.txt", trialNumber);
	myFile.open(name);
	numKilled = 0;
	numDied = 0;
	episodeNumber=0;
	adviceCount = 0;
	cutOffEpisode = false;


	initQ();
	
}

//void SeminarAIModule::endExperiment(){
  //myFile.close();
//}

void SeminarAIModule::onEnd(bool isWinner)
{
}

void SeminarAIModule::onFrame()
{
  if (show_visibility_data)
    drawVisibilityData();

  if (show_bullets)
    drawBullets();

  if (Broodwar->isReplay())
    return;

  drawStats();
  if (Broodwar->getFrameCount()%30==0){
	  //Broodwar->sendText("Beep");
	  
	  getState();
	  std::string result ("");
	  if (justEnded){
	    if (episodeNumber >= numEpisodes){
		    restartExperiment();
		}
		//Broodwar->sendText("about to startEpisode()");
		executeAction(sa->startEpisode(m_State, &result));
		//Broodwar->printf("startEpisode(): %s", result.c_str());

		//Original debug text
		//myFile << "startEpisode: " << episodeNumber << ": " << result.c_str() << "\r\n";

		numSteps = 0;
		episodeReward = 0.0;
		justEnded = false;
	  }
	  else{
		 // Broodwar->sendText("about to step() with STEPREWARD=%f", STEPREWARD);
		  if (numSteps > 1000){
			  cutOffEpisode = true;
			  //Broodwar->printf("cut off");
		  }else{
			  cutOffEpisode = false;
		  }
		  int a = sa->step(STEPREWARD,m_State, &result, episodeNumber, showLearning, cutOffEpisode);
		  episodeReward += STEPREWARD;
		  Broodwar->sendText("step(): %i, %s, action: %i", numSteps, result.c_str(), a);
		  Broodwar->printf("MAL SEMINAR 2014-2015");
		  Broodwar->printf("alpha: %f,lambda: %f",alpha,lambda);
		  executeAction(a);
		  numSteps++;
	  }
  }
}

void SeminarAIModule::onSendText(std::string text)
{
  if (text=="/show bullets")
  {
    show_bullets = !show_bullets;
  } else if (text=="/speedUp"){
	  Broodwar->setLocalSpeed(0);
  } else if (text=="/speedDown"){
	  Broodwar->setLocalSpeed(-1);
  } else if (text=="/humanOn"){
	  humanOn = true;
  } else if (text=="/humanOff"){
	  humanOn = false;
  } else if (text=="/show players")
  {
    showPlayers();
  } else if (text=="/show forces")
  {
    showForces();
  } else if (text=="/show visibility")
  {
    show_visibility_data=!show_visibility_data;
  } else
  {
    Broodwar->printf("You typed '%s'!",text.c_str());
    Broodwar->sendText("%s",text.c_str());
  }
}

void SeminarAIModule::onReceiveText(BWAPI::Player* player, std::string text)
{
  Broodwar->printf("%s said '%s'", player->getName().c_str(), text.c_str());
}

void SeminarAIModule::onPlayerLeft(BWAPI::Player* player)
{
  Broodwar->sendText("%s left the game.",player->getName().c_str());
}

void SeminarAIModule::onNukeDetect(BWAPI::Position target)
{
//  if (target!=Positions::Unknown)
//    Broodwar->printf("Nuclear Launch Detected at (%d,%d)",target.x(),target.y());
//  else
//    Broodwar->printf("Nuclear Launch Detected");
}

void SeminarAIModule::onUnitDiscover(BWAPI::Unit* unit)
{
//  if (!Broodwar->isReplay() && Broodwar->getFrameCount()>1)
//    Broodwar->sendText("A %s [%x] has been discovered at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
}

void SeminarAIModule::onUnitEvade(BWAPI::Unit* unit)
{
//  if (!Broodwar->isReplay() && Broodwar->getFrameCount()>1)
//    Broodwar->sendText("A %s [%x] was last accessible at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
}

void SeminarAIModule::onUnitShow(BWAPI::Unit* unit)
{
//  if (!Broodwar->isReplay() && Broodwar->getFrameCount()>1)
//    Broodwar->sendText("A %s [%x] has been spotted at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
}

void SeminarAIModule::onUnitHide(BWAPI::Unit* unit)
{
//  if (!Broodwar->isReplay() && Broodwar->getFrameCount()>1)
//    Broodwar->sendText("A %s [%x] was last seen at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
}

void SeminarAIModule::onUnitDestroy(BWAPI::Unit* unit)
{
  if (!Broodwar->isReplay() && Broodwar->getFrameCount()>1)
  {
    int iOwnHealth = 0;
	int iEnemyHealth = 0;
	if (unit->getPlayer()!=Broodwar->self()){
		iOwnHealth = getMyHealth();
		//Broodwar->sendText("I just killed a %s, for a total of %d units (I've died %d times), and have health left of %d",unit->getType().getName().c_str(), ++numKilled, numDied, iOwnHealth );
        ++numKilled;
	}
	else{
		iEnemyHealth = getEnemyHealth();
		++numDied;
		//Broodwar->sendText("My %s was just killed, for a total of %d dead units (I've killed %d), and enemy has health left of %d",unit->getType().getName().c_str(), ++numDied, numKilled, iEnemyHealth);

	}
    //Broodwar->sendText("about to endEpisode() with %d", (iOwnHealth - iEnemyHealth));
	//myFile << "trial: " << trialNumber << " endEpisode: " << episodeNumber << " deltaHealth: " << (iOwnHealth - iEnemyHealth) << " reward: " << (episodeReward + (double)(iOwnHealth - iEnemyHealth)) << " steps: " << numSteps << "\r\n";
	myFile << episodeNumber << "," << (episodeReward + (double)(iOwnHealth - iEnemyHealth)) << "," << numSteps << "\r\n";
	Broodwar->printf("trial %d, episode %d, reward %f (%d to %d)", trialNumber, episodeNumber, (episodeReward + (double)(iOwnHealth - iEnemyHealth)), numKilled, numDied);
	sa->endEpisode((double)iOwnHealth - iEnemyHealth, episodeNumber, showLearning, cutOffEpisode);
    justEnded = true;
	cutOffEpisode = false;
	adviceCount = 0;
	episodeNumber++;
  }
}


void SeminarAIModule::onUnitRenegade(BWAPI::Unit* unit)
{
  if (!Broodwar->isReplay())
    Broodwar->sendText("A %s [%x] is now owned by %s",unit->getType().getName().c_str(),unit,unit->getPlayer()->getName().c_str());
}

DWORD WINAPI AnalyzeThread()
{
  return 0;
}

void SeminarAIModule::drawStats()
{
  std::set<Unit*> myUnits = Broodwar->self()->getUnits();
  Broodwar->drawTextScreen(5,0,"I have %d units:",myUnits.size());
  std::map<UnitType, int> unitTypeCounts;
  for(std::set<Unit*>::iterator i=myUnits.begin();i!=myUnits.end();i++)
  {
    if (unitTypeCounts.find((*i)->getType())==unitTypeCounts.end())
    {
      unitTypeCounts.insert(std::make_pair((*i)->getType(),0));
    }
    unitTypeCounts.find((*i)->getType())->second++;
  }
  int line=1;
  for(std::map<UnitType,int>::iterator i=unitTypeCounts.begin();i!=unitTypeCounts.end();i++)
  {
    Broodwar->drawTextScreen(5,16*line,"- %d %ss",(*i).second, (*i).first.getName().c_str());
    line++;
  }
}

void SeminarAIModule::drawBullets()
{
  std::set<Bullet*> bullets = Broodwar->getBullets();
  for(std::set<Bullet*>::iterator i=bullets.begin();i!=bullets.end();i++)
  {
    Position p=(*i)->getPosition();
    double velocityX = (*i)->getVelocityX();
    double velocityY = (*i)->getVelocityY();
    if ((*i)->getPlayer()==Broodwar->self())
    {
      Broodwar->drawLineMap(p.x(),p.y(),p.x()+(int)velocityX,p.y()+(int)velocityY,Colors::Green);
      Broodwar->drawTextMap(p.x(),p.y(),"\x07%s",(*i)->getType().getName().c_str());
    }
    else
    {
      Broodwar->drawLineMap(p.x(),p.y(),p.x()+(int)velocityX,p.y()+(int)velocityY,Colors::Red);
      Broodwar->drawTextMap(p.x(),p.y(),"\x06%s",(*i)->getType().getName().c_str());
    }
  }
}

void SeminarAIModule::drawVisibilityData()
{
  for(int x=0;x<Broodwar->mapWidth();x++)
  {
    for(int y=0;y<Broodwar->mapHeight();y++)
    {
      if (Broodwar->isExplored(x,y))
      {
        if (Broodwar->isVisible(x,y))
          Broodwar->drawDotMap(x*32+16,y*32+16,Colors::Green);
        else
          Broodwar->drawDotMap(x*32+16,y*32+16,Colors::Blue);
      }
      else
        Broodwar->drawDotMap(x*32+16,y*32+16,Colors::Red);
    }
  }
}

void SeminarAIModule::drawTerrainData()
{
}

void SeminarAIModule::showPlayers()
{
  std::set<Player*> players=Broodwar->getPlayers();
  for(std::set<Player*>::iterator i=players.begin();i!=players.end();i++)
  {
    Broodwar->printf("Player [%d]: %s is in force: %s",(*i)->getID(),(*i)->getName().c_str(), (*i)->getForce()->getName().c_str());
  }
}

void SeminarAIModule::showForces()
{
  std::set<Force*> forces=Broodwar->getForces();
  for(std::set<Force*>::iterator i=forces.begin();i!=forces.end();i++)
  {
    std::set<Player*> players=(*i)->getPlayers();
    Broodwar->printf("Force %s has the following players:",(*i)->getName().c_str());
    for(std::set<Player*>::iterator j=players.begin();j!=players.end();j++)
    {
      Broodwar->printf("  - Player [%d]: %s",(*j)->getID(),(*j)->getName().c_str());
    }
  }
}

void SeminarAIModule::onUnitComplete(BWAPI::Unit *unit)
{
}
void SeminarAIModule::onSaveGame(std::string gameName)
{
}
void SeminarAIModule::onUnitMorph(BWAPI::Unit* unit)
{
}

void SeminarAIModule::onUnitCreate(BWAPI::Unit* unit)
{
}

int SeminarAIModule::getMyHealth()
{
	int total=0;
    for(std::set<Unit*>::const_iterator i=Broodwar->self()->getUnits().begin();i!=Broodwar->self()->getUnits().end();i++)
    {
		total += (*i)->getHitPoints();
    }
    return total;
}

int SeminarAIModule::getEnemyHealth()
{
	int total=0;
    for(std::set<Unit*>::const_iterator i=Broodwar->enemy()->getUnits().begin();i!=Broodwar->enemy()->getUnits().end();i++)
    {
		total += (*i)->getHitPoints();
    }
    return total;
}


//0 my X position (0-1000)
//1 my Y position (0-1000)
//2 Straight-line distance between me and enemy (0-1000)
//3 difference in hit points between me and enemy (-50 - 50)
//4 enemy is moving/attacking? (0 or 1)
//5 angle of enemy relative to me (pi - pi)
void SeminarAIModule::getState()
{
  //Broodwar->sendText("getState()");
  int i=0;
for (i =0;i<10;i++)
m_State[i]=0;

  i=0;
  for(std::set<Unit*>::const_iterator ownUnit=Broodwar->self()->getUnits().begin();ownUnit!=Broodwar->self()->getUnits().end();ownUnit++)
  {
	Position myPos = (*ownUnit)->getPosition();
	m_State[i++]=myPos.x(); //0
	m_State[i++]=myPos.y(); //1
	for(std::set<Unit*>::const_iterator enemyUnit=Broodwar->enemy()->getUnits().begin();enemyUnit!=Broodwar->enemy()->getUnits().end();enemyUnit++)
    {
		m_State[i++]=(*ownUnit)->getDistance(*enemyUnit); //2 Straight-line distance
		//m_State[i++]=BWTA::getGroundDistance( (*ownUnit)->getTilePosition(), (*enemyUnit)->getTilePosition() );
		m_State[i++]=(*ownUnit)->getHitPoints() - (*enemyUnit)->getHitPoints(); //3
		m_State[i++]=0; //4
		if ((*enemyUnit)->isAttacking() || (*enemyUnit)->isMoving() || (*enemyUnit)->isAttackFrame() || (*enemyUnit)->isStartingAttack())
			m_State[i-1]=1; //4
		Position enemyPos = (*enemyUnit)->getPosition(); 
		m_State[i++] = getAngle(myPos, enemyPos); //5
		//Broodwar->printf("dist %f, groundDist %f, angle %f ", m_State[2], BWTA::getGroundDistance( (*ownUnit)->getTilePosition(), (*enemyUnit)->getTilePosition()), m_State[5]);
	}
  }
  //myFile << "state: ";
  //for (int j=0; j<i; j++) {
//	  myFile << j << ": " << m_State[j] << "  ";
  //}
  //myFile << "\r\n";
}



void SeminarAIModule::initQ(){

  double ranges[ NUMFEATURES ];
  double minValues[ NUMFEATURES ];
  double resolutions[ NUMFEATURES ];

  Broodwar->sendText("initQ()");

  minValues[0] = 0.0;
  minValues[1] = 0.0;
  minValues[2] = 0.0;
  minValues[3] = -50.0;
  minValues[4] = 0.0;
  minValues[5]= -3.2;

  ranges[0] = 1000.0;
  ranges[1] = 10000.0;
  ranges[2] = 1000.0;
  ranges[3] = 50.0;
  ranges[4] = 1.0;
  ranges[5] = 3.2;

  resolutions[0] = dDISTDIVISOR;
  resolutions[1] = dDISTDIVISOR;
  resolutions[2] = dDISTDIVISOR;
  resolutions[3] = dHEALTHDIVISOR;
  resolutions[4] = 0.9;
  resolutions[5] = dRADIANDIVISOR;

  FunctionApproximator* FA;
 // CMAC* TeacherCMAC;

  FA          = new CMAC(NUMFEATURES, NUMACTIONS, ranges, minValues, resolutions);
 // TeacherCMAC = new CMAC(NUMFEATURES, NUMACTIONS, ranges, minValues, resolutions);
  
  isLearning = true; //CRITICAL TEMP
  std::string result;
  // Learn from scratch
  sa = new SarsaAgent(alpha,lambda,NUMFEATURES, NUMACTIONS, isLearning, FA, &result, "");
  
  // Test loading weights from a file
  // sa = new SarsaAgent(alpha,lambda,NUMFEATURES, NUMACTIONS, isLearning, FA, &result, "weights_250_0.wts");
  
  //Original debug text
  //myFile << "startEpisode: " << episodeNumber << ": " << result.c_str() << "\r\n";
  Broodwar->printf("%s", result.c_str());
	
}

/*
void SeminarAIModule::episodeStart(){
	getState();
  if (m_State->isInitialized){
	  Broodwar->sendText("START enemyDist:%d enemyHitPoints:%d hitPoints:%d",  
						   m_State->iDiscretizedDistanceToEnemy, 
						   m_State->iDiscretizedEnemyHitPoints,
						   m_State->iDiscretizedHitPoints);
	  selectAction();
  }
}

void SeminarAIModule::episodeStep(){
	float fReward = STEPREWARD;
 getState();
 if (m_State->isInitialized){
	 Broodwar->sendText("STEP enemyDist:%d enemyHitPoints:%d hitPoints:%d reward:%f",  
							   m_State->iDiscretizedDistanceToEnemy, 
							   m_State->iDiscretizedEnemyHitPoints,
							   m_State->iDiscretizedHitPoints,
							   fReward);
	 selectAction();
 }
 }

void SeminarAIModule::episodeEnd(int iMyHealth, int iEnemyHealth){
/*
	float fReward = (float)iMyHealth - (float) iEnemyHealth;
  getState();
  if (m_State->isInitialized)
	  Broodwar->sendText("END enemyDist:%d enemyHitPoints:%d hitPoints:%d reward:%f",  
						   m_State->iDiscretizedDistanceToEnemy, 
						   m_State->iDiscretizedEnemyHitPoints,
						   m_State->iDiscretizedHitPoints,
						   fReward);

}
*/

//action 0: stop
//action 1: Attack enemyUnit
//action 2: move towards enemyUnit
//action 3: move South
//action 4: move North
//action 5: move East
//action 6: move West
void SeminarAIModule::executeAction(int iActionNum){

	//Broodwar->printf("executeAction: %d", iActionNum);
	if (!humanOn){


		for(std::set<Unit*>::const_iterator ownUnit=Broodwar->self()->getUnits().begin();ownUnit!=Broodwar->self()->getUnits().end();ownUnit++)
		{
			if (1 && (iActionNum == 1 || iActionNum == 2)) {
  			  
			  for(std::set<Unit*>::const_iterator enemyUnit=Broodwar->enemy()->getUnits().begin();enemyUnit!=Broodwar->enemy()->getUnits().end();enemyUnit++)
			  {
				  if ((*enemyUnit)->isVisible()){
					if (iActionNum == 1){
						(*ownUnit)->attack((*enemyUnit)->getPosition());
					}
					else if (iActionNum == 2){
						(*ownUnit)->move((*enemyUnit)->getPosition());
					}
				  }
				  else{
					(*ownUnit)->stop();
				  }
			  }
			}
			else if (iActionNum == 0){
				(*ownUnit)->stop();
			}		
			else if (iActionNum >= 3 && iActionNum <= 6){
				Position* myPos = new Position((*ownUnit)->getPosition());
				Position* newPos;
				switch(iActionNum){
					case 3:
						newPos = new Position(myPos->x(), myPos->y()+30);
						(*ownUnit)->move(*newPos);
						break;
					case 4:
						newPos = new Position(myPos->x(), myPos->y()-30);
						(*ownUnit)->move(*newPos);
						break;
					case 5:
						newPos = new Position(myPos->x()+30, myPos->y());
						(*ownUnit)->move(*newPos);
						break;
					case 6:
						newPos = new Position(myPos->x()-30, myPos->y());
						(*ownUnit)->move(*newPos);
				}
				//delete myPos;
				//delete newPos;
				//Broodwar->printf("executeAction: %d from %d %d to %d %d", iActionNum, myPos->x(), myPos->y(), newPos->x(), newPos->y());
			}
		}
	}
}


double SeminarAIModule::getAngle(Position myPos, Position enemyPos){
  double deltaX = myPos.x() - enemyPos.y();
  double deltaY = myPos.y() - enemyPos.y();
  //tan(theta) = opposite/adjacent = delta y / delta x
  return atan2(deltaY, deltaX);
}
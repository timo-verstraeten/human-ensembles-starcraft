#include "SarsaAgent.h"


using namespace std;
extern bool advicegiven;
// return a random integer in [0, maxRange)
inline int range_rand( int maxRange )
{
	return (int) ( (((double)rand()) / RAND_MAX) * maxRange );
}

inline int range_rand( int minRange, int maxRange )
{
	return range_rand( maxRange - minRange ) + minRange;
}

SarsaAgent::SarsaAgent( double alph, double lambd, int numFeatures, int numActions, bool bLearn,
					   FunctionApproximator *anFA,
					   std::string* returnNote,
					   char *loadWeightsFile
					   ): 
SMDPAgent( numFeatures, numActions )
{

	bLearning = bLearn;
	steps = 0;
	totalAdvice = 0;
	adviceCount = 0;
	exploreCount = 0;

	alpha = alph;//0.05;
	gamma = 1.0;
	lambda = lambd;
	epsilon = 0.1;

	epochNum = 0;
	lastAction = -1;

	FA = anFA;
	std::ostringstream os;

	if ( strlen( loadWeightsFile ) > 0 )
		loadWeights( loadWeightsFile );

	bFollowTeacher = false;
	

    *returnNote += os.str(); //tempString;

}

int SarsaAgent::startEpisode( double state[], std::string* returnNote )
{
	std::string tempString;
	FA->decayTraces( 0 );
	FA->setState( state );
	std::ostringstream os;
	os << epochNum;
	for ( int a = 0; a < getNumActions(); a++ ) {
		Q[ a ] = FA->computeQ( a );
		os << " Q[" << a << "]=" << Q[a];
	}
	//*returnNote += os.str(); //tempString;

	lastAction = selectAction(returnNote);


	FA->updateTraces( lastAction );

	return lastAction;
}

int SarsaAgent::step( double reward, double state[], std::string* returnNote, int episodeNumber,
					  bool showLearning, bool cutOffEpisode )
{
	int effectiveEp;
	steps++;
	std::ostringstream os; //TEMP
	os <<endl<<" REWARD: " << reward;
		*returnNote += os.str();
	double delta = reward - Q[ lastAction ];
	FA->setState( state );
	for ( int a = 0; a < getNumActions(); a++ ) {
		Q[ a ] = FA->computeQ( a );
		os <<endl<< Q[a]; //TEMP
		*returnNote += os.str();
	}
	
	// Hook to end episode after too many steps
	if (cutOffEpisode == true){
		// move towards the enemy
		*returnNote += " Trying to die";
		return 2;

	// If this episode we are showing what we know, get our action and return it immediatly
	}else if(showLearning && ((episodeNumber % 2) != 0)){
		*returnNote += " NoLearn";
		lastAction = selectAction(returnNote);
		return lastAction;
	}


	lastAction = selectAction(returnNote);


	if ( !bLearning ){
		*returnNote = *returnNote + " NoLearn";
		return lastAction;
	}

	//Learn based on lastAction
	delta += Q[ lastAction ];
	FA->updateWeights( delta, alpha );
	Q[ lastAction ] = FA->computeQ( lastAction ); // need to redo because weights changed
	FA->decayTraces( gamma * lambda );

	for ( int a = 0; a < getNumActions(); a++ ) {  //clear other than F[a]
		if ( a != lastAction ) {
			FA->clearTraces( a );
		}
	}
	FA->updateTraces( lastAction );  //replace/set traces F[a]

	return lastAction;
}

void SarsaAgent::endEpisode( double reward, int episodeNum, bool showLearning, bool cutOffEpisode ){

	epochNum++;
	if ( bLearning && lastAction != -1 ) { /* otherwise we never ran on this episode */

		/* finishing up the last episode */
		/* assuming gamma = 1  -- if not,error*/
		if ( gamma != 1.0)
			cerr << "We're assuming gamma's 1" << endl;
		if (!cutOffEpisode && !(showLearning && (episodeNum % 2 != 0))){
			double delta = reward - Q[ lastAction ];
			FA->updateWeights( delta, alpha );
		}
		ofstream log;
		log.open("steps.log");
		log << steps;
		log.close();
	}
	lastAction = -1;
}

int SarsaAgent::selectAction(std::string* returnNote)
{
	int action;
	*returnNote += " In Select";
	// Epsilon-greedy
	if (((double)rand() / RAND_MAX) < epsilon) {
		/* explore */
		action = range_rand( getNumActions() );
		*returnNote += " RandA";
	}
	else{
		action = argmaxQ(returnNote);
		*returnNote += " ArgMax";
	}
	return action;
}

bool SarsaAgent::loadWeights( char *filename )
{
	//int file = open( filename, O_RDONLY );
	ifstream file;
	file.open(filename);

	FA->read( &file );
	file.close();
	return false;
}

bool SarsaAgent::saveWeights( char *filename )
{
	//int file = open( filename, O_CREAT | O_WRONLY, 0664 );
	ofstream file;
	file.open(filename);

	FA->write( &file );
	file.close();
	return false;
}

// Returns index (action) of largest entry in Q array, breaking ties randomly 
int SarsaAgent::argmaxQ(std::string* returnNote)
{
	int bestAction = 0;
	double bestValue = Q[ bestAction ];
	int numTies = 0;
	for ( int a = bestAction + 1; a < getNumActions(); a++ ) {
		double value = Q[ a ];
		if ( value > bestValue ) {
			bestValue = value;
			bestAction = a;
		}
		else if ( value == bestValue ) {
			numTies++;
			if ( range_rand( numTies + 1 ) == 0 ) {
				bestValue = value;
				bestAction = a;
			}
		}
	}

	//*returnNote += " bestQ=";
	//std::ostringstream os;
	//os << bestValue;
	//std::string str = os.str();
	//*returnNote += str;
	return bestAction;
}


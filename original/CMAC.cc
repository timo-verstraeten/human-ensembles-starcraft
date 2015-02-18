#include "CMAC.h"

#define TILINGS_PER_GROUP 1

using namespace std;
std::ofstream ofs("gap.txt", std::ofstream::app); //TEST
// return a random integer in [0, maxRange)
inline int range_rand( int maxRange )
{
	return (int) ( (((double)rand()) / RAND_MAX) * maxRange );
}


CMACBase::CMACBase( int numF, int numA, double r[], double m[], double res[] ):
FunctionApproximator( numF, numA, r, m, res )
{
	minimumTrace = 0.01;

	numNonzeroTraces = 0;
	for ( int i = 0; i < RL_MEMORY_SIZE; i++ ) {
		weights[ i ] = 0;
		traces[ i ] = 0;
	}

	srand( (unsigned int) 0 );
	int tmp[ 2 ];
	float tmpf[ 2 ];
	colTab = new collision_table( RL_MEMORY_SIZE, 1 );

	GetTiles( tmp, 1, 1, tmpf, 0 );  // A dummy call to set the hashing table    
}


int CMAC::getTeacherSuggestion(double s[], int numActions)
{
		
	double Q[10];
	setState( s );
	for ( int a = 0; a < numActions; a++ ) {
		Q[ a ] = computeQ( a );
	}
	for ( int a = 0; a < numActions; a++ ) {
		clearTraces(a);
	}
	int bestAction = 0;
	double bestValue = Q[ bestAction ];

//for (int i =0;i<10;i++) //TEMP
//ofs <<Q[i]<<"is"<<i<<" ";
//ofs<<endl;

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

//for (int i =0;i<10;i++)
//ofs <<Q[i]<<" ";
//ofs<<endl;



	return bestAction;
}

int CMAC::getTeacherSuggestion(double s[], int numActions,double threshold,int Qindex,int studentAction)
{
//	bool mistakeCorrecting,stateImportance;
//	
//	if ((Qindex==9) || (Qindex == 1))
//		stateImportance = true;
//	else
//		stateImportance = false;
//
//	if (studentAction != -1)
//		mistakeCorrecting = true;
//	else
//		mistakeCorrecting = false;
//
//
//	double Q[10];
//	setState( s );
//	for ( int a = 0; a < numActions; a++ ) {
//		Q[ a ] = computeQ( a );
//	}
//	for ( int a = 0; a < numActions; a++ ) {
//		clearTraces(a);
//	}
//	int bestAction = 0;
//	double bestValue = Q[ bestAction ];
//
////	for (int i =0;i<10;i++) //TEMP
////ofs <<Q[i]<<"is"<<i<<" ";
////ofs<<endl;
//
//findBestQ(Q,10,bestValue,bestAction);
//
////int numTies = 0;
////	for ( int a = bestAction + 1; a < getNumActions(); a++ ) {
////		double value = Q[ a ];
////		if ( value > bestValue ) {
////			bestValue = value;
////			bestAction = a;
////		}
////		else if ( value == bestValue ) {
////			numTies++;
////			if ( range_rand( numTies + 1 ) == 0 ) {
////				bestValue = value;
////				bestAction = a;
////			}
////		}
////	}
//
//for (int i =0;i<10;i++)
//ofs <<Q[i]<<" ";
//ofs<<endl;
//
//
//
//	return bestAction;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool mistakeCorrecting,stateImportance;
	
	if ((Qindex==9) || (Qindex == 1))
		stateImportance = true;
	else
		stateImportance = false;

	if (studentAction != -1)
		mistakeCorrecting = true;
	else
		mistakeCorrecting = false;

	const int length = 7;
	double Q[7];

	setState( s );
	for ( int a = 0; a < numActions; a++ ) {
		Q[ a ] = computeQ( a );
	}
	for ( int a = 0; a < numActions; a++ ) {
		clearTraces(a);
	}
//
int bestAction = 0;
double bestValue =0;
//for (int i =0;i<10;i++) //TEMP
//ofs <<Q[i]<<"is"<<i<<" ";
//ofs<<endl;
findBestQ(Q,length,bestValue,bestAction);


int compAction = 0; // initializing the value and the action with which the best will be COMPared
double compValue =0;

if (stateImportance)
if (Qindex==9)
{
	double tempQ[length];
	for (int i = 0;i<length;i++)
	{
		tempQ[i] = -Q[i]; //the best action of the opposite array is the worst action
	//ofs<<tempQ[i]<<endl;
	}
	findBestQ(tempQ,length,compValue,compAction);
	compValue *= -1;
//ofs<<bestAction<<"*"<<compAction<<"*"<<bestValue - compValue<<endl;

}
else if(Qindex == 1)
{
	double tempQ[length];
	for (int i = 0;i<length;i++)
	{
		if (i == bestAction)
		tempQ[i] = -1000; //make it the worst
		else
		tempQ[i] = Q[i];
	}
	findBestQ(tempQ,length,compValue,compAction); //the best action of the array with its original best action removed, is the 2nd best action
}

//threshold  =-1000; //TEST

int suggestion = bestAction;


if (((stateImportance) && ((bestValue - compValue) < threshold)) || ((mistakeCorrecting) && (studentAction==bestAction)))
suggestion = -1;



//ofs<<"mistake"<<mistakeCorrecting<<"Importance"<<stateImportance<<"sugg"<<suggestion<<"stud"<<studentAction<<"best"<<bestAction<<"*"<<bestValue<<"comp"<<compAction<<"*"<<compValue<<endl;


return suggestion;	


}



void CMAC::findBestQ(double Q[], int length,double &Value,int &Action)
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
Value = bestValue;
Action = bestAction;
return;
//
//int bestAction = 0;
//	double bestValue = Q[ bestAction ];
//	int numTies = 0;
//	for ( int a = bestAction + 1; a < length; a++ ) {
//		double value = Q[ a ];
//		if ( value > bestValue ) {
//			bestValue = value;
//			bestAction = a;
//		}
//		else if ( value == bestValue ) {
//			numTies++;
//			if ( range_rand( numTies + 1 ) == 0 ) {
//				bestValue = value;
//				bestAction = a;
//			}
//		}
//	}
//Value = bestValue;
//Action = bestAction;
//return;
}



void CMACBase::setState( double s[] )
{
	FunctionApproximator::setState( s );

	loadTiles();
}

void CMACBase::updateWeights( double delta, double alpha )
{
	double tmp = delta * alpha / numTilings;
	for ( int i = 0; i < numNonzeroTraces; i++ ) {
		int f = nonzeroTraces[ i ];
		if ( f > RL_MEMORY_SIZE || f < 0 )
			cerr << "f is too big or too small!!" << f << endl;
		weights[ f ] += tmp * traces[ f ];
	}
}

// Decays all the (nonzero) traces by decay_rate, removing those below minimum_trace 
void CMACBase::decayTraces( double decayRate )
{
	int f;
	for ( int loc = numNonzeroTraces - 1; loc >= 0; loc-- ) {
		f = nonzeroTraces[ loc ];
		if ( f > RL_MEMORY_SIZE || f < 0 )
			cerr << "DecayTraces: f out of range " << f << endl;
		traces[ f ] *= decayRate;
		if ( traces[ f ] < minimumTrace )
			clearExistentTrace( f, loc );
	}
}

// Clear any trace for feature f      
void CMACBase::clearTrace( int f)
{
	if ( f > RL_MEMORY_SIZE || f < 0 )
		cerr << "ClearTrace: f out of range " << f << endl;
	if ( traces[ f ] != 0 )
		clearExistentTrace( f, nonzeroTracesInverse[ f ] );
}

// Clear the trace for feature f at location loc in the list of nonzero traces 
void CMACBase::clearExistentTrace( int f, int loc )
{
	if ( f > RL_MEMORY_SIZE || f < 0 )
		cerr << "ClearExistentTrace: f out of range " << f << endl;
	traces[ f ] = 0.0;
	numNonzeroTraces--;
	nonzeroTraces[ loc ] = nonzeroTraces[ numNonzeroTraces ];
	nonzeroTracesInverse[ nonzeroTraces[ loc ] ] = loc;
}

// Set the trace for feature f to the given value, which must be positive   
void CMACBase::setTrace( int f, double newTraceValue )
{
	if ( f > RL_MEMORY_SIZE || f < 0 )
		cerr << "SetTraces: f out of range " << f << endl;
	if ( traces[ f ] >= minimumTrace )
		traces[ f ] = newTraceValue;         // trace already exists              
	else {
		while ( numNonzeroTraces >= RL_MAX_NONZERO_TRACES )
			increaseMinTrace(); // ensure room for new trace              
		traces[ f ] = newTraceValue;
		nonzeroTraces[ numNonzeroTraces ] = f;
		nonzeroTracesInverse[ f ] = numNonzeroTraces;
		numNonzeroTraces++;
	}
}

// Set the trace for feature f to the given value, which must be positive   
void CMACBase::updateTrace( int f, double deltaTraceValue )
{
	setTrace( f, traces[ f ] + deltaTraceValue );
}

// Try to make room for more traces by incrementing minimum_trace by 10%,
// culling any traces that fall below the new minimum                      
void CMACBase::increaseMinTrace()
{
	minimumTrace *= 1.1;
	cerr << "Changing minimum_trace to " << minimumTrace << endl;
	for ( int loc = numNonzeroTraces - 1; loc >= 0; loc-- ) { // necessary to loop downwards    
		int f = nonzeroTraces[ loc ];
		if ( traces[ f ] < minimumTrace )
			clearExistentTrace( f, loc );
	}
}

void CMACBase::read( ifstream* file )
{
	//::read( fd, (char *) weights, RL_MEMORY_SIZE * sizeof(double) );
	//colTab->restore( fd );

	for (int i=0; i<RL_MEMORY_SIZE; i++) {
		*file >> weights[i];
	}
	colTab->restore( file );
}

void CMACBase::write( ofstream* file )
{
	for (int i=0; i<RL_MEMORY_SIZE; i++) {
		*file << weights[i] << ' ';
	}
	//::write( fd, (char *) weights, RL_MEMORY_SIZE * sizeof(double) );
	//colTab->save( fd );
	//colTab->save(file);
}


///////////////////////////////

CMAC::CMAC( int numF, int numA, double r[], double m[], double res[] ):
CMACBase( numF, numA, r, m, res ) {}

void CMAC::loadTiles()
{
	int tilingsPerGroup = TILINGS_PER_GROUP;  /* num tilings per tiling group */
	numTilings = 0;

	/* These are the 'tiling groups'  --  play here with representations */
	/* One tiling for each state variable */
	for ( int v = 0; v < getNumFeatures(); v++ ) {
		for ( int a = 0; a < getNumActions(); a++ ) {
			GetTiles1( &(tiles[ a ][ numTilings ]), tilingsPerGroup, colTab,
				(float) (state[ v ] / getResolution( v )), a , v );
		}  
		numTilings += tilingsPerGroup;
	}
	if ( numTilings > RL_MAX_NUM_TILINGS )
		cerr << "TOO MANY TILINGS! " << numTilings << endl;
}

double CMAC::computeQ( int action )
{
	double q = 0;
	for ( int j = 0; j < numTilings; j++ ) {
		q += weights[ tiles[ action ][ j ] ];
	}
	return q;
	// return q - some constant
}

void CMAC::clearTraces( int action )
{
	for ( int j = 0; j < numTilings; j++ )
		clearTrace( tiles[ action ][ j ] );
}

void CMAC::updateTraces( int action )
{
	for ( int j = 0; j < numTilings; j++ )      //replace/set traces F[a]
		setTrace( tiles[ action ][ j ], 1.0 );  
}

///////////////////////////////////

CMAC_RBF::CMAC_RBF( int numF, int numA, double r[], double m[], double res[] ):
CMACBase( numF, numA, r, m, res ) 
{
	RBF_spread = 4;
	RBF_sigma = 0.25; //*met 5/20/05 .5;
	RBF_tiles = (int)(RBF_spread * RBF_sigma + 0.5);
}

void CMAC_RBF::loadTiles()
{
	int tilingsPerGroup = TILINGS_PER_GROUP;  /* num tilings per tiling group */
	double tiling_displace = 1.0 / tilingsPerGroup;  // YL
	numTilings = 0;

	/* These are the 'tiling groups'  --  play here with representations */
	/* One tiling for each state variable */
	for ( int v = 0; v < getNumFeatures(); v++ ) {
		double nx = state[ v ] / getResolution( v );
		for ( int a = 0; a < getNumActions(); a++ ) {
			for ( int i = -RBF_tiles; i <= RBF_tiles; i++ ) {
				GetTiles1( &(tiles[ a ][ i + RBF_tiles ][ numTilings ]), 
					tilingsPerGroup, colTab, (float)nx + i, a, v );

				for ( int k = 0; k < tilingsPerGroup; k++ ) {
					// the center for this tile
					double c = floor(nx) + 0.5 + k * tiling_displace;
					normalizedDistances[ a ][ i + RBF_tiles ][ numTilings + k ] 
					= fabs( nx + i - c );
				}
			}
		}
		numTilings += tilingsPerGroup;
	}
	if ( numTilings > RL_MAX_NUM_TILINGS )
		cerr << "TOO MANY TILINGS! " << numTilings << endl;
}

///////////////////

void CMAC_RBF::clearTraces( int action )
{
	for ( int i = 0; i <= 2*RBF_tiles + 1; i++ )
		for ( int j = 0; j < numTilings; j++ )
			clearTrace( tiles[ action ][ i ][ j ] );
}

void CMAC_RBF::updateTraces( int action )
{
	for ( int i = 0; i <= 2*RBF_tiles + 1; i++ )
		for ( int j = 0; j < numTilings; j++ )      //replace/set traces F[a]
			setTrace( tiles[ action ][ i ][ j ], 
			rbf( normalizedDistances[ action ][ i ][ j ] ) );
}

////////////////////////////

double CMAC_RBF::computeQ( int action )
{
	double q = 0;
	// normalizedDistances are calculated in LoadTiles
	for ( int i = 0; i <= 2*RBF_tiles + 1; i++ ) {
		for ( int j = 0; j < numTilings; j++ ) {
			q += weights[ tiles[ action ][ i ][ j ] ] 
			* rbf( normalizedDistances[ action ][ i ][ j ] );
		}
	}

	return q;
}


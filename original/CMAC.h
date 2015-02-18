
#ifndef CMAC_H
#define CMAC_H

#include <math.h>
#include "FuncApprox.h"
#include <iostream>
#include <fstream>

#define RL_MEMORY_SIZE 1048576
#define RL_MAX_NONZERO_TRACES 100000
#define RL_MAX_NUM_TILINGS 6000

#include "tiles2.h"

class CMACBase: public FunctionApproximator
{
  double minimumTrace;
  int nonzeroTraces[ RL_MAX_NONZERO_TRACES ];
  int numNonzeroTraces;
  int nonzeroTracesInverse[ RL_MEMORY_SIZE ];

 protected:
  double weights[ RL_MEMORY_SIZE ];
  double traces [ RL_MEMORY_SIZE ];

  int numTilings;

  collision_table *colTab;

  void clearTrace( int f );
  void clearExistentTrace( int f, int loc );
  void setTrace( int f, double newTraceValue );
  void updateTrace( int f, double deltaTraceValue );
  void increaseMinTrace();

  virtual void loadTiles() = 0;

 public:
  CMACBase( int numF, int numA, double r[], double m[], double res[] );
  virtual void setState( double s[] );

  virtual void updateWeights( double delta, double alpha );

  virtual void decayTraces( double decayRate );

  //virtual void read ( int fd );
  //virtual void write( int fd );
  virtual void read ( std::ifstream* file );
  virtual void write( std::ofstream* file );


};

class CMAC: public CMACBase
{
  int tiles[ MAX_ACTIONS ][ RL_MAX_NUM_TILINGS ];

 protected:
  virtual void loadTiles();

 public:
  CMAC( int numF, int numA, double r[], double m[], double res[] );
  virtual int getTeacherSuggestion(double s[], int numActions);
  virtual int getTeacherSuggestion(double s[], int numActions,double threshold,int Qindex,int studentAction);

  virtual double computeQ( int action );

  virtual void clearTraces( int action );
  virtual void updateTraces( int action );

private:
  void findBestQ(double Q[], int length,double &Value,int &Action);


};

#define MAX_RBF_SPREAD 50

class CMAC_RBF: public CMACBase
{
  int RBF_spread;
  double RBF_sigma;
  int RBF_tiles;

  int tiles[ MAX_ACTIONS ][ MAX_RBF_SPREAD ][ RL_MAX_NUM_TILINGS ];
  double normalizedDistances[ MAX_ACTIONS ][ MAX_RBF_SPREAD ][ RL_MAX_NUM_TILINGS ];

 protected:
  virtual void loadTiles();

  inline double rbf( double x ) { x /= RBF_sigma;   return exp(- x*x/2); }

 public:
  CMAC_RBF( int numF, int numA, double r[], double m[], double res[] );

  virtual double computeQ( int action );

  virtual void clearTraces( int action );
  virtual void updateTraces( int action );


};

#endif

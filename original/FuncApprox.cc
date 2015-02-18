
#include "FuncApprox.h"

FunctionApproximator::FunctionApproximator( int numF, int numA, 
					    double r[],
					    double m[],
					    double res[] )
{
  numFeatures = numF;
  numActions  = numA;

  for ( int i = 0; i < numFeatures; i++ ) {
    ranges     [ i ] = r  [ i ];
    minValues  [ i ] = m  [ i ];
    resolutions[ i ] = res[ i ];
  }
}


void FunctionApproximator::setState( double s[] )
{
  for ( int i = 0; i < numFeatures; i++ ) {
    state[ i ] = s[ i ];
  }
}

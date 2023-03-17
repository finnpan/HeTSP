#include <assert.h>
#include <random>

#include "indi.h"

void Utils::Permutation(int* a, int n)
{
    int r = 0;
    for (int i = 0; i < n; ++i) {
        a[i] = i;
    }
    for (int i = 0; i < n - 1; ++i) {
        r = rand() % (n - (i + 1)) + i + 1;
        std::swap(a[i], a[r]);
    }
}

Indi::Indi()
{                
  fN = 0;
  fLink = NULL;
  fEvaluationValue = 0;
}
 
Indi::~Indi()
{
  for ( int i = 0; i < fN; ++i ) 
    delete[] fLink[ i ];
  delete[] fLink;
}

void Indi::Define( int N )
{
  fN = N;
  
  fLink = new int* [ fN ];
  for( int i = 0; i < fN; ++i ) 
    fLink[ i ] = new int [ 2 ];
} 

Indi& Indi::operator = ( const Indi& src )
{
  fN = src.fN;

  for ( int i = 0; i < fN; ++i ) 
    for ( int j = 0; j < 2; ++j ) 
      fLink[i][j] = src.fLink[i][j];
  fEvaluationValue = src.fEvaluationValue;

  return *this;
}

bool Indi::operator == ( const Indi& src )
{
  int curr,next,pre;

  if( fN != src.fN )  
    return false;
  if( fEvaluationValue != src.fEvaluationValue )  
    return false;
  
  curr = 0;
  pre = -1;
  for( int i = 0; i < fN; ++i )
  {
    if( fLink[curr][0] == pre ) 
      next = fLink[curr][1];
    else 
      next = fLink[curr][0];
	
    if( src.fLink[curr][0] != next && src.fLink[curr][1] != next ) 
    {
      return false;
    }

    pre = curr;    
    curr = next; 
  }

  return true;
}


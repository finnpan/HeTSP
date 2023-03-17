#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "Evaluator.h"
#include "Cross.h"
#include "Kopt.h"
#include "env.h"

Environment::Environment()
{
  fEvaluator = new Evaluator();
  fIndexForMating = nullptr;
  tCurPop = nullptr;
  fCross = nullptr;
  fKopt = nullptr;
}


Environment::~Environment()
{
  delete fEvaluator;
  delete [] fIndexForMating;
  delete [] tCurPop;
  delete fCross;
  delete fKopt;
}


void Environment::Define()
{
  srand(1111);
  fEvaluator->SetInstance( fFileNameTSP );
  int N = fEvaluator->Ncity;

  fIndexForMating = new int [ fNumOfPop + 1 ];  

  tCurPop = new Indi [ fNumOfPop ];
  for ( int i = 0; i < fNumOfPop; ++i )
    tCurPop[i].Define( N );

  tBest.Define( N );

  fCross = new Cross( N );
  fCross->eval = fEvaluator;                 
  fCross->fNumOfPop = fNumOfPop;             

  fKopt = new Kopt( N );
  fKopt->eval = fEvaluator;
  fKopt->SetInvNearList(); 
}


void Environment::DoIt()
{
  this->fTimeStart = clock();   

  this->InitPop();                       

  this->fTimeInit = clock();    

  this->Init();

  while( 1 )
  {
    this->SetAverageBest();
    printf( "%d: %lld %lf\n", fCurNumOfGen, fBestValue, fAverageValue );

    if( this->TerminationCondition() ) break;

    this->SelectForMating();

    for( int s =0; s < fNumOfPop; ++s )
    {
      this->GenerateKids( s );     ; 
    }
    ++fCurNumOfGen;
  }

  this->fTimeEnd = clock();   
}
 

void Environment::Init()
{
  fAccumurateNumCh = 0;
  fCurNumOfGen = 0;
  fStagBest = 0;
} 


bool Environment::TerminationCondition()
{
  if ( fAverageValue - fBestValue < 0.001 )  
    return true;

  if (fStagBest > int(1500 / fNumOfKids)) {
      return true;
  }

  return false;
}


void Environment::SetAverageBest() 
{
  EvalType stockBest = tBest.fEvaluationValue;
  
  fAverageValue = 0.0;
  fBestIndex = 0;
  fBestValue = tCurPop[0].fEvaluationValue;
  
  for(int i = 0; i < fNumOfPop; ++i ){
    fAverageValue += tCurPop[i].fEvaluationValue;
    if( tCurPop[i].fEvaluationValue < fBestValue ){
      fBestIndex = i;
      fBestValue = tCurPop[i].fEvaluationValue;
    }
  }
  
  tBest = tCurPop[ fBestIndex ];
  fAverageValue /= (double)fNumOfPop;

  if (tBest.fEvaluationValue < stockBest) {
      fStagBest = 0;
  }
  else {
      fStagBest++;
  }
}


void Environment::InitPop()
{
  for ( int i = 0; i < fNumOfPop; ++i ){ 
    fKopt->MakeRandSol( tCurPop[ i ] );    /* Make a random tour */
    fKopt->DoIt( tCurPop[ i ] );           /* Apply the local search with the 2-opt neighborhood */ 
  }
}


void Environment::SelectForMating()
{
  /* fIndexForMating[] <-- a random permutation of 0, ..., fNumOfPop-1 */
  Utils::Permutation( fIndexForMating, fNumOfPop ); 
  fIndexForMating[ fNumOfPop ] = fIndexForMating[ 0 ];
}


void Environment::GenerateKids( int s )
{
  fCross->SetParents( tCurPop[fIndexForMating[s]], tCurPop[fIndexForMating[s+1]], fNumOfKids );  
  
  /* Note: tCurPop[fIndexForMating[s]] is replaced with a best offspring solutions in tCorss->DoIt(). 
     fEegeFreq[][] is also updated there. */
  fCross->DoIt( tCurPop[fIndexForMating[s]], tCurPop[fIndexForMating[s+1]], fNumOfKids, 1 );

  fAccumurateNumCh += fCross->fNumOfGeneratedCh;
}

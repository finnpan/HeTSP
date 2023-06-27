#ifndef __ENVIRONMENT__
#define __ENVIRONMENT__

#include "time.h"
#include "indi.h"

class Evaluator;
class Kopt;
class Cross;
class Environment {
public:
  Environment(); 
  ~Environment();
  void Define();                         /* Define the variables */

  void DoIt();                           /* Main procedure of the GA */
  void Init();                           /* Initialization of the GA */
  bool TerminationCondition();           /* Decide whether to proceed to next stage 
					    (or treminate the GA) */
  void SetAverageBest();                 /* Compute average and best tour lengths of the population */
  void InitPop();                        /* Create an initial population */
  void SelectForMating();                /* Determine a set of pairs of parents at each generation */
  void GenerateKids( int s );            /* Generate offspring solutions from a selected pair of  
                                            parents. Selection for survival is also performed here. */

  Evaluator* fEvaluator;                /* Distance of the edges */
  Cross* fCross;                        /* Eede assembly crossover */
  Kopt* fKopt;                          /* Local search with the 2-opt neighborhood */
  const char *fFileNameTSP;                    /* File name of an TSP instance */

  int fNumOfPop;                         /* Number of population members (N_pop in the paper) */
  int fNumOfKids;                        /* Number of offspring solutions (N_ch in the paper) */
  Indi* tCurPop;                        /* Current population members */
  Indi tBest;                           /* Best solution in the current population */
  int fCurNumOfGen;                      /* The current number of generations */
  long int fAccumurateNumCh;             /* The accumulated number of offspring solutions */

  int fStagBest;
  double fAverageValue;                  /* The average tour lengths of the population */
  EvalType fBestValue;                        /* The tour lenght of the best tour in the population */
  int fBestIndex;                        /* Index of the best population member */
  int* fIndexForMating;                  /* Mating list (r[] in the paper) */

  clock_t fTimeStart, fTimeInit, fTimeEnd;  /* Use them to measure the execution time */
};


#endif


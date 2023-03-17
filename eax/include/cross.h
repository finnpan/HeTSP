#ifndef __Cross__
#define __Cross__

#include "indi.h"

class Evaluator;
class Cross {
public:
  Cross( int N );
  ~Cross();
  void SetParents( const Indi& tPa1, const Indi& tPa2,     /* Set information of the parent tours */
		   int numOfKids ); 
  void DoIt( Indi& tKid, Indi& tPa2, int numOfKids,        /* Main procedure of EAX */
			int flagP ); 

  int fNumOfGeneratedCh;
  const Evaluator* eval;
  int fNumOfPop;

private:
  void SetABcycle( const Indi& parent1, const Indi& parent2, /* Step 2 of EAX */
		   int numOfKids );
  void FormABcycle();                                   /* Store an AB-cycle found */
  void ChangeSol( Indi& tKid, int ABnum, int type );   /* Apply an AB-cycle to an intermediate solution */
  void MakeCompleteSol( Indi& tKid );                  /* Step 5 of EAX */
  void MakeUnit();                                      /* Step 5-1 of EAX */ 
  void BackToPa1( Indi& tKid );                        /* Undo the parent p_A */
  void GoToBest( Indi& tKid );                         /* Modify tKid to the best offspring solution */

  void CheckValid( Indi& indi );                               /* For debug */
  
private:
  int fFlagImp;         
  int fN;
  int r,exam;
  int exam_flag;
  int **near_data;
  int *koritsu, *bunki, *kori_inv, *bun_inv;
  int koritsu_many,bunki_many;
  int st,ci,pr,stock,st_appear;
  int *check_koritsu;
  int *fRoute;
  int flag_st,flag_circle,pr_type;
  int ch_dis;
  int **fABcycle;
  int *fPermu;
  int fNumOfABcycleInESet;
  int fNumOfABcycle;
  int fPosiCurr;
  int fMaxNumOfABcycle;

  int *fC;
  int *fJun; 
  int *fOrd1, *fOrd2; 

  // Speed Up Start
  int *fOrder;    
  int *fInv;      
  int **fSegment; 
  int *fSegUnit;  
  		       
  int fNumOfUnit; 
  int fNumOfSeg;  
  int *fSegPosiList;
  int fNumOfSPL;    
  int *LinkAPosi;   
  int **LinkBPosi;  
  int *fPosiSeg;    
  int *fNumOfElementInUnit; 
  int *fCenterUnit;         
  int fNumOfElementInCU;    
  int *fListOfCenterUnit;   
  int fNumOfSegForCenter;   
  int *fSegForCenter;       

  EvalType *fGainAB;             
  EvalType fGainModi;            
  int fNumOfModiEdge;				 
  int fNumOfBestModiEdge;				 
  int **fModiEdge;				 
  int **fBestModiEdge;			
  int fNumOfAppliedCycle;
  int fNumOfBestAppliedCycle;
  int *fAppliedCylce;
  int *fBestAppliedCylce;
  // Speed Up End

  int fNumOfABcycleInEset;
  int *fABcycleInEset;
  int fDis_AB;     
  int fBest_Num_E;
};


#endif

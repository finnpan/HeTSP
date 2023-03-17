#include <random>
#include <assert.h>
#include <algorithm>
#include "Evaluator.h"
#include "cross.h"

Cross::Cross( int N )
{
  fMaxNumOfABcycle = 2000; /* Set an appropriate value (2000 is usually enough) */

  fN = N;

  near_data = new int* [ fN ];
  for ( int j = 0; j < fN; ++j ) 
    near_data[j] = new int [ 5 ];

  fABcycle = new int* [ fMaxNumOfABcycle ];
  for ( int j = 0; j < fMaxNumOfABcycle; ++j ) 
    fABcycle[j] = new int [ 2*fN + 4 ];

  koritsu = new int [ fN ];
  bunki = new int [ fN ];
  kori_inv = new int [ fN ];
  bun_inv = new int [ fN ];
  check_koritsu = new int [ fN ];
  fRoute = new int [ 2*fN + 1 ];
  fPermu = new int [ fMaxNumOfABcycle ];

  fC = new int [ 2*fN+4 ];
  fJun = new int[ fN+ 1 ];
  fOrd1 = new int [ fN ];
  fOrd2 = new int [ fN ];

  // Speed Up Start
  fOrder = new int [ fN ];
  fInv = new int [ fN ];
  fSegment = new int* [ fN ];
  for ( int j = 0; j < fN; ++j ) 
    fSegment[ j ] = new int [ 2 ];
  fSegUnit = new int [ fN ]; 
  fSegPosiList = new int[ fN ];
  LinkAPosi = new int [ fN ];
  LinkBPosi = new int* [ fN ];
  for ( int j = 0; j < fN; ++j ) 
    LinkBPosi[ j ] = new int [ 2 ];
  fPosiSeg = new int [ fN ];
  fNumOfElementInUnit = new int [ fN ]; 
  fCenterUnit = new int [ fN ]; 
  for ( int j = 0; j < fN; ++j ) 
    fCenterUnit[ j ] = 0;
  fListOfCenterUnit = new int [ fN+2 ]; 
  fSegForCenter = new int [ fN ]; 
  fGainAB = new EvalType [ fN ]; 
  fModiEdge = new int* [ fN ]; 				 
  for ( int j = 0; j < fN; ++j ) 
    fModiEdge[ j ] = new int [ 4 ]; 				 
  fBestModiEdge = new int* [ fN ]; 				 
  for ( int j = 0; j < fN; ++j ) 
    fBestModiEdge[ j ] = new int [ 4 ]; 				 
  fAppliedCylce = new int [ fN ];
  fBestAppliedCylce = new int [ fN ];
  // Speed Up End

  fABcycleInEset = new int [ fMaxNumOfABcycle ];
}

Cross::~Cross()
{
  delete [] koritsu;
  delete [] bunki;
  delete [] kori_inv;
  delete [] bun_inv;
  delete [] check_koritsu;
  delete [] fRoute;
  delete [] fPermu;

  for ( int j = 0; j < fN; ++j ) 
    delete[] near_data[ j ];
  delete[] near_data;

  for ( int j = 0; j < fMaxNumOfABcycle; ++j ) 
    delete[] fABcycle[ j ];
  delete[] fABcycle;

  delete [] fC;
  delete [] fJun; 
  delete [] fOrd1; 
  delete [] fOrd2; 


  // Speed Up Start
  delete [] fOrder;
  delete [] fInv;

  for ( int j = 0; j < fN; ++j ) 
    delete[] fSegment[ j ];
  delete[] fSegment;
  delete[] fSegUnit;
  delete [] fSegPosiList;
  delete [] LinkAPosi;
  for ( int j = 0; j < fN; ++j ) 
    delete[] LinkBPosi[ j ];
  delete [] LinkBPosi;
  delete [] fPosiSeg;
  delete [] fNumOfElementInUnit; 
  delete [] fCenterUnit;
  delete [] fListOfCenterUnit;
  delete [] fSegForCenter;
  delete [] fGainAB;

  for ( int j = 0; j < fN; ++j ) 
    delete[] fModiEdge[ j ];
  delete [] fModiEdge;
  for ( int j = 0; j < fN; ++j ) 
    delete[] fBestModiEdge[ j ];
  delete [] fBestModiEdge;
  delete [] fAppliedCylce;
  delete [] fBestAppliedCylce;
  // Speed Up End
  
  delete [] fABcycleInEset;
}

void Cross::SetParents( const Indi& tPa1, const Indi& tPa2, int numOfKids )
{
  this->SetABcycle( tPa1, tPa2, numOfKids ); 

  fDis_AB = 0;   

  int curr, next, st, pre;
  st = 0;
  curr=-1;
  next = st;
  for( int i = 0; i < fN; ++i )
  {
    pre=curr;
    curr=next;
    if( tPa1.fLink[curr][0] != pre ) 
      next = tPa1.fLink[ curr ][ 0 ];
    else 
      next=tPa1.fLink[curr][1];
    
    if( tPa2.fLink[ curr ][ 0 ] != next && tPa2.fLink[ curr ][ 1 ] != next )  
      ++fDis_AB; 

    fOrder[ i ] = curr;
    fInv[ curr ] = i;
  }

  assert( next == st );
}


void Cross::DoIt( Indi& tKid, Indi& tPa2, int numOfKids, int flagP)
{
  int Num;     
  int jnum; 
  EvalType gain;
  EvalType BestGain;  
  double pointMax, point;

  if ( numOfKids <= fNumOfABcycle ) 
    Num = numOfKids;
  else 
    Num = fNumOfABcycle;

  Utils::Permutation( fPermu, fNumOfABcycle ); 


  fNumOfGeneratedCh = 0;
  pointMax = 0.0;
  BestGain = 0;       
  fFlagImp = 0;  

  for( int j =0; j < Num; ++j )
  { 
    fNumOfABcycleInEset = 0;
    jnum = fPermu[ j ];
    fABcycleInEset[ fNumOfABcycleInEset++ ] = jnum; 

    fNumOfSPL = 0;          
    gain = 0;               
    fNumOfAppliedCycle = 0; 
    fNumOfModiEdge = 0;	    

    fNumOfAppliedCycle = fNumOfABcycleInEset; 
    for( int k = 0; k < fNumOfAppliedCycle; ++k ){   
      fAppliedCylce[ k ] = fABcycleInEset[ k ]; 
      jnum = fAppliedCylce[ k ];
      this->ChangeSol( tKid, jnum, flagP );
      gain += fGainAB[ jnum ];                       
    }

    this->MakeUnit();                                   
    this->MakeCompleteSol( tKid );                      
    gain += fGainModi;                                  
    
    ++fNumOfGeneratedCh;

    point = (double)gain;                         
    tKid.fEvaluationValue = tKid.fEvaluationValue - gain; 
    
    // if( pointMax < point ){
    if( pointMax < point && (2 * fBest_Num_E < fDis_AB || 
			     tKid.fEvaluationValue != tPa2.fEvaluationValue ) ){   
      pointMax = point;
      BestGain = gain;        
      fFlagImp = 1;  

      fNumOfBestAppliedCycle = fNumOfAppliedCycle;
      for( int s = 0; s < fNumOfBestAppliedCycle; ++s )
	fBestAppliedCylce[ s ] = fAppliedCylce[ s ];
      
      fNumOfBestModiEdge = fNumOfModiEdge;	  	
      for( int s = 0; s < fNumOfBestModiEdge; ++s ){
	fBestModiEdge[ s ][ 0 ] = fModiEdge[ s ][ 0 ];
	fBestModiEdge[ s ][ 1 ] = fModiEdge[ s ][ 1 ];
	fBestModiEdge[ s ][ 2 ] = fModiEdge[ s ][ 2 ];
	fBestModiEdge[ s ][ 3 ] = fModiEdge[ s ][ 3 ];
      }	

    }

    this->BackToPa1( tKid ); 
    tKid.fEvaluationValue = tKid.fEvaluationValue + gain;
  }

  if( fFlagImp == 1 ){           
    this->GoToBest( tKid ); 
    tKid.fEvaluationValue = tKid.fEvaluationValue - BestGain;
  }
}


void Cross::SetABcycle( const Indi& tPa1, const Indi& tPa2, int numOfKids )
{
  bunki_many=0; koritsu_many=0;
  for( int j = 0; j < fN ; ++j )
  {
    near_data[j][1]=tPa1.fLink[j][0];
    near_data[j][3]=tPa1.fLink[j][1];

    near_data[j][0] = 2;
    
    koritsu[koritsu_many]=j;
    koritsu_many++;

    near_data[j][2]=tPa2.fLink[j][0];
    near_data[j][4]=tPa2.fLink[j][1];
  }
  for(int j = 0; j < fN; ++j ) 
  {
    check_koritsu[j]=-1;
    kori_inv[koritsu[j]]=j;
  }

  /**************************************************/

  fNumOfABcycle=0; 
  flag_st=1;                   
  while(koritsu_many!=0)
  {                                                               
    if(flag_st==1)          
    {
      fPosiCurr=0;
      r=rand()%koritsu_many;
      st=koritsu[r];    
      check_koritsu[st]=fPosiCurr;
      fRoute[fPosiCurr]=st;
      ci=st;
      pr_type=2;
    }
    else if(flag_st==0)    
    {
      ci=fRoute[fPosiCurr];   
    }
                        
    flag_circle=0;
    while(flag_circle==0)
    {
      fPosiCurr++;
      pr=ci;
      
      switch(pr_type)
      {
      case 1:                 
	ci=near_data[pr][fPosiCurr%2+1];
	break;
      case 2:   
	r=rand()%2;
	ci=near_data[pr][fPosiCurr%2+1+2*r];
	if(r==0) std::swap(near_data[pr][fPosiCurr%2+1],near_data[pr][fPosiCurr%2+3]);
	break;
      case 3:   
	ci=near_data[pr][fPosiCurr%2+3];
      }

      fRoute[fPosiCurr]=ci;
      
      if(near_data[ci][0]==2) 
      {   
	if(ci==st)            
	{        
	  if(check_koritsu[st]==0) 
	  {        
	    if((fPosiCurr-check_koritsu[st])%2==0)  
	    {                  
	      if(near_data[st][fPosiCurr%2+1]==pr)
	      {
		std::swap(near_data[ci][fPosiCurr%2+1],near_data[ci][fPosiCurr%2+3]); 
	      }
	      st_appear = 1;
	      this->FormABcycle();
	      if( fNumOfABcycle == numOfKids ) goto LLL;
	      if( fNumOfABcycle == fMaxNumOfABcycle ) goto LLL;

	      flag_st=0;
	      flag_circle=1;
	      pr_type=1; 
	    }
	    else
	    {
            std::swap(near_data[ci][fPosiCurr%2+1],near_data[ci][fPosiCurr%2+3]);
	      pr_type=2;
	    }
	    check_koritsu[st]=fPosiCurr;
	  } 
	  else                     
	  {         
	    st_appear = 2;
	    this->FormABcycle();
	    if( fNumOfABcycle == numOfKids ) goto LLL;
	    if( fNumOfABcycle == fMaxNumOfABcycle ) goto LLL;

	    flag_st=1;
	    flag_circle=1;
	  }
	}
	else if(check_koritsu[ci]==-1) 
	{
	  check_koritsu[ci]=fPosiCurr;
	  if(near_data[ci][fPosiCurr%2+1]==pr)
	  {
          std::swap(near_data[ci][fPosiCurr%2+1],near_data[ci][fPosiCurr%2+3]);
	  }
	  pr_type=2;
	}
	else if(check_koritsu[ci]>0)   
	{
        std::swap(near_data[ci][fPosiCurr%2+1],near_data[ci][fPosiCurr%2+3]);
	  if((fPosiCurr-check_koritsu[ci])%2==0)  
	  {
	    st_appear = 1;
	    this->FormABcycle();
	    if( fNumOfABcycle == numOfKids ) goto LLL;
	      if( fNumOfABcycle == fMaxNumOfABcycle ) goto LLL;
	      
	    flag_st=0;
	    flag_circle=1;
	    pr_type=1;
	  }
	  else
	  {
          std::swap(near_data[ci][(fPosiCurr+1)%2+1],near_data[ci][(fPosiCurr+1)%2+3]);
	    pr_type=3;
	  }  
	}
      }
      else if(near_data[ci][0]==1)    
      {
	if(ci==st)                    
        {
	  st_appear = 1;
	  this->FormABcycle();
	  if( fNumOfABcycle == numOfKids ) goto LLL;
	  if( fNumOfABcycle == fMaxNumOfABcycle ) goto LLL;

	  flag_st=1;
	  flag_circle=1;
	}
	else pr_type=1;
      }
    }
  }
                                       
  while(bunki_many!=0)
  {            
    fPosiCurr=0;   
    r=rand()%bunki_many;
    st=bunki[r];
    fRoute[fPosiCurr]=st;
    ci=st;
    
    flag_circle=0;
    while(flag_circle==0)
    { 
      pr=ci; 
      fPosiCurr++;
      ci=near_data[pr][fPosiCurr%2+1]; 
      fRoute[fPosiCurr]=ci;
      if(ci==st)                       
      {
	st_appear = 1;
	this->FormABcycle();
	if( fNumOfABcycle == numOfKids ) goto LLL;
	if( fNumOfABcycle == fMaxNumOfABcycle ) goto LLL;
	
	flag_circle=1;
      }
    }
  }

LLL: ;

  if( fNumOfABcycle == fMaxNumOfABcycle ){
    printf( "fMaxNumOfABcycle(%d) must be increased\n", fMaxNumOfABcycle );
    exit( 1 );
  }
}


void Cross::FormABcycle()
{
  int j;
  int st_count;
  int edge_type;
  int st,ci, stock;
  int cem;                   
  EvalType diff;
 
  if(fPosiCurr%2==0) edge_type=1; 
  else edge_type=2;               
  st=fRoute[fPosiCurr];
  cem=0;
  fC[cem]=st;    

  st_count=0;
  while(1)
  {
    cem++;
    fPosiCurr--;
    ci=fRoute[fPosiCurr];
    if(near_data[ci][0]==2)
    {
      koritsu[kori_inv[ci]]=koritsu[koritsu_many-1];
      kori_inv[koritsu[koritsu_many-1]]=kori_inv[ci];
      koritsu_many--;
      bunki[bunki_many]=ci;
      bun_inv[ci]=bunki_many;
      bunki_many++;
    }
    else if(near_data[ci][0]==1)
    {
      bunki[bun_inv[ci]]=bunki[bunki_many-1];
      bun_inv[bunki[bunki_many-1]]=bun_inv[ci];
      bunki_many--;
    }
             
    near_data[ci][0]--;
    if(ci==st) st_count++;
    if(st_count==st_appear) break;
    fC[cem]=ci;  
  }

  if(cem==2)
    return;

  fABcycle[fNumOfABcycle][0]=cem;    

  if(edge_type==2)
  {
    stock=fC[0];
    for( int j=0;j<cem-1;j++) fC[j]=fC[j+1];
    fC[cem-1]=stock;
  }
  
  for( int j=0;j<cem;j++) 
    fABcycle[fNumOfABcycle][j+2]=fC[j];
  fABcycle[fNumOfABcycle][1]=fC[cem-1];
  fABcycle[fNumOfABcycle][cem+2]=fC[0];
  fABcycle[fNumOfABcycle][cem+3]=fC[1];

  fC[ cem ] = fC[ 0 ]; 
  fC[ cem+1 ] = fC[ 1 ]; 
  diff = 0;
  for( j = 0; j < cem/2; ++j ) 
  {
    diff = diff + eval->fEdgeDis[fC[2*j]][fC[1+2*j]]
                - eval->fEdgeDis[fC[1+2*j]][fC[2+2*j]];
  }
  fGainAB[fNumOfABcycle] = diff;
  ++fNumOfABcycle;
}

void Cross::ChangeSol( Indi& tKid, int ABnum, int type )
{
  int j;
  int cem,r1,r2,b1,b2;
  int po_r1, po_r2, po_b1, po_b2; 

  cem=fABcycle[ABnum][0];  
  fC[0]=fABcycle[ABnum][0];

  if(type==2)   
  {
    for(j=0;j<cem+3;j++) fC[cem+3-j]=fABcycle[ABnum][j+1];
  }
  else for(j=1;j<=cem+3;j++) fC[j]=fABcycle[ABnum][j];

  for(j=0;j<cem/2;j++)
  {                           
    r1=fC[2+2*j];r2=fC[3+2*j];
    b1=fC[1+2*j];b2=fC[4+2*j];

    if(tKid.fLink[r1][0]==r2)
      tKid.fLink[r1][0]=b1;
    else 
      tKid.fLink[r1][1]=b1;
    if(tKid.fLink[r2][0]==r1) 
      tKid.fLink[r2][0]=b2;
    else
      tKid.fLink[r2][1]=b2;   

    po_r1 = fInv[ r1 ]; 
    po_r2 = fInv[ r2 ]; 
    po_b1 = fInv[ b1 ]; 
    po_b2 = fInv[ b2 ]; 
    
    if( po_r1 == 0 && po_r2 == fN-1 )
      fSegPosiList[ fNumOfSPL++ ] = po_r1;
    else if( po_r1 == fN-1 && po_r2 == 0 )
      fSegPosiList[ fNumOfSPL++ ] = po_r2;
    else if( po_r1 < po_r2 )
      fSegPosiList[ fNumOfSPL++ ] = po_r2;
    else if( po_r2 < po_r1 )
      fSegPosiList[ fNumOfSPL++ ] = po_r1;
    else
      assert( 1 == 2 );
    
    LinkBPosi[ po_r1 ][ 1 ] = LinkBPosi[ po_r1 ][ 0 ];
    LinkBPosi[ po_r2 ][ 1 ] = LinkBPosi[ po_r2 ][ 0 ];
    LinkBPosi[ po_r1 ][ 0 ] = po_b1; 
    LinkBPosi[ po_r2 ][ 0 ] = po_b2; 
  }
}


void Cross::MakeCompleteSol( Indi& tKid )
{
  int j,j1,j2;
  int st,pre,curr,next,a,b,c,d,aa,bb,a1,b1;
  int min_unit_city; 
  int near_num;
  int center_un;               
  int select_un;               
  EvalType diff,max_diff;
  int nearMax;

  fGainModi = 0;         

  while( fNumOfUnit != 1 )
  {    
    min_unit_city = fN + 12345;
    for( int u = 0; u < fNumOfUnit; ++u ) 
    {
      if( fNumOfElementInUnit[ u ] < min_unit_city )
      {
	center_un = u;
        min_unit_city = fNumOfElementInUnit[ u ];
      }
    }  

    st = -1;
    fNumOfSegForCenter = 0;   
    for( int s = 0; s < fNumOfSeg; ++s ){
      if( fSegUnit[ s ] == center_un ){
	int posi = fSegment[ s ][ 0 ];
	st = fOrder[ posi ];    
	fSegForCenter[  fNumOfSegForCenter++ ] = s; 
      }
    } 
    assert( st != -1 );

    curr = -1;
    next = st;
    fNumOfElementInCU = 0;
    while(1){ 
      pre = curr;
      curr = next;
      fCenterUnit[ curr ] = 1;     
      fListOfCenterUnit[ fNumOfElementInCU ] = curr;
      ++fNumOfElementInCU;

      if( tKid.fLink[ curr ][ 0 ] != pre )
	next = tKid.fLink[ curr ][ 0 ];
      else 
	next = tKid.fLink[ curr ][ 1 ]; 

      if( next == st ) break;
    }       
    fListOfCenterUnit[ fNumOfElementInCU ] = fListOfCenterUnit[ 0 ];
    fListOfCenterUnit[ fNumOfElementInCU+1 ] = fListOfCenterUnit[ 1 ];

    assert( fNumOfElementInCU == fNumOfElementInUnit[ center_un ] );

    max_diff = std::numeric_limits<EvalType>::min();
    a1 = -1; b1 = -1;
    nearMax = 10; /* N_near (see Step 5.3 in Section 2.2 of the Online Supplement) */
    /* nearMax must be smaller than or equal to eva->fNearNumMax (kopt.cpp ) */

  RESTART:;
    for( int s = 1; s <= fNumOfElementInCU; ++s )  
    { 
      a = fListOfCenterUnit[ s ];

      for( near_num = 1; near_num <= nearMax; ++near_num )   
      {
	c = eval->fNearCity[ a ][ near_num ];
	if( fCenterUnit[ c ] == 0 )   
	{
	  for( j1 = 0; j1 < 2; ++j1 )
	  {
	    b = fListOfCenterUnit[ s-1+2*j1 ];
            for( j2 = 0; j2 < 2; ++j2 )
	    {
	      d = tKid.fLink[ c ][ j2 ];
	      diff = eval->fEdgeDis[a][b] + eval->fEdgeDis[c][d] -
                     eval->fEdgeDis[a][c] - eval->fEdgeDis[b][d];
	      if( diff > max_diff ) 
	      { 
	        aa = a; bb = b; a1 = c; b1 = d;
	        max_diff = diff;
	      }
	      diff = eval->fEdgeDis[a][b] + eval->fEdgeDis[d][c] - 
		     eval->fEdgeDis[a][d] - eval->fEdgeDis[b][c];
	      if( diff > max_diff ) 
	      {
	        aa = a; bb = b; a1 = d; b1 = c;
	        max_diff = diff;
	      } 
	    }
	  }
	}
      }
    }

    if( a1 == -1 && nearMax == 10 ){  /* This value must also be changed if nearMax is chenged above */
      nearMax = 50;
      goto RESTART;
    }    
    else if( a1 == -1 && nearMax == 50  )
    {       
      int r = rand() % ( fNumOfElementInCU - 1 );
      a = fListOfCenterUnit[ r ];
      b = fListOfCenterUnit[ r+1 ];
      for( j = 0; j < fN; ++j )
      {
	if( fCenterUnit[ j ] == 0 )
        {
	  aa = a; bb = b;
	  a1 = j;
	  b1 = tKid.fLink[ j ][ 0 ];
	  break;
	}
      }
      max_diff = eval->fEdgeDis[aa][bb] + eval->fEdgeDis[a1][b1] -
         	 eval->fEdgeDis[a][a1] - eval->fEdgeDis[b][b1];
    }  

    if( tKid.fLink[aa][0] == bb ) tKid.fLink[aa][0]=a1;
    else tKid.fLink[aa][1] = a1;
    if( tKid.fLink[bb][0] == aa ) tKid.fLink[bb][0] = b1;
    else tKid.fLink[bb][1] = b1;   
    if( tKid.fLink[a1][0] == b1 ) tKid.fLink[a1][0] = aa;
    else tKid.fLink[a1][1] = aa;
    if( tKid.fLink[b1][0] == a1 ) tKid.fLink[b1][0] = bb;
    else tKid.fLink[b1][1] = bb; 


    fModiEdge[ fNumOfModiEdge ][ 0 ] = aa;
    fModiEdge[ fNumOfModiEdge ][ 1 ] = bb;
    fModiEdge[ fNumOfModiEdge ][ 2 ] = a1;
    fModiEdge[ fNumOfModiEdge ][ 3 ] = b1;
    ++fNumOfModiEdge;


    fGainModi += max_diff;
    

    int posi_a1 = fInv[ a1 ];  
    select_un = -1;
    for( int s = 0; s < fNumOfSeg; ++s ){
      if( fSegment[ s ][ 0 ] <= posi_a1 && posi_a1 <=  fSegment[ s ][ 1 ] ){
	select_un = fSegUnit[ s ];       
	break;
      }
    } 
    assert( select_un != -1 );

    for( int s = 0; s < fNumOfSeg; ++s ){
      if( fSegUnit[ s ] == select_un )
	fSegUnit[ s ] = center_un;
    }
    fNumOfElementInUnit[ center_un ] += fNumOfElementInUnit[ select_un ];
    
    for( int s = 0; s < fNumOfSeg; ++s ){
      if( fSegUnit[ s ] == fNumOfUnit - 1 )
	fSegUnit[ s ] = select_un;
    }
    fNumOfElementInUnit[ select_un ] = fNumOfElementInUnit[ fNumOfUnit - 1 ];
    --fNumOfUnit;

    for( int s = 0; s < fNumOfElementInCU; ++s ){
      c = fListOfCenterUnit[ s ];
      fCenterUnit[ c ] = 0;
    }
  }
}  


void Cross::MakeUnit()                    
{
  int flag = 1; 
  for( int s = 0; s < fNumOfSPL; ++s ){
    if( fSegPosiList[ s ] == 0 ){
      flag = 0;
      break;
    }
  }
  if( flag == 1 ) 
  {
    fSegPosiList[ fNumOfSPL++ ] = 0;

    LinkBPosi[ fN-1 ][ 1 ]  = LinkBPosi[ fN-1 ][ 0 ];
    LinkBPosi[ 0 ][ 1 ] = LinkBPosi[ 0 ][ 0 ];
    LinkBPosi[ fN-1 ][ 0 ] = 0; 
    LinkBPosi[ 0 ][ 0 ] = fN-1;

  }

  std::sort( fSegPosiList, fSegPosiList + fNumOfSPL );


  fNumOfSeg = fNumOfSPL;
  for( int s = 0; s < fNumOfSeg-1; ++s ){
    fSegment[ s ][ 0 ] = fSegPosiList[ s ];
    fSegment[ s ][ 1 ] = fSegPosiList[ s+1 ]-1;
  }

  fSegment[ fNumOfSeg-1 ][ 0 ] = fSegPosiList[ fNumOfSeg-1 ];
  fSegment[ fNumOfSeg-1 ][ 1 ] = fN - 1;


  for( int s = 0; s < fNumOfSeg; ++s ){
    LinkAPosi[ fSegment[ s ][ 0 ] ] = fSegment[ s ][ 1 ];
    LinkAPosi[ fSegment[ s ][ 1 ] ] = fSegment[ s ][ 0 ];
    fPosiSeg[ fSegment[ s ][ 0 ] ] = s;
    fPosiSeg[ fSegment[ s ][ 1 ] ] = s;
  }

  for( int s = 0; s < fNumOfSeg; ++s )
    fSegUnit[ s ] = -1;
  fNumOfUnit = 0; 

  int p_st, p1, p2, p_next, p_pre; 
  int segNum;

  while(1)
  {
    flag = 0;
    for( int s = 0; s < fNumOfSeg; ++s ){
      if( fSegUnit[ s ] == -1 ){
	p_st = fSegment[ s ][ 0 ]; 
	p_pre = -1;
	p1 = p_st;
	flag = 1;
	break;
      }
    }
    if( flag == 0 )
      break;
    
    while(1)
    {
      segNum = fPosiSeg[ p1 ];
      fSegUnit[ segNum ] = fNumOfUnit;

      p2 = LinkAPosi[ p1 ];
      p_next = LinkBPosi[ p2 ][ 0 ];
      if( p1 == p2 ){
	if( p_next == p_pre )
	  p_next = LinkBPosi[ p2 ][ 1 ];
      } 
      
      if( p_next == p_st ){
	++fNumOfUnit;
	break;
      }

      p_pre = p2;
      p1 = p_next;
    }
  }
  

  for( int s = 0; s < fNumOfUnit; ++s )
    fNumOfElementInUnit[ s ] = 0; 
  
  int unitNum = -1;
  int tmpNumOfSeg = -1;
  for( int s = 0; s < fNumOfSeg; ++s ){
    if( fSegUnit[ s ] != unitNum ){
      ++tmpNumOfSeg;
      fSegment[ tmpNumOfSeg ][ 0 ] = fSegment[ s ][ 0 ];
      fSegment[ tmpNumOfSeg ][ 1 ] = fSegment[ s ][ 1 ];
      unitNum = fSegUnit[ s ];
      fSegUnit[ tmpNumOfSeg ] = unitNum;
      fNumOfElementInUnit[ unitNum ] += 
	fSegment[ s ][ 1 ] - fSegment[ s ][ 0 ] + 1;
    }
    else
    {
      fSegment[ tmpNumOfSeg ][ 1 ] = fSegment[ s ][ 1 ];
      fNumOfElementInUnit[ unitNum ] += 
	fSegment[ s ][ 1 ] - fSegment[ s ][ 0 ] + 1;
    }
  }
  fNumOfSeg = tmpNumOfSeg + 1;  
}


void Cross::BackToPa1( Indi& tKid )
{
  int aa, bb, a1, b1; 
  int jnum;

  for( int s = fNumOfModiEdge -1; s >= 0; --s ){ 
    aa = fModiEdge[ s ][ 0 ];
    a1 = fModiEdge[ s ][ 1 ];   // ここを変更に注意 
    bb = fModiEdge[ s ][ 2 ];   // ここを変更に注意 
    b1 = fModiEdge[ s ][ 3 ];

    if( tKid.fLink[aa][0] == bb ) tKid.fLink[aa][0] = a1;
    else tKid.fLink[aa][1] = a1;
    if( tKid.fLink[b1][0] == a1 ) tKid.fLink[b1][0] = bb;
    else tKid.fLink[b1][1] = bb; 
    if( tKid.fLink[bb][0] == aa ) tKid.fLink[bb][0] = b1;
    else tKid.fLink[bb][1] = b1;   
    if( tKid.fLink[a1][0] == b1 ) tKid.fLink[a1][0] = aa;
    else tKid.fLink[a1][1] = aa;
  }
  
  for( int s = 0; s < fNumOfAppliedCycle; ++s ){
    jnum = fAppliedCylce[ s ];
    this->ChangeSol( tKid, jnum, 2 );
  }
}

void Cross::GoToBest( Indi& tKid )
{
  int aa, bb, a1, b1; 
  int jnum;

  for( int s = 0; s < fNumOfBestAppliedCycle; ++s ){
    jnum = fBestAppliedCylce[ s ];
    this->ChangeSol( tKid, jnum, 1 );
  }

  for( int s = 0; s < fNumOfBestModiEdge; ++s )
  { 
    aa = fBestModiEdge[ s ][ 0 ];
    bb = fBestModiEdge[ s ][ 1 ];   
    a1 = fBestModiEdge[ s ][ 2 ];   
    b1 = fBestModiEdge[ s ][ 3 ];

    if( tKid.fLink[aa][0] == bb ) tKid.fLink[aa][0]=a1;
    else tKid.fLink[aa][1] = a1;
    if( tKid.fLink[bb][0] == aa ) tKid.fLink[bb][0] = b1;
    else tKid.fLink[bb][1] = b1;   
    if( tKid.fLink[a1][0] == b1 ) tKid.fLink[a1][0] = aa;
    else tKid.fLink[a1][1] = aa;
    if( tKid.fLink[b1][0] == a1 ) tKid.fLink[b1][0] = bb;
    else tKid.fLink[b1][1] = bb; 
  }
}

void Cross::CheckValid( Indi& indi )
{
  int curr, pre, next, st;
  int count;

  st = 0;
  curr = -1;
  next = st;

  count = 0;
  while(1){ 
    pre = curr;
    curr = next;
    ++count;
    if( indi.fLink[ curr ][ 0 ] != pre )
      next = indi.fLink[ curr ][ 0 ];
    else 
      next = indi.fLink[ curr ][ 1 ]; 
    
    if( next == st ) break;

    if( count > fN ){
      printf( "Invalid = %d\n", count );
      break;
    }
  }       
  if( count != fN )
      printf( "Invalid = %d\n", count );
}


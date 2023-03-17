#include <stdio.h>

#include "env.h"

int main( int argc, char* argv[] )
{
  Environment* gEnv = new Environment();

  gEnv->fNumOfPop = 100;
  gEnv->fNumOfKids = 30;
  gEnv->fFileNameTSP = "..\\..\\testcase\\fnl4461.tsp";

  gEnv->Define();
  gEnv->DoIt();

  delete gEnv;

  return 0;
}

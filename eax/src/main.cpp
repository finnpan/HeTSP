#include <stdio.h>

#include "env.h"

int main( int argc, char* argv[] )
{
  if (argc < 2) {
    printf("please input instance file!\n");
    return 0;
  }
  Environment* gEnv = new Environment();

  gEnv->fNumOfPop = 100;
  gEnv->fNumOfKids = 30;
  gEnv->fFileNameTSP = argv[1];

  gEnv->Define();
  gEnv->DoIt();

  delete gEnv;

  return 0;
}

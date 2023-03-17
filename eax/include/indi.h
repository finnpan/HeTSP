#ifndef __INDI__
#define __INDI__

using EvalType = long long;

namespace Utils {
	void Permutation (int* a, int n);
};

class Indi {
public:
  Indi();
  ~Indi();
  void Define( int N );
  Indi& operator = ( const Indi& src );   /* Copy */
  bool operator == (  const Indi& indi2 ); /* Return true if two tours are the same, false otherwise */

  int fN;                 /* Number of cities */
  int** fLink;            /* fLink[i][]: two vertices adjacent to i */
  EvalType fEvaluationValue;   /* Tour length of */
};

#endif

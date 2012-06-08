#include "itbase.h"
#include <ostream>
#include <fstream>
using namespace std;
using std::cout;
using std::endl;
using namespace itpp;


#ifdef NO_LAPACK
#define __THIS_PROGRAM_WILL_NOT_RUN__
#endif

#ifdef NO_CBLAS
#define __THIS_PROGRAM_WILL_NOT_RUN__
#endif

#ifdef __THIS_PROGRAM_WILL_NOT_RUN__
int main() { cout << "LAPACK and CBLAS is needed for this test program" << endl; }
#else

int main()
{
  ofstream rf("r.dat");
  int i,n;
  int M,N;
  
  M = 16;
  N = 16; // 4*M;
  
  cout << "Real systems:" << endl << endl;
  mat A, B, X;
  vec b, x;
  double f;
  double df;
  double delta;
  
  A = randn(N,M);
  b = randn(N);

  // passband
  for (i=0;i<N;i++) {
	df = (double)i/(N-1);
	f = M_2PI*df/2.0;
	b(i) = 1.0;
	if (i >= N/2) b(i) = 0;
	for (n=0;n<M;n++) {	  A(i,n) = cos(n*f);	}
  }
  
  cout << "Starting LS SOLVE...";
  //    A = randn(4,4);
  //    b = randn(4);
  x = ls_solve(A, b);
  

  cout << "Square system: Ax=b" << endl
	   << "============================" << endl
	   << "A=" << A << endl
	   << "b=" << b << endl
	   << "x=" << x << endl << endl;
  
  for (i=1;i<M;i++) 	  rf << x(M-i) << "\n";
  rf << 2*x(0) << "\n";
  for (i=1;i<M;i++) 	  rf << x(i) << "\n";
  //    A = randn(2,4);
  //    b = randn(2);
  //    x = ls_solve_ud(A, b);
  
  rf.close();
  return 0;
  
}

#endif

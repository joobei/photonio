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
  int i,n,k;
  int M,N;
  
  M = 33;
  N = 4*M;
  
  cout << "Real systems:" << endl << endl;
  mat Q(N,M);
  vec d(N);
  vec w(N);
  vec p(N);
  vec a(M);
  double f;
  double ff;
  double delta;
  double bs;


  // Desired/Weight functions
  for (i=0;i<N;i++) {
	d(i) = 1.0;
 	w(i) = 1.0;
	if (i >= N/2) d(i) = 0;
  }


  for (i=0;i<N;i++) {
	f = M_2PI*i/(2*(N-1));
	bs = 0;
	for (k=0;k<N;k++) {
	  ff = M_2PI*k/(2*(N-1));
	  bs += w(k)*d(k)*cos(ff);
	}
	p(i) = bs;

	for (n=0;n<M;n++) {	 
	  bs = 0;
	  for (k=0;k<N;k++) {
		ff = M_2PI*k/(2*(N-1));
		bs += w(k)*cos(ff)*cos(n*f);	
	  }
	  Q(i,n) = bs;
	}
  }
  
  cout << "Starting LS SOLVE...";
  a = ls_solve_od(Q, p);
  

  cout << "Square system: Qa=p" << endl
	   << "============================" << endl
	   << "Q=" << Q << endl
	   << "p=" << p << endl
	   << "a=" << a << endl << endl;
  
  for (i=1;i<M;i++) 	  rf << a(M-i) << "\n";
                          rf << 2*a(0) << "\n";
  for (i=1;i<M;i++) 	  rf << a(i) << "\n";

  rf.close();
  return 0;
}

#endif

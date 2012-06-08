#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;
#include <chebyshev.h>
#include <elliptic.h>
#include <butterworth.h>
#include <fir.h>
#include <remez_fir.h>
namespace SPUC {
  void gaussian_fir(fir<double>& gaussf, double bt, double spb);
  void butterworth_fir(fir<double>& butfir, double spb);
  void create_remez_lpfir(fir<double>& remezfir, double* edge, double* fx, double* wtx);
}

using namespace SPUC;
int main(int argv, char* argc[]) {

  int i;
  double x;
  double imp;
  double y;
  int TAPS = 31;
  double edge[20];
  double fx[10];
  double  wtx[10];
  edge[0] = 0.0;
  edge[1] = 0.08;
  edge[2] = 0.16;
  edge[3] = 0.50;
  fx[0] = 1.0;
  fx[1] = 0.0;
  wtx[0] = 1;
  wtx[1] = 1;

  chebyshev<double>   CPF(0.15,12,0.01);
  elliptic            EPF(0.2,8,40,0.5);
  butterworth<double> BPF(0.15,12,0.01);
  fir<double> BF(TAPS);
  fir<double> RF(TAPS);
  fir<double> GF(TAPS);

  create_remez_lpfir(RF,edge,fx,wtx);
  butterworth_fir(BF, 0.15);
  gaussian_fir(GF, 0.25, 8);

  ofstream IMP("impulses.dat");

  imp = 1;
  for (i=0;i<1000;i++) {
	IMP << EPF.clock(imp) << " "
		<< CPF.clock(imp) << " "
		<< BPF.clock(imp) << " "
		<< BF.clock(imp) << " "
		<< RF.clock(imp) << " "
		<< GF.clock(imp) << " "
	    << "\n";
	imp = 0;
  }
  IMP.close();
  return(1);
}

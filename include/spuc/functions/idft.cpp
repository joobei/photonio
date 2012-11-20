/* *************************************************************************
cmplx<double> idft(cmplx<double> *x, int n)
*************************************************************************/
#include <spuc.h>
#include <complex.h>
#include <iostream>
#include <fstream>
using namespace std;
namespace SPUC {
void idft(complex<double> *y, int n)
{
  int i,j,k,l;
  double scale;

  /*  n inverse dft length */
  complex<double> *w = new complex<double> [n+1];
  complex<double> *x = new complex<double> [n+1];
  complex<double> mult;

  /*  calculate the w values recursively */
  //  complex<double> w_inc = expj(TWOPI/n);
  //  complex<double> w_x = complex<double>(1,0);
  for (j=0; j<n; j++) {
	//    w[j] = w_x;
	//    w_x *= w_inc;
	x[j] = y[j];
  }

  /*  start inverse fft */
  for (l=0; l<n; l++) {
	y[l] = 0;
    for (j=0; j<n ; j++) {
	  mult  = x[j]*expj(TWOPI*l*j/(n));
	  y[l] += mult;
	  //	  cout << "Sum = " << y[l].real() << " " << y[l].imag();
	  //	  cout << " Mult = " << mult.real() << " " << mult.imag();
	  //	  cout << " l = " << l << " j = " << j << "\n";
	} 
  }

#define SCALEFFT
#ifdef SCALEFFT
  /*  scale all results by 1/n */
  scale = (double)(1.0/n);
  for (i=0; i<n; i++) {
	//	cout << "Result = " << y[i].real() << " " << y[i].imag() << "\n";
 	y[i] = scale*y[i];
	//	cout << "Scaled = " << y[i].real() << " " << y[i].imag() << "\n";
  }
#endif

  delete [] x;
  //  delete [] w;

}
void dft(complex<double> *y, int n)
{
  int i,j,k,l;
  double scale;

  /*  n inverse dft length */
  complex<double> *x  = new complex<double> [n+1];

  for (j=0; j<n; j++) x[j] = y[j];

  /*  start inverse fft */
  for (l=0; l<n; l++) {
	y[l] = 0;
    for (j=0; j<n ; j++) {
	  y[l] += x[j]*expj(-TWOPI*j*l/n);
	} 
  }
  delete [] x;
}
}

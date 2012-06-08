/*
 * SPUC - Signal processing using C++ - A DSP library
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#include <math.h>
#include <spuc.h>
#include <complex.h>
namespace SPUC {
/*! 
  \addtogroup fir FIR filters
*/

/*! 
  \author Tony Kirke,  Copyright(c) 2001 
  
*/
//!
//!:
//! \ingroup fir
//! function:  io
//! \brief bessel function for kaiser window
double io(double x)
{
  const double t = 1.e-08;
  double y = 0.5*x;
  double e = 1.0;
  double de = 1.0;
  int i;
  double xi;
  double sde;
  for (i=1;i<26;i++) {
	xi = i;
	de *= y/xi;
	sde = de*de;
	e += sde;
	if ((e*t-sde) > 0) break;
  }
  return(e);  
}
//!  \ingroup fir
//! \brief hamming window \f$ w(n) = alpha + beta*cos( 2*\pi*(n-1)/(nf-1) )\f$
void hamming(double* w, long nf,double alpha, double beta)
{
  // nf = filter length in samples
  // alpha = constant of window
  // beta = constant of window--generally beta=1-alpha
  long odd = nf%2;
  double xi;
  for (int i=0;i<nf;i++) {
	xi = i;
	if (odd) xi += 0.5;
	w[i] = alpha + beta*cos(TWOPI*xi/nf);
  }
}
//
//!  \ingroup fir
//! \brief hanning window \f$ w(n) = 0.5( 1 - cos( 2*\pi*n/(nf-1) )\f$
void hanning(double* w, long nf)
{
  // nf = filter length in samples
  // alpha = constant of window
  // beta = constant of window--generally beta=1-alpha
  long odd = nf%2;
  double xi;
  for (int i=0;i<nf;i++) {
	xi = i;
	if (odd) xi += 0.5;
	w[i] = 0.5*(1 - cos(TWOPI*xi/nf));
  }
}
//:
//!  \ingroup fir
//! \brief Blackman Window	\f$ w[x] = 0.42 - 0.5*cos(2*\pi*x/nf) + 0.08*cos(2*\pi*x/nf)\f$
void blackman(double* w, long nf)
{
  // nf = filter length in samples
  // alpha = constant of window
  // beta = constant of window--generally beta=1-alpha
  long odd = nf%2;
  double xi;
  for (int i=0;i<nf;i++) {
	xi = i;
	if (odd) xi += 0.5;
	w[i] = 0.42 - 0.5*cos(TWOPI*xi/nf) + 0.08*cos(2*TWOPI*xi/nf);
  }
}
//!  \ingroup fir
//! \brief kaiser window
void kaiser(double* w,long nf, double beta)
{
  // nf = filter length in samples
  // beta = parameter of kaiser window
  double io(double b);
  double bes = 1.0/io(beta);
  long i;
  long odd = nf%2;
  double xi;
  double xind = (nf-1)*(nf-1);
  for (i=0;i<nf;i++) {
	if (odd) xi = i + 0.5;
	else xi = i;
	xi = 4*xi*xi;
	w[i]  = io(beta*sqrt(1.-xi/xind))*bes;
  }
}
//!  \ingroup fir
//! \brief chebyshev window
//! subroutine to generate chebyshev window parameters when
//! one of the three parameters nf,dp and df is unspecified
void chebc(double nf, double dp, double df, double n,double x0) {
//
// nf = filter length (in samples)
// dp = filter ripple (absolute scale)
// df = normalized transition width of filter
// n = (nf+1)/2 = filter half length
// x0 = (3-c0)/(1+c0) with c0=cos(pi*df) = chebyshev window constant
// xn = nf-1
//
  double xn;
  double c1, c0, c2, x;
  double coshin(double x);
  if (nf == 0) {
	// dp,df specified, determine nf
	c1 = coshin((1.+dp)/dp);
	c0 = cos(PI*df);
	x = 1. + c1/coshin(1./c0);
	// increment by 1 to give nf which meets or exceeds specs on dp and df
	nf = x + 1.0;
	n = (nf+1)/2;
	xn = nf - 1;
	x0 = (3.-cos(TWOPI*df))/(1.+cos(TWOPI*df));
	return;
  }
  if (df == 0) {
	// nf,dp specified, determine df
	xn = nf - 1;
	c1 = coshin((1.+dp)/dp);
	c2 = cosh(c1/xn);
	df = acos(1./c2)/PI;
	x0 = (3.-cos(TWOPI*df))/(1.+cos(TWOPI*df));
  }
  // nf,df specified, determine dp
  xn = nf - 1;
  c0 = cos(PI*df);
  c1 = xn*coshin(1./c0);
  dp = 1./(cosh(c1)-1.);
  x0 = (3.-cos(2.*PI*df))/(1.+cos(2.*PI*df));
}
//!  \ingroup fir
//!  \brief dolph chebyshev window design
void cheby(double* w, long nf, long n, long ieo, double dp, double df, double x0) {
  /*! parameters
	- nf = filter length in samples
	- w = window array of size n
	- n = half length of filter = (nf+1)/2
	- ieo = even-odd indicator--ieo=0 for nf even
	- dp = window ripple on an absolute scale
	- df = normalized transition width of window
	- x0 = window parameter related to transition width
	- xn = nf-1
  */
  double coshin(double x);
  double xn = nf - 1;
  double fnf = nf;
  double alpha = (x0+1.)/2.;
  double beta = (x0-1.)/2.;
  double c2 = xn/2.;
  double p;
  complex<double> pie;
  double twn;
  double xi;
  double sum;
  double xj;
  double f;
  double x;
  double c1;
  int j;

  for (int i=1;i<=nf;i++) {
	xi = i - 1;
	f = xi/fnf;
	x = alpha*cos(TWOPI*f) + beta; 
	if ((fabs(x)-1.) > 0) {
	  p = dp*cos(c2*acos(x));
	} else {
	  p = dp*cosh(c2*coshin(x));
	}
	pie = complex<double>(0.0,p);
  //
  // for even length filters use a one-half sample delay
  // also the frequency response is antisymmetric in frequency
  //
  if (ieo != 1) {
	pie = p*expj(-f*PI);
	if (i > (nf/2+1)) pie *= -1;
  }
  // use dft to give window
  twn = TWOPI/fnf;
  for (i=1;i<n+1;i++) {
	xi = i-1;
	sum = 0.;
	for (j=1;j<nf+1;j++) {
	  xj = j - 1;
	  //	  sum += real(pie[j])*cos(twn*xj*xi) + imag(pie[j])*sin(twn*xj*xi);
	  sum += real(pie)*cos(twn*xj*xi) + imag(pie)*sin(twn*xj*xi);
	}
	w[i] = sum;
  }
  c1 = w[1];
  for (i=1;i<n+1;i++)  w[i] /= c1;
  }
}

} // namespace SPUC 

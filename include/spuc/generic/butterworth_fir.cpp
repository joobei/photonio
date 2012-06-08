// 
// Copyright(c) 1993-1996 Tony Kirke
// author="Tony Kirke" *
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
#include <stdio.h>
#include <fir.h>
#include <spuc.h>
namespace SPUC {
/*! 
  \addtogroup fir FIR filters
*/
//! \brief calculates the sampled butterworth (max flat) filter impulse response
//! \ingroup fir
void butterworth_fir(fir<double>& butfir, double spb)
{
/*! 

  With B(p) the butterworth response we have:

\f$ B(p)  = \displaystyle\sum_{k=1}^{N/2} \frac{\lambda(k)*(1+\alpha(k)*p/wc)}{1+2*cos(\beta(k))*p/wc + (p/wc)^2)}.\f$


	   with<p>
	   N = Order of Butterworth filter (always even)<p>
\f$	   \beta(k) = (2 * k - 1) / (2 * N)         k = 1, 2, ..  N/2 \f$<p>
\f$	   \alpha(k) = sin((N/2 - 1) * \beta(k)) / sin(N * \beta(k) / 2) \f$<p>

\f$ \lambda(p)  = \frac{\sin(N*\beta(k)/2)}{sin(\beta(k))}. \displaystyle\prod_{m=1,m!=k}^{N/2} 2*cos(\beta(k) - cos(\beta(m))) \f$


	  The impulse response of B(p) can be found by realizing that:

\f$ \frac{p+a}{(p+a)^2 + w^2} <-> e^{-at} * cos(w*t) \f$

\f$ \frac{w}{(p+a)^2 + w^2} <-> e^{-at} * sin(w*t) \f$

and that B(p) can be written as a linear combination of the these two
expressions:

\f$ \frac{x*(p+a)+y*w}{(p+a)^2 + w^2} <-> e^{-at}*x*cos(w*t) + y* sin(w*t) \f$

We find after some algebra:<p>
\f$ x(k) = \alpha(k) \f$<p>
\f$ w(k) = \sqrt{1 - cos(\beta(k)) * cos((\beta(k)))} \f$<p>
\f$ a(k) = cos(\beta(k)) \f$<p>
\f$ y(k) = (1 - \alpha(k) * cos(\beta(k)) / w(k) \f$<p>
Also used is the time scaling rule for Fourier transforms:
|a| * y(at) <--> Y(f/a)

*/
  int end, i, i2,j;
  double x, xend,t;
  int taps = butfir.num_taps;
  int spbi = (int)floor(1.0/spb+0.5);
  int ord = (int)floor(taps/spbi+0.5);
  if (ord%2) ord += 1; // make even
  int order = ord;
  end = ord / 2;
  xend = (double) end;
  double* beta = new double[end];
  double* alpha = new double[end];
  double* ak = new double[end];
  double* xk = new double[end];
  double* yk = new double[end];
  double* wk = new double[end];
  double* lamda = new double[end];

  for (i = 0; i < end; i++) 	{ 
	x = (double) i + 1.0; 
	beta[i] = PI * (2.0 * x - 1.0) / (2.0 * ord); 
	lamda[i] = sin(xend * beta[i]) / sin(beta[i]); 
	alpha[i] = sin((xend - 1.0) * beta[i]) / sin(xend * beta[i]); 
	ak[i] = cos(beta[i]); 
	wk[i] = sqrt(1 - ak[i] * ak[i]); 
	xk[i] = alpha[i];
	yk[i] = ( 1.0 - alpha[i] * ak[i]) / wk[i]; 
  }

  for (i =0; i < end; i++) 
	for (i2 = 0; i2 < end ; i2++) 
	  if (i2 != i) lamda[i] /= ( 2.0 * ( ak[i] - cos(beta[i2])));

  double ht;
  for (j=0;j<taps;j++) {
	ht = 0.0;
 	t = TWOPI*j/(double)spbi;
	for(i = 0; i < end; i++)
		ht += lamda[i] * ::exp(-t * ak[i]) * ( xk[i] * cos(t * wk[i]) + yk[i] * sin(t * wk[i]));
	butfir.coeff[j] = TWOPI*ht;
  }
  delete [] ak;
  delete [] wk;
  delete [] yk;
  delete [] xk;
  delete [] lamda;
}
} // namespace SPUC 

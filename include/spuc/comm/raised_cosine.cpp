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
//
#ifndef RAISEDCOS
#define RAISEDCOS
#include <math.h>
#include <complex.h>
#include <fir.h>
#include <fir_interp.h>
namespace SPUC {
//using namespace SPUC;
//! \ingroup fir
void root_raised_cosine(fir<long>& rcfir, double alpha, int rate) {
	extern double root_raised_cosine_imp(double alpha, double xin, double rate, long num_taps);
	int i;
	int num_taps = rcfir.num_taps;
	for (i=0;i*rate<num_taps;i++) {
		double off = i*rate;
		rcfir.coeff[i] = (long)root_raised_cosine_imp(alpha,double(i), (double)rate, num_taps);
	} 
}

//! \ingroup fir
void root_raised_cosine(fir<complex<long> >& rcfir, double alpha, int rate, int bits=10) {
	extern double root_raised_cosine_imp(double alpha, double xin, double rate, long num_taps);
	int i;
	int num_taps = rcfir.num_taps;
	int amplitude = (1 << (bits-2));
	for (i=0;i<num_taps;i++) {
		rcfir.coeff[i] = (long)(amplitude*root_raised_cosine_imp(alpha,double(i), (double)rate, num_taps));
	} 
}
//! \ingroup fir
void root_raised_cosine(fir<complex<long> >& rcfir, double alpha, int rate, 
						int bits=10, double scale=1.0) {
	extern double root_raised_cosine_imp(double alpha, double xin, double rate, long num_taps);

	int i;
	int num_taps = rcfir.num_taps;
	double amplitude = scale*(1 << (bits-2));
	for (i=0;i<num_taps;i++) {
		rcfir.coeff[i] = (long)floor(amplitude*root_raised_cosine_imp(
							alpha,double(i), (double)rate, num_taps)+0.5);
	} 
}

//! \ingroup fir
void root_raised_cosine(fir<complex<double> >& rcfir, double alpha, int rate) {
	extern double root_raised_cosine_imp(double alpha, double xin, double rate, long num_taps);
	int i;
	int num_taps = rcfir.num_taps;
	for (i=0;i<num_taps;i++) {
		rcfir.coeff[i] = root_raised_cosine_imp(alpha,double(i), (double)rate, num_taps);
	} 
}
//! \ingroup fir
void root_raised_cosine(fir_interp<complex<double> >& rcfir, double alpha, int rate) {
	extern double root_raised_cosine_imp(double alpha, double xin, double rate, long num_taps);
	int i;
	int num_taps = rcfir.num_taps;
	for (i=0;i<num_taps;i++) {
		rcfir.coeff[i] = root_raised_cosine_imp(alpha,double(i), (double)rate, num_taps);
	} 
}


//! \ingroup fir
void root_raised_cosine(fir<double>& rcfir, double alpha, int rate) {
	extern double root_raised_cosine_imp(double alpha, double xin, double rate, long num_taps);
	int i;
	int num_taps = rcfir.num_taps;
	for (i=0;i<num_taps;i++) {
		rcfir.coeff[i] = root_raised_cosine_imp(alpha,double(i), (double)rate, num_taps);
	} 
}

//! \ingroup fir
void raised_cosine(fir<long>& rcfir, double alpha, int rate) {
	extern double raised_cosine_imp(double alpha, double xin, double rate, long num_taps);
	int i;
	int num_taps = rcfir.num_taps;
	for (i=0;i<num_taps;i++) {
		rcfir.coeff[i] = (long)raised_cosine_imp(alpha,double(i), (double)rate, num_taps);
	} 
}

//! \ingroup fir
void raised_cosine(fir<double>& rcfir, double alpha, int rate) {
	extern double raised_cosine_imp(double alpha, double xin, double rate, long num_taps);
	int i;
	int num_taps = rcfir.num_taps;
	for (i=0;i<num_taps;i++) {
		rcfir.coeff[i] = raised_cosine_imp(alpha,double(i), (double)rate, num_taps);
	} 
}

#ifndef PI
#define PI 3.1415926535
#endif
double raised_cosine_imp(double alpha, double xin, double rate, long num_taps)
//-----------------------------------------------------------------------
//       Calculates the raised cosine pulse shape given the excess 
//       bandwidth value beta and the index.
//-----------------------------------------------------------------------
{
        double x1,x2,rc1;
        
        double xindx = xin-num_taps/2;
        x1 = PI*xindx/rate;
        x2 = 1-(4*alpha*alpha*(xindx/rate)*(xindx/rate));
		if (x1==0) return(1);
        if (x2 == 0) {
 	  		x2 = 8*alpha*(xindx/rate)*(xindx/rate);
          	rc1=sin(x1)*sin(alpha*x1)/x2;
		} else {
          rc1=(sin(x1)*cos(alpha*x1))/(x1*x2);
		}
        return(rc1);
}
double root_raised_cosine_imp(double alpha, double xin, double rate, long num_taps)
//-----------------------------------------------------------------------
//       Calculates the square root raised cosine pulse shape given the 
//		excess bandwidth value beta and the index.
//-----------------------------------------------------------------------
{
        double x1,x2,x3;
        double nom,denom;
        
        double xindx = xin-num_taps/2;
        x1 = PI*xindx/rate;          
        x2 = 4*alpha*xindx/rate;
        x3 = x2*x2-1;
		if (x3!=0) {
			if (x1!=0) nom = cos((1+alpha)*x1) +  sin((1-alpha)*x1)/(4*alpha*xindx/rate);
			else nom = cos((1+alpha)*x1) + (1-alpha)*PI/(4*alpha);
			denom = x3*PI;
		} else {
			if (alpha==1) return(-1);
			x3 = (1-alpha)*x1;
			x2 = (1+alpha)*x1;
			nom  = sin(x2)*(1+alpha)*PI - cos(x3)*((1-alpha)*PI*rate)/(4*alpha*xindx) 
					+ sin(x3)*rate*rate/(4*alpha*xindx*xindx);
			denom = -32*PI*alpha*alpha*xindx/rate;
	}
	return(4*alpha*nom/denom);

}
} // namespace SPUC 
#endif

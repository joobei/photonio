// 
// author="Tony Kirke"
// Copyright(c) 2005 Tony Kirke
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
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <spuc.h>
#include <math.h>
#include <complex.h>
#include <spuc_math.h>
#include <iir_1st.h>
#include <iir_2nd.h>
using namespace std;
namespace SPUC {
/*! 
  \addtogroup iir IIR filters
*/

/*!  \brief  Template Class for Elliptic low pass iir filter
  \ingroup iir
*/
//! Elliptic low pass filter.
//! Specify cut-off, order and stopband attenuation and passband ripple on construction
//! Filter is bilinear transformation of analog Elliptic
class elliptic
{
 private:
	long order;
	long odd;
	long n2;
	complex<double>* roots;
	complex<double>* zeros;
	iir_2nd<double>* iir;
	iir_1st<double>* iir_1;
	double gain;
	double* coeff;
	double epi;

 public:
	//! Constructor, fcd = cut-off (1=sampling rate)
	//! ord = Filter order
	//! stopattn = stopband attenuation ripple in dB
	//! ripple = passband ripple in dB
	elliptic(double fcd, long ord=1, double stopattn = 60.0, double ripple=0.1);
	//! Destructor
	~elliptic() {
	  delete [] roots;
	  delete [] zeros;
	  if (odd) delete iir_1;
	  delete [] iir;
	}
	//! Reset history
	void reset();
	//! print coefficients
	void print();
	//! Clock in sample and get output.
	double clock(double in);
 private:
	//! Do bilinear transformation
	void bilinear(long n2,complex<double>* r);
	//! Get 2nd order IIR coefficients
	void get_coeff(long n2);
	void get_roots(double ripple, double stopbandAtten, long order);
	double lamda_plane(double k, double m, int n, double eps);
	void s_plane(int n, double u, double m, double k, double Kk, double wca);
	// elliptic functions......
	double ellik(double phi,double k);
	double ellpk(double k);
	int ellpj( double u, double m, double* sn, double* cn, double* dn);
	double msqrt(double q);
};
} // namespace SPUC 

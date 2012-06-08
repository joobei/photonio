// 
// Copyright(c) 1993-1996 Tony Kirke
/* Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  This software is provided "as is" 
 * without express or implied warranty.

*/
#include <complex.h>
#include <fir.h>
#include <fir_interp.h>
namespace SPUC {
//! \file
//! \brief Root Raised Cosine and Raised Cosine functions
//!  \author Tony Kirke,  Copyright(c) 2001 
//!  
extern void root_raised_cosine(fir<long>& rcfir, double alpha, int rate);
extern void root_raised_cosine(fir<complex<long> >& rcfir, double alpha, int rate, int bits);
extern void root_raised_cosine(fir<complex<long> >& rcfir, double alpha, int rate, int bits, double scale);
extern void root_raised_cosine(fir<complex<double> >& rcfir, double alpha, int rate);
extern void root_raised_cosine(fir_interp<complex<double> >& rcfir, double alpha, int rate);
extern void root_raised_cosine(fir<double>& rcfir, double alpha, int rate);
extern void raised_cosine(fir<long>& rcfir, double alpha, int rate);
extern void raised_cosine(fir<double>& rcfir, double alpha, int rate);
extern double raised_cosine_imp(double alpha, double xin, double rate, long num_taps);
extern double root_raised_cosine_imp(double alpha, double xin, double rate, long num_taps);
}

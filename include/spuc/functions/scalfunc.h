/*---------------------------------------------------------------------------*
 *                                   IT++			             *
 *---------------------------------------------------------------------------*
 * Copyright (c) 1995-2002 by Tony Ottosson, Thomas Eriksson, Pål Frenger,   *
 * Tobias Ringström, and Jonas Samuelsson.                                   *
 *                                                                           *
 * Permission to use, copy, modify, and distribute this software and its     *
 * documentation under the terms of the GNU General Public License is hereby *
 * granted. No representations are made about the suitability of this        *
 * software for any purpose. It is provided "as is" without expressed or     *
 * implied warranty. See the GNU General Public License for more details.    *
 *---------------------------------------------------------------------------*/

/*! 
  \file 
  \brief Definitions of scalar functions
  \author Tony Ottosson and Pål Frenger

  1.18

  2002/12/19 23:56:44
*/

#ifndef __scalfunc_h
#define __scalfunc_h

#include <cmath>
#include <algorithm>
#include <cstdlib>

namespace SPUC {
/*!
  \defgroup miscfunc Miscellaneous functions
*/


#ifndef DOXYGEN_SHOULD_SKIP_THIS

#ifndef M_PI
#  define M_PI 3.14159265358979323846
#endif

#ifndef PI
const double PI=M_PI;
#endif

const float maxfloat=1.0E30F;

#ifndef M_LOG2E
#  define M_LOG2E	1.4426950408889634074
#endif
#ifndef M_SQRT2
#  define M_SQRT2	1.41421356237309504880
#endif

#endif //DOXYGEN_SHOULD_SKIP_THIS



// These functions are part of C99. But apparently not in Visual C++.
#ifdef _MSC_VER

/*! 
  \brief Error function
  \ingroup errorfunc
*/
double erf(double x);

/*! 
  \brief Complementary error function
  \ingroup errorfunc
*/
double erfc(double x);
#endif

/*! 
  \brief Q-function
  \ingroup errorfunc
*/
double Qfunc(double x);

/*! 
  \brief Inverse of error function
  \ingroup errorfunc
*/
double erfinv(double x);

/*! 
  \brief Gamma function
  \ingroup miscfunc
*/
double gamma(double x);

// Not supported on Visual C++
#ifdef _MSC_VER
double lgamma(double x);
extern int signgam;
double cbrt(double x);
#endif

#ifndef __CYGWIN__
/*!
  \defgroup logexpfunc Log and Exponential functions
  \brief Log and exponential functions
*/
/*! 
  \brief Base-2 logarithm
  \addtogroup logexpfunc
*/
inline double log2(double x) { return M_LOG2E*log(x); }
#endif 

/*! 
  \brief Base-b logarithm
  \addtogroup logexpfunc
*/
inline double logb(double b, double x) { return log(x)/log(b); }

/*! 
  \brief Sinc function. sinc(x) = sin(pi*x)/pi*x
  \ingroup miscfunc
*/
inline double sinc(double x) { return ( (x==0) ? 1 : (sin(PI*x)/PI/x) ); }

/*! \addtogroup miscfunc */
//!@{

#ifdef _MSC_VER
//! Round to nearest integer
inline double rint(double x) { return floor(x+0.5); }
#endif

//! Round to nearest integer
inline int round_i(double x) { return int(rint(x)); }
//! The nearest larger integer
inline int ceil_i(double x) { return int(ceil(x)); }
//! The nearest smaller integer
inline int floor_i(double x) { return int(floor(x)); }
//! Round to nearest integer, return result in double
inline double round(double x) { return rint(x); }

//! Return true if x is an integer
inline bool is_int(double x) { double dummy; return (modf(x, &dummy) == 0.0); }

//! Return true if x is an even integer
inline bool is_even(int x) { return ((x&1) == 0); }
//!@}

/*! \addtogroup logexpfunc */
//!@{

//! Calculate how many bits are needed to represent the integer n
inline int needed_bits(int n)
{
    int b=0;
	//    it_assert(n>0,"needed_bits(n): n must be greater than zero!");
    n--; while (n) {	n>>=1; b++; }
    return b;
}

//! The number of bits needed to encode {\em n} symbols. (Yes, it is exact!)
inline int needed_bits(double n) { 
	//	it_assert(n>0,"n_bits()"); 
	return int(ceil(log2(n))); }

//! Integer 2^x
#define pow2i(x) ((x)<0 ? 0 : (1<<(x)))
//! Calculate two to the power of x (2^x)
inline int pow2(int x) { return pow2i(x); }

//! Calculate two to the power of x (2^x)
inline double pow2(double x) { return pow(2.0, x); }
//! Calculate ten to the power of x (10^x)
inline double pow10(double x) { return pow(10.0, x); }

//! Decibel of x (10*log10(x))
inline double dB(double x) { return 10.0 * log10(x); }
//! Inverse of decibel
inline double inv_dB(double x) { return pow(10.0, x/10.0); }
//!@}

/*! \addtogroup miscfunc */
//!@{

//! Convert to Gray Code
inline int gray_code(int x) { return x^(x>>1); }

//! Compute the binomial coefficient "n over k" as a float.
double binom(int n, int k);

//! Compute the binomial coefficient "n over k" as an integer.
int binom_i(int n, int k);

//! Compute the base 10 logarithm of the binomial coefficient "n over k".
double log_binom(int n, int k);

//! Convert radians to degrees
inline double rad_to_deg(double x) { return 180.0 / M_PI * x; }
//! Convert degrees to radians
inline double deg_to_rad(double x) { return M_PI / 180.0 * x; }
//!@}

#if !defined(__GLIBC__) || __GLIBC__ < 2

/*! \addtogroup miscfunc */
//!@{

//! Arcus sinhyp
double asinh(double x);
//! Arcus coshyp
double acosh(double x);
//! Arcus tanhyp
double atanh(double x);
//!@}

#endif

/*! \addtogroup miscfunc */
//!@{

//! Square of x
inline double sqr(double x) { return x*x; }
//! The reminder of the division x/y
inline double rem(double x, double y) {return fmod(x,y);}
//! The sign of x
inline double sign(double x) { return x==0.0 ? 0.0 : (x<0.0 ? -1.0 : 1.0); }
//! The sign of x
inline double sgn(double x) { return x==0.0 ? 0.0 : (x<0.0 ? -1.0 : 1.0); }

//! Absolute value
inline signed char abs(signed char x) { return x>0 ? x : -x; }
//! Absolute value
inline short abs(short x) { return x>0 ? x : -x; }
#ifdef _MSC_VER // Included in cmath for gcc
//! Absolute value
inline double abs(double x) { return fabs(x); }
#endif

//double sigmoid(double x) { return 1.0/(1.0+exp(-x)); }

//! Calculates factorial coefficient for index <= 170.
double fact(int index);

//! Calculates the modulus, i.e. the signed reminder after division
long mod(long k, long n);

/*!
  \brief returns the greatest common divisor (GCD) \a g of the elements \a a and \a b.

  \a a and \a b must be non-negative integers. \a gdc(0,0) is 0 by convention; all other
  GCDs are positive integers.
*/
long gcd(long a, long b);

//! Integrate over the function \c f from \c A to \c B with tolerance \c tol
double integrate(double (*f)(double), double A, double B, double tol=0.001);

//!@}
}
#endif // __scalfunc_h

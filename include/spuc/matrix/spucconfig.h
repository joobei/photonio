/*---------------------------------------------------------------------------*
 *                                   IT++			             *
 *---------------------------------------------------------------------------*
 * Copyright (c) 1995-2001 by Tony Ottosson, Thomas Eriksson, Pål Frenger,   *
 * Tobias Ringström, and Jonas Samuelsson.                                   *
 *                                                                           *
 * Permission to use, copy, modify, and distribute this software and its     *
 * documentation under the terms of the GNU General Public License is hereby *
 * granted. No representations are made about the suitability of this        *
 * software for any purpose. It is provided "as is" without expressed or     *
 * implied warranty. See the GNU General Public License for more details.    *
 *---------------------------------------------------------------------------*/
//! 
//! Original code modified by Tony Kirke Feb 1, 2003
//! author="Tony Kirke" *
//  SPUC - Signal processing using C++ - A DSP library

/*! 
  \file 
  \brief Some it++ specific configurations and definitions

  1.6

  2002/12/19 23:56:44
*/

#ifndef __itconfig_h
#define __itconfig_h

#include <complex.h>
namespace SPUC {

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define ITPP_DEFAULT_EXCEPTIONS 0
#endif //DOXYGEN_SHOULD_SKIP_THIS

/*
//! Output stream operator for complex numbers
template <class T> std::ostream&
operator<<(std::ostream &os, const complex<T> &x)
{
    os <<  real (x);
    if (imag(x) >= 0)
	os << '+' << imag (x);
    else
	os << imag (x);
    return os << 'i';
}
*/
//! Type definition of \c double_complex
//typedef complex<double> double_complex;
//! Type definition of \c float_complex
//typedef complex<float> float_complex;


// --------- Special Visual C++ directives -----------------------------
#ifdef _MSC_VER

#pragma warning( disable : 4275 )

// Disable the warning about debug symbols being longer than 255 characters
#pragma warning( disable : 4786 )

#define TEMPLATE_FUN
#define long_long __int64
#ifdef min
#  undef min
#endif
#ifdef max
#  undef max
#endif

namespace std {
    template <class T>
    inline const T& min(const T& a, const T& b) {
	return b < a ? b : a;
    }
    template <class T>
    inline const T& max(const T& a, const T& b) {
	return  a < b ? b : a;
    }
}

#else

//! Define of TEMPLATE_FUN
#define TEMPLATE_FUN <>

//! Define of long_long
#define long_long long long

#endif
} // namespace SPUC
#endif // __itconfig_h

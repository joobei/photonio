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
//! 
//! Original code modified by Tony Kirke Feb 1, 2003
//! author="Tony Kirke" *
//  SPUC - Signal processing using C++ - A DSP library

/*! 
  \file 
  \brief Definitions of special operations on vectors and matricies optimized for speed
  \author Tony Ottosson and Tobias Ringstrom

  1.6

  2002/11/05 00:40:01
*/

#ifndef __fastmath_h
#define __fastmath_h

#include <binary.h>    // inclusion of this include made it possible to compile in M$VC; WHY??
#include <vector.h>
#include <matrix.h>
namespace SPUC {
//#include "scalfunc.h"

/*! 
  \relates Mat
  \brief Calculates m=m-v*v'*m
*/
void sub_v_vT_m(mat &m, const vec &v);

/*! 
  \relates Mat
  \brief Calculates m=m-m*v*v'
*/
void sub_m_v_vT(mat &m, const vec &v);
}
#endif // __fastmath_h
















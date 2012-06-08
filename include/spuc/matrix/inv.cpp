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
  \brief Implementation of matrix inversion routines
  \author Tony Ottosson

  1.2

  2002/10/02 15:37:18
*/

#include "inv.h"
#include "vector.h"
//#include "spucassert.h"
#include "lapack.h"

namespace SPUC {

bool inv(const mat &X, mat &Y)
{
  // it_assert1(X.rows() == X.cols(), "inv: matrix is not square");

  int m = X.rows(), info, lwork;
  lwork = m; // may be choosen better

  ivec p(m);
  Y = X;
  vec work(lwork);

  dgetrf_(&m, &m, Y._data(), &m, p._data(), &info); // LU-factorization
  if (info!=0)
    return false;

  dgetri_(&m, Y._data(), &m, p._data(), work._data(), &lwork, &info);
  return (info==0);
}

mat inv(const mat &X)
{
  mat Y;
  inv(X, Y);
  return Y;
}

bool inv(const cmat &X, cmat &Y)
{
  // it_assert1(X.rows() == X.cols(), "inv: matrix is not square");

  int m = X.rows(), info, lwork;
  lwork = m; // may be choosen better

  ivec p(m);
  Y = X;
  cvec work(lwork);

  zgetrf_(&m, &m, Y._data(), &m, p._data(), &info); // LU-factorization
  if (info!=0)
    return false;

  zgetri_(&m, Y._data(), &m, p._data(), work._data(), &lwork, &info);
  return (info==0);
}

cmat inv(const cmat &X)
{
  cmat Y;
  inv(X, Y);
  return Y;
}
}

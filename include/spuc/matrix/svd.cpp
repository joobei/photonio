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
  \brief Implementation of Singular Value Decompositions 
  \author Tony Ottosson and Tobias Ringström

  1.10

  2002/10/13 14:31:32
*/

#include "svd.h"
#include "matfunc.h"
//#include "elmatfunc.h"
#include "specmat.h"
#include "eigen.h"
#include "fastmath.h"
#include "lapack.h"

//using namespace std;
using namespace SPUC;

#ifdef HAVE_LAPACK

bool svd(const mat &A, vec &S)
{
  char jobu='N', jobvt='N';
  int m, n, lda, ldu, ldvt, lwork, info;
  m = lda = ldu = A.rows();
  n = ldvt = A.cols();
  lwork = max(3*min(m,n)+max(m,n), 5*min(m,n));
  
  mat U, V;
  //U.set_size(m,m, false);
  //V.set_size(n,n, false);
  S.set_size(min(m,n), false);
  vec work(lwork);
  
  mat B(A);

  dgesvd_(&jobu, &jobvt, &m, &n, B._data(), &lda, S._data(), U._data(), &ldu, V._data(), &ldvt, work._data(), &lwork, &info);

  return (info==0);
}


bool svd(const cmat &A, vec &S)
{
  char jobu='N', jobvt='N';
  int m, n, lda, ldu, ldvt, lwork, info;
  m = lda = ldu = A.rows();
  n = ldvt = A.cols();
  lwork = 2*min(m,n)+max(m,n);
  
  cvec U, V;
  //U.set_size(m,m, false);
  //V.set_size(n,n, false);
  S.set_size(min(m,n), false);
  cvec work(lwork);
  vec rwork(max(1, 5*min(m, n)));
  
  cmat B(A);

  zgesvd_(&jobu, &jobvt, &m, &n, B._data(), &lda, S._data(), U._data(), &ldu, V._data(), &ldvt, work._data(), &lwork, rwork._data(), &info);

  return (info==0);
}


bool svd(const mat &A, mat &U, vec &S, mat &V)
{
  char jobu='A', jobvt='A';
  int m, n, lda, ldu, ldvt, lwork, info;
  m = lda = ldu = A.rows();
  n = ldvt = A.cols();
  lwork = max(3*min(m,n)+max(m,n), 5*min(m,n));
  
  U.set_size(m,m, false);
  V.set_size(n,n, false);
  S.set_size(min(m,n), false);
  vec work(lwork);
  
  mat B(A);

  dgesvd_(&jobu, &jobvt, &m, &n, B._data(), &lda, S._data(), U._data(), &ldu, V._data(), &ldvt, work._data(), &lwork, &info);

  V = transpose(V); // This is probably slow!!!

  return (info==0);
}


bool svd(const cmat &A, cmat &U, vec &S, cmat &V)
{
  char jobu='A', jobvt='A';
  int m, n, lda, ldu, ldvt, lwork, info;
  m = lda = ldu = A.rows();
  n = ldvt = A.cols();
  lwork = 2*min(m,n)+max(m,n);
  
  U.set_size(m,m, false);
  V.set_size(n,n, false);
  S.set_size(min(m,n), false);
  cvec work(lwork);
  vec rwork(max(1, 5*min(m, n)));
  
  cmat B(A);

  zgesvd_(&jobu, &jobvt, &m, &n, B._data(), &lda, S._data(), U._data(), &ldu, V._data(), &ldvt, work._data(), &lwork, rwork._data(), &info);

  V = transpose(conj(V)); // This is slow!!!

  return (info==0);
}


vec svd(const mat &A)
{
  vec S;
  svd(A, S);
  return S;
}

vec svd(const cmat &A)
{
  vec S;
  svd(A, S);
  return S;
}

#else // SVD need LAPACK otherwise you will get an error message
#ifdef XXX
bool svd(const mat &A, vec &S)
{
  it_error("svd for cmat only available if the LAPACK support in it++ is enabled");
  return false;
}

bool svd(const cmat &A, vec &S)
{
  it_error("svd for cmat only available if the LAPACK support in it++ is enabled");
  return false;
}

bool svd(const mat &A, mat &U, vec &S, mat &V)
{
  it_error("svd for cmat only available if the LAPACK support in it++ is enabled");
  return false;
}

bool svd(const cmat &A, cmat &U, vec &S, cmat &V)
{
  it_error("svd for cmat only available if the LAPACK support in it++ is enabled");
  return false;
}

vec svd(const mat &A)
{
  it_error("svd for cmat only available if the LAPACK support in it++ is enabled");
  vec temp;
  return temp;
}

vec svd(const cmat &A)
{
  it_error("svd for cmat only available if the LAPACK support in it++ is enabled");
  vec temp;
  return temp;
}
#endif
#endif // HAVE_LAPACK


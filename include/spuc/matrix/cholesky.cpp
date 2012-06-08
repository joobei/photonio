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
  \brief Implementation of Cholesky factorisation functions
  \author Tony Ottosson

  1.6

  2002/10/02 15:37:18
*/

#include <algorithm>
#include <cassert>
#include "spucconfig.h"
#include "cholesky.h"
#include "lapack.h"
using namespace SPUC;
namespace SPUC {
#ifdef HAVE_LAPACK
bool chol(const mat &X, mat &F)
{
  char uplo='U';
  int n, lda, info;
  n = lda = X.rows();

  F = X; // input matrix is overwritten
  
  dpotrf_(&uplo, &n, F._data(), &lda, &info);

  // Set lower part to zero
  for (int i=0; i<n; i++)
    for(int j=i+1; j<n; j++)
      F(j,i) = 0;

  return (info==0);
}


bool chol(const cmat &X, cmat &F)
{
  char uplo='U';
  int n, lda, info;
  n = lda = X.rows();

  F = X; // input matrix is overwritten
  
  zpotrf_(&uplo, &n, F._data(), &lda, &info);

  // Set lower part to zero
  for (int i=0; i<n; i++)
    for(int j=i+1; j<n; j++)
      F(j,i) = 0;

  return (info==0);
}

cmat chol(const cmat &X)
{
    cmat F;
    if (!chol(X, F)) {
	it_warning("cholesky factorization didn't succeed");
    }
  
    return F;
}

#else

bool chol(const mat &X, mat &F)
{
    assert(X.cols() == X.rows());
    int i,j, k, n=X.cols();

    F=X;
  
    for(k=0;k<n-1;k++) {
	F(k,k)=sqrt(F(k,k));
	if (F(k,k) <= 0.0) { // not positive definite matrix X (sqrt(F(j,j)) > 0 )
	    return false;
	}
	// F(k,k,k+1,n-1)/=F(k,k);
	for (i=k+1; i<n; i++) F(k,i)/=F(k,k);

	for (j=k+1;j<n;j++) {
      
	    /* Should be: F(j,j,j,n-1)-=F(k,k,j,n-1)*F(k,j);
	       but this is to slow. Instead directly access the matrix data-structure */
	    for (i=j; i<n; i++) {
		F._elem(j,i) -= F._elem(k,i) * F._elem(k,j);
	    }
      
	} // j
    } // k
  
    F(n-1,n-1)=sqrt(F(n-1,n-1));
    if (F(k,k) <= 0.0) { // not positive definite matrix X (sqrt(F(j,j)) > 0 )
	return false;
    }
  
    // set all lower triangular elements to zero
    for (i=0;i<n;i++) {
	for (j=i+1;j<n;j++) {
	    F(j,i)=0;
	}
    }

    return true;
}


bool chol(const cmat &X, cmat &F)
{
//  it_error("chol() for cmat only available if the LAPACK support in it++ is enabled");
  return false;
}

cmat chol(const cmat &X)
{
//  it_error("chol() for cmat only available if the LAPACK support in it++ is enabled");
  cmat temp;
  return temp;
}

#endif

mat chol(const mat &X)
{
	bool chol(const mat &X, mat &F);
    mat F;
	bool a = chol(X,F);
    if (!a) {
	  //	it_warning("cholesky factorization didn't succeed");
    }
  
    return F;
}


bool chol(const mat &X, int p, mat &F)
{
    assert(X.cols() == X.rows() && p <= X.cols()/2);
    int n = X.cols(), i, j, k, l;
    double temp;

    F=X;
  
    for (j=0;j<n;j++) {
		for (k=MAX(0,j-p); k<j; k++) {
			l=MIN(k+p,n-1);

	    /* Should be: F(j,j,j,l)-=F(k,j)*F(k,k,j,l);
	       but this is to slow. Instead directly access the matrix data-structure */
	    for (i=j; i<=l; i++) {
		F._elem(j,i) -= F._elem(k,i) * F._elem(k,i);
	    }

	} // k
	l=MIN(j+p,n-1);
	temp=sqrt(F(j,j));
	if (temp <= 0.0) { // not positive definite
	    return false;
	}
	//F(j,j,j,l)/=temp;
	for (i=j; i<=l; i++) F(j,i)/=temp;
    } // j

    // set all lower triangular elements to zero
    for (i=0;i<n;i++) {
	for (j=i+1;j<MIN(i+p+1,n);j++) {
	    F(j,i)=0;
	}
    }

    return true;
}


mat chol(const mat &X, int p)
{
	bool chol(const mat &X, int p, mat &F);
    mat F;
    if (!chol(X, p, F)) {
//	it_error_if(1,"cholesky factorization not possible");
    }
  
    return F;
}
}

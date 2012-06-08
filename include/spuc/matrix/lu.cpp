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
  \brief Implementation of LU factorisation functions.
  \author Tony Ottosson

  1.7

  2002/10/02 15:37:18
*/
#include "vector.h"
#include "matrix.h"
#include "specmat.h"

#include "lu.h"
//#include "matfunc.h"
//#include "elmatfunc.h"
#include "lapack.h"
namespace SPUC {



#ifdef HAVE_LAPACK
bool lu(const mat &X, mat &L, mat &U, ivec &p)
{
  assert(X.rows() == X.cols()); // , "lu: matrix is not quadratic");
  //int m, n, lda, info;
  //m = n = lda = X.rows();
  int m = X.rows(), info;

  mat A(X);
  L.set_size(m, m, false); 
  U.set_size(m, m, false); 
  p.set_size(m, false);

  dgetrf_(&m, &m, A._data(), &m, p._data(), &info);

  for (int i=0; i<m; i++) {
    for (int j=i; j<m; j++) {
      if (i == j) { // diagonal
	L(i,j) = 1;
	U(i,j) = A(i,j);
      } else { // upper and lower triangular parts
	L(i,j) = U(j,i) = 0;
	L(j,i) = A(j,i);
	U(i,j) = A(i,j);
      }
    }
  }
  
  p = p - 1; // Fortran counts from 1  

  return (info==0);
}

// Slower than not using LAPACK when matrix size smaller than approx 20.
bool lu(const cmat &X, cmat &L, cmat &U, ivec &p)
{
  assert(X.rows() == X.cols()); // , "lu: matrix is not quadratic");
  //int m, n, lda, info;
  //m = n = lda = X.rows();
  int m = X.rows(), info;

  cmat A(X);
  L.set_size(m, m, false); 
  U.set_size(m, m, false); 
  p.set_size(m, false);

  zgetrf_(&m, &m, A._data(), &m, p._data(), &info);

  for (int i=0; i<m; i++) {
    for (int j=i; j<m; j++) {
      if (i == j) { // diagonal
	L(i,j) = 1;
	U(i,j) = A(i,j);
      } else { // upper and lower triangular parts
	L(i,j) = U(j,i) = 0;
	L(j,i) = A(j,i);
	U(i,j) = A(i,j);
      }
    }
  }
  
  p = p - 1; // Fortran counts from 1  

  return (info==0);
}


#else

bool lu(const mat &X, mat &L, mat &U, ivec &p)
{
//    it_assert1(X.rows() == X.cols(), "lu: matrix is not quadratic");
  
    int u, k, i, j, n = X.rows();
    double Umax;

    U=X; // temporary matrix

    p.set_size(n,0);
    L.set_size(n,n,0);

    for (k=0; k<n-1; k++) {
    
	// determine u.  Alt. u=max_index(abs(U(k,n-1,k,k)));
	u=k;
	Umax=fabs(U(k,k));
	for (i=k+1; i<n; i++) {
	    if (fabs(U(i,k)) > Umax) {
		Umax=fabs(U(i,k));
		u=i;
	    }
	}

	U.swap_rows(k, u);
    
	p(k)=u;

	if (U(k,k) != 0.0) {
	  //U(k+1,n-1,k,k)/=U(k,k);
	  for(i=k+1;i<n;i++) U(i,k)/=U(k,k);
      
	    /* Should be:  U(k+1,n-1,k+1,n-1)-=U(k+1,n-1,k,k)*U(k,k,k+1,n-1);
	       but this is to slow. Instead work directly on the matrix data-structure. */
	    //int i_pos=(k+1)*U._row_offset(), k_pos=k*U._row_offset();
	    for (i=k+1; i<n; i++) {
		for (j=k+1; j<n; j++) {
		    U._elem(i,j) -= U._elem(i,k) * U._elem(k,j);
		    //U._data()[i_pos+j]-=U._data()[i_pos+k]*U._data()[k_pos+j];
		}
		//i_pos += U._row_offset();
	    }
	} // if
    
    } // k
  
    p(n-1)=n-1;
  
    // Set L and reset all lower elements of U.
    // set all lower triangular elements to zero
    for (i=0; i<n; i++) {
	L(i,i)=1;
	for (j=i+1; j<n; j++) {
	    L(j,i)=U(j,i);
	    U(j,i)=0;
	    L(i,j)=0;
	}
    }

    return true;
}

#endif // HAVE_LAPACK


void interchange_permutations(vec &b, const ivec &p)
{
    assert(b.size() == p.size());
    double temp;
  
    for (int k=0; k<b.size(); k++) {
	temp=b(k);
	b(k)=b(p(k));
	b(p(k))=temp;
    }
}


bmat permutation_matrix(const ivec &p)
{
    assert (p.size() > 0);
    int n = p.size(), k;
    bmat P, identity;
    bvec row_k, row_pk;
    identity=eye_b(n);

  
    for (k=n-1; k>=0; k--) {
	// swap rows k and p(k) in identity
	row_k=identity.get_row(k);
	row_pk=identity.get_row(p(k));
	identity.set_row(k, row_pk);
	identity.set_row(p(k), row_k);
    
	if (k == n-1) {
	    P=identity;
	} else {
	    P*=identity;
	}

	// swap back
	identity.set_row(k, row_k);
	identity.set_row(p(k), row_pk);
    }
    return P;
}
}

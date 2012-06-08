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
  \brief Implementation of functions for solving linear equation systems.
  \author Tony Ottosson

  1.4

  2002/02/14 20:54:59
*/
#include <spuc.h>
#include <cassert>
//#include "spucconfig.h"
#include <vector.h>
#include <matrix.h>
#include "specmat.h"
#include "lu.h"
#include "fastmath.h"
#include "ls_solve.h"
namespace SPUC {
vec ls_solve(const mat &A, const vec &b)
{
	vec ls_solve(const mat &L, const mat &U, const vec &b);
    mat L, U;
    ivec p;
    vec btemp=b;
  
    lu(A, L, U, p);
    interchange_permutations(btemp, p);

    return ls_solve(L, U, btemp); 
}

mat ls_solve(const mat &A, const mat &B)
{
	vec ls_solve(const mat &L, const mat &U, const vec &b);
    mat L, U;
    ivec p;
    vec btemp;
  
    lu(A, L, U, p);

    mat X(B.rows(), B.cols());
  
    for (int i=0; i<B.cols(); i++) {
	btemp=B.get_col(i);
	interchange_permutations(btemp, p);
	X.set_col(i, ls_solve(L, U, btemp));
    }

    return X;
}

vec ls_solve_chol(const mat &A, const vec &b)
{
 	vec ls_solve(const mat &L, const mat &U, const vec &b);
   mat U(A.rows(),A.cols());

//    it_error_if(!chol(A,U), "ls_solve_chol: Linear system not positive definite");
    return ls_solve(transpose(U), U, b);
}

vec ls_solve(const mat &L, const mat &U, const vec &b)
{
    vec x(L.rows());
  
    forward_substitution(L, b, x); // Solve Ly=b, Here y=x
    backward_substitution(U, x, x); // Solve Ux=y, Here x=y
  
    return x;
}

vec ls_solve_chol(const mat &A, int p, const vec &b)
{
vec ls_solve(const mat &L, int p, const mat &U, int q, const vec &b);
    mat U(A.rows(),A.cols());

//    it_error_if(!chol(A, p, U), "ls_solve_chol: Linear system not positive definite");
    return ls_solve(transpose(U), p, U, p, b);
}

vec ls_solve(const mat &L, int p, const mat &U, int q, const vec &b)
{
    vec x(L.rows());
  
    forward_substitution(L, p, b, x); // Solve Ly=b, Here y=x
    backward_substitution(U, q, x, x); // Solve Ux=y, Here x=y
  
    return x;
}

vec ls_solve_od(const mat &A, const vec &b)
{
    int m=A.rows(), n=A.cols();
    double beta;
    mat A2(A), submat;
    vec b2(b), v;

//    it_assert1(m >= n, "The system is under-determined!");
//    it_assert1(m == b.size(), "The number of rows in A must equal the length of b!");
    
    // Perform a Householder QR factorization
    for (int j=0; j<n; j++) {
	house(rvectorize(A2(j, m-1, j, j)), v, beta);
	v *= sqrt(beta);
	// 	submat.ref(A2, j,m-1, j,n-1);
 	submat = A2(j,m-1,j,n-1); // Time-consuming
	sub_v_vT_m(submat, v);
	b2.set_subvector(j,m-1, b2(j,m-1)- v*(v*b2(j,m-1)));
    }

    return backward_substitution(A2(0,n-1,0,n-1), b2(0,n-1));
}

mat ls_solve_od(const mat &A, const mat &B)
{
    int m=A.rows(), n=A.cols(), N=B.cols(), j;
    double beta;
    mat A2(A), B2(B), B3(n,N), submat, submat2;
    vec tmp(n), v;

//    it_assert1(m >= n, "The system is under-determined!");
    //it_assert1(m == B.rows(), "The number of rows in A must equal the number of rows in B!");

    // Perform a Householder QR factorization
    for (j=0; j<n; j++) {
	house(rvectorize(A2(j, m-1, j, j)), v, beta);
	v *= sqrt(beta);
	// 	submat.ref(A2, j,m-1, j,n-1);
 	submat = A2(j,m-1,j,n-1);
	sub_v_vT_m(submat, v);
	// 	submat.ref(B2, j,m-1, 0,N-1);
 	submat = B2(j,m-1,0,N-1);
	sub_v_vT_m(submat, v);
    }

    //    submat.ref(A2, 0,n-1,0,n-1);
    //    submat2.ref(B2, 0,n-1,0,N-1);
    submat = A2(0,n-1,0,n-1);
    submat2 = B2(0,n-1,0,N-1);
    for (j=0; j<N; j++) {
	backward_substitution(submat, submat2.get_col(j), tmp);
	B3.set_col(j, tmp);
    }
    
    return B3;
}

vec backslash(const mat &A, const vec &b)
{
  vec ls_solve(const mat &A, const vec &b);
  vec ls_solve_od(const mat &A, const vec &b);
  int m=A.rows(), n=A.cols();

  if (m == n)
	return ls_solve(A,b);
  else if (m > n)
	return ls_solve_od(A,b);

//    it_error("Cannot solve under-determined linear equation systems!");
    return vec(0);
}

mat backslash(const mat &A, const mat &B)
{
  mat ls_solve(const mat &A, const mat &b);
  mat ls_solve_od(const mat &A, const mat &b);
    int m=A.rows(), n=A.cols();

    if (m == n)
	return ls_solve(A,B);
    else if (m > n)
	return ls_solve_od(A,B);
    
    //it_error("Cannot solve under-determined linear equation systems!");
    return vec(0);
}

vec forward_substitution(const mat &L, const vec &b)
{
    void forward_substitution(const mat &L, const vec &b, vec &x);
    int n = L.rows();
    vec x(n);
  
    forward_substitution(L, b, x);

    return x;
}

void forward_substitution(const mat &L, const vec &b, vec &x)
{
    assert( L.rows() == L.cols() && L.cols() == b.size() && b.size() == x.size() );
    int n = L.rows(), i, j;
    double temp;

    x(0)=b(0)/L(0,0);
    for (i=1;i<n;i++) {
	// Should be: x(i)=((b(i)-L(i,i,0,i-1)*x(0,i-1))/L(i,i))(0); but this is to slow.
	//i_pos=i*L._row_offset();
	temp=0;
	for (j=0; j<i; j++) {
	    temp += L._elem(i,j) * x(j);
	    //temp+=L._data()[i_pos+j]*x(j);
	}
	x(i) = (b(i)-temp)/L._elem(i,i);
	//x(i)=(b(i)-temp)/L._data()[i_pos+i];
    }
}

vec forward_substitution(const mat &L, int p, const vec &b)
{
 void forward_substitution(const mat &L, int p, const vec &b, vec &x);
   int n = L.rows();
    vec x(n);
  
    forward_substitution(L, p, b, x);

    return x;
}

void forward_substitution(const mat &L, int p, const vec &b, vec &x)
{
    assert( L.rows() == L.cols() && L.cols() == b.size() && b.size() == x.size() && p <= L.rows()/2 );
    int n = L.rows(), i, j;

    x=b;
  
    for (j=0;j<n;j++) {
	x(j)/=L(j,j);
	for (i=j+1;i<MIN(j+p+1,n);i++) {
	    x(i)-=L(i,j)*x(j);
	}
    }
}

vec backward_substitution(const mat &U, const vec &b)
{
void backward_substitution(const mat &U, const vec &b, vec &x);
    vec x(U.rows());
    backward_substitution(U, b, x);

    return x;
}

void backward_substitution(const mat &U, const vec &b, vec &x)
{
    assert( U.rows() == U.cols() && U.cols() == b.size() && b.size() == x.size() );
    int n = U.rows(), i, j;
    double temp;

    x(n-1)=b(n-1)/U(n-1,n-1);
    for (i=n-2; i>=0; i--) {
	// Should be: x(i)=((b(i)-U(i,i,i+1,n-1)*x(i+1,n-1))/U(i,i))(0); but this is too slow.
	temp=0;
	//i_pos=i*U._row_offset();
	for (j=i+1; j<n; j++) {
	    temp += U._elem(i,j) * x(j);
	    //temp+=U._data()[i_pos+j]*x(j);
	}
	x(i) = (b(i)-temp)/U._elem(i,i);
	//x(i)=(b(i)-temp)/U._data()[i_pos+i];
    }
}

vec backward_substitution(const mat &U, int q, const vec &b)
{
void backward_substitution(const mat &U, int q, const vec &b, vec &x);
    vec x(U.rows());
    backward_substitution(U, q, b, x);

    return x;
}

void backward_substitution(const mat &U, int q, const vec &b, vec &x)
{
    assert( U.rows() == U.cols() && U.cols() == b.size() && b.size() == x.size() && q <= U.rows()/2);
    int n = U.rows(), i, j;

    x=b;
  
    for (j=n-1; j>=0; j--) {
	x(j) /= U(j,j);
	for (i=MAX(0,j-q); i<j; i++) {
	    x(i)-=U(i,j)*x(j);
	}
    }
}
}

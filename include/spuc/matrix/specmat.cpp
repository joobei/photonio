/*---------------------------------------------------------------------------*
 *                                   IT++			             *
 *---------------------------------------------------------------------------*
 * Copyright (c) 1995-2001 by Tony Ottosson, Thomas Eriksson, P�l Frenger,   *
 * Tobias Ringstr�m, and Jonas Samuelsson.                                   *
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
  \brief Implementation of special vectors and matrices.
  \author Tony Ottosson, Tobias Ringstr�m and P�l Frenger

  1.3

  2002/12/19 23:56:46
*/
#include <math.h>
#ifndef M_PI
#define M_PI 3.14
#endif

#include <complex.h>
#include "specmat.h"
//#include "matrix_functions.h"
#include "binary.h"
namespace SPUC {
ivec find(const bvec &invector)
{
    assert(invector.size()>0);
    ivec temp(invector.size());
    int pos=0;
    for (int i=0;i<invector.size();i++) {
	if (invector(i)==bin(1)) {
	    temp(pos)=i;pos++;
	}
    }
    temp.set_size(pos, true);
    return temp;
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#define CREATE_SET_FUNS(typef,typem,name,value) \
typef name(int size)                             \
{                                                \
    typef t(size);                               \
    t = value;                                   \
    return t;                                    \
}                                                \
                                                 \
typem name(int rows, int cols)                   \
{                                                \
    typem t(rows, cols);                         \
    t = value;                                   \
    return t;                                    \
}

#define CREATE_EYE_FUN(type,name,zero,one) \
type name(int size) {                      \
    type t(size,size);                     \
    t = zero;                              \
    for (int i=0; i<size; i++)             \
	t(i,i) = one;                      \
    return t;                              \
}

CREATE_SET_FUNS(  vec,   mat, ones,    1.0)
CREATE_SET_FUNS( bvec,  bmat, ones_b,  bin(1))
CREATE_SET_FUNS( ivec,  imat, ones_i,  1)
CREATE_SET_FUNS( cvec,  cmat, ones_c,  complex<double>(1.0))

CREATE_SET_FUNS(  vec,   mat, zeros,    0.0)
CREATE_SET_FUNS( bvec,  bmat, zeros_b,  bin(0))
CREATE_SET_FUNS( ivec,  imat, zeros_i,  0)
CREATE_SET_FUNS( cvec,  cmat, zeros_c,  complex<double>(0.0))

CREATE_EYE_FUN(mat,   eye,    0.0,    1.0)
CREATE_EYE_FUN(bmat,  eye_b,  bin(0), bin(1))
CREATE_EYE_FUN(imat,  eye_i,  0,      1)
CREATE_EYE_FUN(cmat,  eye_c,  complex<double>(0.0), complex<double>(1.0))

template <class T>
void eye(int size, Mat<T> &m)
{
    m.set_size(size, size, false);
    m = T(0);
    for (int i=size-1; i>=0; i--)
	m(i,i) = T(1);
}

#endif //DOXYGEN_SHOULD_SKIP_THIS

vec impulse(int size) {
    vec	t(size);
    t.clear();
    t[0]=1.0;
    return t;
}

vec linspace(double from, double to, int points) {
    if (points<2){
	// This is the "Matlab definition" of linspace
	vec output(1);
	output(0)=to;
	return output;
    }
    else{
	vec	output(points);
	double step = (to - from) / double(points-1);
	int	i;
	for (i=0; i<points; i++)
	    output(i) = from + i*step;
	return output;
    }
}

vec hamming(int size)
{
    vec	t(size);
    
    if (size == 1)
	t(0) = 0.08;
    else
	for (int i=0;i<size;i++)
	    t[i]=(0.54-0.46*cos(2.0*M_PI*i/(size-1)));
    
    return t;
}

vec hanning(int size)
{
    vec	t(size);
    
    for (int i=0;i<size;i++)
	t(i) = 0.5 * (1.0 - cos(2.0*M_PI*(i+1)/(size+1)));
    
    return t;
}

vec triang(int size)
{
    vec	t(size);
    
    if (size % 2) { // Odd
	for (int i=0; i<size/2; i++)
	    t(i) = t(size-i-1) = 2.0*(i+1)/(size+1);
	t(size/2) = 1.0;
    } else
	for (int i=0; i<size/2; i++)
	    t(i) = t(size-i-1) = (2.0*i+1)/size;

    return t;
}

vec sqrt_win(int size)
{
    vec	t(size);
    
    if (size % 2) { // Odd
	for (int i=0; i<size/2; i++)
	    t(i) = t(size-i-1) = sqrt(2.0*(i+1)/(size+1));
	t(size/2) = 1.0;
    } else
	for (int i=0; i<size/2; i++)
	    t(i) = t(size-i-1) = sqrt((2.0*i+1)/size);

    return t;
}

// Construct a Hadamard-imat of size "size"
imat hadamard(int size) {	
    int i,k,l,pow2,logsize;
    imat H(size,size);
    logsize=(int)(log((double)size)/log(2.0)+0.5);
//    it_assert1(pow2i(logsize)==size,"hadamard size not a power of 2");
    H(0,0)=1;H(0,1)=1;H(1,0)=1;H(1,1)=-1;
	
    for (i=1;i<logsize;i++) {
// 	pow2=(int)round(pow(2,i));  // Unbeliveably slow
	pow2 = 1<<i;
	for (k=0;k<pow2;k++) {
	    for (l=0;l<pow2;l++) {
		H(k,l)=H(k,l);
		H(k+pow2,l)=H(k,l);
		H(k,l+pow2)=H(k,l);
		H(k+pow2,l+pow2)=(-1)*H(k,l);
	    }
	}
    }
    return H;
}

imat jacobsthal(int p)
{
    int quadratic_residue;
    imat out(p,p);
    int i, j;

    out = -1; // start with all elements equal to "-1"
  
    // Generate a complete list of quadratic residues
    for (i=0; i<(p-1)/2; i++) {
	quadratic_residue=((i+1)*(i+1))%p;
	for (j=0; j<p; j++) { // set this element in all rows (col-row)=quadratic_residue
	    out(j, (j+quadratic_residue)%p)=1;
	}
    }

    // set diagonal elements to zero
    for (i=0; i<p; i++) {
	out(i,i)=0;
    }
    return out;
}

imat conference(int n)
{
//    it_assert1(n%4 == 2, "conference(int n); wrong size");
    int pm=n-1; // p must be odd prime, not checked
    imat out(n,n);

    out.set_submatrix(1,n-1,1,n-1, jacobsthal(pm));
    out.set_submatrix(0,0,1,n-1, 1);
    out.set_submatrix(1,n-1,0,0, 1);
    out(0,0)=0;

    return out;
}

mat rotation_matrix(int dim, int plane1, int plane2, double angle)
{
    mat m;
    double c = cos(angle), s = sin(angle);
  
    //it_assert(plane1>=0 && plane2>=0 &&
	      //plane1<dim && plane2<dim && plane1!=plane2,
	      //"Invalid arguments to rotation_matrix()");

    m.set_size(dim, dim, false);
    m = 0.0;
    for (int i=0; i<dim; i++)
	m(i,i) = 1.0;

    m(plane1, plane1) = c;
    m(plane1, plane2) = -s;
    m(plane2, plane1) = s;
    m(plane2, plane2) = c;

    return m;
}

void house(const vec &x, vec &v, double &beta)
{
    double sigma, mu;
    int n = x.size();

    v = x;
    if (n == 1) {
	v(0) = 1.0;
	beta = 0.0;
	return;
    }
    sigma = energy(x(1, n-1));
    v(0) = 1.0;
    if (sigma == 0.0)
	beta = 0.0;
    else {
	mu = sqrt(x(0)*x(0) + sigma);
	if (x(0) <= 0.0)
	    v(0) = x(0) - mu;
	else
	    v(0) = -sigma / (x(0) + mu);
	beta = 2 * (v(0)*v(0)) / (sigma + (v(0)*v(0)));
	v /= v(0);
    }
}

void givens(double a, double b, double &c, double &s)
{
    double t;
    
    if (b == 0) {
	c = 1.0;
	s = 0.0;
    }
    else {
	if (fabs(b) > fabs(a)) {
	    t = -a/b;
	    s = -1.0 / sqrt(1 + t*t);
	    c = s * t;
	}
	else {
	    t = -b/a;
	    c = 1.0 / sqrt(1 + t*t);
	    s = c * t;
	}
    }
}

void givens(double a, double b, mat &m)
{
    double t, c, s;

    m.set_size(2,2);
    
    if (b == 0) {
	m(0,0) = 1.0;
	m(1,1) = 1.0;
	m(1,0) = 0.0;
	m(0,1) = 0.0;
    }
    else {
	if (fabs(b) > fabs(a)) {
	    t = -a/b;
	    s = -1.0 / sqrt(1 + t*t);
	    c = s * t;
	}
	else {
	    t = -b/a;
	    c = 1.0 / sqrt(1 + t*t);
	    s = c * t;
	}
	m(0,0) = c;
	m(1,1) = c;
	m(0,1) = s;
	m(1,0) = -s;
    }
}

mat givens(double a, double b)
{
    mat m(2,2);
    givens(a, b, m);
    return m;
}


void givens_t(double a, double b, mat &m)
{
    double t, c, s;

    m.set_size(2,2);
    
    if (b == 0) {
	m(0,0) = 1.0;
	m(1,1) = 1.0;
	m(1,0) = 0.0;
	m(0,1) = 0.0;
    }
    else {
	if (fabs(b) > fabs(a)) {
	    t = -a/b;
	    s = -1.0 / sqrt(1 + t*t);
	    c = s * t;
	}
	else {
	    t = -b/a;
	    c = 1.0 / sqrt(1 + t*t);
	    s = c * t;
	}
	m(0,0) = c;
	m(1,1) = c;
	m(0,1) = -s;
	m(1,0) = s;
    }
}

mat givens_t(double a, double b)
{
    mat m(2,2);
    givens_t(a, b, m);
    return m;
}

//! Template instantiation of eye
template void eye(int, mat &);
//! Template instantiation of eye
template void eye(int, bmat &);
//! Template instantiation of eye
template void eye(int, imat &);
//! Template instantiation of eye
template void eye(int, cmat &);
}

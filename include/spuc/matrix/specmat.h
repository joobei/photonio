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
  \brief Definitions of special vectors and matrices
  \author Tony Ottosson, Tobias Ringstr�m and P�l Frenger

  1.7

  2002/12/19 23:56:44
*/

#ifndef __specmat_h
#define __specmat_h

#include "vector.h"
#include "matrix.h"
//#include "converters.h"
//#include "scalfunc.h"
namespace SPUC {
/*! 
  \brief Return a integer vector with indicies where bvec == 1
  \ingroup miscfunc
*/
ivec find(const bvec &invector);

/*! \defgroup specmat Special Matricies */

//!\addtogroup specmat
//!@{

//! A float vector of ones
vec ones(int size);
//! A Binary vector of ones
bvec ones_b(int size);
//! A Int vector of ones
ivec ones_i(int size);
//! A float Complex vector of ones
cvec ones_c(int size);

//! A float (rows,cols)-matrix of ones
mat ones(int rows, int cols);
//! A Binary (rows,cols)-matrix of ones
bmat ones_b(int rows, int cols);
//! A Int (rows,cols)-matrix of ones
imat ones_i(int rows, int cols);
//! A Double Complex (rows,cols)-matrix of ones
cmat ones_c(int rows, int cols);

//! A Double vector of zeros
vec zeros(int size);
//! A Binary vector of zeros
bvec zeros_b(int size);
//! A Int vector of zeros
ivec zeros_i(int size);
//! A Double Complex vector of zeros
cvec zeros_c(int size);

//! A Double (rows,cols)-matrix of zeros
mat zeros(int rows, int cols);
//! A Binary (rows,cols)-matrix of zeros
bmat zeros_b(int rows, int cols);
//! A Int (rows,cols)-matrix of zeros
imat zeros_i(int rows, int cols);
//! A Double Complex (rows,cols)-matrix of zeros
cmat zeros_c(int rows, int cols);

//! A Double (size,size) unit matrix
mat eye(int size);
//! A Binary (size,size) unit matrix
bmat eye_b(int size);
//! A Int (size,size) unit matrix
imat eye_i(int size);
//! A Double Complex (size,size) unit matrix
cmat eye_c(int size);
//! A non-copying version of the eye function.
template <class T>
void eye(int size, Mat<T> &m);

//! Impulse vector
vec impulse(int size);
//! Linspace (works in the same way as the matlab version)
vec linspace(double from, double to, int length = 100);
//! A 2^size by 2^size Hadamard matrix
imat hadamard(int size);

/*!
  \brief Jacobsthal matrix.
  
  Constructs an p by p matrix Q where p is a prime (not checked).
  The elements in Q {qij} is given by qij=X(j-i), where X(x) is the
  Legendre symbol given as:

  <ul>
  <li> X(x)=0 if x is a multiple of p, </li> 
  <li> X(x)=1 if x is a quadratic residue modulo p, </li>
  <li> X(x)=-1 if x is a quadratic nonresidue modulo p. </li>
  </ul>

  See Wicker "Error Control Systems for digital communication and storage", p. 134
  for more information on these topics. Do not check that p is a prime.
*/
imat jacobsthal(int p);

/*! 
  \brief Conference matrix.
  
  Constructs an n by n matrix C, where n=p^m+1=2 (mod 4) and p is a odd prime (not checked).
  This code only work with m=1, that is n=p+1 and p odd prime. The valid sizes
  of n is then n=6, 14, 18, 30, 38, ... (and not 10, 26, ...).
  C has the property that C*C'=(n-1)I, that is it has orthogonal rows and columns
  in the same way as Hadamard matricies. However, one element in each row (on the
  diagonal) is zeros. The others are {-1,+1}.
  
  For more details see pp. 55-58 in MacWilliams & Sloane "The theory of error correcting codes",
  North-Holland, 1977.
  */
imat conference(int n);

//!@}

/*! \defgroup windfunc Windows */

//!\addtogroup windfunc
//!@{

//! A length size hamming window
vec hamming(int size);
//! A length size hamming window
vec hanning(int size);
//! A length size triangular window
vec triang(int size);
//! A length size square root window (=sqrt(triang(size)))
vec sqrt_win(int size);
//!@}


/*! 
  \brief Create a rotation matrix that rotates the given plane \c angle radians. Note that the order of the planes are important!
  \ingroup miscfunc
*/
mat rotation_matrix(int dim, int plane1, int plane2, double angle);

/*! 
  \brief Calcualte the Householder vector
  \ingroup miscfunc
*/
void house(const vec &x, vec &v, double &beta);

/*! 
  \brief Calculate the Givens rotation values
  \ingroup miscfunc
*/
void givens(double a, double b, double &c, double &s);

/*! 
  \brief Calculate the Givens rotation matrix
  \ingroup miscfunc
*/
void givens(double a, double b, mat &m);

/*! 
  \brief Calculate the Givens rotation matrix
  \ingroup miscfunc
*/
mat givens(double a, double b);

/*! 
  \brief Calculate the transposed Givens rotation matrix
  \ingroup miscfunc
*/
void givens_t(double a, double b, mat &m);

/*! 
  \brief Calculate the transposed Givens rotation matrix
  \ingroup miscfunc
*/
mat givens_t(double a, double b);

/*! 
  \relates Vec
  \brief Vector of length 1
*/
template <class T>
Vec<T> vec_1(T v0)
{
    Vec<T> v(1);
    v(0) = v0;
    return v;
}

/*! 
  \relates Vec
  \brief Vector of length 2
*/
template <class T>
Vec<T> vec_2(T v0, T v1)
{
    Vec<T> v(2);
    v(0) = v0;
    v(1) = v1;
    return v;
}

/*! 
  \relates Vec
  \brief Vector of length 3
*/
template <class T>
Vec<T> vec_3(T v0, T v1, T v2)
{
    Vec<T> v(3);
    v(0) = v0;
    v(1) = v1;
    v(2) = v2;
    return v;
}

/*! 
  \relates Mat
  \brief Matrix of size 1 by 1
*/
template <class T>
Mat<T> mat_1x1(T m00)
{
    Mat<T> m(1,1);
    m(0,0) = m00;
    return m;
}

/*! 
  \relates Mat
  \brief Matrix of size 1 by 2
*/
template <class T>
Mat<T> mat_1x2(T m00, T m01)
{
    Mat<T> m(1,2);
    m(0,0) = m00; m(0,1) = m01;
    return m;
}

/*! 
  \relates Mat
  \brief Matrix of size 2 by 1
*/
template <class T>
Mat<T> mat_2x1(T m00,
	       T m10)
{
    Mat<T> m(2,1);
    m(0,0) = m00;
    m(1,0) = m10;
    return m;
}

/*! 
  \relates Mat
  \brief Matrix of size 2 by 2
*/
template <class T>
Mat<T> mat_2x2(T m00, T m01,
	       T m10, T m11)
{
    Mat<T> m(2,2);
    m(0,0) = m00; m(0,1) = m01;
    m(1,0) = m10; m(1,1) = m11;
    return m;
}

/*! 
  \relates Mat
  \brief Matrix of size 1 by 3
*/
template <class T>
Mat<T> mat_1x3(T m00, T m01, T m02)
{
    Mat<T> m(1,3);
    m(0,0) = m00; m(0,1) = m01; m(0,2) = m02;
    return m;
}

/*! 
  \relates Mat
  \brief Matrix of size 3 by 1
*/
template <class T>
Mat<T> mat_3x1(T m00,
	       T m10,
	       T m20)
{
    Mat<T> m(3,1);
    m(0,0) = m00;
    m(1,0) = m10;
    m(2,0) = m20;
    return m;
}

/*! 
  \relates Mat
  \brief Matrix of size 2 by 3
*/
template <class T>
Mat<T> mat_2x3(T m00, T m01, T m02,
	       T m10, T m11, T m12)
{
    Mat<T> m(2,3);
    m(0,0) = m00; m(0,1) = m01; m(0,2) = m02;
    m(1,0) = m10; m(1,1) = m11; m(1,2) = m12;
    return m;
}

/*! 
  \relates Mat
  \brief Matrix of size 3 by 2
*/
template <class T>
Mat<T> mat_3x2(T m00, T m01,
	       T m10, T m11,
	       T m20, T m21)
{
    Mat<T> m(3,2);
    m(0,0) = m00; m(0,1) = m01;
    m(1,0) = m10; m(1,1) = m11;
    m(2,0) = m20; m(2,1) = m21;
    return m;
}

/*! 
  \relates Mat
  \brief Matrix of size 3 by 3
*/
template <class T>
Mat<T> mat_3x3(T m00, T m01, T m02,
	       T m10, T m11, T m12,
	       T m20, T m21, T m22)
{
    Mat<T> m(3,3);
    m(0,0) = m00; m(0,1) = m01; m(0,2) = m02;
    m(1,0) = m10; m(1,1) = m11; m(1,2) = m12;
    m(2,0) = m20; m(2,1) = m21; m(2,2) = m22;
    return m;
}
} // namespace SPUC 

#endif // __specmat_h

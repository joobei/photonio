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
  \brief Definitions of eigenvalue decomposition functions
  \author Tony Ottosson

  1.6

  2002/10/13 19:26:17
*/

#ifndef __eigen_h
#define __eigen_h

#include <vector.h>
#include <matrix.h>


#ifdef HAVE_LAPACK

/*! 
  \ingroup matrixdecomp
  \brief Calculates the eigenvalues and eigenvectors of a symmetric real matrix

  The Eigenvalues \f$\mathbf{d}(d_0, d_1, \ldots, d_{n-1})\f$ and the eigenvectors
  \f$\mathbf{v}_i, \: i=0, \ldots, n-1\f$ of the real and symmetric \f$n \times n\f$
  matrix \f$\mathbf{A}\f$ satisfies
  \f[
     \mathbf{A} \mathbf{v}_i = d_i \mathbf{v}_i\: i=0, \ldots, n-1.
  \f]
  The eigenvectors are the columns of the matrix V.
  True is returned if the calculation was successful. Otherwise false.
*/
bool eig_sym(const mat &A, vec &d, mat &V);

/*!
  \ingroup matrixdecomp
  \brief Calculates the eigenvalues of a symmetric real matrix

  The Eigenvalues \f$\mathbf{d}(d_0, d_1, \ldots, d_{n-1})\f$ and the eigenvectors
  \f$\mathbf{v}_i, \: i=0, \ldots, n-1\f$ of the real and symmetric \f$n \times n\f$
  matrix \f$\mathbf{A}\f$ satisfies
  \f[
     \mathbf{A} \mathbf{v}_i = d_i \mathbf{v}_i\: i=0, \ldots, n-1.
  \f]
  True is returned if the calculation was successful. Otherwise false.
*/
bool eig_sym(const mat &A, vec &d);

/*!
  \ingroup matrixdecomp
  \brief Calculates the eigenvalues of a symmetric real matrix

  The Eigenvalues \f$\mathbf{d}(d_0, d_1, \ldots, d_{n-1})\f$ and the eigenvectors
  \f$\mathbf{v}_i, \: i=0, \ldots, n-1\f$ of the real and symmetric \f$n \times n\f$
  matrix \f$\mathbf{A}\f$ satisfies
  \f[
     \mathbf{A} \mathbf{v}_i = d_i \mathbf{v}_i\: i=0, \ldots, n-1.
  \f]
*/
vec eig_sym(const mat &A);

/*!
  \ingroup matrixdecomp
  \brief Calculates the eigenvalues and eigenvectors of a hermitian complex matrix

  The Eigenvalues \f$\mathbf{d}(d_0, d_1, \ldots, d_{n-1})\f$ and the eigenvectors
  \f$\mathbf{v}_i, \: i=0, \ldots, n-1\f$ of the complex and hermitian \f$n \times n\f$
  matrix \f$\mathbf{A}\f$ satisfies
  \f[
     \mathbf{A} \mathbf{v}_i = d_i \mathbf{v}_i\: i=0, \ldots, n-1.
  \f]
  The eigenvectors are the columns of the matrix V.
  True is returned if the calculation was successful. Otherwise false.
*/
bool eig_sym(const cmat &A, vec &d, cmat &V);

/*!
  \ingroup matrixdecomp
  \brief Calculates the eigenvalues of a hermitian complex matrix

  The Eigenvalues \f$\mathbf{d}(d_0, d_1, \ldots, d_{n-1})\f$ and the eigenvectors
  \f$\mathbf{v}_i, \: i=0, \ldots, n-1\f$ of the complex and hermitian \f$n \times n\f$
  matrix \f$\mathbf{A}\f$ satisfies
  \f[
     \mathbf{A} \mathbf{v}_i = d_i \mathbf{v}_i\: i=0, \ldots, n-1.
  \f]
  True is returned if the calculation was successful. Otherwise false.
*/
bool eig_sym(const cmat &A, vec &d);

/*!
  \ingroup matrixdecomp
  \brief Calculates the eigenvalues of a hermitian complex matrix

  The Eigenvalues \f$\mathbf{d}(d_0, d_1, \ldots, d_{n-1})\f$ and the eigenvectors
  \f$\mathbf{v}_i, \: i=0, \ldots, n-1\f$ of the complex and hermitian \f$n \times n\f$
  matrix \f$\mathbf{A}\f$ satisfies
  \f[
     \mathbf{A} \mathbf{v}_i = d_i \mathbf{v}_i\: i=0, \ldots, n-1.
  \f]
*/
vec eig_sym(const cmat &A);

/*!
  \ingroup matrixdecomp
  \brief Caclulates the eigenvalues and eigenvectors of a real non-symmetric matrix

  The Eigenvalues \f$\mathbf{d}(d_0, d_1, \ldots, d_{n-1})\f$ and the eigenvectors
  \f$\mathbf{v}_i, \: i=0, \ldots, n-1\f$ of the real \f$n \times n\f$
  matrix \f$\mathbf{A}\f$ satisfies
  \f[
     \mathbf{A} \mathbf{v}_i = d_i \mathbf{v}_i\: i=0, \ldots, n-1.
  \f]
  The eigenvectors are the columns of the matrix V.
  True is returned if the calculation was successful. Otherwise false.
*/
bool eig(const mat &A, cvec &d, cmat &V);

/*!
  \ingroup matrixdecomp
  \brief Caclulates the eigenvalues of a real non-symmetric matrix

  The Eigenvalues \f$\mathbf{d}(d_0, d_1, \ldots, d_{n-1})\f$ and the eigenvectors
  \f$\mathbf{v}_i, \: i=0, \ldots, n-1\f$ of the real \f$n \times n\f$
  matrix \f$\mathbf{A}\f$ satisfies
  \f[
     \mathbf{A} \mathbf{v}_i = d_i \mathbf{v}_i\: i=0, \ldots, n-1.
  \f]
  True is returned if the calculation was successful. Otherwise false.
*/
bool eig(const mat &A, cvec &d);

/*!
  \ingroup matrixdecomp
  \brief Caclulates the eigenvalues of a real non-symmetric matrix

  The Eigenvalues \f$\mathbf{d}(d_0, d_1, \ldots, d_{n-1})\f$ and the eigenvectors
  \f$\mathbf{v}_i, \: i=0, \ldots, n-1\f$ of the real \f$n \times n\f$
  matrix \f$\mathbf{A}\f$ satisfies
  \f[
     \mathbf{A} \mathbf{v}_i = d_i \mathbf{v}_i\: i=0, \ldots, n-1.
  \f]
*/
cvec eig(const mat &A);

/*!
  \ingroup matrixdecomp
  \brief Calculates the eigenvalues and eigenvectors of a complex non-hermitian matrix

  The Eigenvalues \f$\mathbf{d}(d_0, d_1, \ldots, d_{n-1})\f$ and the eigenvectors
  \f$\mathbf{v}_i, \: i=0, \ldots, n-1\f$ of the complex \f$n \times n\f$
  matrix \f$\mathbf{A}\f$ satisfies
  \f[
     \mathbf{A} \mathbf{v}_i = d_i \mathbf{v}_i\: i=0, \ldots, n-1.
  \f]
  The eigenvectors are the columns of the matrix V.
  True is returned if the calculation was successful. Otherwise false.
*/
bool eig(const cmat &A, cvec &d, cmat &V);

/*!
  \ingroup matrixdecomp
  \brief Calculates the eigenvalues of a complex non-hermitian matrix

  The Eigenvalues \f$\mathbf{d}(d_0, d_1, \ldots, d_{n-1})\f$ and the eigenvectors
  \f$\mathbf{v}_i, \: i=0, \ldots, n-1\f$ of the complex \f$n \times n\f$
  matrix \f$\mathbf{A}\f$ satisfies
  \f[
     \mathbf{A} \mathbf{v}_i = d_i \mathbf{v}_i\: i=0, \ldots, n-1.
  \f]
  True is returned if the calculation was successful. Otherwise false.
*/
bool eig(const cmat &A, cvec &d);

/*!
  \ingroup matrixdecomp
  \brief Calculates the eigenvalues of a complex non-hermitian matrix

  The Eigenvalues \f$\mathbf{d}(d_0, d_1, \ldots, d_{n-1})\f$ and the eigenvectors
  \f$\mathbf{v}_i, \: i=0, \ldots, n-1\f$ of the complex \f$n \times n\f$
  matrix \f$\mathbf{A}\f$ satisfies
  \f[
     \mathbf{A} \mathbf{v}_i = d_i \mathbf{v}_i\: i=0, \ldots, n-1.
  \f]
*/
cvec eig(const cmat &A);


#endif // HAVE_LAPACK

#endif // __eigen_h

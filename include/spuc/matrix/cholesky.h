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
  \brief Definitions of Cholesky factorisation functions
  \author Tony Ottosson

  1.8

  2002/10/13 19:26:17
*/

#ifndef __cholesky_h
#define __cholesky_h

#include <vector.h>
#include <matrix.h>
namespace SPUC {
/*! \defgroup matrixdecomp Matrix Decompositions
 */
//!@{

/*! 
  \brief Cholesky factorisation of real symmetric and positive definite matrix
  
  The Cholesky factorisation of a real symmetric positive-definite matrix \f$\mathbf{X}\f$
  of size \f$n \times n\f$ is given by
  \f[
     \mathbf{X} = \mathbf{F}^T \mathbf{F}
  \f]
  where \f$\mathbf{F}\f$ is an upper trangular \f$n \times n\f$ matrix.

  Returns true if calcuation succeeded. False otherwise.
*/
bool chol(const mat &X, mat &F);

/*! 
  \brief Cholesky factorisation of real symmetric and positive definite matrix
  
  The Cholesky factorisation of a real symmetric positive-definite matrix \f$\mathbf{X}\f$
  of size \f$n \times n\f$ is given by
  \f[
     \mathbf{X} = \mathbf{F}^T \mathbf{F}
  \f]
  where \f$\mathbf{F}\f$ is an upper trangular \f$n \times n\f$ matrix.
*/
mat chol(const mat &X);


#ifdef HAVE_LAPACK

/*! 
  \brief Cholesky factorisation of complex hermitian and positive-definite matrix
  
  The Cholesky factorisation of a hermitian positive-definite matrix \f$\mathbf{X}\f$
  of size \f$n \times n\f$ is given by
  \f[
     \mathbf{X} = \mathbf{F}^H \mathbf{F}
  \f]
  where \f$\mathbf{F}\f$ is an upper trangular \f$n \times n\f$ matrix.

  Returns true if calcuation succeeded. False otherwise.

  If \c X is positive definite, true is returned and \c F=chol(X) 
  produces an upper triangular \c F. If also \c X is symmetric then \c F'*F = X.
  If \c X is not positive definite, false is returned.
*/
bool chol(const cmat &X, cmat &F);

/*! 
  \brief Cholesky factorisation of complex hermitian and positive-definite matrix
  
  The Cholesky factorisation of a hermitian positive-definite matrix \f$\mathbf{X}\f$
  of size \f$n \times n\f$ is given by
  \f[
     \mathbf{X} = \mathbf{F}^H \mathbf{F}
  \f]
  where \f$\mathbf{F}\f$ is an upper trangular \f$n \times n\f$ matrix.
*/
cmat chol(const cmat &X);

#endif


/*! 
  \brief Cholesky factorisation of an n by n band-matrix X. Bandwidth p.
  
  If X is positive definite, true is returned and F=chol(X) produces
  an upper triangular F.
  If also X is symmetric then F'*F = X.
  If X is not positive definite, false is returned.

  Uses n*(p^2+3*p) flops and n sqrt() (if n >> p).
  Uses Alg. 4.3.5 (outer product version) in Golub & van Loan
  "Matrix computations", 3rd ed., p. 156.
*/
bool chol(const mat &X, int p, mat &F);

/*! 
  \brief Cholesky factorisation of a band matrix.
  
  Cholesky factorisation of an n by n band-matrix X. Bandwidth p.
  If X is positive definite, F=chol(X) produces an upper triangular F.
  If also X is symmetric then F'*F = X.
  If X is not positive definite, an error message is printed.

  Uses n*(p^2+3*p) flops and n sqrt() (if n >> p).
  Uses Alg. 4.3.5 (outer product version) in Golub & van Loan
  "Matrix computations", 3rd ed., p. 156.
*/
mat chol(const mat &X, int p);
//!@}
}
#endif // __cholesky_h

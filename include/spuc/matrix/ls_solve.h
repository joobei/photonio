/*---------------------------------------------------------------------------*
 *                                   IT++			             *
 *---------------------------------------------------------------------------*
 * Copyright (c) 1995-2001 by Tony Ottosson, Thomas Eriksson, Pål Frenger,   *
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
  \brief Definitions of functions for solving linear equation systems
  \author Tony Ottosson

  1.4

  2001/12/02 18:29:07
*/

#ifndef __ls_solve_h
#define __ls_solve_h

#include "vector.h"
#include "matrix.h"
#include "cholesky.h"
namespace SPUC {
/*! \defgroup linearequations Solving Linear Equation Systems
 */
//!@{

/*! \brief Solve linear equation system by LU factorisation.

  Solves Ax=b, where A is a n by n matrix.
  Requires n^3/3+2*n^2 flops.
*/
vec ls_solve(const mat &A, const vec &b);

/*! \brief Solve multiple linear equations by LU factorisation.
  
  Solves AX=B. Here A is a nonsingular n by n matrix, X and B n by p matricies
  such that X=[x1 x2 ... xp], B=[b1 b2 ... bp]. Hence, the equations 
  A*xj=bj is solved for all j=1..p.
  Requires n^3/3+2*p*n^2 flops.
  The Algorithm is taken from Golub and van Loan,
  "Matrix Computations", 3rd ed., p.121.
*/
mat ls_solve(const mat &A, const mat &b);

/*! \brief Solve linear equation system by Cholesky factorisation.
  
  Solves Ax=b, where A is a symmetric postive definite n by n matrix.
  Requires n^3/3+2*n^2 flops.
*/
vec ls_solve_chol(const mat &A, const vec &b);

/*! \brief Solve linear equation system, when LU-factorisation is given.
  
  Solves Ax=b, where A=LU obtained by some factorization algorithm
  Assumes that L and U is nonsingular. Requires 2*n^2 flops.
  Uses Alg. 3.1.1 and 3.1.2 in Golub & van Loan "Matrix computations", 
  3rd ed., p. 89.
*/
vec ls_solve(const mat &L, const mat &U, const vec &b);

/*! \brief Solve linear (band) equation system by Cholesky factorisation.
  
  Solves Ax=b, where A is a symmetric postive definite n by n
  band-matrix with bandwidth p.
  Requires about  n*(p^2+3*p)+4np flops (n >> p).
*/
vec ls_solve_chol(const mat &A, int p, const vec &b);

/*! \brief Solve linear (band) equation system, when LU-factorisation is given.
  
  Solves Ax=b, where A=LU obtained by some factorization algorithm
  Assumes that L and U is nonsingular band-matricies with lower bandwidth p
  and upper bandwidth q, respectively. Requires about 2n*(p+q) flops (if n >> p,
  q). Uses Alg. 4.3.2 and 4.3.3 in Golub & van Loan "Matrix computations", 
  3rd ed., p. 153.
*/
vec ls_solve(const mat &L, int p, const mat &U, int q, const vec &b);

/*! \brief Solves overdetermined linear equation systems.

  Solves Ax=b, where A is a m by n matrix and m>=n.
  Requires approximately 2*n^2(m-n/3) flops.
*/
vec ls_solve_od(const mat &A, const vec &b);

/*! \brief Solves overdetermined linear equation systems.

  Solves Ax=B, where A is a m by n matrix and m>=n.
  Requires approximately 2*n^2(m-n/3) flops.
*/
mat ls_solve_od(const mat &A, const mat &B);

/*! \brief A general linear equation system solver.

  Tries to emulate the backslash operator in Matlab by calling
  ls_solve(A,b) or ls_solve_od(A,b).
*/
vec backslash(const mat &A, const vec &b);

/*! \brief A general linear equation system solver.

  Tries to emulate the backslash operator in Matlab by calling
  ls_solve(A,B) or ls_solve_od(A,B).
*/
mat backslash(const mat &A, const mat &B);

/*! \brief Forward substitution of square matrix.
  
  Solves Lx=b, where L is a lower triangular n by n matrix.
  Assumes that L is nonsingular. Requires n^2 flops.
  Uses Alg. 3.1.1 in Golub & van Loan "Matrix computations", 3rd ed., p. 89.
*/
vec forward_substitution(const mat &L, const vec &b);

/*! \brief Forward substitution of square matrix.
  
  Solves Lx=b, where L is a lower triangular n by n matrix.
  Assumes that L is nonsingular. Requires n^2 flops.
  Uses Alg. 3.1.1 in Golub & van Loan "Matrix computations", 3rd ed., p. 89.
*/
void forward_substitution(const mat &L, const vec &b, vec &x);

/*! \brief Forward substitution of band matricies.
  
  Solves Lx=b, where L is a lower triangular n by n band-matrix with lower
  bandwidth p.
  Assumes that L is nonsingular. Requires about 2np flops (if n >> p).
  Uses Alg. 4.3.2 in Golub & van Loan "Matrix computations", 3rd ed., p. 153.
*/
vec forward_substitution(const mat &L, int p, const vec &b);

/*! \brief Forward substitution of band matricies.
  
  Solves Lx=b, where L is a lower triangular n by n band-matrix with
  lower bandwidth p.
  Assumes that L is nonsingular. Requires about 2np flops (if n >> p).
  Uses Alg. 4.3.2 in Golub & van Loan "Matrix computations", 3rd ed., p. 153.
*/
void forward_substitution(const mat &L, int p, const vec &b, vec &x);

/*! \brief Backward substitution of square matrix.
  
  Solves Ux=b, where U is a upper triangular n by n matrix.
  Assumes that U is nonsingular. Requires n^2 flops.
  Uses Alg. 3.1.2 in Golub & van Loan "Matrix computations", 3rd ed., p. 89.
*/
vec backward_substitution(const mat &U, const vec &b);

/*! \brief Backward substitution of square matrix.
  
  Solves Ux=b, where U is a upper triangular n by n matrix.
  Assumes that U is nonsingular. Requires n^2 flops.
  Uses Alg. 3.1.2 in Golub & van Loan "Matrix computations", 3rd ed., p. 89.
*/
void backward_substitution(const mat &U, const vec &b, vec &x);

/*! \brief Backward substitution of band matrix.
  
  Solves Ux=b, where U is a upper triangular n by n matrix band-matrix with
  upper bandwidth q.
  Assumes that U is nonsingular. Requires about 2nq flops (if n >> q).
  Uses Alg. 4.3.3 in Golub & van Loan "Matrix computations", 3rd ed., p. 153.
*/
vec backward_substitution(const mat &U, int q, const vec &b);

/*! \brief Backward substitution of band matrix.
  
  Solves Ux=b, where U is a upper triangular n by n matrix band-matrix with
  upper bandwidth q.
  Assumes that U is nonsingular. Requires about 2nq flops (if n >> q).
  Uses Alg. 4.3.3 in Golub & van Loan "Matrix computations", 3rd ed., p. 153.
*/
void backward_substitution(const mat &U, int q, const vec &b, vec &x);

//!@}
}
#endif // __ls_solve_h




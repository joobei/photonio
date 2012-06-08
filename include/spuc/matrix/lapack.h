/*---------------------------------------------------------------------------*
 *                                   IT++			             *
 *---------------------------------------------------------------------------*
 * Copyright (c) 1995-2003 by Tony Ottosson, Thomas Eriksson, Pål Frenger,   *
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
  \brief Lapack header functions. For internal use only.
  \author Tony Ottosson

  1.10

  2003/01/08 13:28:04
*/

#ifndef DOXYGEN_SHOULD_SKIP_THIS
namespace SPUC {
extern "C" {

// Fix for MKL Windows version so that naming is consistent with the 5.x MKL LAPACK libraries
#ifdef LAPACK_NO_UNDERSCORE
#define dgetrf_ dgetrf
#define zgetrf_ zgetrf

#define dgetri_ dgetri
#define zgetri_ zgetri

#define dgesvd_ dgesvd
#define zgesvd_ zgesvd

#define dsyev_ dsyev
#define zheev_ zheev

#define dgeev_ dgeev
#define zgeev_ zgeev

#define dpotrf_ dpotrf
#define zpotrf_ zpotrf
#endif

typedef complex<double> double_complex;
/* LU factorization
   a is of size m*n and with lda rows.
   ipiv is the permutation vector of rows. Row i should be replaced by row ipiv(i).
   info=0 if OK. info=-i if ith value is illegal. info=i factorization OK but the system is singular if solved.
*/
void dgetrf_(int *m, int *n, double *a, int *lda, int *ipiv, int *info);
void zgetrf_(int *m, int *n, double_complex *a, int *lda, int *ipiv, int *info);


/* Inverting a matrix of an LU-factored general matrix (first call xGETRF)
   a is of square size n*n with lda rows containing the factorization as returned by xGETRF
   ipiv is vector as returned by xGETRF
   lwork >= n
   output: a is overwritten by the inverse
   info=0 if OK. info=-i if ith parameter is illegal. info=i the ith diagonal element = 0 and U is singular.
*/
void dgetri_(int *n, double *a, int *lda, int *ipiv, double *work, int *lwork, int *info);
void zgetri_(int *n, double_complex *a, int *lda, int *ipiv, double_complex *work, int *lwork, int *info);

/* SVD of a general rectangular matrix A = U S V^H
   a is of size m*n and with lda rows.
   Output: s with sorted singular values (vector)
           u, and vt (for U and V^H). U is m*m, and V^H is n*n
   jobu='A','S','O','N'. Different versions. 'A' = all columns of U calculated and returned in u.
   jobvt='A','S','O','N'. Different versions. 'A' = all columns of V^H calculated and returned in vt.
   ldu = no rows in U
   ldvt = no rows in V^H
   info = 0 successful, =-i ith parameter is illegal, =i did not converge

   work is a workspace vector of size lwork.
   lwork >= max(3*min(m,n)+max(m,n), 5*min(m,n)) for double
   lwork >= 2*min(m,n)+max(m,n) for double_complex
   Good performance. Make lwork larger!
   rwork is a workspace array for complex version. Size max(1, 5*min(m,n)).
 
*/
void dgesvd_(char *jobu, char *jobvt, int *m, int *n, double *a, int *lda, double *s, double *u, int *ldu, double *vt, int *ldvt, double *work, int *lwork, int *info);
void zgesvd_(char *jobu, char *jobvt, int *m, int *n, double_complex *a, int *lda, double *s, double_complex *u, int *ldu, double_complex *vt, int *ldvt, double_complex *work, int *lwork, double *rwork, int *info);

/* Eigenvalues and eigenvectors of a symmetric/hermitian matrix A

*/
void dsyev_(char *jobz, char *uplo, int *n, double *a, int *lda, double *w, double *work, int *lwork, int *info);
void zheev_(char *jobz, char *uplo, int *n, double_complex *a, int *lda, double *w, double_complex *work, int *lwork, double *rwork, int *info);


/* Eigenvalues and eigenvectors of a general matrix A

*/
void dgeev_(char *jobvl, char *jobvr, int *n, double *a, int *lda, double *wr, double *wi, double *vl, int *ldvl, double *vr, int *ldvr, double *work, int *lwork, int *info);
void zgeev_(char *jobvl, char *jobvr, int *n, double_complex *a, int *lda, double_complex *w, double_complex *vl, int *ldvl, double_complex *vr, int *ldvr, double_complex *work, int *lwork, double *rwork, int *info);

/* Cholesky factorization

*/
void dpotrf_(char *uplo, int *n, double *a, int *lda, int *info);
void zpotrf_(char *uplo, int *n, double_complex *a, int *lda, int *info);

} // extern C
}
#endif //DOXYGEN_SHOULD_SKIP_THIS

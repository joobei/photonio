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
  \brief Definitions of functions on vectors and matrices
  \author Tony Ottosson

  1.17

  2002/12/19 23:56:44
*/

#ifndef __matfunc_h
#define __matfunc_h

#include "vector.h"
#include "matrix.h"
//#include "converters.h"
//#include "scalfunc.h"
namespace SPUC {
/*! 
  \relates Vec
  \brief Length of vector
*/
template<class T>
int length(const Vec<T> &v) { return v.length(); }

/*! 
  \relates Vec
  \brief Length of vector
*/
template<class T>
int size(const Vec<T> &v) { return v.length(); }

/*! 
  \relates Vec
  \brief Sum of all elements in the vector 
*/
template<class T>
T sum(const Vec<T> &v);

/*! 
  \relates Vec
  \brief Sum of square of the elements in a vector
*/
template<class T>
T sum_sqr(const Vec<T> &v);

/*! 
  \relates Vec
  \brief Maximum value of vector
*/
template<class T>
T max(const Vec<T> &v);

/*! 
  \relates Vec
  \brief Minimum value of vector
*/
template<class T>
T min(const Vec<T> &in);

/*! 
  \relates Vec
  \brief Return the postion of the maximum element in the vector
*/
template<class T>
int max_index(const Vec<T> &in);

/*! 
  \relates Vec
  \brief Return the postion of the minimum element in the vector
*/
template<class T>
int min_index(const Vec<T> &in);

/*! 
  \relates Vec
  \brief The product of all elements in the vector
*/
template<class T>
T product(const Vec<T> &v);

/*! 
  \relates Vec
  \brief Vector cross product. Vectors need to be of size 3
*/
template<class T>
Vec<T> cross(const Vec<T> &v1, const Vec<T> &v2);

/*! 
  \relates Vec
  \brief The mean value
*/
double mean(const vec &v);

/*! 
  \relates Vec
  \brief The mean value
*/
complex<double> mean(const cvec &v);

/*! 
  \relates Vec
  \brief The mean value
*/
double mean(const svec &v);

/*! 
  \relates Vec
  \brief The mean value
*/
double mean(const ivec &v);

/*! 
  \relates Vec
  \brief The geometric mean value
*/
template<class T>
double geometric_mean(const Vec<T> &v);

/*! 
  \relates Vec
  \brief The median
*/
template<class T>
double median(const Vec<T> &v);

/*! 
  \relates Vec
  \brief Calculate the 2-norm: norm(v)=sqrt(sum(abs(v).^2))
*/
template<class T>
double norm(const Vec<T> &v);

/*! 
  \relates Vec
  \brief Calculate the 2-norm: norm(v)=sqrt(sum(abs(v).^2))
*/
template<>
double norm(const cvec &v);

/*! 
  \relates Vec
  \brief Calculate the p-norm: norm(v,p)=sum(abs(v).^2)^(1/p)
*/
template<class T>
double norm(const Vec<T> &v, int p);

/*! 
  \relates Vec
  \brief Calculate the p-norm: norm(v,p)=sum(abs(v).^2)^(1/p)
*/
template<>
double norm(const cvec &v, int p);

/*! 
  \relates Vec
  \brief The variance of the elements in the vector. Normalized with N-1 to be unbiased.

template<class T>
double variance(const Vec<T> &v);

  \relates Vec
  \brief The variance of the elements in the vector. Normalized with N-1 to be unbiased.

template<>
double variance(const cvec &v);
*/	
/*!
  \relates Vec
  \brief Calculate the energy: squared 2-norm. energy(v)=sum(v.^2)
*/
template<class T>
double energy(const Vec<T> &v);

/* ACTION: Rewrite for double only ???
template<class T>
dvec normalize(Vec<T> &v1, T radius);
*/

/*! 
  \relates Vec
  \brief Reverse the input vector
*/
template<class T>
Vec<T> reverse(const Vec<T> &in);

/*! 
  \relates Vec
  \brief Repeat each element in the vector norepeats times in sequence
*/
template<class T>
Vec<T> repeat(const Vec<T> &v, int norepeats);

/*! 
  \relates Vec
  \brief Apply arbitrary function to a vector
*/
template<class T, class fT>
Vec<T> apply_function(fT (*f)(fT), const Vec<T> &data);

/*! 
  \relates Vec
  \brief Sort the the vector in increasing order
*/
template<class T>
void sort(Vec<T> &data);

/*! 
  \relates Vec
  \brief Return an index vector corresponding to a sorted vector (increasing order)
*/
template<class T>
ivec sort_index(const Vec<T> &data);

/*! 
  \relates Vec
  \brief Zero-pad a vector to size n
*/
template<class T>
Vec<T> zero_pad(const Vec<T> &v, int n);

/*! 
  \relates Vec
  \brief Zero-pad a vector to the nearest greater power of two
*/
template<class T>
Vec<T> zero_pad(const Vec<T> &v);

/*! 
  \relates Vec
  \brief Zero-pad a matrix to size rows x cols
*/
template<class T>
Mat<T> zero_pad(const Mat<T> &v, int rows, int cols);

//---------- Template functions for Mat<T> ----------------------

/*! 
  \relates Mat
  \brief Sum of all elements in the matrix 
*/
template<class T>
T sum(const Mat<T> &m);

/*! 
  \relates Mat
  \brief Sum of square of the elements in the matrix
*/
template<class T>
T sum_sqr(const Mat<T> &m);

/*! 
  \relates Mat
  \brief Maximum value of matrix
*/
template<class T>
T max(const Mat<T> &m);

/*! 
  \relates Mat
  \brief Minimum value of matrix
*/
template<class T>
T min(const Mat<T> &m);

/*! 
  \relates Mat
  \brief The index of the element of maximum value
*/
template<class T>
void max_index(const Mat<T> &in, int &row, int &col);

/*! 
  \relates Mat
  \brief The index of the element of minimum value
*/
template<class T>
void min_index(const Mat<T> &in, int &row, int &col);

/*! 
  \relates Mat
  \brief The product of all elements in the vector
*/
template<class T>
T product(const Mat<T> &m);

/*! 
  \relates Mat
  \brief The mean value
*/
double mean(const mat &m);

/*! 
  \relates Mat
  \brief The mean value
*/
complex<double> mean(const cmat &m);

/*! 
  \relates Mat
  \brief The mean value
*/
double mean(const smat &m);

/*! 
  \relates Mat
  \brief The mean value
*/
double mean(const imat &m);

/*! 
  \relates Mat
  \brief Returns a diagonal matrix whith the elements of the vector \c in on the diagonal and zeros elsewhere.

  The size of the return matrix will be \f$n \times n\f$, where \f$n\f$ is the length of the input vector \c in.
*/
template<class T>
Mat<T> diag(const Vec<T> &in);

/*! 
  \relates Mat
  \brief Returns in the output wariable \c m a diagonal matrix whith the elements of the vector \c in on the diagonal and zeros elsewhere.
  
  The size of the output matrix \c m will be \f$n \times n\f$, where \f$n\f$ is the length of the input vector \c in.
*/
template<class T>
void diag(const Vec<T> &in, Mat<T> &m);

/*! 
  \relates Mat
  \brief Returns the diagonal elements of the input matrix \c in.

  The input matrix \c m must be a square \f$n \times n\f$ matrix. The size of the output vector will be \f$n\f$.
*/
template<class T>
Vec<T> diag(const Mat<T> &in);

// 
/*!
  \relates Mat
  \brief Returns a matrix with the elements of the input vector \c main on the diagonal and the elements of the input vector \c sup on the diagonal row above.

  If the number of elements in the vector \c main is \f$n\f$, then the number of elements in the input vector 
  \c sup must be \f$n-1\f$. The size of the return matrix will be \f$n \times n\f$.
*/
template<class T>
Mat<T> bidiag(const Vec<T> &main, const Vec<T> &sup);

/*!
  \relates Mat
  \brief Returns in the output variable \c m a matrix with the elements of the input vector \c main on the diagonal and the elements of the input vector \c sup on the diagonal row above.

  If the number of elements in the vector \c main is \f$n\f$, then the number of elements in the input vector 
  \c sup must be \f$n-1\f$. The size of the output matrix \c m will be \f$n \times n\f$.
*/
template<class T>
void bidiag(const Vec<T> &main, const Vec<T> &sup, Mat<T> &m);

/*!
  \relates Mat
  \brief Returns the main diagonal and the diagonal row above in the two output vectors \c main and \c sup.

  The input matrix \c in must be a square \f$n \times n\f$ matrix. The length of the output vector \c main will be \f$n\f$ 
  and the length of the output vector \c sup will be \f$n-1\f$.
*/
template<class T>
void bidiag(const Mat<T> &in, Vec<T> &main, Vec<T> &sup);

/*!
  \relates Mat
  \brief Returns a matrix with the elements of \c main on the diagonal, the elements of \c sup on the diagonal row above, and the elements of \c sub on the diagonal row below.

  If the length of the input vector \c main is \f$n\f$ then the lengths of the vectors \c sup and \c sub 
  must equal \f$n-1\f$. The size of the return matrix will be \f$n \times n\f$.
*/
template<class T>
Mat<T> tridiag(const Vec<T> &main, const Vec<T> &sup, const Vec<T> &sub);

/*!
  \relates Mat
  \brief Returns in the output matrix \c m a matrix with the elements of \c main on the diagonal, the elements of \c sup on the diagonal row above, and the elements of \c sub on the diagonal row below.
  
  If the length of the input vector \c main is \f$n\f$ then the lengths of the vectors \c sup and \c sub 
  must equal \f$n-1\f$. The size of the output matrix \c m will be \f$n \times n\f$.
*/
template<class T>
void tridiag(const Vec<T> &main, const Vec<T> &sup, const Vec<T> &sub, Mat<T> &m);

/*!
  \relates Mat
  \brief Returns the main diagonal, the diagonal row above, and the diagonal row below int the output vectors \c main, \c sup, and \c sub.

  The input matrix \c in must be a square \f$n \times n\f$ matrix. The length of the output vector \c main will be \f$n\f$ 
  and the length of the output vectors \c sup and \c sup will be \f$n-1\f$.
*/
template<class T>
void tridiag(const Mat<T> &in, Vec<T> &main, Vec<T> &sup, Vec<T> &sub);

/*! 
  \relates Mat
  \brief The trace of the matrix, i.e. the sum of the diagonal elements.
*/
template<class T>
T trace(const Mat<T> &in);

/*! 
  \relates Mat
  \brief Transposition of the matrix returning the transposed matrix in out
*/
template<class T>
void transpose(const Mat<T> &m, Mat<T> &out);

/*! 
  \relates Mat
  \brief Transposition of the matrix
*/
template<class T>
Mat<T> transpose(const Mat<T> &m);

/*! 
  \relates Mat
  \brief Repeats each column norepeats times in sequence
*/
template<class T>
Mat<T> repeat(const Mat<T> &m, int norepeats);

/*! 
  \relates Mat
  \brief Apply arbitrary functions to a matrix
*/
template<class T, class fT>
Mat<T> apply_function(fT (*f)(fT), const Mat<T> &data);

//-------------------------------------------------------------

/*!
  \relates Mat
  \brief Row vectorize the matrix [(0,0) (0,1) ... (N-1,N-2) (N-1,N-1)]
*/
template<class T>
Vec<T> rvectorize(const Mat<T> &m);

/*!
  \relates Mat
  \brief Column vectorize the matrix [(0,0) (1,0) ... (N-2,N-1) (N-1,N-1)]
*/
template<class T>
Vec<T> cvectorize(const Mat<T> &m);

/*!
  \relates Mat
  \brief Reshape the matrix into an rows*cols matrix

  The data is taken columnwise from the original matrix and written columnwise into the new matrix.
*/
template<class T>
Mat<T> reshape(const Mat<T> &m, int rows, int cols);

/*!
  \relates Mat
  \brief Reshape the vector into an rows*cols matrix

  The data is element by element from the vector and written columnwise into the new matrix.
*/
template<class T>
Mat<T> reshape(const Vec<T> &m, int rows, int cols);

/*! 
  \relates Vec
  \brief Upsample a vector by incerting \a (usf-1) zeros after each sample
*/ 
template<class T> 
void upsample(const Vec<T> &v, int usf, Vec<T> &u);

/*! 
  \relates Vec
  \brief Upsample a vector by incerting \a (usf-1) zeros after each sample
*/
template<class T>
Vec<T> upsample(const Vec<T> &v, int usf);

/*! 
  \relates Mat
  \brief Upsample each column by incerting \a (usf-1) zeros after each column
*/

template<class T>
void upsample(const Mat<T> &v, int usf, Mat<T> &u);

/*! 
  \relates Mat
  \brief Upsample each column by incerting \a (usf-1) zeros after each column
*/
template<class T>
Mat<T> upsample(const Mat<T> &v, int upsamplefactor);


/*!
  \relates Mat
  \brief Upsample each column by a factor of  \a (usf-1) by linear interpolation
*/
template<class T>
void lininterp(const Mat<T> &v, int usf, Mat<T> &u);

/*!
  \relates Mat
  \brief Upsample each column by a factor of  \a (usf-1) by linear interpolation
*/
template<class T>
Mat<T> lininterp(const Mat<T> &v, int usf);

/*!
  \relates Mat
  \brief Upsample by a factor of  \a (usf-1) by linear interpolation
*/
template<class T>
void lininterp(const Vec<T> &v, int usf, Vec<T> &u);

/*!
  \relates Mat
  \brief Upsample by a factor of  \a (usf-1) by linear interpolation
*/
template<class T>
Vec<T> lininterp(const Vec<T> &v, int usf);
} // namespace SPUC {

#endif // __matfunc_h

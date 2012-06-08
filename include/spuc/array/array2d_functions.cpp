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

/*! 
  \file 
  \brief Implementation of functions on vectors and matrices.
  \author Tony Ottosson

  1.15

  2003/01/15 17:24:17
*/

#include "binary.h"
#include "vector.h"
#include "matfunc.h"
#include "array1d.h"
#include "array2d.h"

namespace SPUC {
//---------- Template functions for Vector<T> ----------------------

#ifndef DOXYGEN_SHOULD_SKIP_THIS
/*
  Zero-pad a matrix to size rows x cols
 */
template<class T>
Array2D<T> zero_pad(const Array2D<T> &m, int rows, int cols)
{
    it_assert(rows>=m.rows() && cols>=m.cols(), "zero_pad() cannot shrink the matrix!");
    Matrix<T> m2(rows, cols);
    m2.set_submatrix(0,m.rows()-1,0,m.cols()-1, m);
    if (cols > m.cols()) // Zero
	m2.set_submatrix(0,m.rows()-1, m.cols(),cols-1, T(0));
    if (rows > m.rows()) // Zero
	m2.set_submatrix(m.rows(), rows-1, 0, cols-1, T(0));

    return m2;
}
//---------- Template functions for Array2D<T> ----------------------

/*
  Sum of elements in a matrix
*/
template<class T>
T sum(const Array2D<T> &v)
{
    int i, j;
    T M=0;
    
    for (i=0; i<v.rows(); i++)
	for (j=0; j<v.cols(); j++)
	    M += v(i,j);
    
    return M;
}

/*
  Sum of square of the elements in a matrix
*/
template<class T>
T sum_sqr(const Array2D<T> &v)
{
    int i, j;
    T M=0;
    
    for (i=0; i<v.rows(); i++)
	for (j=0; j<v.cols(); j++)
	    M += v(i,j) * v(i,j);
    
    return M;
}

/*
  Return the maximum element in the vector
*/
template<class T>
T max(const Array2D<T> &m)
{
    T maxdata = m(0,0);
    int i, j;
	
    for (i=0; i<m.rows(); i++)
	for (j=0; j<m.cols(); j++)
	    if (m(i,j) > maxdata)
		maxdata = m(i,j);
	
    return maxdata;
}

/*
  Return the minimum element in the vector
*/
template<class T>
T min(const Array2D<T> &m)
{
    T mindata = m(0,0);
    int i, j;
	
    for (i=0; i<m.rows(); i++)
	for (j=0; j<m.cols(); j++)
	    if (m(i,j) < mindata)
		mindata = m(i,j);
	
    return mindata;
}

/*
  Return the postion of the maximum element in the matrix
*/
template<class T>
void max_index(const Array2D<T> &m, int &row, int &col)
{
    T maxdata = m(0,0);
    int i, j;

    row = col = 0;
    for (i=0; i<m.rows(); i++)
	for (j=0; j<m.cols(); j++)
	    if (m(i,j) > maxdata) {
		row = i;
		col = j;
		maxdata = m(i,j);
	    }
}

/*
  Return the postion of the minimum element in the matrix
*/
template<class T>
void min_index(const Array2D<T> &m, int &row, int &col)
{
    T mindata = m(0,0);
    int i, j;
	
    row = col = 0;
    for (i=0; i<m.rows(); i++)
	for (j=0; j<m.cols(); j++)
	    if (m(i,j) < mindata) {
		row = i;
		col = j;
		mindata = m(i,j);
	    }
}

/*
  Return the product of all elements in the matrix
*/
template<class T>
T product(const Array2D<T> &m)
{
    int i, j;
    double lnM=0;
    
    for (i=0; i<m.rows(); i++)
	for (j=0; j<m.cols(); j++)
	    lnM += log(m(i,j));
    
    return static_cast<T>(exp(lnM));
}

/*
  Return the mean value of the elements in the matrix
*/
double mean(const mat m)
{
    return sum(m)/(m.rows()*m.cols());
}

/*
  Return the mean value of the elements in the matrix
 */
complex<double> mean(const cmat &m)
{
    return sum(m)/static_cast<double>(m.rows()*m.cols());
}


/*
  Return the mean value of the elements in the matrix
*/
double mean(const imat &m)
{
    return static_cast<double>(sum(m))/(m.rows()*m.cols());
}

/*
  Return a diagonal matrix
*/
template<class T>
Array2D<T> diag(const Array1D<T> &in)
{
    Array2D<T> m(in.size(), in.size());
    m = T(0);
    for (int i=in.size()-1; i>=0; i--)
	m(i,i) = in(i);
    return m;
}

template<class T>
void diag(const Array1D<T> &in, Array2D<T> &m)
{
    m.set_size(in.size(), in.size(), false);
    m = T(0);
    for (int i=in.size()-1; i>=0; i--)
	m(i,i) = in(i);
}

/*
  Return the diagonal of a matrix
*/
template<class T>
Array1D<T> diag(const Array2D<T> &in)
{
    Array1D<T> t(std::min(in.rows(), in.cols()));

    for (int i=0; i<t.size(); i++)
	t(i) = in(i,i);

    return t;
}


template<class T>
Array2D<T> bidiag(const Array1D<T> &main, const Array1D<T> &sup)
{
    it_assert(main.size() == sup.size()+1, "bidiag()");
    
    int n=main.size();
    Array2D<T> m(n, n);
    m = T(0);
    for (int i=0; i<n-1; i++) {
	m(i,i) = main(i);
	m(i,i+1) = sup(i);
    }
    m(n-1,n-1) = main(n-1);
    
    return m;
}

template<class T>
void   bidiag(const Array1D<T> &main, const Array1D<T> &sup, Array2D<T> &m)
{
    it_assert(main.size() == sup.size()+1, "bidiag()");
    
    int n=main.size();
    m.set_size(n, n);
    m = T(0);
    for (int i=0; i<n-1; i++) {
	m(i,i) = main(i);
	m(i,i+1) = sup(i);
    }
    m(n-1,n-1) = main(n-1);
}

template<class T>
void bidiag(const Array2D<T> &m, Array1D<T> &main, Array1D<T> &sup)
{
    it_assert(m.rows() == m.cols(), "bidiag(): Array2D must be square!");
    
    int n=m.cols();
    main.set_size(n);
    sup.set_size(n-1);
    for (int i=0; i<n-1; i++) {
	main(i) = m(i,i);
	sup(i) = m(i,i+1);
    }
    main(n-1) = m(n-1,n-1);
}


template<class T>
Array2D<T> tridiag(const Array1D<T> &main, const Array1D<T> &sup, const Array1D<T> &sub)
{
    it_assert(main.size()==sup.size()+1 && main.size()==sub.size()+1, "bidiag()");
    
    int n=main.size();
    Array2D<T> m(n, n);
    m = T(0);
    for (int i=0; i<n-1; i++) {
	m(i,i) = main(i);
	m(i,i+1) = sup(i);
	m(i+1,i) = sub(i);
    }
    m(n-1,n-1) = main(n-1);
    
    return m;
}

template<class T>
void tridiag(const Array1D<T> &main, const Array1D<T> &sup, const Array1D<T> &sub, Array2D<T> &m)
{
    it_assert(main.size()==sup.size()+1 && main.size()==sub.size()+1, "bidiag()");
    
    int n=main.size();
    m.set_size(n, n);
    m = T(0);
    for (int i=0; i<n-1; i++) {
	m(i,i) = main(i);
	m(i,i+1) = sup(i);
	m(i+1,i) = sub(i);
    }
    m(n-1,n-1) = main(n-1);
}

template<class T>
void tridiag(const Array2D<T> &m, Array1D<T> &main, Array1D<T> &sup, Array1D<T> &sub)
{
    it_assert(m.rows() == m.cols(), "tridiag(): Array2D must be square!");
    
    int n=m.cols();
    main.set_size(n);
    sup.set_size(n-1);
    sub.set_size(n-1);
    for (int i=0; i<n-1; i++) {
	main(i) = m(i,i);
	sup(i) = m(i,i+1);
	sub(i) = m(i+1,i);
    }
    main(n-1) = m(n-1,n-1);
}

/*
  Calculate the trace of a matrix
 */
template<class T>
T trace(const Array2D<T> &in)
{
    return sum(diag(in));
}

/*
  Transpose of a matrix
*/
template<class T>
void transpose(const Array2D<T> &m, Array2D<T> &out)
{
  out.set_size(m.cols(),m.rows(), false);
  
  for (int i=0; i<m.rows(); i++) {
    for (int j=0; j<m.cols(); j++) {
      out(j,i) = m(i,j);
    }
  }
}

/*
  Repeats each column norepeats times in sequence.
*/
template<class T>
Array2D<T> repeat(const Array2D<T> &v, int norepeats)
{
    Array2D<T> temp(v.rows(),v.cols()*norepeats);
    for (int j=0;j<v.cols();j++) {
	for (int i=0;i<norepeats;i++) {
	    temp.set_col(j*norepeats+i,v.get_col(j));
	}
    }
    return temp;
}

/*
 */
template<class T, class fT>
Array2D<T> apply_function(fT (*f)(fT), const Array2D<T> &data)
{
    Array2D<T> out(data.rows(),data.cols());

    for (int i=0;i<out.rows();i++)
	for (int j=0;j<out.cols();j++)
	    out(i,j)=static_cast<T>(f(static_cast<fT>(data(i,j))));

    return out;
}


#define absolut(x) (((x)>0) ? (x) : (-(x)))


/*
  Row vectorize the matrix [(0,0) (0,1) ... (N-1,N-2) (N-1,N-1)]
*/
template<class T>
Array1D<T> rvectorize(const Array2D<T> &m)
{
    int i, j, n=0, r=m.rows(), c=m.cols();
    Array1D<T> v(r * c);
    
    for (i=0; i<r; i++)
	for (j=0; j<c; j++)
	    v(n++) = m(i,j);

    return v;
}


/*
  Column vectorize the matrix [(0,0) (1,0) ... (N-2,N-1) (N-1,N-1)]
*/
template<class T>
Array1D<T> cvectorize(const Array2D<T> &m)
{
    int i, j, n=0, r=m.rows(), c=m.cols();
    Array1D<T> v(r * c);
    
    for (j=0; j<c; j++)
	for (i=0; i<r; i++)
	    v(n++) = m(i,j);

    return v;
}


/*
  Reshape the matrix into an rows*cols matrix taken columnwise from the original matrix
*/
template<class T>
Array2D<T> reshape(const Array2D<T> &m, int rows, int cols)
{
  //  it_assert1(m.rows()*m.cols() == rows*cols, "Array2D<T>::reshape: Sizes must match");
  Array2D<T> temp(rows, cols);
  int i, j, ii=0, jj=0;
  for (j=0; j<m.cols(); j++) {
    for (i=0; i<m.rows(); i++) {
      temp(ii++,jj) = m(i,j);
      if (ii == rows) {
	jj++; ii=0;
      }
    }
  }
  return temp;
}

/*
  Reshape the vector into an rows*cols matrix writing into new matrix columnwise
*/
template<class T>
Array2D<T> reshape(const Array1D<T> &v, int rows, int cols)
{
  //  it_assert1(v.size() == rows*cols, "Array2D<T>::reshape: Sizes must match");
  Array2D<T> temp(rows, cols);
  int i, j, ii=0;
  for (j=0; j<cols; j++) {
    for (i=0; i<rows; i++) {
      temp(i,j) = v(ii++);
    }
  }
  return temp;
}



/*
  Upsample a vector by incerting \a (usf-1) zeros after each sample
*/
template<class T>
void upsample(const Array1D<T> &v, int usf, Array1D<T> &u)
{
  //  it_assert1(usf >= 1, "upsample: upsampling factor must be equal or greater than one" );
  u.set_size(v.length()*usf);
  u.clear();
  for(long i=0;i<v.length();i++)
    u(i*usf)=v(i); 
}


template<class T>
Array1D<T> upsample(const Array1D<T> &v, int usf)
{
  Array1D<T> u;
  upsample(v,usf,u);
  return u;  
}

/*
  Upsample each column by incerting \a (usf-1) zeros after each column
*/
template<class T>
void upsample(const Array2D<T> &v, int usf, Array2D<T> &u)
{
  //  it_assert1(usf >= 1, "upsample: upsampling factor must be equal or greater than one" );
  u.set_size(v.rows(),v.cols()*usf);
  u.clear();
  for (long j=0;j<v.cols();j++)
    u.set_col(j*usf,v.get_col(j));
}

template<class T>
Array2D<T> upsample(const Array2D<T> &v, int usf)
{
  Array2D<T> u;
  upsample(v,usf,u);
  return u;  
}

/*
  Upsample each column by a factor of  \a (usf-1) by linear interpolation
*/
template<class T>
void lininterp(const Array2D<T> &v, int usf, Array2D<T> &u)
{
  //  it_assert1(usf >= 1, "lininterp: upsampling factor must be equal or greater than one" );
  long L = (v.cols()-1)*usf+1;
  u.set_size(v.rows(),L);
  for (long i = 0; i < v.rows(); i++){ 
    for (long j = 0; j < L-1; j++)
      //u(i,j) = (v(i,j/usf) + (j % usf)/((float)usf)*(v(i,(j+usf)/usf)-v(i,j/usf)));  
      u(i,j) = (v(i,j/usf) + (j % usf)/((double)usf)*(v(i,(j+usf)/usf)-v(i,j/usf)));  
    u(i,L-1) = v(i,v.cols()-1);
  }
}

/*
  Upsample each column by a factor of  \a (usf-1) by linear interpolation
*/
template<class T>
Array2D<T> lininterp(const Array2D<T> &v, int usf)
{
  Array2D<T> u;
  upsample(v,usf,u);
  return u;  
}


/*
  Upsample by a factor of  \a (usf-1) by linear interpolation
*/
template<class T>
void lininterp(const Array1D<T> &v, int usf, Array1D<T> &u)
{
  //  it_assert1(usf >= 1, "lininterp: upsampling factor must be equal or greater than one" );
  long L = (v.length()-1)*usf+1;
  u.set_size(L);
  for (long j = 0; j < L-1; j++) {
    //u(j) = (v(j/usf) + (j % usf)/((float)usf)*(v((j+usf)/usf)-v(j/usf)));  
    u(j) = (v(j/usf) + (j % usf)/((double)usf)*(v((j+usf)/usf)-v(j/usf)));  
  }
  u(L-1) = v(v.length()-1);
}


/*
  Upsample by a factor of  \a (usf-1) by linear interpolation
*/
template<class T>
Array1D<T> lininterp(const Array1D<T> &v, int usf)
{
  Array1D<T> u;
  upsample(v,usf,u);
  return u;  
}

#endif //DOXYGEN_SHOULD_SKIP_THIS

// ---------------------- Instantiations -----------------------------------------
#ifdef USE_INST

//! Template instantiation of length
template int length(const vec &v);
//! Template instantiation of length
template int length(const cvec &v);
//! Template instantiation of length
template int length(const ivec &v);
//! Template instantiation of length
template int length(const bvec &v);

//! Template instantiation of sum
template double sum(const vec &v);
//! Template instantiation of sum
template complex<double> sum(const cvec &v);
//! Template instantiation of sum
template int sum(const ivec &v);
//! Template instantiation of sum
template bin sum(const bvec &v);

//! Template instantiation of sum_sqr
template double sum_sqr(const vec &v);
//! Template instantiation of sum_sqr
template complex<double> sum_sqr(const cvec &v);
//! Template instantiation of sum_sqr
template int sum_sqr(const ivec &v);
//! Template instantiation of sum_sqr
template bin sum_sqr(const bvec &v);

//! Template instantiation of max
template double max(const vec &in);
//! Template instantiation of max
template int max(const ivec &in);
//! Template instantiation of min
template double min(const vec &in);
//! Template instantiation of min
template int min(const ivec &in);

//! Template instantiation of max_index
template int max_index(const vec &in);
//! Template instantiation of max_index
template int max_index(const ivec &in);

//! Template instantiation of min_index
template int min_index(const vec &in);
//! Template instantiation of min_index
template int min_index(const ivec &in);

//! Template instantiation of product
template double product(const vec &v);
// Template instantiation of product
//template complex<double> product(const cvec &v);
//! Template instantiation of product
template int product(const ivec &v);

//! Template instantiation of cross
template vec cross(const vec &v1, const vec &v2);
//! Template instantiation of cross
template ivec cross(const ivec &v1, const ivec &v2);

//! Template instantiation of geometric_mean
template double geometric_mean(const vec &v);
//! Template instantiation of geometric_mean
template double geometric_mean(const ivec &v);

//! Template instantiation of median
template double median(const vec &v);
//! Template instantiation of median
template double median(const ivec &v);

//! Template instantiation of norm
template double norm(const  vec &v);
// Template instantiation of norm
//template double norm(const cvec &v); Not needed???
//! Template instantiation of norm
template double norm(const  ivec &v);
// Template instantiation of norm
//template double norm(const  llvec &v);

//! Template instantiation of norm
template double norm(const  vec &v, int p);
// Template instantiation of norm
//template double norm(const cvec &v, int p); Not needed???
//! Template instantiation of norm
template double norm(const  ivec &v, int p);
// Template instantiation of norm
//template double norm(const  llvec &v, int p);

//! Template instantiation of variance
template double variance(const  vec &v);
//! Template instantiation of variance
template double variance(const  ivec &v);

//! Template instantiation of energy
template double energy(const  vec &v);
//! Template instantiation of energy
template double energy(const cvec &v);
//! Template instantiation of energy
template double energy(const  ivec &v);

//! Template instantiation of reverse
template vec reverse(const vec &in);
//! Template instantiation of reverse
template cvec reverse(const cvec &in);
//! Template instantiation of reverse
template ivec reverse(const ivec &in);
//! Template instantiation of reverse
template bvec reverse(const bvec &in);

//! Template instantiation of repeat
template vec repeat(const vec &v, int norepeats);
//! Template instantiation of repeat
template cvec repeat(const cvec &v, int norepeats);
//! Template instantiation of repeat
template ivec repeat(const ivec &v, int norepeats);
//! Template instantiation of repeat
template bvec repeat(const bvec &v, int norepeats);

//! Template instantiation of apply_function
template vec apply_function(float (*f)(float), const vec &data);
//! Template instantiation of apply_function
template vec apply_function(double (*f)(double), const vec &data);
//! Template instantiation of apply_function
template cvec apply_function(complex<double> (*f)(complex<double>), const cvec &data);
//! Template instantiation of apply_function
template ivec apply_function(int (*f)(int), const ivec &data);
//! Template instantiation of apply_function
template bvec apply_function(bin (*f)(bin), const bvec &data);

//! Template instantiation of sort
template void sort(vec &data);
//! Template instantiation of sort
template void sort(ivec &data);
//! Template instantiation of sort
template void sort(bvec &data);

//! Template instantiation of sort_index
template ivec sort_index(const vec &data);
//! Template instantiation of sort_index
template ivec sort_index(const ivec &data);
//! Template instantiation of sort_index
template ivec sort_index(const bvec &data);

//! Template instantiation of zero_pad
template ivec zero_pad(const ivec &v, int n);
//! Template instantiation of zero_pad
template vec zero_pad(const vec &v, int n);
//! Template instantiation of zero_pad
template cvec zero_pad(const cvec &v, int n);
//! Template instantiation of zero_pad
template bvec zero_pad(const bvec &v, int n);

//! Template instantiation of zero_pad
template ivec zero_pad(const ivec &v);
//! Template instantiation of zero_pad
template vec zero_pad(const vec &v);
//! Template instantiation of zero_pad
template cvec zero_pad(const cvec &v);
//! Template instantiation of zero_pad
template bvec zero_pad(const bvec &v);

//! Template instantiation of zero_pad
template mat   zero_pad(const mat &, int, int);
//! Template instantiation of zero_pad
template cmat  zero_pad(const cmat &, int, int);
//! Template instantiation of zero_pad
template imat  zero_pad(const imat &, int, int);
//! Template instantiation of zero_pad
template bmat  zero_pad(const bmat &, int, int);

//! Template instantiation of sum
template double sum(const mat &m);
//! Template instantiation of sum
template complex<double> sum(const cmat &m);
//! Template instantiation of sum
template int sum(const imat &m);
//! Template instantiation of sum
template bin sum(const bmat &m);

//! Template instantiation of sum_sqr
template double sum_sqr(const mat &m);
//! Template instantiation of sum_sqr
template complex<double> sum_sqr(const cmat &m);
//! Template instantiation of sum_sqr
template int sum_sqr(const imat &m);
//! Template instantiation of sum_sqr
template bin sum_sqr(const bmat &m);

//! Template instantiation of max
template double max(const mat &m);
//! Template instantiation of max
template int max(const imat &m);

//! Template instantiation of min
template double min(const mat &m);
//! Template instantiation of min
template int min(const imat &m);

//! Template instantiation of max_index
template void max_index(const mat &m, int &row, int &col);
//! Template instantiation of max_index
template void max_index(const imat &m, int &row, int &col);

//! Template instantiation of min_index
template void min_index(const mat &m, int &row, int &col);
//! Template instantiation of min_index
template void min_index(const imat &m, int &row, int &col);
//! Template instantiation of product
template double product(const mat &m);
// Template instantiation of product
//template complex<double> product(const cmat &v);
//! Template instantiation of product
template int product(const imat &m);

//! Template instantiation of diag
template vec diag(const mat &in);
//! Template instantiation of diag
template cvec diag(const cmat &in);

//! Template instantiation of diag
template void diag(const vec &in, mat &m);
//! Template instantiation of diag
template void diag(const cvec &in, cmat &m);

//! Template instantiation of diag
template mat diag(const vec &v);
//! Template instantiation of diag
template cmat diag(const cvec &v);

//! Template instantiation of bidiag
template mat bidiag(const vec &, const vec &);
//! Template instantiation of bidiag
template cmat bidiag(const cvec &, const cvec &);

//! Template instantiation of bidiag
template void bidiag(const vec &, const vec &, mat &);
//! Template instantiation of bidiag
template void bidiag(const cvec &, const cvec &, cmat &);

//! Template instantiation of bidiag
template void bidiag(const mat &, vec &, vec &);
//! Template instantiation of bidiag
template void bidiag(const cmat &, cvec &, cvec &);

//! Template instantiation of tridiag
template mat tridiag(const vec &main, const vec &, const vec &);
//! Template instantiation of tridiag
template cmat tridiag(const cvec &main, const cvec &, const cvec &);

//! Template instantiation of tridiag
template void tridiag(const vec &main, const vec &, const vec &, mat &);
//! Template instantiation of tridiag
template void tridiag(const cvec &main, const cvec &, const cvec &, cmat &);

//! Template instantiation of tridiag
template void tridiag(const mat &m, vec &, vec &, vec &);
//! Template instantiation of tridiag
template void tridiag(const cmat &m, cvec &, cvec &, cvec &);

//! Template instantiation of trace
template double trace(const mat &in);
//! Template instantiation of trace
template complex<double> trace(const cmat &in);
//! Template instantiation of trace
template int trace(const imat &in);
//! Template instantiation of trace
template bin trace(const bmat &in);

//! Template instantiation of transpose
template void transpose(const mat &m, mat &out);
//! Template instantiation of transpose
template void transpose(const cmat &m, cmat &out);
//! Template instantiation of transpose
template void transpose(const imat &m, imat &out);
//! Template instantiation of transpose
template void transpose(const bmat &m, bmat &out);

//! Template instantiation of transpose
template mat transpose(const mat &m);
//! Template instantiation of transpose
template cmat transpose(const cmat &m);
//! Template instantiation of transpose
template bmat transpose(const bmat &m);

//! Template instantiation of repeat
template mat repeat(const mat &m, int norepeats);
//! Template instantiation of repeat
template cmat repeat(const cmat &m, int norepeats);
//! Template instantiation of repeat
template imat repeat(const imat &m, int norepeats);
//! Template instantiation of repeat
template bmat repeat(const bmat &m, int norepeats);

//! Template instantiation of apply_function
template mat apply_function(float (*f)(float), const mat &data);
//! Template instantiation of apply_function
template mat apply_function(double (*f)(double), const mat &data);
//! Template instantiation of apply_function
template cmat apply_function(complex<double> (*f)(complex<double>), const cmat &data);
//! Template instantiation of apply_function
template imat apply_function(int (*f)(int), const imat &data);
//! Template instantiation of apply_function
template bmat apply_function(bin (*f)(bin), const bmat &data);

//! Template instantiation of rvectorize
template  vec rvectorize(const  mat &m);
//! Template instantiation of rvectorize
template cvec rvectorize(const cmat &m);
//! Template instantiation of rvectorize
template  ivec rvectorize(const  imat &m);
//! Template instantiation of rvectorize
template  bvec rvectorize(const  bmat &m);

//! Template instantiation of cvectorize
template  vec cvectorize(const  mat &m);
//! Template instantiation of cvectorize
template cvec cvectorize(const cmat &m);
//! Template instantiation of cvectorize
template  ivec cvectorize(const  imat &m);
//! Template instantiation of cvectorize
template  bvec cvectorize(const  bmat &m);

//! Template instantiation of reshape
template  mat reshape(const  mat &m, int rows, int cols);
//! Template instantiation of reshape
template cmat reshape(const cmat &m, int rows, int cols);
//! Template instantiation of reshape
template  imat reshape(const  imat &m, int rows, int cols);
//! Template instantiation of reshape
template  bmat reshape(const  bmat &m, int rows, int cols);

//! Template instantiation of reshape
template  mat reshape(const  vec &m, int rows, int cols);
//! Template instantiation of reshape
template cmat reshape(const cvec &m, int rows, int cols);
//! Template instantiation of reshape
template  imat reshape(const  ivec &m, int rows, int cols);
//! Template instantiation of reshape
template  bmat reshape(const  bvec &m, int rows, int cols);

//! Template instantiation of upsample
template vec upsample(const vec &v, int usf);
//! Template instantiation of upsample
template cvec upsample(const cvec &v, int usf);
//! Template instantiation of upsample
template ivec upsample(const ivec &v, int usf);
//! Template instantiation of upsample
template bvec upsample(const bvec &v, int usf);

//! Template instantiation of upsample
template mat upsample(const mat &v, int usf);
//! Template instantiation of upsample
template cmat upsample(const cmat &v, int usf);
//! Template instantiation of upsample
template imat upsample(const imat &v, int usf);
//! Template instantiation of upsample
template bmat upsample(const bmat &v, int usf);

//! Template instantiation of upsample
template void upsample(const vec &v, int usf,  vec & u);
//! Template instantiation of upsample
template void upsample(const cvec &v, int usf,  cvec & u);
//! Template instantiation of upsample
template void upsample(const ivec &v, int usf,  ivec & u);
//! Template instantiation of upsample
template void upsample(const bvec &v, int usf,  bvec & u);

//! Template instantiation of upsample
template void upsample(const mat &v, int usf,  mat & u);
//! Template instantiation of upsample
template void upsample(const cmat &v, int usf,  cmat & u);
//! Template instantiation of upsample
template void upsample(const imat &v, int usf,  imat & u);
//! Template instantiation of upsample
template void upsample(const bmat &v, int usf,  bmat & u);

//! Template instantiation of liniterp
template vec lininterp(const vec &v, int usf);
//! Template instantiation of liniterp
template cvec lininterp(const cvec &v, int usf);

//! Template instantiation of liniterp
template mat lininterp(const mat &v, int usf);
//! Template instantiation of liniterp
template cmat lininterp(const cmat &v, int usf);

//! Template instantiation of liniterp
template void lininterp(const vec &v, int usf,  vec & u);
//! Template instantiation of liniterp
template void lininterp(const cvec &v, int usf,  cvec & u);

//! Template instantiation of liniterp
template void lininterp(const mat &v, int usf,  mat & u);
//! Template instantiation of liniterp
template void lininterp(const cmat &v, int usf,  cmat & u);

#endif

}

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
  \brief Matrix Class Definitions 
  \author Tony Ottosson and Tobias Ringstrom

  1.13

  2003/01/04 00:21:54
*/


#ifndef _matrixh
#define _matrixh

#include <spuc.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <spucconfig.h>
#include <binary.h>
#include <vector.h>
#include <cmath>
//#include <specmat.h>

#ifdef HAVE_CBLAS
#include "base/cblas.h"
#endif

#ifdef XXX
using std::cout;
using std::endl;
using std::string;
using std::ostream;
using std::istream;
using std::istringstream;
using std::getline;
using std::swap;
#endif

namespace SPUC {

//! Define of minimum of two numbers x and y
#define minimum(x,y) (((x)<(y)) ? (x) : (y))
#define Matrix Mat

template<class T> class Vec;
template<class T> class Mat;
class bin;

// ----------------------------- Mat Friends -----------------------------

//! Horizontal concatenation of two matrices
template<class T> Mat<T> concat_horizontal(const Mat<T> &m1, const Mat<T> &m2);
//! Vertical concatenation of two matrices
template<class T> Mat<T> concat_vertical(const Mat<T> &m1, const Mat<T> &m2);
//! Addition of two matricies
template<class T> Mat<T> operator+(const Mat<T> &m1, const Mat<T> &m2);
//! Addition of a matrix and a scalar
template<class T> Mat<T> operator+(const Mat<T> &m, T t);
//! Addition of a scalar and a matrix
template<class T> Mat<T> operator+(T t, const Mat<T> &m);
//! Subtraction of two matrices
template<class T> Mat<T> operator-(const Mat<T> &m1, const Mat<T> &m2);
//! Subtraction of matrix and scalar
template<class T> Mat<T> operator-(const Mat<T> &m, T t);
//! Subtraction of scalar and matrix
template<class T> Mat<T> operator-(T t, const Mat<T> &m);
//! Negation of matrix
template<class T> Mat<T> operator-(const Mat<T> &m);
//! Multiplication of two matricies
template<class T> Mat<T> operator*(const Mat<T> &m1, const Mat<T> &m2);
//! Multiplication of matrix and vector
template<class T> Vec<T> operator*(const Mat<T> &m, const Vec<T> &v);
//! Multiplication of vector and matrix
template<class T> Vec<T> operator*(const Vec<T> &v, const Mat<T> &m);
//! Multiplication of matrix and scalar
template<class T> Mat<T> operator*(const Mat<T> &m, T t);
//! Multiplication of scalar and matrix 
template<class T> Mat<T> operator*(T t, const Mat<T> &m);
//! Element wise multiplication of two matricies
template<class T> Mat<T> elem_mult(const Mat<T> &m1, const Mat<T> &m2);
//! Division of matrix and scalar
template<class T> Mat<T> operator/(const Mat<T> &m, T t);
//! Element wise division of two matricies
template<class T> Mat<T> elem_div(const Mat<T> &m1, const Mat<T> &m2);

/*! 
  \brief Templated Matrix Class
  \author Tony Ottosson and Tobias Ringstrom
  
  Matrices can be of arbitrarily types, but conversions and functions are
  prepared for \c bin, \c short, \c int, \c double, and \c double_complex
  vectors and these are predefined as: \c bmat, \c smat, \c imat, \c mat,
  and \c cmat. \c double and \c double_complex are usually \c double and
  \c complex<double> respectively. However, this can be changed when \
  compiling the it++ (see installation notes for more details).

  Examples:

  Matrix Constructors:
  When constructing a matrix without a dimensions (memory) use
  \code mat temp; \endcode
  For construction of a matrix of a given size use
  \code mat temp(rows, cols); \endcode
  It is also possible to assign the constructed matrix the value and dimension
  of another matrix by
  \code vec temp(inmatrix); \endcode
  If you have explicit values you would like to assign to the matrix it is
  possible to do this using strings as:
  \code
  mat a("0 0.7;5 9.3"); // that is a = [0, 0.7; 5, 9.3]
  mat a="0 0.7;5 9.3";  // the constructor are called implicitly
  \endcode
  It is also possible to change dimension by
  \code temp.set_size(new_rows, new_cols, false); \endcode
  where \c false is used to indicate that the old values in \c temp
  is not copied. If you like to preserve the values use \c true.

  There are a number of methods to access parts of a matrix. Examples are
  \code
  a(5,3);     // Element number (5,3)
  a(5,9,3,5);  // Sub-matrix from rows 5, 6, 7, 8, 9 the columns 3, 4, and 5  
  a.get_row(10);  // Row 10
  a.get_col(10); // Column 10
  \endcode

  It is also possible to modify parts of a vector as e.g. in
  \code
  a.set_row(5, invector);    // Set row 5 to \c invector
  a.set_col(3, invector); // Set column 3 to \c invector
  a.copy_col(1, 5); // Copy column 5 to column 1
  a.swap_cols(1, 5); // Swap the contents of columns 1 and 5
  \endcode

  It is of course also possible to perform the common linear algebra
  methods such as addition, subtraction, and matrix multiplication. Observe
  though, that vectors are assumed to be column-vectors in operations with
  matrices.

  Most elementary functions such as sin(), cosh(), log(), abs(), ..., are
  available as operations on the individual elements of the matrices. Please
  see the individual functions for more details.
*/
template<class T>
class Mat {
 public:
  //! Class constructor
  Mat() { init(); }
  //! Create a matrix of size (inrow, incol)
  Mat(int inrow, int incol) {
    init(); 
//	it_assert1(inrow>=0 && incol>=0, "The rows and columns must be >= 0");
    alloc(inrow, incol); }
  //! Create a copy of the matrix \c m
  Mat(const Mat<T> &m);
  //! Create a copy of the vector \c invector treated as a column vector
  Mat(const Vec<T> &invector);
  //! Set matrix equal to values in string
////  Mat(const std::string &str) { init(); set(str); }
  //! Set matrix equal to values in string
  Mat(const char *str) { init(); set(str); }

  /*! 
    \brief Constructor taking a C-array as input. Copies all data.
    
    By default the matrix is stored as a RowMajor matrix (i.e. listing elements in sequence
    beginning with the first column).
  */
  Mat(T *c_array, int rows, int cols, bool RowMajor = true);

  //! Class destructor
  ~Mat() { free(); }
    
  //! The number of columns
  int cols() const { return no_cols; }
  //! The number of rows
  int rows() const { return no_rows; }
  //! Set size of matrix. If copy = true then keep the data before resizing.
  void set_size(int inrow, int incol, bool copy=false);
  //! Set matrix equal to the all zero matrix
  void zeros();
  //! Set matrix equal to the all zero matrix
  void clear() { zeros(); }
  //! Set matrix equal to the all one matrix
  void ones();
  //! Set matrix equal to values in the string
  bool set(const char *str);
  //! Set matrix equal to values in the string
////  bool set(const std::string &str);

  //! Get element (R,C) from matrix
  const T &operator()(int R,int C) const
    { 
//	  it_assert0(R>=0 && R<no_rows && C>=0 && C<no_cols, "Mat<T>::operator(): index out of range"); 
		 return data[R+C*no_rows]; }
  //! Get element (R,C) from matrix
  T &operator()(int R,int C)
    { 
//	  it_assert0(R>=0 && R<no_rows && C>=0 && C<no_cols, "Mat<T>::operator(): index out of range"); 
	  return data[R+C*no_rows]; }
  //! Get element \c index using linear addressing (by rows)
  T &operator()(int index)
    { 
//	  it_assert0(index<no_rows*no_cols && index>=0,"Mat<T>::operator(): index out of range");
    return data[index]; }
  //! Get element \c index using linear addressing (by rows)
  const T &operator()(int index) const
    { 
//	  it_assert0(index<no_rows*no_cols && index>=0,"Mat<T>::operator(): index out of range");
    return data[index]; }

  /*! 
    \brief Sub-matrix from row \c r1 to row \c r2 and columns \c c1 to \c c2.
    
    Value -1 indicates the last row and column, respectively.
  */
  const Mat<T> operator()(int r1, int r2, int c1, int c2) const;

  //! Get row \c Index  
  Vec<T> get_row(int Index) const ;
  //! Get rows \c r1 through \c r2
  Mat<T> get_rows(int r1, int r2) const;
  //! Get the rows specified by \c indexlist
  Mat<T> get_rows(const Vec<int> &indexlist) const;
  //! Get column \c Index
  Vec<T> get_col(int Index) const ;
  //! Get columns \c c1 through \c c2
  Mat<T> get_cols(int c1, int c2) const;
  //! Get the columns specified by \c indexlist
  Mat<T> get_cols(const Vec<int> &indexlist) const;
  //! Set row \c Index to \c invector
  void set_row(int Index, const Vec<T> &invector);
  //! Set column \c Index to \c invector
  void set_col(int Index, const Vec<T> &invector);
  //! Copy row \c from onto row \c to
  void copy_row(int to, int from);
  //! Copy column \c from onto column \c to
  void copy_col(int to, int from);
  //! Swap the rows \c r1 and \c r2
  void swap_rows(int r1, int r2);
  //! Swap the columns \c c1 and \c c2
  void swap_cols(int c1, int c2);

  //! Set submatrix defined by rows r1,r2 and columns c1,c2 to matrix m
  void set_submatrix(int r1, int r2, int c1, int c2, const Mat<T> &m);
  //! Set all elements of submatrix defined by rows r1,r2 and columns c1,c2 to value t
  void set_submatrix(int r1, int r2, int c1, int c2, const T t);

  //! Concatenate the matrices \c m1 and \c m2 horizontally
  friend Mat<T> concat_horizontal TEMPLATE_FUN(const Mat<T> &m1, const Mat<T> &m2);
  //! Concatenate the matrices \c m1 and \c m2 vertically
  friend Mat<T> concat_vertical TEMPLATE_FUN(const Mat<T> &m1, const Mat<T> &m2);

  //! Set all elements of the matrix equal to \c t
  void operator=(T t);
  //! Set matrix equal to \c m
  void operator=(const Mat<T> &m);
  //! Set matrix equal to the vector \c v, assuming column vector
  void operator=(const Vec<T> &v);
  //! Set matrix equal to values in the string
  void operator=(const char *values) { set(values); }

  //! Addition of matrices
  void operator+=(const Mat<T> &m);
  //! Addition of scalar to matrix
  void operator+=(T t);
  //! Addition of two matrices
  friend Mat<T> operator+TEMPLATE_FUN(const Mat<T> &m1, const Mat<T> &m2);
  //! Addition of matrix and scalar
  friend Mat<T> operator+TEMPLATE_FUN(const Mat<T> &m, T t);
  //! Addition of scalar and matrix
  friend Mat<T> operator+TEMPLATE_FUN(T t, const Mat<T> &m);

  //! Subtraction of matrix
  void operator-=(const Mat<T> &m);
  //! Subtraction of scalar from matrix
  void operator-=(T t);
  //! Subtraction of \c m2 from \c m1
  friend Mat<T> operator-TEMPLATE_FUN(const Mat<T> &m1, const Mat<T> &m2);
  //! Subraction of scalar from matrix
  friend Mat<T> operator-TEMPLATE_FUN(const Mat<T> &m, T t);
  //! Subtract matrix from scalar
  friend Mat<T> operator-TEMPLATE_FUN(T t, const Mat<T> &m);
  //! Subraction of matrix 
  friend Mat<T> operator-TEMPLATE_FUN(const Mat<T> &m);

  //! Matrix multiplication
  void operator*=(const Mat<T> &m);
  //! Multiplication by a scalar
  void operator*=(T t);
  //! Multiplication of two matrices
  friend Mat<T> operator*TEMPLATE_FUN(const Mat<T> &m1, const Mat<T> &m2);
  //! Multiplication of matrix \c m and vector \c v (column vector)
  friend Vec<T> operator*TEMPLATE_FUN(const Mat<T> &m, const Vec<T> &v);
  //! Multiplication of transposed vector \c v and matrix \c m
  friend Vec<T> operator*TEMPLATE_FUN(const Vec<T> &v, const Mat<T> &m);
  //! Multiplication of matrix and scalar
  friend Mat<T> operator*TEMPLATE_FUN(const Mat<T> &m, T t);
  //! Multiplication of scalar and matrix
  friend Mat<T> operator*TEMPLATE_FUN(T t, const Mat<T> &m);
  //! Elementwise multiplication of two matrices
  friend Mat<T> elem_mult TEMPLATE_FUN(const Mat<T> &m1, const Mat<T> &m2);

  //! Division by a scalar
  void operator/=(T t);
  //! Division of matrix with scalar
  friend Mat<T> operator/TEMPLATE_FUN(const Mat<T> &m, T t);
  //! Elementwise division with the current matrix
  void operator/=(const Mat<T> &m);
  //! Elementwise division of matrix \c m1 with matrix \c m2
  friend Mat<T> elem_div TEMPLATE_FUN(const Mat<T> &m1, const Mat<T> &m2);

  //! Compare two matrices. False if wrong sizes or different values
  bool operator==(const Mat<T> &m) const;
  //! Compare two matrices. True if different
  bool operator!=(const Mat<T> &m) const;

  //! Get element (R,C) from matrix without boundary check (Not recommended to use). 
  T &_elem(int R,int C) {  return data[R+C*no_rows]; }
  //! Get element (R,C) from matrix without boundary check (Not recommended to use).
  const T &_elem(int R,int C) const {  return data[R+C*no_rows]; }
  //! Get element \c index using linear addressing (by rows) without boundary check (Not recommended to use).
  T &_elem(int index) { return data[index]; }
  //! Get element \c index using linear addressing (by rows) without boundary check (Not recommended to use).
  const T &_elem(int index) const { return data[index]; }

  //! Access of the internal data structure. Don't use. May be changed!
  T *_data() { return data; }
  //! Access to the internal data structure. Don't use. May be changed!
  const T *_data() const { return data; }
  //! Access to the internal data structure. Don't use. May be changed!
  int _datasize() const { return datasize; }

 protected:
  //! Allocate memory for the matrix
  void alloc(int rows, int cols)
    {
      if ( datasize == rows * cols ) { // Reuse the memory
	no_rows = rows; no_cols = cols;
	return;
      }
      free();  // Free memory (if any allocated)
      if (rows == 0 || cols == 0)
	return;
      
      datasize = rows * cols;
      data = new T[datasize];
      no_rows = rows; no_cols = cols;
      
//      it_assert1(data, "Mat<T>::alloc: Out of memory");
    }

  //! Free the memory space of the matrix
  void free() { delete [] data; init(); }

  //! Protected integer variables
  int datasize, no_rows, no_cols;

  //! Protected data pointer
  T *data;

 private:
  void init()
    {
      data = 0;
      datasize = no_rows = no_cols = 0;
    }
};



/*! 
  \relates Mat
  \brief Output stream for matrices
*/
#ifdef XXX
template <class T>
std::ostream &operator<<(std::ostream &os, const Mat<T> &m);

/*! 
  \relates Mat
  \brief Input stream for matrices
*/
template <class T>
std::istream &operator>>(std::istream &is, Mat<T> &m);
#endif
/*! 
  \relates Mat
  \brief Default Matrix Type
*/
typedef Mat<double> mat;

/*! 
  \relates Mat
  \brief Default Complex Matrix Type
*/
typedef Mat<complex<double> > cmat;

/*! 
  \relates Mat
  \brief Integer matrix
*/
typedef Mat<int> imat;

/*! 
  \relates Mat
  \brief long_long matrix
*/
typedef Mat<long_long> llmat;

/*! 
  \relates Mat
  \brief short int matrix
*/
typedef Mat<short int> smat;

/*! 
  \relates Mat
  \brief bin matrix
*/
typedef Mat<bin> bmat;


//------------------ Inline definitions ----------------------------

template<class T> inline
Mat<T>::Mat(T *c_array, int rows, int cols, bool RowMajor)
{
  init();
  alloc(rows, cols);
  
  if (!RowMajor)
    memcpy(data, c_array, datasize*sizeof(T));
  else { // Row Major
    T *ptr = c_array;
    for (int i=0; i<rows; i++) {
      for (int j=0; j<cols; j++)
	data[i+j*no_rows] = *(ptr++);
    }
  }
}


template<class T> inline
const Mat<T> Mat<T>::operator()(int r1, int r2, int c1, int c2) const
{
  if (r1 == -1) r1 = no_rows-1;
  if (r2 == -1) r2 = no_rows-1;
  if (c1 == -1) c1 = no_cols-1;
  if (c2 == -1) c2 = no_cols-1;

//  it_assert1(r1>=0 && r2>=0 && r1<no_rows && r2<no_rows &&
//	     c1>=0 && c2>=0 && c1<no_cols && c2<no_cols, "operator()(r1,r2,c1,c2)");

//  it_assert1(r2>=r1 && c2>=c1, "Mat<T>::op(): r2>=r1 or c2>=c1 not fulfilled");

  Mat<T> s(r2-r1+1, c2-c1+1);

  for (int i=0;i<s.no_cols;i++)
    memcpy(s.data+i*s.no_rows, data+r1+(c1+i)*no_rows, s.no_rows*sizeof(T));

  return s;
}
// from mat.cpp
template<class T> inline
Mat<T>::Mat(const Mat<T> &m)
{ 
  init();
  alloc(m.no_rows, m.no_cols);
  copy_vector(m.datasize, m.data, data);
}

template<class T> inline
Mat<T>::Mat(const Vec<T> &v)
{
  init();
  set_size(v.length(), 1, false);
  set_col(0,v);
}

template<class T> inline
void Mat<T>::set_size(int inrow, int incol, bool copy)
{
//  it_assert1(inrow>=0 && incol>=0, "Mat<T>::set_size: The rows and columns must be >= 0");
  if (no_rows!=inrow || no_cols!=incol) {
    if (copy) {
      Mat<T> temp(*this);
      int i, j;
      
      alloc(inrow, incol);
      for (i=0;i<inrow;i++)
	for (j=0;j<incol;j++)
	  data[i+j*inrow] = (i<temp.no_rows && j<temp.no_cols) ? temp(i,j) : T(0);
    } else
      alloc(inrow, incol);
  }
}

template<class T> inline
void Mat<T>::zeros()
{
  for(int i=0; i<datasize; i++)
    data[i] = T(0);
}

template<class T> inline
void Mat<T>::ones()
{
  for(int i=0; i<datasize; i++)
    data[i] = T(1);
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS 
/* This piece of code generates the Doxygen warnings like "no matching class member found". However, the code is OK :-) */
#ifdef XXX
template<>
bool bmat::set(const char *values)
{
  istringstream buffer(values);		
  int rows=0, maxrows=10, cols=0, nocols=0, maxcols=10;
  short intemp;

  alloc(maxrows, maxcols);
	
  while(buffer.peek()!=EOF) {
    rows++;
    if (rows > maxrows) {
      maxrows *= 2;
      set_size(maxrows, maxcols, true);
    }
    cols=0;
    while(buffer.peek() != ';' && !buffer.eof()) {
      if (buffer.peek()==',') {
	buffer.get();
      } else {
	cols++;
	if (cols > nocols) {
	  nocols=cols;
	  if (cols > maxcols) {
	    maxcols=maxcols*2;
	    set_size(maxrows, maxcols, true);
	  }
	}
	buffer >> intemp;
	this->operator()(rows-1,cols-1)=intemp;
      }
    }
    if (!buffer.eof())
      buffer.get();
  }
  set_size(rows, nocols, true);

  return true;
}
#endif
#ifdef _MSC_VER
#ifdef XZZ
template<>
bool llmat::set(const char *values)
{
//    it_error("set() does not work for llmat");
    return false;
}
#endif
#endif /* _MSC_VER */

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

template<class T>
bool Mat<T>::set(const char *values)
{
  istringstream buffer(values);	
  int rows=0, maxrows=10, cols=0, nocols=0, maxcols=10;

  alloc(maxrows, maxcols);
	
  while(buffer.peek()!=EOF) {
    rows++;
    if (rows > maxrows) {
      maxrows=maxrows*2;
      set_size(maxrows, maxcols, true);
    }
    
    cols=0;
    while(buffer.peek() != ';' && !buffer.eof()) {
      if (buffer.peek()==',') {
	buffer.get();
      } else {
	cols++;
	if (cols > nocols) {
	  nocols=cols;
	  if (cols > maxcols) {
	    maxcols=maxcols*2;
	    set_size(maxrows, maxcols, true);
	  }
	}
	buffer >> this->operator()(rows-1,cols-1);
      }
    }
    
    if (!buffer.eof())
      buffer.get();
  }
  set_size(rows, nocols, true);

  return true;
}
#ifdef XXX
template<class T>
bool Mat<T>::set(const std::string &str)
{
    return set(str.c_str());
}
#endif
template<class T> inline
Vec<T> Mat<T>::get_row(int Index) const
{
 //   it_assert1(Index>=0 && Index<no_rows, "Mat<T>::get_row: index out of range");
    Vec<T> a(no_cols);

    copy_vector(no_cols, data+Index, no_rows, a._data(), 1);
    return a;
}

template<class T>
Mat<T> Mat<T>::get_rows(int r1, int r2) const
{
 //   it_assert1(r1>=0 && r2<no_rows && r1<=r2, "Mat<T>::get_rows: index out of range");
    Mat<T> m(r2-r1+1, no_cols);

    for (int i=0; i<m.rows(); i++)
      copy_vector(no_cols, data+i+r1, no_rows, m.data+i, m.no_rows);
    
    return m;
}

template<class T>
Mat<T> Mat<T>::get_rows(const Vec<int> &indexlist) const
{
    Mat<T> m(indexlist.size(),no_cols);

    for (int i=0;i<indexlist.size();i++) {
//	it_assert1(indexlist(i)>=0 && indexlist(i)<no_rows, "Mat<T>::get_rows: index out of range");
	copy_vector(no_cols, data+indexlist(i), no_rows, m.data+i, m.no_rows);
    }

    return m;
}

template<class T> inline
Vec<T> Mat<T>::get_col(int Index) const
{
//    it_assert1(Index>=0 && Index<no_cols, "Mat<T>::get_col: index out of range");
    Vec<T> a(no_rows);

    copy_vector(no_rows, data+Index*no_rows, a._data());

    return a;
}

template<class T> inline
Mat<T> Mat<T>::get_cols(int c1, int c2) const
{
//    it_assert1(c1>=0 && c2<no_cols && c1<=c2, "Mat<T>::get_cols: index out of range");
    Mat<T> m(no_rows, c2-c1+1);

    for (int i=0; i<m.cols(); i++)
      copy_vector(no_rows, data+(i+c1)*no_rows, m.data+i*m.no_rows);
    
    return m;
}

template<class T> inline
Mat<T> Mat<T>::get_cols(const Vec<int> &indexlist) const
{
  Mat<T> m(no_rows,indexlist.size());

  for (int i=0; i<indexlist.size(); i++) {
  //  it_assert1(indexlist(i)>=0 && indexlist(i)<no_cols, "Mat<T>::get_cols: index out of range");
    copy_vector(no_rows, data+indexlist(i)*no_rows, m.data+i*m.no_rows);
  }

  return m;
}

template<class T> inline
void Mat<T>::set_row(int Index, const Vec<T> &v)
{
 // it_assert1(Index>=0 && Index<no_rows, "Mat<T>::set_row: index out of range");
 // it_assert1(v.length() == no_cols, "Mat<T>::set_row: lengths doesn't match");

  copy_vector(v.size(), v._data(), 1, data+Index, no_rows);
}

template<class T> inline
void Mat<T>::set_col(int Index, const Vec<T> &v)
{
 // it_assert1(Index>=0 && Index<no_cols, "Mat<T>::set_col: index out of range");
 // it_assert1(v.length() == no_rows, "Mat<T>::set_col: lengths doesn't match");

  copy_vector(v.size(), v._data(), data+Index*no_rows);
}

template<class T> inline
void Mat<T>::copy_row(int to, int from)
{
 // it_assert1(to>=0 && from>=0 && to<no_rows && from<no_rows,
//	     "Mat<T>::copy_row: index out of range");

  if (from == to)
    return;

  copy_vector(no_cols, data+from, no_rows, data+to, no_rows);
}

template<class T> inline
void Mat<T>::copy_col(int to, int from)
{
//  it_assert1(to>=0 && from>=0 && to<no_cols && from<no_cols,
//	     "Mat<T>::copy_col: index out of range");

  if (from == to)
    return;

  copy_vector(no_rows, data+from*no_rows, data+to*no_rows);
}

template<class T> inline
void Mat<T>::swap_rows(int r1, int r2)
{
 // it_assert1(r1>=0 && r2>=0 && r1<no_rows && r2<no_rows,
//	     "Mat<T>::swap_rows: index out of range");

  if (r1 == r2)
    return;

  swap_vector(no_cols, data+r1, no_rows, data+r2, no_rows);
}

template<class T> inline
void Mat<T>::swap_cols(int c1, int c2)
{
//  it_assert1(c1>=0 && c2>=0 && c1<no_cols && c2<no_cols,
//	     "Mat<T>::swap_cols: index out of range");

  if (c1 == c2)
    return;

  swap_vector(no_rows, data+c1*no_rows, data+c2*no_rows);
}

template<class T> inline
void Mat<T>::set_submatrix(int r1, int r2, int c1, int c2, const Mat<T> &m)
{

  if (r1 == -1) r1 = no_rows-1;
  if (r2 == -1) r2 = no_rows-1;
  if (c1 == -1) c1 = no_cols-1;
  if (c2 == -1) c2 = no_cols-1;

 // it_assert1(r1>=0 && r2>=0 && r1<no_rows && r2<no_rows &&
 //            c1>=0 && c2>=0 && c1<no_cols && c2<no_cols, "Mat<T>::set_submatrix(): index out of range");

 // it_assert1(r2>=r1 && c2>=c1, "Mat<T>::set_submatrix: r2<r1 or c2<c1");
 // it_assert1(m.no_rows == r2-r1+1 && m.no_cols == c2-c1+1, "Mat<T>::set_submatrix(): sizes don't match");

  for (int i=0; i<m.no_cols; i++)
    copy_vector(m.no_rows, m.data+i*m.no_rows, data+(c1+i)*no_rows+r1);
}

template<class T> inline
void Mat<T>::set_submatrix(int r1, int r2, int c1, int c2, const T t)
{

  if (r1 == -1) r1 = no_rows-1;
  if (r2 == -1) r2 = no_rows-1;
  if (c1 == -1) c1 = no_cols-1;
  if (c2 == -1) c2 = no_cols-1;

  //  it_assert1(r1>=0 && r2>=0 && r1<no_rows && r2<no_rows &&
  //             c1>=0 && c2>=0 && c1<no_cols && c2<no_cols, "Mat<T>::set_submatrix(): index out of range");

  //  it_assert1(r2>=r1 && c2>=c1, "Mat<T>::set_submatrix: r2<r1 or c2<c1");

  int i, j, pos, rows = r2-r1+1;

  for (i=c1; i<=c2; i++) {
    pos = i*no_rows+r1;
    for (j=0; j<rows; j++) {
      data[pos++] = t;
    }
  }
}

template<class T>
Mat<T> concat_horizontal(const Mat<T> &m1, const Mat<T> &m2)
{
	//    it_assert1(m1.no_rows == m2.no_rows, "Mat<T>::concat_horizontal; wrong sizes");

    Mat<T> temp(m1.no_rows, m1.no_cols+m2.no_cols);
    int i;
  
    for (i=0; i<m1.no_cols; i++) {
	temp.set_col(i, m1.get_col(i));
    }
    for (i=0; i<m2.no_cols; i++) {
	temp.set_col(i+m1.no_cols, m2.get_col(i));
    }

    return temp;
}

template<class T>
Mat<T> concat_vertical(const Mat<T> &m1, const Mat<T> &m2)
{
	//    it_assert1(m1.no_cols == m2.no_cols, "Mat<T>::concat_vertical; wrong sizes");
  
    Mat<T> temp(m1.no_rows+m2.no_rows, m1.no_cols);
	int i;
  
    for (i=0; i<m1.no_rows; i++) {
	temp.set_row(i, m1.get_row(i));
    }
    for (i=0; i<m2.no_rows; i++) {
	temp.set_row(i+m1.no_rows, m2.get_row(i));
    }

    return temp;
}

template<class T> inline
void Mat<T>::operator=(T t)
{
  for (int i=0; i<datasize; i++)
    data[i] = t;
}

template<class T> inline
void Mat<T>::operator=(const Mat<T> &m)
{  
  set_size(m.no_rows,m.no_cols, false);
  if (m.datasize == 0) return;

  copy_vector(m.datasize, m.data, data);
}

template<class T> inline
void Mat<T>::operator=(const Vec<T> &v)
{
	//  it_assert1( (no_rows == 1 && no_cols == v.size()) || (no_cols == 1 && no_rows == v.size()),
	//	      "Mat::op=(vec); wrong size");

 // construct a 1-d column of the vector
  set_size(v.size(), 1, false);
  set_col(0, v);
}

//-------------------- Templated friend functions --------------------------

template<class T> inline
void Mat<T>::operator+=(const Mat<T> &m)
{
  if (datasize == 0)
    operator=(m);
  else {
    int i, j, m_pos=0, pos=0;
	//    it_assert1(m.no_rows==no_rows && m.no_cols==no_cols,"Mat<T>::operator+=: wrong sizes");
    for (i=0; i<no_cols; i++) {
      for (j=0; j<no_rows; j++)
	data[pos+j] += m.data[m_pos+j];
      pos += no_rows;
      m_pos += m.no_rows;
    }
  }
}

template<class T> inline
Mat<T> operator+(const Mat<T> &m1, const Mat<T> &m2)
{
    Mat<T> r(m1.no_rows, m1.no_cols);
    int i, j, m1_pos=0, m2_pos=0, r_pos=0;

	//    it_assert1(m1.no_rows==m2.no_rows && m1.no_cols == m2.no_cols, "Mat<T>::operator+: wrong sizes");
  
    for (i=0; i<r.no_cols; i++) {
	for (j=0; j<r.no_rows; j++)
	    r.data[r_pos+j] = m1.data[m1_pos+j] + m2.data[m2_pos+j];
	// next column
	m1_pos += m1.no_rows;
	m2_pos += m2.no_rows;
	r_pos += r.no_rows;
    }

    return r;
}

template<class T> inline
void Mat<T>::operator+=(T t)
{  
    for (int i=0; i<datasize; i++)
      data[i] += t;
}

template<class T> inline
Mat<T> operator+(const Mat<T> &m, T t)
{
    Mat<T> r(m.no_rows, m.no_cols);

    for (int i=0; i<r.datasize; i++)
	    r.data[i] = m.data[i] + t;

    return r;
}

template<class T> inline
Mat<T> operator+(T t, const Mat<T> &m)
{
    Mat<T> r(m.no_rows, m.no_cols);

    for (int i=0; i<r.datasize; i++)
	    r.data[i] = t + m.data[i];

    return r;
}

template<class T> inline
void Mat<T>::operator-=(const Mat<T> &m)
{
    int i,j, m_pos=0, pos=0;

    if (datasize == 0) {
	set_size(m.no_rows, m.no_cols, false);
	for (i=0; i<no_cols; i++) {
	    for (j=0; j<no_rows; j++)
		data[pos+j] = -m.data[m_pos+j];
	    // next column
	    m_pos += m.no_rows;
	    pos += no_rows;
	}
    } else {
		//	it_assert1(m.no_rows==no_rows && m.no_cols==no_cols,"Mat<T>::operator-=: wrong sizes");
	for (i=0; i<no_cols; i++) {
	    for (j=0; j<no_rows; j++)
		data[pos+j] -= m.data[m_pos+j];
	    // next column
	    m_pos += m.no_rows;
	    pos += no_rows;
	}
    }
}

template<class T> inline
Mat<T> operator-(const Mat<T> &m1, const Mat<T> &m2)
{
    Mat<T> r(m1.no_rows, m1.no_cols);
    int i, j, m1_pos=0, m2_pos=0, r_pos=0;

//    it_assert1(m1.no_rows==m2.no_rows && m1.no_cols == m2.no_cols, "Mat<T>::operator-: wrong sizes");
  
    for (i=0; i<r.no_cols; i++) {
	for (j=0; j<r.no_rows; j++)
	    r.data[r_pos+j] = m1.data[m1_pos+j] - m2.data[m2_pos+j];
	// next column
	m1_pos += m1.no_rows;
	m2_pos += m2.no_rows;
	r_pos += r.no_rows;
    }

    return r;
}

template<class T> inline
void Mat<T>::operator-=(T t)
{  
  for (int i=0; i<datasize; i++)
    data[i] -= t;
}

template<class T> inline
Mat<T> operator-(const Mat<T> &m, T t)
{
    Mat<T> r(m.no_rows, m.no_cols);
    int i, j, m_pos=0, r_pos=0;

    for (i=0; i<r.no_cols; i++) {
	for (j=0; j<r.no_rows; j++)
	    r.data[r_pos+j] = m.data[m_pos+j] - t;
	// next column
	m_pos += m.no_rows;
	r_pos += r.no_rows;
    }

    return r;
}

template<class T> inline
Mat<T> operator-(T t, const Mat<T> &m)
{
    Mat<T> r(m.no_rows, m.no_cols);
    int i, j, m_pos=0, r_pos=0;

    for (i=0; i<r.no_cols; i++) {
	for (j=0; j<r.no_rows; j++)
	    r.data[r_pos+j] = t - m.data[m_pos+j];
	// next column
	m_pos += m.no_rows;
	r_pos += r.no_rows;
    }

    return r;
}


template<class T> inline
Mat<T> operator-(const Mat<T> &m)
{
    Mat<T> r(m.no_rows, m.no_cols);
    int i, j, m_pos=0, r_pos=0;

    for (i=0; i<r.no_cols; i++) {
	for (j=0; j<r.no_rows; j++)
	    r.data[r_pos+j] = -m.data[m_pos+j];
	// next column
	m_pos += m.no_rows;
	r_pos += r.no_rows;
    }

    return r;
}

// -------- Multiplication operator -------------

#ifdef HAVE_CBLAS  // double and complex specializations using fast CBLAS routines
#ifndef DOXYGEN_SHOULD_SKIP_THIS
#ifndef _MSC_VER
template<>
#endif
void mat::operator*=(const mat &m)
{
	//    it_assert1(no_cols == m.no_rows,"mat::operator*=: wrong sizes");
    mat r(no_rows, m.no_cols); // unnecessary memory??

    cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, no_rows, m.no_cols, no_cols, 1.0,
		          data, no_rows, m.data, m.no_rows, 0.0, r.data, r.no_rows);

    operator=(r); // time consuming
}
#ifndef _MSC_VER
template<>
#endif
void cmat::operator*=(const cmat &m)
{
	//    it_assert1(no_cols == m.no_rows,"cmat::operator*=: wrong sizes");
    double_complex alpha = double_complex(1.0), beta = double_complex(0.0);
    cmat r(no_rows, m.no_cols); // unnecessary memory??

    cblas_zgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, no_rows, m.no_cols, no_cols, (const void *)&alpha,
		data, no_rows, m.data, m.no_rows, (const void*)&beta, &r.data, r.no_rows);

    operator=(r); // time consuming
}
#endif /* DOXYGEN_SHOULD_SKIP_THIS */
#endif /* HAVE_CBLAS */

template<class T> inline
void Mat<T>::operator*=(const Mat<T> &m)
{
	//    it_assert1(no_cols == m.no_rows,"Mat<T>::operator*=: wrong sizes");
    Mat<T> r(no_rows, m.no_cols); // this consumes unnessesary memory
 
    T tmp;
  
    int i,j,k, r_pos=0, pos=0, m_pos=0;
  
    for (i=0; i<r.no_cols; i++) {
	for (j=0; j<r.no_rows; j++) {
	    tmp = T(0);
	    pos = 0;
	    for (k=0; k<no_cols; k++) {
		tmp += data[pos+j] * m.data[m_pos+k];
		pos += no_rows;
	    }
	    r.data[r_pos+j] = tmp;
	}
	r_pos += r.no_rows;
	m_pos += m.no_rows;
    }
    operator=(r); // time consuming
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#ifdef HAVE_CBLAS  // double and complex specializations using fast CBLAS routines
#ifndef _MSC_VER
template<>
#endif
mat operator*(const mat &m1, const mat &m2)
{
	//    it_assert1(m1.no_cols == m2.no_rows,"mat::operator*: wrong sizes");
    mat r(m1.no_rows, m2.no_cols);

    cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, m1.no_rows, m2.no_cols, m1.no_cols, 1.0,
		m1.data, m1.no_rows, m2.data, m2.no_rows, 0.0, r.data, r.no_rows);
    
    return r;
}

#ifndef _MSC_VER
template<>
#endif
cmat operator*(const cmat &m1, const cmat &m2)
{
	//    it_assert1(m1.no_cols == m2.no_rows,"cmat::operator*: wrong sizes");
    double_complex alpha = double_complex(1.0), beta = double_complex(0.0);
    cmat r(m1.no_rows, m2.no_cols);

    cblas_zgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, m1.no_rows, m2.no_cols, m1.no_cols, (const void *)&alpha,
		m1.data, m1.no_rows, m2.data, m2.no_rows, (const void*)&beta, r.data, r.no_rows);

    return r;
}
#endif
#endif //DOXYGEN_SHOULD_SKIP_THIS

template<class T> inline
Mat<T> operator*(const Mat<T> &m1, const Mat<T> &m2)
{
	//    it_assert1(m1.no_cols == m2.no_rows,"Mat<T>::operator*: wrong sizes");
    Mat<T> r(m1.no_rows, m2.no_cols);

    T tmp;
    int i, j, k;
    T *tr=r.data, *t1, *t2=m2.data;

    for (i=0; i<r.no_cols; i++) {
	for (j=0; j<r.no_rows; j++) {
	    tmp = T(0); t1 = m1.data+j;
	    for (k=m1.no_cols; k>0; k--) {
		tmp += *(t1) * *(t2++);
		t1 += m1.no_rows;
	    }
	    *(tr++) = tmp; t2 -= m2.no_rows;
	}
	t2 += m2.no_rows;
    }
  
    return r;
}

template<class T> inline
void Mat<T>::operator*=(T t)
{    
  for (int i=0; i<datasize; i++)
    data[i] *= t;
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#ifdef HAVE_CBLAS  // double and complex specializations using fast CBLAS routines
#ifndef _MSC_VER
template<>
#endif
vec operator*(const mat &m, const vec &v)
{
	//    it_assert1(m.no_cols == v.size(),"mat::operator*: wrong sizes");
    vec r(m.no_rows);
  
    cblas_dgemv(CblasColMajor, CblasNoTrans, m.no_rows, m.no_cols, 1.0, m.data, m.no_rows, v._data(), 1,
		0.0, r._data(), 1);

    return r;
}

#ifndef _MSC_VER
template<>
#endif
cvec operator*(const cmat &m, const cvec &v)
{
	//    it_assert1(m.no_cols == v.size(),"cmat::operator*: wrong sizes");
    double_complex alpha = double_complex(1.0), beta = double_complex(0.0);
    cvec r(m.no_rows);

    cblas_zgemv(CblasColMajor, CblasNoTrans, m.no_rows, m.no_cols, (const void *)&alpha, m.data, m.no_rows, 
		v._data(), 1, (const void*)&beta, r._data(), 1);

    return r;
}
#endif
#endif //DOXYGEN_SHOULD_SKIP_THIS

template<class T> inline
Vec<T> operator*(const Mat<T> &m, const Vec<T> &v)
{
	//    it_assert1(m.no_cols == v.size(),"Mat<T>::operator*: wrong sizes");
    Vec<T> r(m.no_rows);
    int i, k, m_pos;
  
    for (i=0; i<m.no_rows; i++) {
	r(i) = T(0);
	m_pos = 0;
	for (k=0; k<m.no_cols; k++) {
	    r(i) += m.data[m_pos+i] * v(k);
	    m_pos += m.no_rows;
	}
    }
  
    return r;
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#ifdef HAVE_CBLAS  // double and complex specializations using fast CBLAS routines
#ifndef _MSC_VER
template<>
#endif //_MSC_VER
vec operator*(const vec &v, const mat &m)
{
	//    it_assert1(m.no_rows == v.size(),"Mat<T>::operator*: wrong sizes");
    vec r(m.no_cols);
  
    cblas_dgemv(CblasColMajor, CblasTrans, m.no_rows, m.no_cols, 1.0, m.data, m.no_rows, v._data(), 1,
		0.0, r._data(), 1);

    return r;
}

#ifndef _MSC_VER
template<>
#endif //_MSC_VER
cvec operator*(const cvec &v, const cmat &m)
{
	//    it_assert1(m.no_rows == v.size(),"cmat::operator*: wrong sizes");
    double_complex alpha = double_complex(1.0), beta = double_complex(0.0);
    cvec r(m.no_cols);
  
    cblas_zgemv(CblasColMajor, CblasTrans, m.no_rows, m.no_cols, (const void *)&alpha, m.data, m.no_rows,
		v._data(), 1, (const void*)&beta, r._data(), 1);

    return r;
}
#endif //HAVE_CBLAS
#endif //DOXYGEN_SHOULD_SKIP_THIS

template<class T> inline
Vec<T> operator*(const Vec<T> &v, const Mat<T> &m)
{
//    it_assert1(m.no_rows == v.size(),"Mat<T>::operator*: wrong sizes");
    Vec<T> r(m.no_cols);
    int i, k, m_pos=0;
  
    for (i=0; i<m.no_cols; i++) {
	r(i) = T(0);
	for (k=0; k<m.no_rows; k++) {
	    r(i) += m.data[m_pos+k] * v(k);
	}
	m_pos += m.no_rows;
    }
  
    return r;
}


template<class T> inline
Mat<T> operator*(const Mat<T> &m, T t)
{
  Mat<T> r(m.no_rows, m.no_cols);

  for (int i=0; i<r.datasize; i++)
    r.data[i] = m.data[i] * t;

  return r;
}


template<class T> inline
Mat<T> operator*(T t, const Mat<T> &m)
{
  Mat<T> r(m.no_rows, m.no_cols);

  for (int i=0; i<r.datasize; i++)
    r.data[i] = m.data[i] * t;

  return r;
}


template<class T> inline
Mat<T> elem_mult(const Mat<T> &m1, const Mat<T> &m2)
{
	//  it_assert1(m1.no_rows==m2.no_rows && m1.no_cols == m2.no_cols, "Mat<T>::elem_mult: wrong sizes");
  Mat<T> r(m1.no_rows, m1.no_cols);
  
  for (int i=0; i<r.datasize; i++)
    r.data[i] = m1.data[i] * m2.data[i];

    return r;
}

template<class T> inline
void Mat<T>::operator/=(T t)
{  
  for (int i=0; i<datasize; i++)
    data[i] /= t;
}

template<class T> inline
Mat<T> operator/(const Mat<T> &m, T t)
{
  Mat<T> r(m.no_rows, m.no_cols);
  
  for (int i=0; i<r.datasize; i++)
    r.data[i] = m.data[i] / t;
  
  return r;
}

template<class T> inline
void Mat<T>::operator/=(const Mat<T> &m)
{
	//  it_assert1(m.no_rows==no_rows && m.no_cols==no_cols, "Mat<T>::operator/=: wrong sizes");
  
  for (int i=0; i<datasize; i++)
    data[i] /= m.data[i];
}

template<class T> inline
Mat<T> elem_div(const Mat<T> &m1, const Mat<T> &m2)
{
	//    it_assert1(m1.no_rows==m2.no_rows && m1.no_cols == m2.no_cols, "Mat<T>::elem_div: worng sizes");
    Mat<T> r(m1.no_rows, m1.no_cols);
  
    for (int i=0; i<r.datasize; i++)
      r.data[i] = m1.data[i] / m2.data[i];

    return r;
}

template<class T>
bool Mat<T>::operator==(const Mat<T> &m) const
{
  if (no_rows!=m.no_rows || no_cols != m.no_cols) return false;
  for (int i=0;i<datasize;i++) {
    if (data[i]!=m.data[i]) return false;
  }
  return true;
}


template<class T>
bool Mat<T>::operator!=(const Mat<T> &m) const
{
    if (no_rows != m.no_rows || no_cols != m.no_cols) return true;
    for (int i=0;i<datasize;i++) {
	if (data[i]!=m.data[i]) return true;
    }
    return false;
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#ifdef XXX
template <class T>
ostream &operator<<(ostream &os, const Mat<T> &m)
{
    int i;
    
    switch (m.rows()) {
    case 0 :
	os << "[]";
	break;
    case 1 :
	os << '[' << m.get_row(0) << ']';
	break;
    default:
	os << '[' << m.get_row(0) << endl;
	for (i=1; i<m.rows()-1; i++)
	    os << ' ' << m.get_row(i) << endl;
	os << ' ' << m.get_row(m.rows()-1) << ']';
    }

    return os;
}

template <class T>
istream &operator>>(istream &is, Mat<T> &m)
{
    string str, row;
    bool first_row=true;

    while (1) {
	getline(is, row);
	if (is.eof() || row.empty())
	    break;
	if (!str.empty())
	    str += ';';
	if (row[row.length()-1] == ']') {
	    str += row.substr(0, row.length()-1);
	    break;
	} 
	str += row;
	if (first_row && row.find(';') < row.length())
	    break;
	first_row = false;
    }
    m.set(str);

    return is;
}
#endif
#endif //DOXYGEN_SHOULD_SKIP_THIS


#ifndef DOXYGEN_SHOULD_SKIP_THIS

template<class T>
T sum(const Vec<T> &v)
{
    T M=0;
    
    for (int i=0;i<v.length();i++)
	M+=v[i];
    
    return M;
}

/* T sum_sqr(const Vec<T> &v)
*/
template<class T>
T sum_sqr(const Vec<T> &v)
{
    T M=0;
    
    for (int i=0; i<v.length(); i++)
	M += v[i] * v[i];
    
    return M;
}

template<class T>
T max(const Vec<T> &in)
{
    T	maxdata=in[0];
    for (int i=1;i<in.length();i++)
	if (in[i]>maxdata)
	    maxdata=in[i];
    return maxdata;
}

template<class T>
T min(const Vec<T> &in)
{
    T mindata=in[0];
    for (int i=1;i<in.length();i++)
	if (in[i]<mindata)
	    mindata=in[i];
    return mindata;
}

/*
  Return the postion of the maximum element in the vector
*/
template<class T>
int max_index(const Vec<T> &in)
{
    int	maxindex=0;
    for (int i=0;i<in.length();i++) 
	if (in[i]>in[maxindex])
	    maxindex=i;
    return maxindex;
}

/*
  Return the postion of the minimum element in the vector
 */
template<class T>
int min_index(const Vec<T> &in)
{
    int	minindex=0;
    for (int i=0;i<in.length();i++)
	if (in[i]<in[minindex])
	    minindex=i;
    return minindex;
}

/*
  Return the product of all elements in the vector
*/
template<class T>
T product(const Vec<T> &v)
{
    double lnM=0;
    for (int i=0;i<v.length();i++)
	lnM += log(static_cast<double>(v[i]));
    return static_cast<T>(exp(lnM));
}

/*
  Vector cross product
*/
template<class T>
Vec<T> cross(const Vec<T> &v1, const Vec<T> &v2)
{
    it_assert( v1.size() == 3 && v2.size() == 3, "cross: vectors should be of size 3");

    Vec<T> r(3);

    r(0) = v1(1) * v2(2) - v1(2) * v2(1);
    r(1) = v1(2) * v2(0) - v1(0) * v2(2);
    r(2) = v1(0) * v2(1) - v1(1) * v2(0);
    
    return r;
}

template<class T>
double geometric_mean(const Vec<T> &v)
{
    return exp(log(product(v))/v.length());
}

/*
  Return the median value of the elements in the vector
*/
template<class T>
double median(const Vec<T> &v)
{
    Vec<T> invect(v);
    sort(invect);
    return (double)(invect[(invect.length()-1)/2]+invect[invect.length()/2])/2.0;
}


//  Inefficient, but correct version:
/*
template<class T>
double variance(const Vec<T> &v) {
    int 	len = v.length();
    return (double)(pow((double)norm(v,2),2)-pow((double)abs(mean(v)),2)*len)/(len-1);
}
*/
/*
  Calculate the 2-norm: norm(v)=sqrt(sum(abs(v).^2))
*/
template<class T>
double norm(const Vec<T> &v)
{
    int i;
    double e=0.0;
    
    for (i=0; i<v.length(); i++)
		e += SQR( v[i] );

    return sqrt(e);
}
/*
template< > double norm(const cvec &v)
{
    int i;
    double e=0.0;
    
    for (i=0; i<v.length(); i++)
	e += std::norm(v[i]);

    return sqrt(e);
}
*/
/*
  Calculate the p-norm: norm(v,p)=sum(abs(v).^2)^(1/p)
*/
template<class T>
double norm(const Vec<T> &v, int p)
{
    int i;
    double e=0;
    
    for (i=0;i<v.length();i++)
	e+=pow(fabs(v[i]),(double)p); // fabs() shoud be OK

    return pow(e,1.0/(double)p);
}
/*
template<double_complex>
double norm(const Vec<double_complex> &v, int p)
{
    int i;
    double e=0;
    
    for (i=0;i<v.length();i++)
	e+=pow(std::norm(v[i]), p/2.0); // Yes, 2.0 is correct!

    return pow(e,1.0/(double)p);
}
*/
/*
  Return the variance of the elements in the vector. Normalized with N-1 to be unbiased.
 */
template<class T>
double variance(const Vec<T> &v)
{
    int len = v.length();
    const T *p=v._data();
    T sum=0.0;
	T sq_sum=0.0;

    for (int i=0; i<len; i++, p++) {
		sum += *p;
		sq_sum += SQR(*p * *p);
    }
    
    return (double)(sq_sum - SQR(sum)/len) / (len-1);
}


/*
  Calculate the energy: squared 2-norm. energy(v)=sum(v.^2)
*/
template<class T>
double energy(const Vec<T> &v)
{
    return SQR(norm(v));
}

/* 
template<class T>
Vec<T> normalize(Vec<T> &v1, T radius) {
  dvec temp(v1.length());
  int i;
  T e=norm(v1),r;
  if (e!=0) {
    r=radius/e;
    for (i=0;i<v1.length();i++) {
      v1[i]*=r;
    }
  }
  return temp;
}
*/

/*
  Reverse the input vector
*/
template<class T>
Vec<T> reverse(const Vec<T> &in)
{
    int i, s=in.length();
  
    Vec<T> out(s);
    for (i=0;i<s;i++)
	out[i]=in[s-1-i];
    return out;
}

/*
  Repeat each element in the vector norepeats times in sequence
*/
template<class T>
Vec<T> repeat(const Vec<T> &v, int norepeats)
{
    Vec<T> temp(v.length()*norepeats);
  
    for(int i=0;i<v.length();i++) {
	for(int j=0;j<norepeats;j++)
	    temp(i*norepeats+j)=v(i);
    }
    return temp;
}

/*
  Apply arbitrary function to a vector
 */
template<class T, class fT>
Vec<T> apply_function(fT (*f)(fT), const Vec<T> &data)
{
    Vec<T> out(data.length());
  
    for (int i=0;i<data.length();i++) 
	out[i]=static_cast<T>(f(static_cast<fT>(data[i])));
    return out;
}

/*
This is probably som old code that should be removed.
Commented out by Pål Frenger 2003-01-15

template <class T>
void insertion_sort(Vec<T> &v)
{
    if (v.size() < 2)
	return;
    
    if (v[1] < v[0])
	swap(v[0], v[1]);
	
    for (int i=2; i<v.size(); i++) {
	if (v[i] < v[i-1]) {
	    T tmp = v[i];
	    v[i] = v[i-1];
	    j = i-2;
	    while (j>0 && tmp<v[j]) {
		v[j+1] = v[j];
		j--;
	    }
	    v[j] = tmp;
	}
    }
}
*/

template<class T> void QS(int low, int high, Vec<T> &data) {
    int plow, phigh;
    T a,test;
	
    if (high>low) {
	a=data[low];
	plow=low;
	phigh=high;
	test=data[phigh];
	while (plow<phigh) {
	    if (test<a) {
		data[plow]=test;
		plow++;
		test=data[plow];
	    } else {
		data[phigh]=test;
		phigh--;
		test=data[phigh];
	    }
	}
	data[plow]=a;
	QS(low,plow-1,data);
	QS(plow+1,high,data);
    }
}

// Instantiation of help functions
template void QS(int low, int high, vec &data);
template void QS(int low, int high, svec &data);
template void QS(int low, int high, ivec &data);
template void QS(int low, int high, bvec &data);

/*
  Sort the the vector in increasing order
*/
template<class T>
void sort(Vec<T> &data)
{
    QS(0,data.size()-1,data);
}

template<class T>
void QSindex(int low, int high, ivec &indexlist, const Vec<T> &data)
{
    int plow,phigh,testindex,aindex;
    T a,test;

    if (high>low) {
	aindex=indexlist[low];
	a=data[aindex];
	plow=low;
	phigh=high;
	testindex=indexlist[phigh];
	test=data[testindex];
	while (plow<phigh) {
	    if (test<a) {
		indexlist[plow]=testindex;
		plow++;
		testindex=indexlist[plow];
		test=data[testindex];
	    } else {
		indexlist[phigh]=testindex;
		phigh--;
		testindex=indexlist[phigh];
		test=data[testindex];
	    }
	}
	indexlist[plow]=aindex;
	QSindex(low,plow-1,indexlist,data);
	QSindex(plow+1,high,indexlist,data);
    }
}

// Instantiation of help functions
template void QSindex(int low, int high, ivec &indexlist, const vec &data);
template void QSindex(int low, int high, ivec &indexlist, const svec &data);
template void QSindex(int low, int high, ivec &indexlist, const ivec &data);
template void QSindex(int low, int high, ivec &indexlist, const bvec &data);

/*
  Return an index vector corresponding to a sorted vector (increasing order)
*/
template<class T>
ivec sort_index(const Vec<T> &data)
{
    int N=data.length(),i;
    ivec indexlist(N);
  
    for(i=0;i<N;i++) {
	indexlist(i)=i;
    }
    QSindex(0,N-1,indexlist,data);
    return indexlist;
}

/*
  Zero-pad a vector to size n
 */
template<class T>
Vec<T> zero_pad(const Vec<T> &v, int n)
{
    it_assert(n>=v.size(), "zero_pad() cannot shrink the vector!");
    Vec<T> v2(n);
    v2.set_subvector(0, v.size()-1, v);
    if (n > v.size())
	v2.set_subvector(v.size(), n-1, T(0));

    return v2;
}

/*
  Zero-pad a vector to the nearest greater power of two
 */
template<class T>
Vec<T> zero_pad(const Vec<T> &v)
{
    int n = pow2(needed_bits(v.size()));
    
    return n==v.size() ? v : zero_pad(v, n);
}

/*
  Zero-pad a matrix to size rows x cols
 */
template<class T>
Mat<T> zero_pad(const Mat<T> &m, int rows, int cols)
{
    it_assert(rows>=m.rows() && cols>=m.cols(), "zero_pad() cannot shrink the matrix!");
    Mat<T> m2(rows, cols);
    m2.set_submatrix(0,m.rows()-1,0,m.cols()-1, m);
    if (cols > m.cols()) // Zero
	m2.set_submatrix(0,m.rows()-1, m.cols(),cols-1, T(0));
    if (rows > m.rows()) // Zero
	m2.set_submatrix(m.rows(), rows-1, 0, cols-1, T(0));

    return m2;
}
//---------- Template functions for Mat<T> ----------------------

/*
  Sum of elements in a matrix
*/
template<class T>
T sum(const Mat<T> &v)
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
T sum_sqr(const Mat<T> &v)
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
T max(const Mat<T> &m)
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
T min(const Mat<T> &m)
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
void max_index(const Mat<T> &m, int &row, int &col)
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
void min_index(const Mat<T> &m, int &row, int &col)
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
T product(const Mat<T> &m)
{
    int i, j;
    double lnM=0;
    
    for (i=0; i<m.rows(); i++)
	for (j=0; j<m.cols(); j++)
	    lnM += log(m(i,j));
    
    return static_cast<T>(exp(lnM));
}
/*
  Return a diagonal matrix
*/
template<class T>
Mat<T> diag(const Vec<T> &in)
{
    Mat<T> m(in.size(), in.size());
    m = T(0);
    for (int i=in.size()-1; i>=0; i--)
	m(i,i) = in(i);
    return m;
}

template<class T>
void diag(const Vec<T> &in, Mat<T> &m)
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
Vec<T> diag(const Mat<T> &in)
{
    Vec<T> t(std::min(in.rows(), in.cols()));

    for (int i=0; i<t.size(); i++)
	t(i) = in(i,i);

    return t;
}


template<class T>
Mat<T> bidiag(const Vec<T> &main, const Vec<T> &sup)
{
    it_assert(main.size() == sup.size()+1, "bidiag()");
    
    int n=main.size();
    Mat<T> m(n, n);
    m = T(0);
    for (int i=0; i<n-1; i++) {
	m(i,i) = main(i);
	m(i,i+1) = sup(i);
    }
    m(n-1,n-1) = main(n-1);
    
    return m;
}

template<class T>
void   bidiag(const Vec<T> &main, const Vec<T> &sup, Mat<T> &m)
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
void bidiag(const Mat<T> &m, Vec<T> &main, Vec<T> &sup)
{
    it_assert(m.rows() == m.cols(), "bidiag(): Matrix must be square!");
    
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
Mat<T> tridiag(const Vec<T> &main, const Vec<T> &sup, const Vec<T> &sub)
{
    it_assert(main.size()==sup.size()+1 && main.size()==sub.size()+1, "bidiag()");
    
    int n=main.size();
    Mat<T> m(n, n);
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
void tridiag(const Vec<T> &main, const Vec<T> &sup, const Vec<T> &sub, Mat<T> &m)
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
void tridiag(const Mat<T> &m, Vec<T> &main, Vec<T> &sup, Vec<T> &sub)
{
    it_assert(m.rows() == m.cols(), "tridiag(): Matrix must be square!");
    
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
T trace(const Mat<T> &in)
{
    return sum(diag(in));
}

/*
  Transpose of a matrix
*/
template<class T>
void transpose(const Mat<T> &m, Mat<T> &out)
{
  out.set_size(m.cols(),m.rows(), false);
  
  for (int i=0; i<m.rows(); i++) {
    for (int j=0; j<m.cols(); j++) {
      out(j,i) = m(i,j);
    }
  }
}

/*
  Transpose of a matrix
*/
template<class T>
Mat<T> transpose(const Mat<T> &m)
{
  Mat<T> temp(m.cols(),m.rows());
  
  for (int i=0; i<m.rows(); i++) {
    for (int j=0; j<m.cols(); j++) {
      temp(j,i) = m(i,j);
    }
  }
  return temp;
}

/*
  Repeats each column norepeats times in sequence.
*/
template<class T>
Mat<T> repeat(const Mat<T> &v, int norepeats)
{
    Mat<T> temp(v.rows(),v.cols()*norepeats);
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
Mat<T> apply_function(fT (*f)(fT), const Mat<T> &data)
{
    Mat<T> out(data.rows(),data.cols());

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
Vec<T> rvectorize(const Mat<T> &m)
{
    int i, j, n=0, r=m.rows(), c=m.cols();
    Vec<T> v(r * c);
    
    for (i=0; i<r; i++)
	for (j=0; j<c; j++)
	    v(n++) = m(i,j);

    return v;
}


/*
  Column vectorize the matrix [(0,0) (1,0) ... (N-2,N-1) (N-1,N-1)]
*/
template<class T>
Vec<T> cvectorize(const Mat<T> &m)
{
    int i, j, n=0, r=m.rows(), c=m.cols();
    Vec<T> v(r * c);
    
    for (j=0; j<c; j++)
	for (i=0; i<r; i++)
	    v(n++) = m(i,j);

    return v;
}


/*
  Reshape the matrix into an rows*cols matrix taken columnwise from the original matrix
*/
template<class T>
Mat<T> reshape(const Mat<T> &m, int rows, int cols)
{
  it_assert1(m.rows()*m.cols() == rows*cols, "Mat<T>::reshape: Sizes must match");
  Mat<T> temp(rows, cols);
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
Mat<T> reshape(const Vec<T> &v, int rows, int cols)
{
	//  it_assert1(v.size() == rows*cols, "Mat<T>::reshape: Sizes must match");
  Mat<T> temp(rows, cols);
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
void upsample(const Vec<T> &v, int usf, Vec<T> &u)
{
	//  it_assert1(usf >= 1, "upsample: upsampling factor must be equal or greater than one" );
  u.set_size(v.length()*usf);
  u.clear();
  for(long i=0;i<v.length();i++)
    u(i*usf)=v(i); 
}


template<class T>
Vec<T> upsample(const Vec<T> &v, int usf)
{
  Vec<T> u;
  upsample(v,usf,u);
  return u;  
}

/*
  Upsample each column by incerting \a (usf-1) zeros after each column
*/
template<class T>
void upsample(const Mat<T> &v, int usf, Mat<T> &u)
{
	//  it_assert1(usf >= 1, "upsample: upsampling factor must be equal or greater than one" );
  u.set_size(v.rows(),v.cols()*usf);
  u.clear();
  for (long j=0;j<v.cols();j++)
    u.set_col(j*usf,v.get_col(j));
}

template<class T>
Mat<T> upsample(const Mat<T> &v, int usf)
{
  Mat<T> u;
  upsample(v,usf,u);
  return u;  
}

/*
  Upsample each column by a factor of  \a (usf-1) by linear interpolation
*/
template<class T>
void lininterp(const Mat<T> &v, int usf, Mat<T> &u)
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
Mat<T> lininterp(const Mat<T> &v, int usf)
{
  Mat<T> u;
  upsample(v,usf,u);
  return u;  
}


/*
  Upsample by a factor of  \a (usf-1) by linear interpolation
*/
template<class T>
void lininterp(const Vec<T> &v, int usf, Vec<T> &u)
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
Vec<T> lininterp(const Vec<T> &v, int usf)
{
  Vec<T> u;
  upsample(v,usf,u);
  return u;  
}


#endif //DOXYGEN_SHOULD_SKIP_THIS
}
#endif
 

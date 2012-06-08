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
  \brief Templated Vector Class Definitions
  \author Tony Ottosson and Tobias Ringström

  1.16

  2003/01/04 00:21:54
*/

#ifndef _vectorh
#define _vectorh

#include <cmath>
#include <sstream>
#include <iostream>

#ifdef HAVE_CBLAS
#include "cblas.h"
#endif

// This needs to be included before algorithm included in scalfunc for g++-2.7.2
#include <binary.h>
#include <spucconfig.h>
#include <algorithm>
//#include "spucassert.h"
#include <complex.h>
#include <cstdlib>
#include <cstring>
#define Vector Vec
namespace SPUC {

//! Default vector class of type \c double. Usually double
template<class T> class Vec;
//! Default matrix class of type \c double. Usually double
template<class T> class Mat;
class bin;

//----------------- Vec Friends -------------------------
//! Addition of two vectors
template<class T> Vec<T> operator+(const Vec<T> &v1, const Vec<T> &v2);
//! Addition of a vector and a scalar
template<class T> Vec<T> operator+(const Vec<T> &v, T t);
//! Addition of a scalar and a vector
template<class T> Vec<T> operator+(T t, const Vec<T> &v);
//! Subtraction of a vector from a vector
template<class T> Vec<T> operator-(const Vec<T> &v1, const Vec<T> &v2);
//! Subtraction of a scalar from a vector
template<class T> Vec<T> operator-(const Vec<T> &v, T t);
//! Subtraction of vector from scalar. Results in a vector
template<class T> Vec<T> operator-(T t, const Vec<T> &v);
//! Negation of vector
template<class T> Vec<T> operator-(const Vec<T> &v);
//! Inner (dot) product of two vectors v1 and v2
template<class T> T dot(const Vec<T> &v1, const Vec<T> &v2);
template<class T> T dot_prod(const Vec<T> &v1, const Vec<T> &v2);
//! Inner (dot) product of two vectors v1 and v2
template<class T> T operator*(const Vec<T> &v1, const Vec<T> &v2)
  { return dot(v1, v2); }
//! Outer product of two vectors v1 and v2
template<class T> Mat<T> outer_product(const Vec<T> &v1, const Vec<T> &v2);
//! Multiplication of a vector and a scalar
template<class T> Vec<T> operator*(const Vec<T> &v, T t);
//! Multiplication of a scalar and a vector. Results in a vector
template<class T> Vec<T> operator*(T t, const Vec<T> &v);
//! Elementwise multiplication of the two vectors
template<class T> Vec<T> elem_mult(const Vec<T> &v1, const Vec<T> &v2);
//! Elementwise multiplication of the three vectors
template<class T> Vec<T> elem_mult(const Vec<T> &v1, const Vec<T> &v2, const Vec<T> &v3);
//! Elementwise multiplication of the four vectors
template<class T> Vec<T> elem_mult(const Vec<T> &v1, const Vec<T> &v2, const Vec<T> &v3, const Vec<T> &v4);

//! Division of all elements in \c v with \c t
template<class T> Vec<T> operator/(const Vec<T> &v, T t);
//! Division of \c t with all elements in \c v
template<class T> Vec<T> operator/(const T t, const Vec<T> &v);
//! Elementwise division
template<class T> Vec<T> elem_div(const Vec<T> &v1, const Vec<T> &v2);
//! Elementwise division of scalar \c t and vector \c v
template<class T> Vec<T> elem_div(const T t, const Vec<T> &v);

//! Append element \c a to the end of the vector \c v
template<class T> Vec<T> concat(const Vec<T> &v, const T a);
//! Concat element \c a to the beginning of the vector \c v
template<class T> Vec<T> concat(const T a, const Vec<T> &v);
//! Concat vectors \c v1 and \c v2
template<class T> Vec<T> concat(const Vec<T> &v1,const Vec<T> &v2);
//! Concat vectors \c v1, \c v2 and \c v3
template<class T> Vec<T> concat(const Vec<T> &v1, const Vec<T> &v2, const Vec<T> &v3);
//! Concat vectors \c v1, \c v2, \c v3 and \c v4
template<class T> Vec<T> concat(const Vec<T> &v1, const Vec<T> &v2, const Vec<T> &v3, const Vec<T> &v4);
//! Concat vectors \c v1, \c v2 \c v3, \c v4 and \c v5
template<class T> Vec<T> concat(const Vec<T> &v1, const Vec<T> &v2, const Vec<T> &v3, 
				const Vec<T> &v4, const Vec<T> &v5);

/*! 
  \brief Templated vectors
  \author Tony Ottosson and Tobias Ringstrom

  Vectors can be of arbitrarily types, but conversions and functions are
  prepared for \c bin, \c short, \c int, \c double, and \c double_complex
  vectors and these are predefined as: \c bvec, \c svec, \c ivec, \c vec,
  and \c cvec. \c double and \c double_complex are \c double and
  \c complex<double> respectively.

  Examples:

  Vector Constructors:
  When constructing a vector without a length (memory) use
  \code vec temp; \endcode
  For construction of a vector of a given length use
  \code vec temp(length); \endcode
  It is also possible to assign the constructed vector the value and size
  of another vector by
  \code vec temp(invector); \endcode
  If you have explicit values you would like to assign to the vector it is
  possible to do this using strings as:
  \code
  vec a("0 0.7 5 9.3"); // that is a = [0, 0.7, 5, 9.3]
  vec a="0 0.7 5 9.3";  // the constructor are called implicitly
  ivec b="0:5";  // that is b = [0, 1, 2, 3, 4, 5]
  vec c="3:2.5:13";  // that is c = [3, 5.5, 8, 10.5, 13]
  \endcode
  It is also possible to change length by
  \code temp.set_size(new_length, false); \endcode
  where \c false is used to indicate that the old values in \c temp
  is not copied. If you like to preserve the values use \c true.

  There are a number of methods to access parts of a vector. Examples are
  \code
  a(5);     // Element number 5
  a(5,9);  // Elements 5, 6, 7, 8, and 9
  a.left(10);  // The 10 most left elements (the first)
  a.right(10); // The 10 most right elements (the last)
  a.mid(5, 7); // 7 elements starting from element 5
  \endcode

  It is also possible to modify parts of a vector as e.g. in
  \code
  a.del(5);    // deletes element number 5
  a.ins(3.4, 9); // inserts the element 3.4 at position 9
  a.replace_mid(12, b); // replaces elements from 12 with the vector b
  \endcode

  It is of course also possible to perform the common linear algebra
  methods such as addition, subtraction, and scalar product (*). Observe
  though, that vectors are assumed to be column-vectors in operations with
  matrices.

  Most elementary functions such as sin(), cosh(), log(), abs(), ..., are
  available as operations on the individual elements of the vectors. Please
  see the individual functions for more details.
*/
template<class T>
class Vec {
 public:
  //! Constructor
  Vec() { init(); }
  //! Constructor
  explicit Vec(int size) { 
//	  it_assert1(size>=0,"Negative size in Vec::Vec(int)"); 
	  init(); alloc(size); }
  //! Constructor
  Vec(const Vec<T> &v);
  //! Constructor
  Vec(const char *values) { init(); set(values); }
  //! Constructor
  Vec(const string &values) { init(); set(values); }
  //! Constructor taking a C-array as input. Copies all data.
  Vec(T *c_array, int size) { init(); alloc(size); memcpy(data, c_array, size*sizeof(T)); }
  //! Destructor
  ~Vec() { free(); }

  //! The size of the vector
  int length() const { return datasize; }
  //! The size of the vector
  int size() const { return datasize; }

  //! Set length of vector. if copy = true then keeping the old values
  void set_length(int size, bool copy=false) { set_size(size,copy); }
  //! Set length of vector. if copy = true then keeping the old values
  void set_size(int size, bool copy=false);
  //! Set the vector to the all zero vector
  void zeros() { for (int i=0; i<datasize; i++) {data[i]=T(0);} }
  //! Set the vector to the all zero vector
  void clear() { zeros(); }
  //! Set the vector to the all one vector
  void ones() { for (int i=0; i<datasize; i++) {data[i]=T(1);} }
  //! Set the vector equal to the values in the \c str string
  bool set(const char *str);
  //! Set the vector equal to the values in the \c str string
  bool set(const string &str);

  //! C-style index operator. First element is 0
  T operator[](int i) const { 
	  //it_assert0(i>=0&&i<datasize, "operator[]");
	  return data[i]; }
  //! Index operator. First element is 0
  T operator()(int i) const { 
//	  it_assert0(i>=0&&i<datasize, "operator()"); 
	  return data[i]; }
  //! C-style index operator. First element is 0
  T &operator[](int i) { 
//		it_assert0(i>=0&&i<datasize, "operator[]"); 
		return data[i]; }
  //! Index operator. First element is 0
  T &operator()(int i) { 
	  //it_assert0(i>=0&&i<datasize, "operator()"); 
	return data[i]; }
  //! Sub-vector with elements from \c i1 to \c i2. Index -1 indicates the last element.
  const Vec<T> operator()(int i1, int i2) const;
  //! Sub-vector where the elements are given by the list \c indexlist
  const Vec<T> operator()(const Vec<int> &indexlist) const;

  //! Addition of vector
  void operator+=(const Vec<T> &v);
  //! Addition of scalar
  void operator+=(T t) { for (int i=0;i<datasize;i++) data[i]+=t; }
  //! Addition of two vectors
  friend Vec<T> operator+TEMPLATE_FUN(const Vec<T> &v1, const Vec<T> &v2);
  //! Addition of a vector and a scalar
  friend Vec<T> operator+TEMPLATE_FUN(const Vec<T> &v, T t);
  //! Addition of a scalar and a vector
  friend Vec<T> operator+TEMPLATE_FUN(T t, const Vec<T> &v);
  
  //! Subtraction of vector
  void operator-=(const Vec<T> &v);
  //! Subtraction of scalar
  void operator-=(T t) { for (int i=0;i<datasize;i++) data[i]-=t; }
  //! Subtraction of \c v2 from \c v1
  friend Vec<T> operator-TEMPLATE_FUN(const Vec<T> &v1, const Vec<T> &v2);
  //! Subtraction of scalar from vector
  friend Vec<T> operator-TEMPLATE_FUN(const Vec<T> &v, T t);
  //! Sutraction of vector from scalar
  friend Vec<T> operator-TEMPLATE_FUN(T t, const Vec<T> &v);
  //! Negation of vector
  friend Vec<T> operator-TEMPLATE_FUN(const Vec<T> &v);
  
  //! Multiply with a scalar
  void operator*=(T t) { for (int i=0;i<datasize;i++) data[i] *= t; }
  //! Inner (dot) product
  friend T operator*TEMPLATE_FUN(const Vec<T> &v1, const Vec<T> &v2);
  //! Inner (dot) product
  friend T dot TEMPLATE_FUN(const Vec<T> &v1, const Vec<T> &v2);
  friend T dot_prod TEMPLATE_FUN(const Vec<T> &v1, const Vec<T> &v2);
  //! Outer product of two vectors v1 and v2
  friend Mat<T> outer_product TEMPLATE_FUN(const Vec<T> &v1, const Vec<T> &v2);
  //! Elementwise multiplication of vector and scalar
  friend Vec<T> operator*TEMPLATE_FUN(const Vec<T> &v, T t);
  //! Elementwise multiplication of vector and scalar 
  friend Vec<T> operator*TEMPLATE_FUN(T t, const Vec<T> &v);
  //! Elementwise multiplication
  friend Vec<T> elem_mult TEMPLATE_FUN(const Vec<T> &v1, const Vec<T> &v2);
  //! Elementwise multiplication of three vectors
  friend Vec<T> elem_mult TEMPLATE_FUN(const Vec<T> &v1, const Vec<T> &v2, const Vec<T> &v3);  
  //! Elementwise multiplication of four vectors
  friend Vec<T> elem_mult TEMPLATE_FUN(const Vec<T> &v1, const Vec<T> &v2, const Vec<T> &v3, const Vec<T> &v4);

  //! Elementwise division
  void operator/=(T t) { for (int i=0;i<datasize;i++) data[i]/=t; }
  //! Elementwise division
  friend Vec<T> operator/TEMPLATE_FUN(const Vec<T> &v, T t);
  //! Elementwise division
  friend Vec<T> operator/TEMPLATE_FUN(const T t, const Vec<T> &v);
  //! Elementwise division
  void operator/=(const Vec<T> &v);
  //! Elementwise division
  friend Vec<T> elem_div TEMPLATE_FUN(const Vec<T> &v1, const Vec<T> &v2);
  //! Elementwise division
  friend Vec<T> elem_div TEMPLATE_FUN(const T t, const Vec<T> &v);
  
  //! Get the elements in the vector where \c binlist is \c 1
  Vec<T> get(const Vec<bin> &binlist) const;
  //! Get the right \c nr elements from the vector
  Vec<T> right(int nr) const;
  //! Get the left \c nr elements from the vector
  Vec<T> left(int nr) const;
  //! Get the middle part of vector from \c start including \c nr elements
  Vec<T> mid(int start, int nr) const;
  //! Split the vector into two parts at element \c pos. Return the first part and keep the second.
  Vec<T> split(int pos);
  //! Shift in element \c In at position 0 \c n times
  void shift_right(T In, int n=1);
  //! Shift in vector \c In at position 0
  void shift_right(const Vec<T> &In);
  //! Shift out the \c n left elements and a the same time shift in the element \c at last position \c n times
  void shift_left(T In, int n=1);
  //! Shift in vector \c In at last position
  void shift_left(const Vec<T> &In);

  //! Append element \c a to the end of the vector \c v
  friend Vec<T> concat TEMPLATE_FUN(const Vec<T> &v, const T a);
  //! Concat element \c a to the beginning of the vector \c v
  friend Vec<T> concat TEMPLATE_FUN(const T a, const Vec<T> &v);
  //! Concat vectors \c v1 and \c v2
  friend Vec<T> concat TEMPLATE_FUN(const Vec<T> &v1,const Vec<T> &v2);
  //! Concat vectors \c v1, \c v2 and \c v3
  friend Vec<T> concat TEMPLATE_FUN(const Vec<T> &v1, const Vec<T> &v2, const Vec<T> &v3);

  //! Set subvector defined by indicies i1 to i2 to vector v
  void set_subvector(int i1, int i2, const Vec<T> &v);
  //! Set subvector defined by indicies i1 to i2 to constant t
  void set_subvector(int i1, int i2, const T t);
  //! Replace the elements from \c pos by the vector \c v
  void replace_mid(int pos, const Vec<T> &v);
  //! Delete element number \c index
  void del(int index);
  //! Insert element \c in at \c index
  void ins(int index, T in);
  //! Insert vector \c in at \c index
  void ins(int index, const Vec<T> &in);

  //! Assign all elements in vector to \c t
  void operator=(T t) { for (int i=0;i<datasize;i++) data[i] = t; }
  //! Assign vector the value and length of \c v
  void operator=(const Vec<T> &v);
  //! Assign vector equal to the 1-dimensional matrix \c m
  void operator=(const Mat<T> &m);
  //! Assign vector the values in the string \c values
  void operator=(const char *values) { set(values); }

  //! Elementwise equal to the scalar
  Vec<bin> operator==(const T value);
  //! Elementwise not-equal to the scalar
  Vec<bin> operator!=(const T value);
  //! Elementwise less than the scalar
  Vec<bin> operator<(const T value);
  //! Elementwise less than and equal to the scalar
  Vec<bin> operator<=(const T value);
  //! Elementwise greater than the scalar
  Vec<bin> operator>(const T value);
  //! Elementwise greater than and equal to the scalar
  Vec<bin> operator>=(const T value);

  //! Compare two vectors. False if wrong sizes or different values
  bool operator==(const Vec<T> &v) const;
  //! Compare two vectors. True if different
  bool operator!=(const Vec<T> &v) const;

  //! Index operator without boundary check. Not recommended to use.
  T &_elem(int i) { return data[i]; }
  //! Index operator without boundary check. Not recommended to use.
  T _elem(int i) const { return data[i]; }

  //! Get the pointer to the internal structure. Not recommended to use.
  T *_data() { return data; }

  //! Get the pointer to the internal structure. Not recommended to use.
  const T *_data() const { return data; }
    
 protected:
  //! Allocate storage for a vector of length \c size.
  void alloc(int size)
    {
      if ( datasize == size ) return;

      free();  // Free memory (if any allocated)
      if (size == 0) return;
    
      data = new T[size];
      datasize=size;
//      it_assert1(data, "Out of memory in Vec::alloc()");
    }

  //! Free the storage space allocated by the vector
  void free() { delete [] data;  init(); }
   
  //! The current number of elements in the vector
  int datasize;
  //! A pointer to the data area
  T *data;

 private:
  void init() { data = 0; datasize = 0; }
};

/*!
  \relates Vec
  \brief Stream output of vector
*/
#ifdef XXX
template <class T>
std::ostream &operator<<(std::ostream& os, const Vec<T> &v);

/*! 
  \relates Vec
  \brief Stream input of vector
*/
template <class T>
std::istream &operator>>(std::istream& is, Vec<T> &v);
#endif

/*! 
  \relates Vec
  \brief Definition of double vector type
*/
typedef Vec<double> vec;

/*! 
  \relates Vec
  \brief Definition of double_complex vector type
*/
typedef Vec<complex<double> > cvec;

/*! 
  \relates Vec
  \brief Definition of integer vector type
*/
typedef Vec<int> ivec;


/*! 
  \relates Vec
  \brief Definition of long_long vector type
*/
typedef Vec<long_long> llvec;

/*! 
  \relates Vec
  \brief Definition of short vector type
*/
typedef Vec<short int> svec;

/*! 
  \relates Vec
  \brief Definition of binary vector type
*/
typedef Vec<bin> bvec;

/*------------------ Inline definitions ----------------------------*/

template<class T> inline
const Vec<T> Vec<T>::operator()(int i1, int i2) const
{
  if (i1 == -1)	i1 = datasize-1;
  if (i2 == -1) i2 = datasize-1;

//  it_assert1(i1>=0 && i2>=0 && i1<datasize && i2<datasize, "Vec<T>::operator()(i1,i2): indicies out of range");
//  it_assert1(i2>=i1, "Vec<T>::op(i1,i2): i2 >= i1 necessary");

   Vec<T> s(i2-i1+1);
   memcpy(s.data, data+i1, s.datasize*sizeof(T));

   return s;
}


template<class T> inline
Vec<T>::Vec(const Vec<T> &v)
{
  init();
  alloc(v.datasize);
  copy_vector(datasize, v.data, data);
}

template<class T>
void Vec<T>::set_size(int size, bool copy)
{
	//  it_assert1(size >= 0, "New size must not be negative in Vec::set_size()");
  if (size!=datasize) {
    if (copy) {
      Vec<T> temp(*this);
    
      alloc(size);
      for (int i=0; i<size; i++)
	data[i] = i < temp.datasize ? temp.data[i] : T(0);
    } else
      alloc(size);
  }
}

template<class T>
bool Vec<T>::set(const char *values)
{
    istringstream buffer(values);	
    T b,c;
    int pos=0,maxpos=10;
  
    alloc(maxpos); 
	
    while(buffer.peek()!=EOF) {

	switch (buffer.peek()) {
	case ':': // reads format a:b:c or a:b
	    buffer.get();
	    if (!buffer.eof()) {
		buffer >> b;
	    }			 
	    if (!buffer.eof() && buffer.peek() == ':') {
		buffer.get();
		if (!buffer.eof()) {
		    buffer >> c;
				       				
		    while((T)(data[pos-1]+b-c) <= (T)0) {
			pos++;
			if (pos > maxpos) {
			    maxpos=maxpos*2;
			    set_size(maxpos, true);
			}
			data[pos-1]=data[pos-2]+b;
		    }
		}			
	    } else {
		while(data[pos-1]<b) {
		    pos++;
		    if (pos > maxpos) {
			maxpos=maxpos*2;
			set_size(maxpos, true);
		    }
		    data[pos-1]= (T)data[pos-2]+(T)1;
		}
	    }
	    break;
      
	case ',':
	    buffer.get();
	    break;
	    
	default:
	    pos++;
	    if (pos > maxpos) {
		maxpos *= 2;
		set_size(maxpos, true);
	    }			
	    buffer >> data[pos-1];
	    break;
	}
	
    }
    set_size(pos, true);

    return true;
}
//#ifdef XXX
template<class T>
bool Vec<T>::set(const string &str)
{
    return set(str.c_str());
}
//#endif
template<class T>
const Vec<T> Vec<T>::operator()(const Vec<int> &indexlist) const
{
    Vec<T> temp(indexlist.length());
    for (int i=0;i<indexlist.length();i++) {
	// it_assert((indexlist(i)>=0) && (indexlist(i) < datasize), "Vec<T>::operator()(ivec &): index outside range");
	temp(i)=data[indexlist(i)];
    }
    return temp;
}

template<class T> inline
void Vec<T>::operator+=(const Vec<T> &v)
{
    int i;

    if (datasize == 0) { // if not assigned a size.
	alloc(v.datasize);
	for (i=0; i<v.datasize; i++)
	    data[i] = v.data[i];	
    } else {
	// it_assert1(datasize==v.datasize, "Vec<T>::operator+=: wrong sizes");
	for (i=0; i<datasize; i++)
	    data[i] += v.data[i];
    }
}

template<class T> inline
Vec<T> operator+(const Vec<T> &v1, const Vec<T> &v2)
{
    int i;
    Vec<T> r(v1.datasize);

    // it_assert1(v1.datasize==v2.datasize, "Vec<T>::operator+: wrong sizes");
    for (i=0; i<v1.datasize; i++)
	r.data[i] = v1.data[i] + v2.data[i];

    return r;
}

template<class T> inline
Vec<T> operator+(const Vec<T> &v, T t)
{
    int i;
    Vec<T> r(v.datasize);

    for (i=0; i<v.datasize; i++)
	r.data[i] = v.data[i] + t;

    return r;
}

template<class T> inline
Vec<T> operator+(T t, const Vec<T> &v)
{
    int i;
    Vec<T> r(v.datasize);

    for (i=0; i<v.datasize; i++)
	r.data[i] = t + v.data[i];

    return r;
}

template<class T> inline
void Vec<T>::operator-=(const Vec<T> &v)
{
    int i;

    if (datasize == 0) { // if not assigned a size.
	alloc(v.datasize);
	for (i=0; i<v.datasize; i++)
	    data[i] = -v.data[i];      
    } else {
	// it_assert1(datasize==v.datasize, "Vec<T>::operator-=: wrong sizes");
	for (i=0; i<datasize; i++)
	    data[i] -= v.data[i];
    }
}

template<class T> inline
Vec<T> operator-(const Vec<T> &v1, const Vec<T> &v2)
{
    int i;
    Vec<T> r(v1.datasize);

    // it_assert1(v1.datasize==v2.datasize, "Vec<T>::operator-: wrong sizes");
    for (i=0; i<v1.datasize; i++)
	r.data[i] = v1.data[i] - v2.data[i];

    return r;
}

template<class T> inline
Vec<T> operator-(const Vec<T> &v, T t)
{
    int i;
    Vec<T> r(v.datasize);

    for (i=0; i<v.datasize; i++)
	r.data[i] = v.data[i] - t;

    return r;
}

template<class T> inline
Vec<T> operator-(T t, const Vec<T> &v)
{
    int i;
    Vec<T> r(v.datasize);

    for (i=0; i<v.datasize; i++)
	r.data[i] = t - v.data[i];

    return r;
}

template<class T> inline
Vec<T> operator-(const Vec<T> &v)
{
    int i;
    Vec<T> r(v.datasize);

    for (i=0; i<v.datasize; i++)
	r.data[i] = -v.data[i];

    return r;
}
 
#ifndef DOXYGEN_SHOULD_SKIP_THIS

template<class T> inline
T dot(const Vec<T> &v1, const Vec<T> &v2)
{
    int i;
    T r=T(0);

    // it_assert1(v1.datasize==v2.datasize, "Vec<T>::dot: wrong sizes");
    for (i=0; i<v1.datasize; i++)
	r += v1.data[i] * v2.data[i];

    return r;
}

#endif //DOXYGEN_SHOULD_SKIP_THIS

template<class T> inline
Mat<T> outer_product(const Vec<T> &v1, const Vec<T> &v2)
{
    int i, j;

    // it_assert1(v1.datasize>0 && v2.datasize>0, "outer_product:: Vector of zero size");

    Mat<T> r(v1.datasize, v2.datasize);

    for (i=0; i<v1.datasize; i++) {
      for (j=0; j<v2.datasize; j++) {
	r(i,j) = v1.data[i] * v2.data[j];
      }
    }

    return r;
}

/* This is currently not fast enough !!! 
#ifdef HAVE_CBLAS  // double and complex specializations using CBLAS
template<>
vec operator*(const vec &v, const double t)
{
  vec r(v.size());
  cblas_dcopy(v.size(), v._data(), 1, r._data(), 1);

  cblas_dscal(r.size(), t, r._data(), 1);

  return r;
}

template<>
cvec operator*(const cvec &v, const double_complex t)
{
  cvec r(v);

  cblas_zscal(r.size(), (const void *)&t, r._data(), 1);

  return r;
}
#endif
*/

template<class T> inline
Vec<T> operator*(const Vec<T> &v, T t)
{
    int i;
    Vec<T> r(v.datasize);

    for (i=0; i<v.datasize; i++)
	r.data[i] = v.data[i] * t;

    return r;
}

template<class T> inline
Vec<T> operator*(T t, const Vec<T> &v)
{
    int i;
    Vec<T> r(v.datasize);

    for (i=0; i<v.datasize; i++)
	r.data[i] = t * v.data[i];

    return r;
}

template<class T> inline
Vec<T> elem_mult(const Vec<T> &v1, const Vec<T> &v2)
{
    int i;
    Vec<T> r(v1.datasize);

    // it_assert1(v1.datasize==v2.datasize, "Vec<T>::elem_mult: wrong sizes");
    for (i=0; i<v1.datasize; i++)
	r.data[i] = v1.data[i] * v2.data[i];

    return r;
}

template<class T> inline
Vec<T> elem_mult(const Vec<T> &v1, const Vec<T> &v2, const Vec<T> &v3)
{
    int i;
    Vec<T> r(v1.datasize);

    // it_assert1(v1.datasize==v2.datasize, "Vec<T>::elem_mult: wrong sizes");
    // it_assert1(v2.datasize==v3.datasize, "Vec<T>::elem_mult: wrong sizes");
    for (i=0; i<v1.datasize; i++)
	r.data[i] = v1.data[i] * v2.data[i] * v3.data[i];

    return r;
}

template<class T> inline
Vec<T> elem_mult(const Vec<T> &v1, const Vec<T> &v2, const Vec<T> &v3, const Vec<T> &v4)
{
    int i;
    Vec<T> r(v1.datasize);

    // it_assert1(v1.datasize==v2.datasize, "Vec<T>::elem_mult: wrong sizes");
    // it_assert1(v2.datasize==v3.datasize, "Vec<T>::elem_mult: wrong sizes");
    // it_assert1(v3.datasize==v4.datasize, "Vec<T>::elem_mult: wrong sizes");
    for (i=0; i<v1.datasize; i++)
	r.data[i] = v1.data[i] * v2.data[i] * v3.data[i] * v4.data[i];

    return r;
}

template<class T> inline
Vec<T> operator/(const Vec<T> &v, T t)
{
    int i;
    Vec<T> r(v.datasize);

    for (i=0; i<v.datasize; i++)
	r.data[i] = v.data[i] / t;

    return r;
}

template<class T> inline
Vec<T> operator/(const T t, const Vec<T> &v)
{
    int i;
    Vec<T> r(v.datasize);

    for (i=0; i<v.datasize; i++)
	r.data[i] = t / v.data[i];

    return r;
}

template<class T> inline
void Vec<T>::operator/=(const Vec<T> &v)
{
    int i;

    // it_assert1(datasize==v.datasize, "Vec<T>::operator/=: wrong sizes");
    for (i=0; i<datasize; i++)
	data[i] /= v.data[i];
}

template<class T> inline
Vec<T> elem_div(const Vec<T> &v1, const Vec<T> &v2)
{
    int i;
    Vec<T> r(v1.datasize);

    // it_assert1(v1.datasize==v2.datasize, "elem_div: wrong sizes");
    for (i=0; i<v1.datasize; i++)
	r.data[i] = v1.data[i] / v2.data[i];

    return r;
}

template<class T> inline
Vec<T> elem_div(const T t, const Vec<T> &v)
{
    int i;
    Vec<T> r(v.datasize);

    for (i=0; i<v.datasize; i++)
	r.data[i] = t / v.data[i];

    return r;
}

template<class T>
Vec<T> Vec<T>::get(const Vec<bin> &binlist) const
{
    // it_assert1(datasize == binlist.size(), "Vec<T>::get(bvec &): wrong sizes");
    Vec<T> temp(binlist.length());
    int j=0;
  
    for (int i=0;i<binlist.length();i++) {
	if (binlist(i) == bin(1)) {
	    temp(j)=data[i];
	    j++;
	}
    }
    temp.set_size(j, true);
    return temp;
}

template<class T> inline
Vec<T> Vec<T>::right(int nr) const
{
    // it_assert1(nr<=datasize, "Vec<T>::right: index out of range");
    Vec<T> temp(nr);
    if (nr!=0) {
      copy_vector(nr, &data[datasize-nr], &temp[0]);
    }
    return temp;
}

template<class T> inline
Vec<T> Vec<T>::left(int nr) const
{
    // it_assert1(nr<=datasize, "Vec<T>::left: index out of range");
    Vec<T> temp(nr);
    if (nr!=0) {
      copy_vector(nr, &data[0], &temp[0]);
    }
    return temp;
}

template<class T> inline
Vec<T> Vec<T>::mid(int start, int nr) const
{
    // it_assert1((start>=0)&& ((start+nr)<=datasize), "Vec<T>::mid: indexing out of range");
    Vec<T> temp(nr);

    if (nr!=0) {
      copy_vector(nr, &data[start], &temp[0]);
    }
    return temp;
}

template<class T>
Vec<T> Vec<T>::split(int Position)
{
    // it_assert1((Position>=0) && (Position<=datasize), "Vec<T>::split: index out of range");
    Vec<T> Temp1(Position);
    Vec<T> Temp2(datasize-Position);
    int	 i;

    for (i=0;i<Position;i++) {
	Temp1[i]=data[i];
    }
    for (i=Position;i<datasize;i++) {
	Temp2[i-Position]=data[i];
    }
    (*this)=Temp2;
    return Temp1;
}

template<class T>
void Vec<T>::shift_right(T In, int n)
{
    int i=datasize;

    // it_assert1(n>=0, "Vec<T>::shift_right: index out of range");
    while (--i >= n)
	data[i] = data[i-n];
    while (i >= 0)
	data[i--] = In;
}

template<class T>
void Vec<T>::shift_right(const Vec<T> &In)
{
    int	i;

    for (i=datasize-1; i>=In.datasize; i--)
	data[i]=data[i-In.datasize];
    for (i=0; i<In.datasize; i++)
	data[i]=In[i];
}

template<class T>
void Vec<T>::shift_left(T In, int n)
{
    int i;

    // it_assert1(n>=0, "Vec<T>::shift_left: index out of range");
    for (i=0; i<datasize-n; i++) 
	data[i] = data[i+n];
    while (i < datasize)
	data[i++] = In;
}

template<class T>
void Vec<T>::shift_left(const Vec<T> &In)
{
    int	i;

    for (i=0; i<datasize-In.datasize; i++)
	data[i]=data[i+In.datasize];
    for (i=datasize-In.datasize; i<datasize; i++)
	data[i]=In[i-datasize+In.datasize];
}

template<class T>
Vec<T> concat(const Vec<T> &v, const T a)
{
    Vec<T> temp(v.size()+1);

    for (int i=0; i<v.size(); i++)
	temp(i) = v(i);
    temp(v.size()) = a;

    return temp;
}

template<class T>
Vec<T> concat(const T a, const Vec<T> &v)
{
    Vec<T> temp(v.size()+1);

    temp(0) = a;

    for (int i=0; i<v.size(); i++)
	temp(i+1) = v(i);

    return temp;
}

template<class T>
Vec<T> concat(const Vec<T> &v1, const Vec<T> &v2)
{
    int i;
    Vec<T> temp(v1.size()+v2.size());

    for (i=0;i<v1.size();i++) {
	temp[i] = v1[i];
    }
    for (i=0;i<v2.size();i++) {
	temp[v1.size()+i] = v2[i];
    }
    return temp;
}

template<class T>
Vec<T> concat(const Vec<T> &v1, const Vec<T> &v2, const Vec<T> &v3)
{
  // There should be some error control?
    int i;
    Vec<T> temp(v1.size()+v2.size()+v3.size());

    for (i=0;i<v1.size();i++) {
	temp[i] = v1[i];
    }
    for (i=0;i<v2.size();i++) {
	temp[v1.size()+i] = v2[i];
    }
    for (i=0;i<v3.size();i++) {
	temp[v1.size()+v2.size()+i] = v3[i];
    }
    return temp;
}

template<class T>
Vec<T> concat(const Vec<T> &v1, const Vec<T> &v2, const Vec<T> &v3, const Vec<T> &v4)
{
  // There should be some error control?
    int i;
    Vec<T> temp(v1.size()+v2.size()+v3.size()+v4.size());

    for (i=0;i<v1.size();i++) {
	temp[i] = v1[i];
    }
    for (i=0;i<v2.size();i++) {
	temp[v1.size()+i] = v2[i];
    }
    for (i=0;i<v3.size();i++) {
	temp[v1.size()+v2.size()+i] = v3[i];
    }
    for (i=0;i<v4.size();i++) {
	temp[v1.size()+v2.size()+v3.size()+i] = v4[i];
    }
    return temp;
}

template<class T>
Vec<T> concat(const Vec<T> &v1, const Vec<T> &v2, const Vec<T> &v3, const Vec<T> &v4, const Vec<T> &v5)
{
  // There should be some error control?
    int i;
    Vec<T> temp(v1.size()+v2.size()+v3.size()+v4.size()+v5.size());

    for (i=0;i<v1.size();i++) {
	temp[i] = v1[i];
    }
    for (i=0;i<v2.size();i++) {
	temp[v1.size()+i] = v2[i];
    }
    for (i=0;i<v3.size();i++) {
	temp[v1.size()+v2.size()+i] = v3[i];
    }
    for (i=0;i<v4.size();i++) {
	temp[v1.size()+v2.size()+v3.size()+i] = v4[i];
    }
    for (i=0;i<v5.size();i++) {
	temp[v1.size()+v2.size()+v3.size()+v4.size()+i] = v5[i];
    }
    return temp;
}

template<class T> inline
void Vec<T>::set_subvector(int i1, int i2, const Vec<T> &v)
{
  if (i1 == -1)	i1 = datasize-1;
  if (i2 == -1) i2 = datasize-1;
  
  // it_assert1(i1>=0 && i2>=0 && i1<datasize && i2<datasize, "Vec<T>::set_subvector(): indicies out of range");
  // it_assert1(i2>=i1, "Vec<T>::set_subvector(): i2 >= i1 necessary");
  // it_assert1(i2-i1+1 == v.datasize, "Vec<T>::set_subvector(): wrong sizes");
  
  copy_vector(v.datasize, v.data, data+i1);
}

template<class T>
void Vec<T>::set_subvector(int i1, int i2, const T t)
{
  if (i1 == -1)	i1 = datasize-1;
  if (i2 == -1) i2 = datasize-1;
  
  // it_assert1(i1>=0 && i2>=0 && i1<datasize && i2<datasize, "Vec<T>::set_subvector(): indicies out of range");
  // it_assert1(i2>=i1, "Vec<T>::set_subvector(): i2 >= i1 necessary");

  for (int i=i1;i<=i2;i++)
    data[i] = t;
}

template<class T>
void Vec<T>::replace_mid(int pos, const Vec<T> &v)
{
    // it_assert1((pos>=0) && ((pos+v.length())<=datasize), "Vec<T>::replace_mid: indexing out of range");
    copy_vector(v.datasize, v.data, &data[pos]);
}

template<class T>
void Vec<T>::del(int index)
{
    // it_assert1((index>=0) && (index<datasize), "Vec<T>::del: index out of range");
    Vec<T> Temp(*this);
    int i;

    set_size(datasize-1, false);
    for (i=0;i<index;i++) {
	data[i]=Temp[i];
    }
    for (i=index;i<datasize;i++) {
	data[i]=Temp[i+1];
    }
}

template<class T>
void Vec<T>::ins(int index, T in)
{
    // it_assert1((index>=0) && (index<datasize), "Vec<T>::ins: index out of range");
    Vec<T> Temp(*this);

    set_size(datasize+1, false);

    copy_vector(index, Temp.data, data);
    data[index]=in;
    copy_vector(Temp.datasize-index, &Temp[index], &data[index+1]);
}

template<class T>
void Vec<T>::ins(int index, const Vec<T> &in)
{
    // it_assert1((index>=0) && (index<datasize), "Vec<T>::ins: index out of range");
    Vec<T> Temp(*this);

    set_size(datasize+in.length(), false);
    copy_vector(index, Temp.data, data);
    copy_vector(in.size(), in.data, &data[index]);
    copy_vector(Temp.datasize-index, &Temp[index], &data[index+in.size()]);
}

template<class T> inline
void Vec<T>::operator=(const Vec<T> &v)
{
  set_size(v.datasize, false);
  copy_vector(datasize, v.data, data);
}

template<class T> inline
void Vec<T>::operator=(const Mat<T> &m)
{
    // it_assert1( (m.cols() == 1 && datasize == m.rows()) ||
	//		(m.rows() == 1 && datasize == m.cols()), "vec::op=(mat); wrong size");

    if (m.cols() == 1) {
      set_size(m.rows(), false);
      copy_vector(m.rows(), m._data(), data);
    } else if (m.rows() == 1) {
      set_size(m.cols(), false);
      copy_vector(m.cols(), m._data(), m.rows(), data, 1);
    }
	//	else it_error("vec::op=(mat); wrong size");
}

// Elementwise comparisons with a scalar T

template<class T>
bvec Vec<T>::operator==(const T value)
{
    // it_assert(datasize > 0, "Vec<T>::operator==: vector must have size > 0");
    Vec<T> invector(*this);
    bvec temp(invector.length());

    for (int i=0;i<invector.length();i++)
	temp(i)=(invector(i)==value);

    return temp;
}

template<class T>
bvec Vec<T>::operator!=(const T value)
{
    // it_assert(datasize > 0, "Vec<T>::operator!=: vector must have size > 0");
    Vec<T> invector(*this);
    bvec temp(invector.length());

    for (int i=0;i<invector.length();i++)
	temp(i)=(invector(i)!=value);

    return temp;
}

template<class T>
bvec Vec<T>::operator<(const T value)
{
    // it_assert(datasize > 0, "Vec<T>::operator<: vector must have size > 0");
    Vec<T> invector(*this);
    bvec temp(invector.length());

    for (int i=0;i<invector.length();i++)
	temp(i)=(invector(i)<value);

    return temp;
}

template<class T>
bvec Vec<T>::operator<=(const T value)
{
    // it_assert(datasize > 0, "Vec<T>::operator<=: vector must have size > 0");
    Vec<T> invector(*this);
    bvec temp(invector.length());

    for (int i=0;i<invector.length();i++)
	temp(i)=(invector(i)<=value);

    return temp;
}

template<class T>
bvec Vec<T>::operator>(const T value)
{
    // it_assert(datasize > 0, "Vec<T>::operator>: vector must have size > 0");
    Vec<T> invector(*this);
    bvec temp(invector.length());

    for (int i=0;i<invector.length();i++)
	temp(i)=(invector(i)>value);

    return temp;
}

template<class T>
bvec Vec<T>::operator>=(const T value)
{
    // it_assert(datasize > 0, "Vec<T>::operator>=: vector must have size > 0");
    Vec<T> invector(*this);
    bvec temp(invector.length());

    for (int i=0;i<invector.length();i++)
	temp(i)=(invector(i)>=value);

    return temp;
}

template<class T>
bool Vec<T>::operator==(const Vec<T> &invector) const
{
// OBS ! if wrong size, return false
    if (datasize!=invector.datasize) return false;
    for (int i=0;i<datasize;i++) {
	if (data[i]!=invector.data[i]) return false;
    }
    return true;
}

template<class T>
bool Vec<T>::operator!=(const Vec<T> &invector) const
{
    if (datasize!=invector.datasize) return true;
    for (int i=0;i<datasize;i++) {
	if (data[i]!=invector.data[i]) return true;
    }
    return false;
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS

template <class T>
ostream &operator<<(ostream& os, const Vec<T> &v)
{
    int i, sz=v.length();
  
    os << "[" ;
    for (i=0; i<sz; i++) {
	os << v(i);
	if (i < sz-1)
	    os << " ";
    }
    os << "]" ;

    return os;
}

template <class T>
istream &operator>>(istream& is, Vec<T> &v)
{
    string str;

    getline(is, str);
    if (is.eof())
	v.set_size(0, false);
    else
	v.set(str);

    return is;
}

#endif //DOXYGEN_SHOULD_SKIP_THIS


#ifndef DOXYGEN_SHOULD_SKIP_THIS

template<class T> void copy_vector(const int n, const T *x, T *y);
template<class T> void copy_vector(const int n, const T *x, const int incx, T *y, const int incy);
template<class T> void swap_vector(const int n, T *x, T *y);
template<class T> void swap_vector(const int n, T *x, const int incx, T *y, const int incy);

/*
  Copy vector x to vector y. Both vectors are of size n
*/
#ifdef HAVE_CBLAS
template<> inline
void copy_vector(const int n, const double *x, double *y)
{
  cblas_dcopy(n, x, 1, y, 1);	
}

template<> inline
void copy_vector(const int n, const double_complex *x, double_complex *y)
{
  cblas_zcopy(n, x, 1, y, 1);		
}
#endif

template<class T> inline
void copy_vector(const int n, const T *x, T *y)
{
  memcpy(y, x, (unsigned int)n*sizeof(T));
}

/*
  Copy vector x to vector y. Both vectors are of size n
  vector x elements are stored linearly with element increament incx
  vector y elements are stored linearly with element increament incx
*/
#ifdef HAVE_CBLAS
template<> inline
void copy_vector(const int n, const double *x, const int incx, double *y, const int incy)
{
  cblas_dcopy(n, x, incx, y, incy);	
}

template<> inline
void copy_vector(const int n, const double_complex *x, const int incx, double_complex *y, const int incy)
{
  cblas_zcopy(n, x, incx, y, incy);		
}
#endif

template<class T> inline
void copy_vector(const int n, const T *x, const int incx, T *y, const int incy)
{
  for (int i=0;i<n; i++)
    y[i*incy] = x[i*incx];
}

/*
  Swap vector x to vector y. Both vectors are of size n
*/

#ifdef HAVE_CBLAS
template<> inline
void swap_vector(const int n, double *x, double *y)
{
  cblas_dswap(n, x, 1, y, 1);	
}

template<> inline
void swap_vector(const int n, double_complex *x, double_complex *y)
{
  cblas_zswap(n, x, 1, y, 1);		
}
#endif

template<class T> inline
void swap_vector(const int n, T *x, T *y)
{
  for (int i=0; i<n; i++)
     swap(x[i], y[i]);
}

/*
  Swap vector x to vector y. Both vectors are of size n
  vector x elements are stored linearly with element increament incx
  vector y elements are stored linearly with element increament incx
*/
#ifdef HAVE_CBLAS
template<> inline
void swap_vector(const int n, double *x, const int incx, double *y, const int incy)
{
  cblas_dswap(n, x, incx, y, incy);	
}

template<> inline
void swap_vector(const int n, double_complex *x, const int incx, double_complex *y, const int incy)
{
  cblas_zswap(n, x, incx, y, incy);		
}
#endif

template<class T> inline
void swap_vector(const int n, T *x, const int incx, T *y, const int incy)
{
	//  void swap(int *a, int *b);
  for (int i=0; i<n; i++)
	  swap(x[i*incx], y[i*incy]);
}

#endif /* DOXYGEN_SHOULD_SKIP_THIS */
} //
#endif

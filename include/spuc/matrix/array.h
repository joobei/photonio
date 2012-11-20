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
  \brief Array class (container)
  \author Tobias Ringstrom
  
  This file is not separated into a .h and a .cpp file. The reason is to avoid problems with template initializations of this class.
  An \c Array<type> can contain any type and it is not possible to initialize and pre-compile all types that might be put into an \c Array.
  
  1.10
  
  2003/01/04 00:21:53
*/

#ifndef __array_h
#define __array_h

#include <spucconfig.h>
//#include <spucassert.h>
#include <iostream>
namespace SPUC {
/*! 
  \brief General array class

  This class is a general linear array class for arbitrary types. The operations
  and functions are the same as for the vector Vec class (except for the arithmetics).
  
  For rarely used types you will need to instantiate the class by
  \code
  template class Array<type>;
  \endcode

  The following example shows how to define an Array of vectors:
  \code
  vec a = randn(10);
  vec b = randn(20);
  vec c = randn(30);
  Array<vec> my_array(3);
  my_array(0) = a;
  my_array(1) = b;
  my_array(2) = c;
  \endcode
*/
template<class T>
class Array {
 public:
  //! Default constructor
  Array();
  //! Create an Array of size \c n
  Array(int n);
  //! Create a copy of \c a
  Array(const Array<T> &a);
  //! Default destructor
  virtual ~Array();
	
  //! Get the \c i element
  T &operator()(int i) {
//    it_assert0(i>=0&&i<ndata,"Array::operator()"); 
	  return data[i]; }
  //! Get the \c i element
  T operator()(int i) const {
    //it_assert0(i>=0&&i<ndata,"Array::operator()");
	  return data[i]; }
  //! Sub-array from element \c i1 to element \c i2
  Array<T> operator()(int i1, int i2) const;
  //! Sub-array with the elements given by the integer Array
  Array<T> operator()(const Array<int> &indices) const;
  
  //! Assignment operator
  void operator=(T e);
  //! Assignment operator
  void operator=(const Array<T> &a);
	
  //! Append element \c e to the end of the Array \c a
  //  friend Array<T> concat TEMPLATE_FUN(const Array<T> &a1, const T e);
  //! Concat element \c e to the beginning of the Array \c a
  //  friend Array<T> concat TEMPLATE_FUN(const T e, const Array<T> &a);
  //! Concat Arrays \c a1 and \c a2
  //  friend Array<T> concat TEMPLATE_FUN(const Array<T> &a1,const Array<T> &a2);
  //! Concat Arrays \c a1, \c a2 and \c a3
  //  friend Array<T> concat TEMPLATE_FUN(const Array<T> &a1, const Array<T> &a2, const Array<T> &a3);

  //! Returns the number of data elements in the array object
  int size() const { return ndata; }
  //! Returns the number of data elements in the array object
  int length() const { return ndata; }
  //! Resizing an Array<T>.
  void set_size(int n, bool copy=false);
  //! Resizing an Array<T>.
  void set_length(int n, bool copy=false) { set_size(n, copy); }

  //! Shift in data at position 0. return data at last position.
  T shift_right(T e);
  //! Shift in array at position 0. return data at last position.
  Array<T> shift_right(const Array<T> &a);
  //! Shift in data at position Ndata()-1. return data at last position.
  T shift_left(T e);
  //! Shift in array at position Ndata()-1. return data at last position.
  Array<T> shift_left(const Array<T> &a);
  //! Swap elements i and j.
  void swap(int i, int j);

  //! Set the subarray defined by indicies i1 to i2 to Array<T> a.
  void set_subarray(int i1, int i2, const Array<T> &a);
  //! Set the subarray defined by indicies i1 to i2 the element value t.
  void set_subarray(int i1, int i2, const T t);

//protected:
 private:
  bool in_range(int i) { return ((i<ndata) && (i>=0)); }
  int ndata;
  T *data;

 private:
  void alloc(int n);
  void free();
};

// --------------------------- Implementation starts here ----------------------------------

template<class T>
Array<T>::Array()
{
    data = 0;
    ndata = 0;
}

template<class T>
Array<T>::Array(int n)
{
    alloc(n);
}

template<class T>
Array<T>::Array(const Array<T> &a)
{

  data=NULL;
  ndata=0;
  alloc(a.ndata);
  for (int i=0; i<a.ndata; i++)
    data[i] = a.data[i];
}

template<class T>
Array<T>::~Array()
{
  free();
}

template<class T>
void Array<T>::alloc(int n)
{
  if (n == 0) {
    data = NULL;
    ndata = 0;
  }
  else {
    data = new T[n];
//    it_assert1(data!=0, "Out of memory in Array::alloc");
  }
  ndata = n;
}

template<class T>
void Array<T>::free()
{

  delete [] data;
	
  data = 0;
  ndata = 0;
}

template<class T>
Array<T> Array<T>::operator()(int i1, int i2) const
{
	//    it_assert0(i1>=0 && i2>=0 && i1<ndata && i2<ndata && i2>=i1, "Array::operator()(i1,i2)");
    Array<T> s(i2-i1+1);
    int i;
	
    for (i=0; i<s.ndata; i++)
	s.data[i] = data[i1+i];
	
    return s;
}

template<class T>
Array<T> Array<T>::operator()(const Array<int> &indices) const
{
    Array<T> a(indices.size());

    for (int i=0; i<a.size(); i++) {
		//	it_assert0(indices(i)>=0&&indices(i)<ndata,"Array::operator()(indicies)");
	a(i) = data[indices(i)];
    }

    return a;
}

template<class T>
void Array<T>::operator=(const Array<T> &a)
{
    set_size(a.ndata);
    for (int i=0; i<ndata; i++)
	data[i] = a.data[i];
}


template<class T>
void Array<T>::operator=(T e)
{
    for (int i=0; i<ndata; i++)
	data[i] = e;
}


template<class T>
void Array<T>::set_size(int sz, bool copy)
{
    int i, min;
    T *tmp;

    if (ndata == sz)
      return;
		
    if (copy) {
      tmp = data;
      min = ndata < sz ? ndata : sz;
      alloc(sz);
      for (i=0; i<min; i++)
	data[i] = tmp[i];
      delete [] tmp;
    } else {
      free();
      alloc(sz);
    }
    ndata = sz;
}

template<class T>
T Array<T>::shift_right(T x)
{
    T ret;

	//    it_assert1(ndata>0, "shift_right");
    ret = data[ndata-1];
    for (int i=ndata-1; i>0; i--)
	data[i] = data[i-1];
    data[0] = x;
	
    return ret;
}


template<class T>
Array<T> Array<T>::shift_right(const Array<T> &a)
{
    int	i;
    Array<T> out(a.ndata);

    it_assert1(a.ndata<=ndata, "Shift Array too large");
    for (i=0; i<a.ndata; i++)
	out.data[i] = data[ndata-a.ndata+i];
    for (i=ndata-1; i>=a.ndata; i--)
	data[i] = data[i-a.ndata];
    for (i=0; i<a.ndata; i++)
	data[i] = a.data[i];
	
    return out;
}

template<class T>
T Array<T>::shift_left(T x)
{
    T temp = data[0];
	
    for (int i=0; i<ndata-1; i++)
	data[i]=data[i+1];
    data[ndata-1] = x;
	
    return temp;
}

template<class T>
Array<T> Array<T>::shift_left(const Array<T> &a)
{
    int	i;
    Array<T> out(a.ndata);

    it_assert1(a.ndata<=ndata, "Shift Array too large");
    for (i=0; i<a.ndata; i++)
	out.data[i] = data[i];
    for (i=0; i<ndata-a.ndata; i++) {
      // out.data[i] = data[i]; removed. Is not necessary
	data[i] = data[i+a.ndata];
    }
    for (i=ndata-a.ndata; i<ndata; i++)
	data[i] = a.data[i-ndata+a.ndata];
	
    return out;
}

template<class T>
void Array<T>::swap(int i, int j)
{
    it_assert1(in_range(i) && in_range(j) , "Shift Array too large");
    
    T temp = data[i];
    data[i] = data[j];
    data[j] = temp;
}

template<class T>
void Array<T>::set_subarray(int i1, int i2, const Array<T> &a)
{
  if (i1 == -1)	i1 = ndata-1;
  if (i2 == -1) i2 = ndata-1;
  
  //  it_assert1(in_range(i1) && in_range(i2), "Array<T>::set_subarray(): indicies out of range");
  //  it_assert1(i2>=i1, "Array<T>::set_subarray(): i2 >= i1 necessary");
  //  it_assert1(i2-i1+1 == a.ndata, "Array<T>::set_subarray(): wrong sizes");

  memcpy(data+i1, a.data, a.ndata*sizeof(T));
}

template<class T>
void Array<T>::set_subarray(int i1, int i2, const T t)
{
  if (i1 == -1)	i1 = ndata-1;
  if (i2 == -1) i2 = ndata-1;
  
  //  it_assert1(in_range(i1) && in_range(i2), "Array<T>::set_subarray(): indicies out of range");
  //  it_assert1(i2>=i1, "Array<T>::set_subarray(): i2 >= i1 necessary");

  for (int i=i1;i<=i2;i++)
    data[i] = t;
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS 
//Doxygen warnings on these functions

template<class T>
Array<T> concat(const Array<T> &a, const T e)
{
    Array<T> temp(a.size()+1);

    for (int i=0; i<a.size(); i++)
	temp(i) = a(i);
    temp(a.size()) = e;

    return temp;
}

template<class T>
Array<T> concat(const T e, const Array<T> &a)
{
    Array<T> temp(a.size()+1);

    temp(0) = e;

    for (int i=0; i<a.size(); i++)
	temp(i+1) = a(i);

    return temp;
}

template<class T>
Array<T> concat(const Array<T> &a1, const Array<T> &a2)
{
    int i;
    Array<T> temp(a1.size()+a2.size());

    for (i=0;i<a1.size();i++) {
	temp(i) = a1(i);
    }
    for (i=0;i<a2.size();i++) {
	temp(a1.size()+i) = a2(i);
    }
    return temp;
}

template<class T>
Array<T> concat(const Array<T> &a1, const Array<T> &a2, const Array<T> &a3)
{
  // There should be some error control?
    int i;
    Array<T> temp(a1.size()+a2.size()+a3.size());

    for (i=0;i<a1.size();i++) {
	temp(i) = a1(i);
    }
    for (i=0;i<a2.size();i++) {
	temp(a1.size()+i) = a2(i);
    }
    for (i=0;i<a3.size();i++) {
	temp(a1.size()+a2.size()+i) = a3(i);
    }
    return temp;
}

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

/*! 
  \relates Array
  \brief Output stream for the Array class
*/
#ifdef XXX
template<class T>
inline std::ostream &operator<<(std::ostream &o, const Array<T> &a)
{
  o << "{";
    for (int i=0; i<a.size()-1; i++)
	o << a(i) << " ";
    if (a.size() > 0)
	o << a(a.size()-1);

    o << "}";

    return o;
}
#endif
} // namespace SPUC 

#endif // __array_h

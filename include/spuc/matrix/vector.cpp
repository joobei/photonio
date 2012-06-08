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
  \brief Templated Vector Class Implementation
  \author Tony Ottosson and Tobias Ringström

  1.20

  2003/01/08 21:59:46
*/
#include "vector.h"
namespace SPUC {
//using namespace std;
//using std::istringstream;

/* 
   This piece of code generates the Doxygen warnings like 
   "no matching class member found". However, the code is OK :-) 
*/

template<>
bool cvec::set(const char *values)//DOXYGEN WARNING
{
    cout << "cvec::Vec(const char *values)  Not implemented" << endl;
    cout << "values = " << values << endl;

    return false;
}
#ifdef XXX
template<>
bool bvec::set(const char *values)//DOXYGEN WARNING
{
  istringstream buffer(values);		
  int pos=0,maxpos=10;
  short intemp;
  
  alloc(maxpos); 
  
  while(buffer.peek()!=EOF) {
    if (buffer.peek()==',') { 
      buffer.get(); 
    } else {
      pos++;
      if (pos > maxpos) {
	maxpos=maxpos*2;
	set_size(maxpos, true);
      }			
      buffer >> intemp;
      data[pos-1]=intemp;
    }
  }
  set_size(pos, true);
  
  return true;
}
#endif
#ifdef _MSC_VER

template<>
bool llvec::set(const char *values)
{
//    // it_error("set() does not work for llvec");
    return false;
}
#endif /* _MSC_VER */


#ifdef HAVE_CBLAS  // double and complex specializations using CBLAS
#ifndef _MSC_VER
template<>
#endif
double dot(const vec &v1, const vec &v2)
{
  // it_assert1(v1.datasize==v2.datasize, "vec::dot: wrong sizes");
  double r=0.0;

  r= cblas_ddot(v1.datasize, v1.data, 1, v2.data, 1);

  return r;
}
#ifndef _MSC_VER
template<>
#endif
double dot_prod(const vec &v1, const vec &v2)
{
  // it_assert1(v1.datasize==v2.datasize, "vec::dot: wrong sizes");
  double r=0.0;

  r= cblas_ddot(v1.datasize, v1.data, 1, v2.data, 1);

  return r;
}

#ifndef _MSC_VER
template<>
#endif
double_complex dot(const cvec &v1, const cvec &v2)
{
  // it_assert1(v1.datasize==v2.datasize, "cvec::dot: wrong sizes");
  double_complex r=0.0;

  cblas_zdotu_sub(v1.datasize, v1.data, 1, v2.data, 1, &r);

  return r;
}
#endif


// Elementwise comparisons with a scalar T

/* This piece of code generates the Doxygen warnings like "no matching class member found". However, the code is OK :-) */

//DOGYGEN WARNINGS ON FOLLOWING 6 LINES!!!!!!!
#ifdef ITXXX
template<> bvec cvec::operator==(const double_complex) { // it_error("operator==: not implemented for complex");
	bvec temp;return temp;}
template<> bvec cvec::operator!=(const double_complex) { // it_error("operator!=: not implemented for complex");
	bvec temp;return temp;}
template<> bvec cvec::operator>(const double_complex)  { // it_error("operator>: not implemented for complex");
	bvec temp;return temp;}
template<> bvec cvec::operator>=(const double_complex) { // it_error("operator>=: not implemented for complex");
	bvec temp;return temp;}
template<> bvec cvec::operator<(const double_complex)  { // it_error("operator<: not implemented for complex");
	bvec temp;return temp;}
template<> bvec cvec::operator<=(const double_complex) { // it_error("operator<=: not implemented for complex");
	bvec temp;return temp;}
#endif



#ifdef _MSC_VER
#ifdef XXX
template <>
ostream &operator<<(ostream& os, const llvec &v)
{
    // it_error("operator<< does not work for llvec (MS Visual C++ only)");

    return os;
}

template <>
ostream &operator<<(ostream& os, const cvec &v)
{
    // it_error("operator<< does not work for cvec (MS Visual C++ only)");
	
    return os;
}
#endif
#endif
}
using namespace SPUC;
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

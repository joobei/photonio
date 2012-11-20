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
  \brief Matrix class implementation
  \author Tony Ottosson and Tobias Ringström

  1.23

  2002/12/19 23:56:45
*/

#include <complex.h>
#include "matrix.h"
using namespace SPUC;
// ---------------------- Instantiations --------------------------------
#ifdef TMPxINST
//------- class instantiations --------
//! Template instantiation of Mat<double>
template class Mat<double>;
//! Template instantiation of Mat<complex<double>>
template class Mat<complex<double>>;
//! Template instantiation of Mat<int>
template class Mat<int>;
//! Template instantiation of Mat<short int>
template class Mat<short int>;

#ifdef _MSC_VER
#pragma warning(disable : 4660)
#endif
//! Template instantiation of Mat<long_long>
template class Mat<long_long>;
//! Template instantiation of Mat<bin>
template class Mat<bin>;
#ifdef _MSC_VER
#pragma warning(default : 4660)
#endif
#endif

#ifdef _MSC_VER
#define TMP_INST
#endif
#ifdef  TMP_INST
//-------------------- Operator instantiations --------------------
//-------- Addition operators ---------------

//! Template instantiation of operator+
template mat operator+(const mat &m1, const mat &m2);
//! Template instantiation of operator+
template cmat operator+(const cmat &m1, const cmat &m2);
//! Template instantiation of operator+
template imat operator+(const imat &m1, const imat &m2);
//! Template instantiation of operator+
template smat operator+(const smat &m1, const smat &m2);
//! Template instantiation of operator+
template bmat operator+(const bmat &m1, const bmat &m2);

//! Template instantiation of operator+
template mat operator+(const mat &m, double t);
//! Template instantiation of operator+
template cmat operator+(const cmat &m, complex<double> t);
//! Template instantiation of operator+
template imat operator+(const imat &m, int t);
//! Template instantiation of operator+
template smat operator+(const smat &m, short t);
//! Template instantiation of operator+
template bmat operator+(const bmat &m, bin t);

//! Template instantiation of operator+
template mat operator+(double t, const mat &m);
//! Template instantiation of operator+
template cmat operator+(complex<double> t, const cmat &m);
//! Template instantiation of operator+
template imat operator+(int t, const imat &m);
//! Template instantiation of operator+
template smat operator+(short t, const smat &m);
//! Template instantiation of operator+
template bmat operator+(bin t, const bmat &m);

//-------- Subraction operators ---------------
//! Template instantiation of operator-
template mat operator-(const mat &m1, const mat &m2);
//! Template instantiation of operator-
template cmat operator-(const cmat &m1, const cmat &m2);
//! Template instantiation of operator-
template imat operator-(const imat &m1, const imat &m2);
//! Template instantiation of operator-
template smat operator-(const smat &m1, const smat &m2);
//! Template instantiation of operator-
template bmat operator-(const bmat &m1, const bmat &m2);

//! Template instantiation of operator-
template mat operator-(const mat &m, double t);
//! Template instantiation of operator-
template cmat operator-(const cmat &m, complex<double> t);
//! Template instantiation of operator-
template imat operator-(const imat &m, int t);
//! Template instantiation of operator-
template smat operator-(const smat &m, short t);
//! Template instantiation of operator-
template bmat operator-(const bmat &m, bin t);

//! Template instantiation of operator-
template mat operator-(double t, const mat &m);
//! Template instantiation of operator-
template cmat operator-(complex<double> t, const cmat &m);
//! Template instantiation of operator-
template imat operator-(int t, const imat &m);
//! Template instantiation of operator-
template smat operator-(short t, const smat &m);
//! Template instantiation of operator-
template bmat operator-(bin t, const bmat &m);

//--------- Unary minus ---------------
//! Template instantiation of operator-
template mat operator-(const mat &m);
//! Template instantiation of operator-
template cmat operator-(const cmat &m);
//! Template instantiation of operator-
template imat operator-(const imat &m);
//! Template instantiation of operator-
template smat operator-(const smat &m);
//! Template instantiation of operator-
template bmat operator-(const bmat &m);

//-------- Multiplication operators ---------------
//! Template instantiation of operator*
template imat operator*(const imat &m1, const imat &m2);
//! Template instantiation of operator*
template smat operator*(const smat &m1, const smat &m2);
//! Template instantiation of operator*
template bmat operator*(const bmat &m1, const bmat &m2);

//! Template instantiation of operator*
template ivec operator*(const imat &m, const ivec &v);
//! Template instantiation of operator*
template svec operator*(const smat &m, const svec &v);
//! Template instantiation of operator*
template bvec operator*(const bmat &m, const bvec &v);

//! Template instantiation of operator*
template ivec operator*(const ivec &v, const imat &m);
//! Template instantiation of operator*
template svec operator*(const svec &v, const smat &m);
//! Template instantiation of operator*
template bvec operator*(const bvec &v, const bmat &m);

//! Template instantiation of operator*
template mat operator*(const mat &m, double t);
//! Template instantiation of operator*
template cmat operator*(const cmat &m, complex<double> t);
//! Template instantiation of operator*
template imat operator*(const imat &m, int t);
//! Template instantiation of operator*
template smat operator*(const smat &m, short t);
//! Template instantiation of operator*
template bmat operator*(const bmat &m, bin t);

//! Template instantiation of operator*
template mat operator*(double t, const mat &m);
//! Template instantiation of operator*
template cmat operator*(complex<double> t, const cmat &m);
//! Template instantiation of operator*
template imat operator*(int t, const imat &m);
//! Template instantiation of operator*
template smat operator*(short t, const smat &m);
//! Template instantiation of operator*
template bmat operator*(bin t, const bmat &m);

// ------------ Elementwise multiplication -----------
//! Template instantiation of elem_mult
template mat elem_mult(const mat &m1, const mat &m2);
//! Template instantiation of elem_mult
template cmat elem_mult(const cmat &m1, const cmat &m2);
//! Template instantiation of elem_mult
template imat elem_mult(const imat &m1, const imat &m2);
//! Template instantiation of elem_mult
template smat elem_mult(const smat &m1, const smat &m2);
//! Template instantiation of elem_mult
template bmat elem_mult(const bmat &m1, const bmat &m2);


// ------------ Division operator -----------
//! Template instantiation of operator/
template mat operator/(const mat &m, double t);
//! Template instantiation of operator/
template cmat operator/(const cmat &m, complex<double> t);
//! Template instantiation of operator/
template imat operator/(const imat &m, int t);
//! Template instantiation of operator/
template smat operator/(const smat &m, short t);
//! Template instantiation of operator/
template bmat operator/(const bmat &m, bin t);

// ------------ Elementwise division -----------
//! Template instantiation of elem_div
template mat elem_div(const mat &m1, const mat &m2);
//! Template instantiation of elem_div
template cmat elem_div(const cmat &m1, const cmat &m2);
//! Template instantiation of elem_div
template imat elem_div(const imat &m1, const imat &m2);
//! Template instantiation of elem_div
template smat elem_div(const smat &m1, const smat &m2);
//! Template instantiation of elem_div
template bmat elem_div(const bmat &m1, const bmat &m2);

// ------------- Concatenations -----------------
//! Template instantiation of concat_horizontal
template mat concat_horizontal(const mat &m1, const mat &m2);
//! Template instantiation of concat_horizontal
template cmat concat_horizontal(const cmat &m1, const cmat &m2);
//! Template instantiation of concat_horizontal
template imat concat_horizontal(const imat &m1, const imat &m2);
//! Template instantiation of concat_horizontal
template smat concat_horizontal(const smat &m1, const smat &m2);
//! Template instantiation of concat_horizontal
template bmat concat_horizontal(const bmat &m1, const bmat &m2);

//! Template instantiation of concat_vertical
template mat concat_vertical(const mat &m1, const mat &m2);
//! Template instantiation of concat_vertical
template cmat concat_vertical(const cmat &m1, const cmat &m2);
//! Template instantiation of concat_vertical
template imat concat_vertical(const imat &m1, const imat &m2);
//! Template instantiation of concat_vertical
template smat concat_vertical(const smat &m1, const smat &m2);
//! Template instantiation of concat_vertical
template bmat concat_vertical(const bmat &m1, const bmat &m2);


//----------- Output stream --------------
//! Template instantiation of output stream
template ostream &operator<<(ostream &os, const mat  &m);
//! Template instantiation of output stream
template ostream &operator<<(ostream &os, const cmat &m);
//! Template instantiation of output stream
template ostream &operator<<(ostream &os, const imat  &m);
//! Template instantiation of output stream
template ostream &operator<<(ostream &os, const smat  &m);
//! Template instantiation of output stream
template ostream &operator<<(ostream &os, const bmat  &m);

//----------- Input stream --------------
//! Template instantiation of input stream
template istream &operator>>(istream &is, mat  &m);
//! Template instantiation of input stream
template istream &operator>>(istream &is, cmat &m);
//! Template instantiation of input stream
template istream &operator>>(istream &is, imat  &m);
//! Template instantiation of input stream
template istream &operator>>(istream &is, smat  &m);
//! Template instantiation of input stream
template istream &operator>>(istream &is, bmat  &m);


// ---------- CBLAS optimized Instantiations (if available) ----------------------
#ifndef HAVE_CBLAS

template mat operator*(const mat &m1, const mat &m2);
template cmat operator*(const cmat &m1, const cmat &m2);

template vec operator*(const mat &m, const vec &v);
template cvec operator*(const cmat &m, const cvec &v);

template vec operator*(const vec &v, const mat &m);
template cvec operator*(const cvec &v, const cmat &m);

#endif
#endif

#ifndef DOXYGEN_SHOULD_SKIP_THIS



/*
  Return the mean value of the elements in the vector
*/
double mean(const vec &v)
{
    return sum(v)/v.length();
}

/*
  Return the mean value of the elements in the vector
*/
complex<double> mean(const cvec &v)
{
    return ((double)(1/v.length())*sum(v));
}

/*
  Return the mean value of the elements in the vector
*/
double mean(const svec &v)
{
    return (double)sum(v)/v.length();
}

/*
  Return the mean value of the elements in the vector
*/
double mean(const ivec &v)
{
    return (double)sum(v)/v.length();
}


/*
  Return the mean value of the elements in the matrix
*/
double mean(const mat &m)
{
    return sum(m)/(m.rows()*m.cols());
}

/*
  Return the mean value of the elements in the matrix
 */
complex<double> mean(const cmat &m)
{
//    return sum(m)/static_cast<complex<double>>(m.rows()*m.cols());
    return ((double)(1/(m.rows()*m.cols()))*sum(m));
}

/*
  Return the mean value of the elements in the matrix
*/
double mean(const smat &m)
{
    return static_cast<double>(sum(m))/(m.rows()*m.cols());
}

/*
  Return the mean value of the elements in the matrix
*/
double mean(const imat &m)
{
    return static_cast<double>(sum(m))/(m.rows()*m.cols());
}

// Annoying boring special cases (complex cvec & dcvec)
//template<>
double xxxvariance(const cvec &v)
{
    int len = v.length();
    double sq_sum=0.0;
    complex<double> sum=0.0;
    const complex<double> *p=v._data();

    for (int i=0; i<len; i++, p++) {
	sum += *p;
	sq_sum += magsq(*p);
    }
    
    return (double)(sq_sum - magsq(sum)/len) / (len-1);
}

#endif //DOXYGEN_SHOULD_SKIP_THIS


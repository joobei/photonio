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
#include "vector.h"
#include "matfunc.h"
#include "array1d.h"

namespace SPUC {

#ifndef DOXYGEN_SHOULD_SKIP_THIS

template<class T> T sum(const Array1D<T>& v)
{
    T M=0;
    for (int i=0;i<v.length();i++)
	M+=v[i];
    return M;
}

/* T sum_sqr(const Array1D<T> &v)
*/
template<class T>
T sum_sqr(const Array1D<T>& v)
{
    T M=0;
    
    for (int i=0; i<v.length(); i++)
	M += v[i] * v[i];
    
    return M;
}

template<class T>
T max(const Array1D<T>& in)
{
    T	maxdata=in[0];
    for (int i=1;i<in.length();i++)
	if (in[i]>maxdata)
	    maxdata=in[i];
    return maxdata;
}

template<class T>
T min(const Array1D<T> &in)
{
    T mindata=in[0];
    for (int i=1;i<in.length();i++)
	if (in[i]<mindata)
	    mindata=in[i];
    return mindata;
}

/*
  Return the postion of the maximum element in the Array1Dtor
*/
template<class T>
int max_index(const Array1D<T>& in)
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
int min_index(const Array1D<T>& in)
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
T product(const Array1D<T>& v)
{
    double lnM=0;
    for (int i=0;i<v.length();i++)
	lnM += log(static_cast<double>(v[i]));
    return static_cast<T>(exp(lnM));
}

/*
  Array1D cross product
*/
template<class T>
Array1D<T> cross(const Array1D<T>& v1, const Array1D<T>& v2)
{
    it_assert( v1.size() == 3 && v2.size() == 3, "cross: vectors should be of size 3");

    Array1D<T> r(3);

    r(0) = v1(1) * v2(2) - v1(2) * v2(1);
    r(1) = v1(2) * v2(0) - v1(0) * v2(2);
    r(2) = v1(0) * v2(1) - v1(1) * v2(0);
    
    return r;
}

/*
  Return the mean value of the elements in the vector
*/
double mean(const vec& v)
{
  return sum(v)/v.length();
}

/*
  Return the mean value of the elements in the vector
*/
complex<double> mean(const cvec& v)
{
    return sum(v)/static_cast<double>(v.length());
}


/*
  Return the mean value of the elements in the vector
*/
double mean(const ivec& v)
{
    return (double)sum(v)/v.length();
}

/*
  Return the geometric mean value of the elements in the vector
*/
template<class T>
double geometric_mean(const Array1D<T>& v)
{
    return exp(log(product(v))/v.length());
}

/*
  Return the median value of the elements in the vector
*/
template<class T>
double median(const Array1D<T>& v)
{
    Array1D<T> invect(v);
    sort(invect);
    return (double)(invect[(invect.length()-1)/2]+invect[invect.length()/2])/2.0;
}

/*
  Calculate the 2-norm: norm(v)=sqrt(sum(abs(v).^2))
*/
template<class T>
double norm(const Array1D<T>& v)
{
    int i;
    double e=0.0;
    
    for (i=0; i<v.length(); i++)
	e += sqr( v[i] );

    return sqrt(e);
}
/*
template<>
double norm(const cvec& v)
{
    int i;
    double E=0.0;
    
    for (i=0; i<v.length(); i++)
	E += std::norm(v[i]);

    return sqrt(E);
}
*/
/*
  Calculate the p-norm: norm(v,p)=sum(abs(v).^2)^(1/p)
*/
template<class T>
double norm(const Array1D<T>& v, int p)
{
    int i;
    double e=0;
    
    for (i=0;i<v.length();i++)
	  e+=pow(fabs(v[i]),(double)p); // fabs() shoud be OK

    return pow(e,1.0/(double)p);
}
/*
template<>
double norm(const cvec& v, int p)
{
    int i;
    double E=0;
    
    for (i=0;i<v.length();i++)
	E+=pow(std::norm(v[i]), p/2.0); // Yes, 2.0 is correct!

    return pow(E,1.0/(double)p);
}
*/
/*
  Return the variance of the elements in the vector. Normalized with N-1 to be unbiased.
 */
template<class T>
double variance(const Array1D<T>& v)
{
    int len = v.length();
    const T *p=v._data();
    double sum=0.0, sq_sum=0.0;

    for (int i=0; i<len; i++, p++) {
	sum += *p;
	sq_sum += *p * *p;
    }
    
    return (double)(sq_sum - sum*sum/len) / (len-1);
}
/*
// Annoying boring special cases (complex cvec&  dcvec)
template<>
double variance(const cvec& v)
{
    int len = v.length();
    double sq_sum=0.0;
    double_complex sum=0.0;
    const double_complex *p=v._data();

    for (int i=0; i<len; i++, p++) {
	sum += *p;
	sq_sum += std::norm(*p);
    }
    
    return (double)(sq_sum - std::norm(sum)/len) / (len-1);
}
*/
/*
  Calculate the energy: squared 2-norm. energy(v)=sum(v.^2)
*/
template<class T>
double energy(const Array1D<T>& v)
{
    return sqr(norm(v));
}

/* 
template<class T>
Array1D<T> normalize(Array1D<T>& v1, T radius) {
  dvec temp(v1.length());
  int i;
  T E=norm(v1),r;
  if (E!=0) {
    r=radius/E;
    for (i=0;i<v1.length();i++) {
      v1[i]*=r;
    }
  }
  return temp;
}
*/



/*
  Repeat each element in the vector norepeats times in sequence
*/
template<class T>
Array1D<T> repeat(const Array1D<T>& v, int norepeats)
{
    Array1D<T> temp(v.length()*norepeats);
  
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
Array1D<T> apply_function(fT (*f)(fT), const Array1D<T>& data)
{
    Array1D<T> out(data.length());
  
    for (int i=0;i<data.length();i++) 
	out[i]=static_cast<T>(f(static_cast<fT>(data[i])));
    return out;
}

template<class T> void QS(int low, int high, Array1D<T>& data) {
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
//template void QS(int low, int high, vec& data);

/*
  Sort the the vector in increasing order
*/
template<class T>
void sort(Array1D<T>& data)
{
    QS(0,data.size()-1,data);
}

template<class T>
void QSindex(int low, int high, ivec& indexlist, const Array1D<T>& data)
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
//template void QSindex(int low, int high, ivec& indexlist, const vec& data);
//template void QSindex(int low, int high, ivec& indexlist, const ivec& data);
//template void QSindex(int low, int high, ivec& indexlist, const bvec& data);

/*
  Return an index vector corresponding to a sorted vector (increasing order)
*/
template<class T>
ivec sort_index(const Array1D<T>& data)
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
Array1D<T> zero_pad(const Array1D<T>& v, int n)
{
    it_assert(n>=v.size(), "zero_pad() cannot shrink the vector!");
    Array1D<T> v2(n);
    v2.set_subvector(0, v.size()-1, v);
    if (n > v.size())
	v2.set_subvector(v.size(), n-1, T(0));

    return v2;
}

/*
  Zero-pad a vector to the nearest greater power of two
 */
template<class T>
Array1D<T> zero_pad(const Array1D<T>& v)
{
    int n = pow2(needed_bits(v.size()));
    
    return n==v.size() ? v : zero_pad(v, n);
}


/*
  Upsample a vector by incerting \a (usf-1) zeros after each sample
*/
template<class T>
void upsample(const Array1D<T>& v, int usf, Array1D<T>& u)
{
  //  it_assert1(usf >= 1, "upsample: upsampling factor must be equal or greater than one" );
  u.set_size(v.length()*usf);
  u.clear();
  for(long i=0;i<v.length();i++)
    u(i*usf)=v(i); 
}


template<class T>
Array1D<T> upsample(const Array1D<T>& v, int usf)
{
  Array1D<T> u;
  upsample(v,usf,u);
  return u;  
}

/*
  Upsample by a factor of  \a (usf-1) by linear interpolation
*/
template<class T>
void lininterp(const Array1D<T>& v, int usf, Array1D<T>& u)
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
Array1D<T> lininterp(const Array1D<T>& v, int usf)
{
  Array1D<T> u;
  upsample(v,usf,u);
  return u;  
}

#endif //DOXYGEN_SHOULD_SKIP_THIS

}

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

/*! 
  \file 
  \brief Implementation of converters between different vector and matrix types.
  \author Tony Ottosson, Tobias Ringström and Pål Frenger

  1.4

  2002/12/19 23:56:45
*/
#ifndef _convertersh
#define _convertersh

#include <scalfunc.h>
#include <binary.h>
#include <vector.h>
#include <matrix.h>
namespace SPUC {
//--------------- Converters for vectors ------------------

#ifndef DOXYGEN_SHOULD_SKIP_THIS

template <class T>
bvec to_bvec(const Vec<T> &v)
{
    bvec temp(v.length());
    for (int i=0;i<v.length();i++)
	temp(i)=bin(v(i));
		
    return temp;
}

template <class T>
svec to_svec(const Vec<T> &v)
{
    svec temp(v.length());
    for (int i=0;i<v.length();i++)
	temp(i)=short(v(i));
		
    return temp;
}

template <class T>
ivec to_ivec(const Vec<T> &v)
{
    ivec temp(v.length());
    for (int i=0;i<v.length();i++)
	temp(i)=int(v(i));
		
    return temp;
}

template <class T>
llvec to_llvec(const Vec<T> &v)
{
    llvec temp(v.length());
    for (int i=0;i<v.length();i++)
	temp(i)=(long_long)(v(i));
		
    return temp;
}

template <class T>
vec to_vec(const Vec<T> &v)
{
    vec temp(v.length());
    for (int i=0;i<v.length();i++)
	temp(i)=double(v(i));
		
    return temp;
}

template <class T>
cvec to_cvec(const Vec<T> &v)
{
    cvec temp(v.length());
    for (int i=0;i<v.length();i++)
	temp(i)=complex<double>(v(i));
		
    return temp;
}

#endif //DOXYGEN_SHOULD_SKIP_THIS

//--------------- Converters for matricies ------------------

#ifndef DOXYGEN_SHOULD_SKIP_THIS

template <class T>
bmat to_bmat(const Mat<T> &m)
{
    bmat temp(m.rows(),m.cols());

    for (int i=0;i<temp.rows();i++) {
	for (int j=0;j<temp.cols();j++) {
	    temp(i,j)=bin(m(i,j));
	}
    }

    return temp;
}

template <class T>
smat to_smat(const Mat<T> &m)
{
    smat temp(m.rows(),m.cols());

    for (int i=0;i<temp.rows();i++) {
	for (int j=0;j<temp.cols();j++) {
	    temp(i,j)=short(m(i,j));
	}
    }
		
    return temp;
}

template <class T>
imat to_imat(const Mat<T> &m)
{
    imat temp(m.rows(),m.cols());

    for (int i=0;i<temp.rows();i++) {
	for (int j=0;j<temp.cols();j++) {
	    temp(i,j)=int(m(i,j));
	}
    }
  
    return temp;
}

template <class T>
llmat to_llmat(const Mat<T> &m)
{
    llmat temp(m.rows(),m.cols());

    for (int i=0;i<temp.rows();i++) {
	for (int j=0;j<temp.cols();j++) {
	    temp(i,j)=(long_long)(m(i,j));
	}
    }
  
    return temp;
}

template <class T>
mat to_mat(const Mat<T> &m)
{
    mat temp(m.rows(),m.cols());

    for (int i=0;i<temp.rows();i++) {
	for (int j=0;j<temp.cols();j++) {
	    temp(i,j)=double(m(i,j));
	}
    }

    return temp;
}

template <class T>
cmat to_cmat(const Mat<T> &m)
{
    cmat temp(m.rows(),m.cols());

    for (int i=0;i<temp.rows();i++) {
	for (int j=0;j<temp.cols();j++) {
	    temp(i,j)=complex<double>(m(i,j));
	}
    }
	  
    return temp;
}

//------------- end of converters for matricies -------------

template <class T>
cvec to_cvec(const Vec<T> &real, const Vec<T> &imag)
{
    assert(real.length()==imag.length());
    cvec temp(real.length());
    for(int i=0;i<real.length();i++)
	temp(i)=complex<double>(real(i),imag(i));
		
    return temp;
}

template <class T>
cmat to_cmat(const Mat<T> &real, const Mat<T> &imag)
{
    it_assert1(real.rows()==imag.rows() && real.cols()==imag.cols(), "to_cmat::real and imag part sizes does not match");
    cmat temp(real.rows(),real.cols());

    for (int i=0;i<temp.rows();i++) {
	for (int j=0;j<temp.cols();j++) {
	    temp(i,j)=complex<double>(real(i,j), imag(i,j));
	}
    }
    
    return temp;
}

#endif /* DOXYGEN_SHOULD_SKIP_THIS */
int bin2dec(const bvec &inbvec, bool msb_first=0);
bvec dec2bin(int length, int index);
bvec dec2bin(int index, bool msb_first);
void dec2bin(int index, bvec &v);
bvec oct2bin(const ivec &octalindex, short keepzeros=0);
ivec bin2oct(const bvec &inbits);
ivec bin2pol(const bvec &inbvec);
bvec pol2bin(const ivec &inpol);

}
#endif

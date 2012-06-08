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
 
#include <converters.h>
namespace SPUC {


//! Specialized implementation of to_ivec for scalar.
ivec to_ivec(int s) {ivec out(1); out(0) = s; return out;}

//! Specialized implementation of to_llvec for scalar.
llvec to_llvec(long_long s) {llvec out(1); out(0) = s; return out;}

//! Specialized implementation of to_vec for scalar.
vec to_vec(double s) {vec out(1); out(0) = s; return out;}

//! Specialized implementation of to_cvec for scalar.
cvec to_cvec(double real, double imag) {cvec out(1); out(0) = complex<double>(real,imag); return out;}

//------------- end of converters for vectors -------------

//--------------- Converters for matricies ------------------

bvec dec2bin(int length, int index)
{
    int i, bintemp = index;
    bvec temp(length);
    
    for (i=length-1; i>=0; i--) {
	temp(i) = bin(bintemp & 1);
	bintemp = (bintemp >> 1);
    }
    return temp;
}

bvec dec2bin(int index, bool msb_first)
{
  int length = needed_bits(index)+1;
  int i, bintemp = index;
  bvec temp(length);
    
  for (i=length-1; i>=0; i--) {
    temp(i) = bin(bintemp & 1);
    bintemp = (bintemp >> 1);
  }
  if (msb_first)
    return temp;
  else
    return reverse(temp);
}

void dec2bin(int index, bvec &v)
{
    int i, bintemp = index;
    v.set_size(needed_bits(index)+1, false);
    
    for (i=v.size()-1; i>=0; i--) {
	v(i) = bin(bintemp & 1);
	bintemp = (bintemp >> 1);
    }
}

int bin2dec(const bvec &inbvec, bool msb_first)
{
    int i, temp=0;
    int sizebvec=inbvec.length();
    if (msb_first) {
	for (i=0; i<sizebvec; i++) {
	    temp+=pow2i(sizebvec-i-1)*int(inbvec(i));
	}
    } else {
	for (i=0; i<sizebvec; i++) {
	    temp+=pow2i(i)*int(inbvec(i));
	}
    }
    return temp;
}

bvec oct2bin(const ivec &octalindex, short keepzeros)
{
    int length = octalindex.length(), i;
    bvec out(3*length);
    for (i=0; i<length; i++) {
	out.replace_mid(3*i,dec2bin(3,octalindex(i)));
    }
    //remove zeros if keepzeros = 0
    if (keepzeros == 0) {
	for (i=0; i<out.length(); i++) {
	    if ( (short)out(i) != 0) { 
		return out.right(out.length()-i);
		break;
	    }
	}
	return bvec("0");
    } else {
	return out;
    }
}

ivec bin2oct(const bvec &inbits)
{
    int start, Itterations = (int)ceil(float(inbits.length())/3.0);
    ivec out(Itterations);
    for (int i=Itterations-1; i>0; i--) {
	start = 3*i - ( 3*Itterations - inbits.length() );
	out(i) = bin2dec(inbits.mid(start,3));
    }
    out(0) = bin2dec( inbits.left(inbits.length()-((Itterations-1)*3)) );
    return out;
}

ivec bin2pol(const bvec &inbvec)
{
    return 1-2*to_ivec(inbvec);
}

bvec pol2bin(const ivec &inpol)
{
    return to_bvec((1-inpol)/2);
}
}

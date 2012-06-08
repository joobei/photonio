// 
// SPUC - Signal processing using C++ - A DSP library
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
  \brief Implementation of a Hamming code class
  \author Tony Ottosson, Modified by Tony Kirke, Feb 1,2003

  1.2

  2001/12/03 19:37:13
*/

#include <binary.h>
//#include "base/scalfunc.h"
//#include "base/converters.h"
//#include "base/matfunc.h"
#include "hammcode.h"
#include "converters.h"
using namespace SPUC;
Hamming_Code::Hamming_Code(short m)
{
    n = (1<<m) - 1;
    k = (1<<m) - m - 1;
    H.set_size(n-k,n,0); 	
    G.set_size(k,n,0);	
    generate_H(); // generate_H must be run before generate_G
    generate_G();
}

void Hamming_Code::generate_H(void)
{
    short i, j, NextPos;
    char NotUsed;
    bvec temp;
    svec indexes(n);
	
    for (i=1; i<=n-k; i++) { indexes(i-1) = (1<< (n-k-i)); }
    NextPos = n-k;
    for (i=1; i<=n; i++) {
	NotUsed = 1;
	for (j=0; j<n; j++) 
	    if (i == indexes(j)) { NotUsed = 0; }
	if (NotUsed) { indexes(NextPos) = i; NextPos = NextPos + 1; }
    }
			
    for (i=0; i<n; i++) {	
	temp = dec2bin(n-k,indexes(i)); //<-CHECK THIS OUT!!!!
	for (j = 0; j < (n-k); j++) {
	    H(j,i) = temp(j);
	}
    }
}

void Hamming_Code::generate_G(void)
{
    short i, j;
    for (i=0; i<k; i++) {
	for(j=0; j<n-k; j++) 
	    G(i,j) = H(j,i+n-k);
    }
	
    for (i=0; i<k; i++) {
	for (j=n-k; j<n; j++) 
	    G(i,j) = 0;
    }
	
    for (i=0; i<k; i++)
	G(i,i+n-k) = 1;
}


bvec Hamming_Code::encode(const bvec &uncoded_bits)
{
    int length = uncoded_bits.length();
    int Itterations = (int)floor( float(length) / k );
    bvec output(Itterations * n);
    int i;
	
    //Code all codewords
    for (i=0; i<Itterations; i++) 
	output.replace_mid(n*i, uncoded_bits.mid(i*k,k) * G );

    return output;
}

bvec Hamming_Code::decode(const bvec &coded_bits)
{
    int length = coded_bits.length();
    int Itterations = (int)floor( float(length) / n );
    svec Hindexes(n);
    bvec temp(n-k);
    bvec coded(n), syndrome(n-k), output(Itterations*k);
    short  isynd, errorpos=0;
    int i, j;
    for (i=0; i<n; i++) {
	for (j=0; j<n-k; j++) 
	    temp(j) = H(j,i);
	Hindexes(i) = bin2dec(temp); 
    }
	
    //Decode all codewords
    for (i=0; i<Itterations; i++) {
	coded = coded_bits.mid(i*n,n);
	syndrome = coded * transpose(H); 
	isynd = bin2dec(syndrome); 
	if (isynd != 0) {
	    for (j=0; j<n; j++) 
		if (Hindexes(j) == isynd) { errorpos = j; };
	    coded(errorpos) += 1;
	}
	output.replace_mid(k*i,coded.right(k));
    }
	
    return output;
}

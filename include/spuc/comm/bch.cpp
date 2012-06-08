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
  \brief Implementation of a BCH encoder/decoder class.
  \author Pål frenger

  1.2

  2001/12/03 19:37:13
*/
#include "binary.h"
#include "converters.h"
#include "specmat.h"
#include "bch.h"
using namespace SPUC;

//---------------------- BCH -----------------------------------


BCH::BCH(int in_n, int in_k, int in_t, ivec genpolynom) {
    n = in_n;
    k = in_k;
    t = in_t;

    //fix the generator polynomial g(x).
    ivec exponents(n-k+1);
    bvec temp = oct2bin(genpolynom);
    for (int i=0; i<temp.length(); i++) {
	exponents(i) = int(-1) + int(temp(temp.length()-i-1));
    }
    g.set(n+1,exponents);
}

bvec BCH::encode(const bvec &uncodedbits) {
  int i, j, degree;
  int itterations = (int)floor( (double)uncodedbits.length() / k );
  gfx m(n+1,k);
  gfx c(n+1,n);
  bvec out(itterations*n), mbit(k), cbit(n);

    for (i=0; i<itterations; i++) {
	//Fix the message polynom m(x).
	mbit = uncodedbits.mid(i*k,k);
	for (j=0; j<k; j++) {
	    degree = int(-1) + int(mbit(j));
	    m[j] = gf(n+1,degree);
	}  
	//Fix the outputbits cbit.
	c = g*m;
	for (j=0; j<n; j++) {
	    if ( c[j] == gf(n+1,0) ) {
		cbit(j) = 1;
	    } else {
		cbit(j) = 0;
	    }
	}
	out.replace_mid(i*n,cbit);
    }
    return out;
}

bvec BCH::decode(const bvec &codedbits) {
  int j, i, degree, kk, foundzeros, cisvalid;
  int itterations = (int)floor( (double)codedbits.length() / n );
    bvec out(itterations*k), rbin(n), mbin(k);
    gfx r(n+1,n-1), c(n+1,n-1), m(n+1,k-1), S(n+1,2*t), Lambda(n+1), OldLambda(n+1), T(n+1), Ohmega(n+1), One(n+1, (char*)"0");
    gf delta(n+1), temp(n+1);
    ivec errorpos;

    for (i=0; i<itterations; i++) {
	//Fix the received polynomial r(x)
	rbin = codedbits.mid(i*n,n);
	for (j=0; j<n; j++) {
	    degree = int(-1) + int(rbin(j));
	    r[j] = gf(n+1,degree);
	}
	//Fix the syndrome polynomial S(x).
	S[0] = gf(n+1,-1);
	for (j=1; j<=2*t; j++) {
	    S[j] =  r(gf(n+1,j));
	}
	if (S.get_true_degree() >= 1) { //Errors in the received word
	    //Itterate to find Lambda(x).
	    kk = 0;                
	    Lambda = gfx(n+1,(char*)"0"); 
	    T = gfx(n+1,(char*)"0");      
	    while (kk<t) {
		Ohmega = Lambda * (S + One);
		delta = Ohmega[2*kk+1];
		OldLambda = Lambda;
		Lambda = OldLambda + delta*( gfx(n+1,(char*)"-1 0")*T );
		if ((delta == gf(n+1,-1)) || (OldLambda.get_degree() > kk)) {
		    T = gfx(n+1,(char*)"-1 -1 0") * T;
		} else {
		    T = ( gfx(n+1,(char*)"-1 0") * OldLambda ) / delta;
		} 
		kk = kk + 1;
	    }   
	    //Find the zeros to Lambda(x).
	    errorpos.set_size(Lambda.get_true_degree(), true);
	    foundzeros = 0;
	    for (j=0; j<=n-1; j++) {
		temp = Lambda( gf(n+1,j) );
		if  (Lambda( gf(n+1,j) ) == gf(n+1,-1) ) {
		    errorpos( foundzeros ) = (n-j) % n;
		    foundzeros +=1;
		    if (foundzeros >= Lambda.get_true_degree()) {
			break;
		    }
		}
	    }
	    //Correct the codeword.
	    for (j=0; j<foundzeros; j++) {
		rbin(errorpos(j)) += 1;
	    }
	    //Reconstruct the corrected codeword.
	    for (j=0; j<n; j++) {
		degree = int(-1) + int(rbin(j));
		c[j] = gf(n+1,degree);
	    }
	    //Code word validation.
	    S[0] = gf(n+1,-1);
	    for (j=1; j<=2*t; j++) {
		S[j] =  c(gf(n+1,j));
	    }
	    if (S.get_true_degree()<=0) { //c(x) is a valid codeword.
		cisvalid = true;  
	    } else {
		cisvalid = false;
	    }
	} else {
	    c = r;
	    cisvalid = true; 
	}
	//Construct the message bit vector.
	if (cisvalid) { //c(x) is a valid codeword.
	    if (c.get_true_degree() > 1) {
		m = divgfx(c,g);
		mbin.clear();
		for (j=0; j<=m.get_true_degree(); j++) {
		    if ( m[j] == gf(n+1,0) ) {
			mbin(j) = 1;
		    }
		}
	    } else { //The zero word was transmitted
		mbin = zeros_b(k);
		m = gfx(n+1,(char*)"-1"); 
	    }
	} else { //Decoder failure.
	    mbin = zeros_b(k);
	    m = gfx(n+1,(char*)"-1");
	}
	out.replace_mid(i*k,mbin);
    }
    return out;
}


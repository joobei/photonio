//! 
//! Modified by Tony Kirke, Feb 1,2003
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
  \brief Implementation of a Reed-Solomon codec class.
  \author Pål Frenger

  1.3

  2002/12/19 23:56:49
*/

#include "reed_solomon.h"
#include <math.h>
using namespace SPUC;

//-------------------- Help Function ----------------------------

//! Local help function
gfx formal_derivate(const gfx &f)  {
    int degree = f.get_true_degree();
    int q = f.get_size();
    int i;
    gfx fprim(q,degree);
    fprim.clear();
    for (i=0; i<=degree-1; i+=2) {
	fprim[i] = f[i+1];
    }
    return fprim;
}

//-------------------- Reed-Solomon ----------------------------
//A Reed-Solomon code is a q^m-ary BCH code of length n = pow(q,m)-1. 
//k = pow(q,m)-1-t. This class works for q==2.
reed_solomon::reed_solomon(int in_m, int in_t) {
	
    m = in_m;
    t = in_t;
    n = (int)(::pow(2,m)-1);
    k = ((int)(::pow(2,m))) -1 - 2*t;
    q = (int)(::pow(2,m));
    int i;
    gfx x(q,(char *)"-1 0");
    ivec alphapow(1);
    g.set(q,(char *)"0");
    for(i=1; i<=2*t; i++) {
	alphapow[0] = i;
	g *= (x-gfx(q,alphapow));
    }
}

bvec reed_solomon::encode(const bvec &uncodedbits) {
  int i, j;
  int itterations = (int)floor( (double)uncodedbits.length() / (k*m) );
    gfx mx(q,k), cx(q,n);
    gf mpow;
    bvec out(itterations*n*m), mbit(k*m), cbit(m);

    for (i=0; i<itterations; i++) {
	//Fix the message polynom m(x).
	for (j=0; j<k; j++) {
	    mpow.set(q,uncodedbits.mid((i*m*k)+(j*m),m));
	    mx[j] = mpow;
	}  
	//Fix the outputbits cbit.
	cx = g*mx;
	for (j=0; j<n; j++) {
	    cbit = cx[j].get_vectorspace();
	    out.replace_mid((i*n*m)+(j*m),cbit);
	}
    }
    return out;
}

bvec reed_solomon::decode(const bvec &codedbits) {
  int j, i, kk, l, L, foundzeros, decoderfailure;
  int itterations = (int)floor( (double)codedbits.length() / (n*m) );
    bvec out(itterations*k*m), mbit(m*k);
    gfx rx(q,n-1), cx(q,n-1), mx(q,k-1), ex(q,n-1), S(q,2*t), Lambda(q), Lambdaprim(q), OldLambda(q), T(q), Ohmega(q);
    gfx dummy(q), One(q,(char*)"0"), Ohmegatemp(q);
    gf delta(q), tempsum(q), rtemp(q), temp(q), Xk(q), Xkinv(q);
    ivec errorpos;

    for (i=0; i<itterations; i++) {
	decoderfailure = false;
	//Fix the received polynomial r(x)
	for (j=0; j<n; j++) {  
		rtemp.set(q,codedbits.mid(i*n*m + j*m, m)); 
		rx[j] = rtemp; 
	}
	//Fix the syndrome polynomial S(x).
	S.clear();
	for (j=1; j<=2*t; j++) { S[j] =  rx(gf(q,j)); }
	if (S.get_true_degree() == 0) {cx = rx; decoderfailure = false; }
	else {//Errors in the received word
	    //Itterate to find Lambda(x).
	    kk = 0; Lambda = gfx(q,(char*)"0"); L = 0; T = gfx(q,(char*)"-1 0");   
	    while (kk<2*t) {
		kk = kk + 1;
		tempsum = gf(q,-1);
		for (l=1; l<=L; l++) { tempsum += Lambda[l] * S[kk-l]; }
		delta = S[kk] - tempsum;
		if (delta != gf(q,-1)) {
		    OldLambda = Lambda;
		    Lambda -= delta*T;
		    if (2*L<kk) { L = kk - L; T = OldLambda/delta;}
		} 
		T = gfx(q,(char*)"-1 0") * T;
	    }
	    //Find the zeros to Lambda(x).
	    errorpos.set_size(Lambda.get_true_degree(), false);
	    errorpos = 0; // clear();
	    foundzeros = 0;
	    for (j=q-2; j>=0; j--) {
		temp = Lambda( gf(q,j) );
		if  (Lambda( gf(q,j) ) == gf(q,-1) ) {
		    errorpos[ foundzeros ] = (n-j) % n;
		    foundzeros +=1;
		    if (foundzeros >= Lambda.get_true_degree()) { break; }
		}
	    }
	    if (foundzeros != Lambda.get_true_degree()) { decoderfailure = false; } 
	    else {
		//Compute Ohmega(x) using the key equation for RS-decoding
		Ohmega.set_degree(2*t);
		Ohmegatemp = Lambda * (One +S);
		for (j=0; j<=2*t; j++) { Ohmega[j] = Ohmegatemp[j]; }
		Lambdaprim = formal_derivate(Lambda);
		//Find the error polynomial
		ex.clear();
		for (j=0; j<foundzeros; j++) {
		    Xk = gf(q,errorpos[j]); Xkinv = gf(q,0) / Xk;
		    ex[errorpos[j]] = (Xk * Ohmega(Xkinv)) / Lambdaprim(Xkinv);
		}
		//Reconstruct the corrected codeword.
		cx = rx + ex;
		//Code word validation
		S.clear(); for (j=1; j<=2*t; j++) { S[j] =  rx(gf(q,j)); }
		if (S.get_true_degree() >= 1) { decoderfailure=false; }
	    }
	}
	//Find the message polynomial
	mbit= 0;
	if (decoderfailure == false) {
	    if (cx.get_true_degree() >= 1) {// A nonzero codeword was transmitted
		mx = divgfx(cx,g);
		for (j=0; j<=mx.get_true_degree(); j++) { mbit.replace_mid(j*m,mx[j].get_vectorspace()); }
	    }
	} 
	out.replace_mid(i*m*k,mbit);
    }
    return out;
}

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
  \brief Implementation of Class for the Extended Golay Code (24,12,8).
  \author Tony Ottosson, Modified by Tony Kirke, Feb 1,2003

  1.2

  2001/12/03 19:37:13
*/

#include <specmat.h>
#include "egolay.h"
#include "commfunc.h"
#include <iostream>
using namespace SPUC;
Extended_Golay::Extended_Golay(void)
{	
    B="0 1 1 1 1 1 1 1 1 1 1 1;1 1 1 0 1 1 1 0 0 0 1 0;1 1 0 1 1 1 0 0 0 1 0 1;1 0 1 1 1 0 0 0 1 0 1 1;1 1 1 1 0 0 0 1 0 1 1 0;1 1 1 0 0 0 1 0 1 1 0 1;1 1 0 0 0 1 0 1 1 0 1 1;1 0 0 0 1 0 1 1 0 1 1 1;1 0 0 1 0 1 1 0 1 1 1 0;1 0 1 0 1 1 0 1 1 1 0 0;1 1 0 1 1 0 1 1 1 0 0 0;1 0 1 1 0 1 1 1 0 0 0 1";

    G = concat_horizontal(eye_b(12), B);
}


bvec Extended_Golay::encode(const bvec &uncoded_bits)
{
    int no_bits = uncoded_bits.length();
    int no_blocks = (int)floor((double)no_bits/12);
    bvec output(24*no_blocks);
    int i;
	
    for (i=0; i<no_blocks; i++) 
	output.replace_mid(24*i, uncoded_bits.mid(i*12,12)*G);

    return output;
}

bvec Extended_Golay::decode(const bvec &coded_bits)
{
    int no_bits = coded_bits.length();
    int no_blocks = (int)floor((double)no_bits/24);
    bvec output(12*no_blocks);
    int i;
    int j;
    bvec S(12),BS(12),r(12),temp(12),e(24),c(24);
    bmat eyetemp = eye_b(12);

    for (i=0; i<no_blocks; i++) {
	r = coded_bits.mid(i*24,24);
	// Step 1. Compute S=G*r.
	S = G*r;
	// Step 2. w(S)<=3. e=(S,0). Goto 8.
	if( weight(S) <= 3 ) {
	    e = concat(S, zeros_b(12)); goto Step8;
	}
	  
	// Step 3. w(S+Ii)<=2. e=(S+Ii,yi). Goto 8.
	for (j=0; j<12; j++) {
			
	    temp = S + B.get_col(j);
	    if ( weight(temp) <=2 ) {
		e = concat(temp, eyetemp.get_row(j)); goto Step8;
	    }
	}

	// STEP 4. Compute B*S
	BS = B*S;

	// Step 5. w(B*S)<=3. e=(0,BS). Goto8.
	if ( weight(BS) <=3 ) {
	    e = concat(zeros_b(12), BS); goto Step8;
	}

	// Step 6. w(BS+Ri)<=2. e=(xi,BS+Ri). Goto 8.
	for (j=0; j<12; j++) {
	    temp = BS + B.get_row(j);
	    if ( weight(temp) <=2 ) {
		e = concat(eyetemp.get_row(j), temp); goto Step8;
	    }
	}

	// Step 7. Uncorrectable erreor pattern. Choose the first 12 bits.
	e = zeros_b(24); goto Step8;
	  
    Step8: // Step 8. c=r+e. STOP
	c = r + e;
	output.replace_mid(i*12, c.left(12));
    }
  
    return output;
}

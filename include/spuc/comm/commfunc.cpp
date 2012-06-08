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
  \brief Implementation of some specific functions useful in communications.
  \author Tony Ottosson, modified by Tony Kirke, Feb 1,2003

  1.2

  2001/12/03 19:37:13
*/

#include "commfunc.h"
#include <specmat.h>
#include <matfunc.h>
#include <binary.h>
namespace SPUC {
bmat graycode(int m)
{
    if (m == 1) {
		bmat out(1,0);
		return(out);
    } else {
	bvec temp(1<<(m-1));
	bmat bb	= graycode(m-1);
	bmat out(1<<m, m);
	out.zeros();
	out.set_col(0, concat(zeros_b(1<<(m-1)), ones_b(1<<(m-1))) );
	for (int i=0; i<m-1; i++) {
	    temp = bb.get_col(i);
	    out.set_col(i+1, concat(temp, reverse(temp)) );
	}
	return out;
    }
}



int hamming_distance(const bvec &a, const bvec &b)
{
    int i, n=0;

//    it_assert1(a.size() == b.size(), "hamming_distance()");
    for (i=0; i<a.size(); i++)
	if (a(i) != b(i))
	    n++;
    
    return n;
}



int weight(const bvec &a)
{
    int i, n=0;

    for (i=0; i<a.size(); i++)
	if (a(i)==bin(1))
	    n++;
    
    return n;
}
} // namespace SPUC 

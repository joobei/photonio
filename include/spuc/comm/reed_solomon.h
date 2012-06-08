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
  \brief Definitions of a Reed-Solomon codec class
  \author Pål Frenger, Modified by Tony Kirke, Feb 1,2003

  1.6

  2003/01/04 00:21:55
*/

#ifndef __reedsolomon_h
#define __reedsolomon_h

#include <gfx.h>
namespace SPUC {
//---------------------- Reed-Solomon --------------------------------------

/*! 
  \ingroup fec
  \brief Reed-Solomon Codes.
  
  
  Uses the Berlkamp-Massey algorithm for decoding as described in: S. B. Wicker,
  "Error Control Systems for digital communication and storage," Prentice Hall.

  The code is ff$2^mff$ - ary of length ff$2^m-1ff$ capable of correcting ff$tff$ errors.
*/
class reed_solomon {
public:
    //! Class constructor for the ff$2^mff$ - ary, ff$tff$ error correcting RS-code.
    reed_solomon(int in_m, int in_t);
    //! Encoder function.
    bvec encode(const bvec &uncodedbits);
    //! Decoder function
    bvec decode(const bvec &codedbits);
    //! Gets the rate of the RS-code.
    double get_rate() { return double(k)/double(n); }
protected:
    //! Internal encoder/decoder parameters
    int m, t, k, n, q;
    //! The generator polynomial of the RS code
    gfx g;
};
}
#endif // __reedsolomon_h

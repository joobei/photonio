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
  \brief Definition of Class for the Extended Golay Code (24,12,8)
  \author Tony Ottosson, Modified by Tony Kirke, Feb 1,2003

  1.4

  2001/12/02 19:40:28
*/

#ifndef __egolay_h
#define __egolay_h

#include <vector.h>
#include <matrix.h>
namespace SPUC {
/*! 
  \ingroup fec
  \brief Extended Golay code (24,12,8).
  \author Tony Ottosson
  
  The code is given in systematic form with the information bits first, followed by
  the parity check bits. The decoder uses the arithmetic decoding algorithm that is
  for example described in Wicker "Error Control Systems for Digital Communication and
  Storage", Prentice Hall, 1995 (page 143).
*/
class Extended_Golay {
public:
  //! Constructor
  Extended_Golay();
  //! Encoder. Will truncate some bits if not \a length = \c integer * 12
  bvec encode(const bvec &uncoded_bits);
  //! Decoder. Will truncate some bits if not \a length = \c integer * 24
  bvec decode(const bvec &coded_bits);
  //! Gets the generator matrix for the code (also the parity check matrix)
  bmat get_G() { return G; }
private:
    bmat B,G;
};
}
#endif // __egolay_h

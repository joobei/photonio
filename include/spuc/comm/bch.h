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
  \brief Definition of a BCH encoder/decoder class
  \author Pål Frenger

  1.5

  2003/01/04 00:21:55
*/

#ifndef _bch_h
#define _bch_h

#include "gfx.h"
namespace SPUC {
/*!
  \defgroup fec Forward Error Correcting Codes
*/

//---------------------- BCH --------------------------------------

/*! 
  \ingroup fec
  \brief Class for binary, narrow-sense BCH codes.
  
  The notation used is found in S. B. Wicker, "Error control systems for
  digital communication and storage", Appendix E, Prentice-Hall, 1995.

  Example: 
  \code BCH bch(31,21,2,"3551") 
  \endcode 
  uses the generator polynomial
  \f$g(x) = x^{10} + x^9 + x^8 + x^6 + x^5 + x^3 + 1\f$, and is capable of
  correcting 2 errors with \a n = 31 and \a k = 21.
*/
class BCH {
public:
  //! Initialize a (n,k)-code that can correct t errors
  BCH(int in_n, int in_k, int in_t, ivec genpolynom);
  //! Encode a bvec of indata
  bvec encode(const bvec &uncodedbits);
  //! Decode a bvec of coded data
  bvec decode(const bvec &codedbits);
  //! Get the code rate
  double get_rate() {return double(k)/double(n); }

//protected:
private:
    int n, k, t;
    gfx g;
};
}
#endif // _bch_h

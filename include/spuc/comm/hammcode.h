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
  \brief Definitions of a Hamming code class
  \author Tony Ottosson, Modified by Tony Kirke, Feb 1,2003

  1.4

  2001/12/02 19:40:28
*/

#ifndef __hamming_h
#define __hamming_h

#include <vector.h>
#include <matrix.h>
namespace SPUC {
/*! 
  \ingroup fec
  \brief Binary Hamming codes
*/
class Hamming_Code {
public:
    //! Constructor for \c hamming(n,k). n = pow(2,m)-1 and k = pow(2,m)-m-1.
    Hamming_Code(short m);
    //! Hamming encoder. Will truncate some bits if not \a length = \c integer * \a k.
    bvec encode(const bvec &uncoded_bits);
    //! Hamming decoder. Will truncate some bits if not \a length = \c integer * \a n.
    bvec decode(const bvec &coded_bits);
    //! Gets the code length \a n.
    short get_n() { return n; };
    //! Gets the number of information bits per code word, \a k.
    short get_k() { return k; };
    //! Gets the parity check matrix for the code.
    bmat get_H() { return H; };
    //! Gets the generator matrix for the code.
    bmat get_G() { return G; };
private:
    short n, k; 
    bmat H, G;
    void generate_H(void);
    void generate_G(void);
};
}
#endif // __hamming_h

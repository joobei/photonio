// 
// Copyright(c) 1993-1996 Tony Kirke
// author="Tony Kirke" *
/*
 * SPUC - Signal processing using C++ - A DSP library
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#ifndef LOOPFILTER
#define LOOPFILTER
#include <spuc.h>
namespace SPUC {
/*! 
  \addtogroup pll       Phase lock loop functions
*/
/*!   \brief Loop Filter for use in PLL circuits
  \ingroup pll
*/
//!:
//!    Template for 2nd order loop filter (for timing/carrier recovery, etc).
//!    Either branch can be enabled/disabled for 1st/2nd order operation
//!    There is no overflow protection on accumulator.
//!    Gains are not normalized.
//! \image html loop_filter.gif
//! \image latex loop_filter.eps
template <class Numeric> class loop_filter
{
 public:
  //! enable first order branch
  long k0_en; 
  //! enable second order branch
  long k1_en; 
  //! First order gain
  Numeric k0; 
  //! second order gain
  Numeric k1; 
  //! Accumulator for k1 branch (should not be written to)
  Numeric k1_acc;

 protected:
  Numeric loop_out;
  Numeric k1_prod, k0_prod;

 public:
  //! Constructor
  loop_filter(void) { k0 = k1 = k1_acc = 0; k0_en = k1_en = 0;}
  //! Reset
  void reset(void) { k1_acc = k1_prod = k0_prod = loop_out = 0; }
  //! Normal call with input, returns output.
  Numeric update(Numeric error) {
    k0_prod = (k0_en) ? error*k0 : 0;
    k1_prod = (k1_en) ? error*k1 : 0;
    loop_out = k1_acc + k0_prod; // Use last k1_acc!
    k1_acc += k1_prod; 
    return(loop_out);
  }
};
} // namespace SPUC 
#endif

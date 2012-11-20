// 
// author="Tony Kirke"
// Copyright(c) 1993-1996 Tony Kirke
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
#ifndef ALLPASS_HALFBAND
#define ALLPASS_HALFBAND
#include "allpass_1.h"
namespace SPUC {
/*! 
  \addtogroup iir IIR filters
*/

/*!   \brief  Template Class for Allpass halfband IIR Filter
  \ingroup iir
*/
//!:
//!   The filter is a combination of 2 Allpass sections of 
//!   the form  G(z) =  (a*z*z + 1)/(z*z+a)
//!   so that the overall H(z) is
//!   H(z) = 1/z*G(z,a0) + G(z,a1)
//!   The combination of these two allpass functions result
//!   in a lowpass/highpass complementary pair. The class only
//!   uses the low pass filter.
//!   This is similar to allpass_iir, except since this is used
//!   as a halfband filter the output samples are decimated by 2. 
//!   So that if the member function clock is called at the higher
//!   (input) sampling rate, the outputs are only valid even second
//!   sample. This is taken advantage off by running the individual
//!   sections effectively at the lower rate. 
//!   The invalid samples are set to zero.
//!   This class needs the allpass_1 class
//! \image html allpass_halfband.gif
//! \image latex allpass_halfband.eps
template <class Numeric> class allpass_halfband
{
 public:
  char even; //! Keeps track of which filter to clock
 protected:   
  //! The 2 1st order allpass filters
  allpass_1<Numeric> A0,A1;
  //! Individual filter outputs
  Numeric out0,out1;
      
 public:
  allpass_halfband(Numeric c0, Numeric c1)
    : A0(c0,1), A1(c1,1) { even = 0;	}
  //! reset
  void reset() { A0.reset(); A1.reset(); out0=out1=0; }
  //! Shift inputs by one time sample and place new sample into array
  Numeric clock(Numeric input) {
	if (!even) {
		out0 = A0.clock(input);
	} else {
		out1 = A1.clock(input);
	}
	even = !even;
	if (even) return(0.5*(out0 + out1));
	else return(0.0); // to indicate that this sample is not calculated
	// Complimentary filter return(0.5*(out0 - out1));
	}
};     
} // namespace SPUC 
#endif

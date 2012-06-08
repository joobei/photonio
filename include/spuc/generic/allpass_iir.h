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
#ifndef ALLPASS_IIR
#define ALLPASS_IIR
#include "allpass_1.h"
namespace SPUC {
/*! 
  \addtogroup iir IIR filters
*/

/*!   \brief  Template Class for 1st Order Allpass IIR filter
  \ingroup iir
*/
//! The filter is a combination of 2 Allpass sections of 
//! the form  G(z) =  (a*z*z + 1)/(z*z+a)
//! so that the overall H(z) is
//! H(z) = 1/z*G(z,a0) + G(z,a1)
//! The combination of these two allpass functions result
//! in a lowpass/highpass complementary pair. The class only
//! uses the low pass filter.
//! This class needs the allpass_1 class
//! \image html allpass_iir.gif
//! \image latex allpass_iir.eps
template <class Numeric> class allpass_iir
{
    protected:   
		allpass_1<Numeric> A0,A1;
		delay<Numeric> dly;
		long delay_size;
       
    public:
        allpass_iir(Numeric c0, Numeric c1, long delay=2) 
			: A0(c0,delay), A1(c1,delay), delay_size(delay)
		{ 
			dly.set_size(delay);
		}
		//! Reset
		void reset() { dly.reset(); A0.reset(), A1.reset() }
           
		//! Shift inputs by one time sample and place new sample into array
		Numeric clock(Numeric input) {
		  Numeric out0,out1;

		  dly.input(input);
		  out0 = A0.clock(input);
		  out1 = A1.clock(dly.check(delay_size/2));
		  return(0.5*(out0 + out1));
		  // Complimentary filter return(0.5*(out0 - out1));
		}
};                                               
} // namespace SPUC 
#endif

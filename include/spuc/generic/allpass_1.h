#ifndef ALLPASS_1
#define ALLPASS_1
#include <delay.h>
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
namespace SPUC {
/*! 
  \addtogroup iir IIR filters
*/

/*!   \brief  Template class for 1st order allpass filter with programmble gain and delay
  \ingroup iir allpass
*/
//!:
//! Template class for 1st-order Allpass filter.
//! The filter is a "first-order" Allpass section of 
//! the form  G(z,n) =  (a*z^n + 1)/(z^n+a)
//! The delay n can be specified
//! \image html allpass_1.gif
//! \image latex allpass_1.eps
template <class Numeric> class allpass_1
{
 protected:  
  //! number of unit sample delays
  long size; 
  Numeric gain;
  delay<Numeric> mult;
  delay<Numeric> sum;
        
 public:
  //! Constructor with gain and delay
  allpass_1(Numeric Gain=0, long s=1) { 
    gain = Gain;
    size = s;
    mult.set_size(size);
    sum.set_size(size);
  }
  //! Reset
  void reset() {
	  mult.reset();
	  sum.reset();
  }
  //! Initializes gain and delay
  void init(Numeric Gain, long s) { 
  gain = Gain;
  size = s;
  mult.set_size(size);
  sum.set_size(size);
}
//! Shift inputs by one time sample and place new sample into array
Numeric clock(Numeric input) {
	Numeric current_sum, current_mult,out;
	
	out = sum.check(size-1);
	current_sum = input + mult.check(size-1);
	current_mult = -gain*current_sum;
	out -= current_mult;
	// update delay lines
	mult.input(current_mult);
	sum.input(current_sum);
	return(out);
	}
};                                               
} // namespace SPUC 
#endif

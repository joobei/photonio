//!  \author Tony Kirke,  Copyright(c) 2001 
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
#ifndef ALLPASS_A1
#define ALLPASS_A1
namespace SPUC {
/*! 
  \addtogroup iir IIR filters
*/

/*! \brief  Template class for 1st order allpass filter with programmble gain and fixed delay
  \ingroup iir
*/
//!:
//! The filter is a "first-order" Allpass section of 
//! the form  G(z,n) =  (a*z + 1)/(z+a)
//! \image html allpass_1.gif
//! \image latex allpass_1.eps
template <class Numeric> class allpass_1a
{
    protected:   
		long size; //! Not used
    	Numeric a0; //! Gain factor
		Numeric adder2; //! registered output of adder
        
    public:
        allpass_1a(Numeric A0, long s=0) : size(s) { 
		  a0 = A0; adder2 = 0;
		}

		void init(Numeric A0, long s=0) { 
		  a0 = A0;
		  adder2 = 0;
		}
		//! Shift inputs by one time sample and place new sample into array
		Numeric clock(Numeric input) {
		  Numeric adder1,next_adder2,mult,out;
		  adder1 = adder2 - input;
		  mult = -a0*adder1;
		  next_adder2 = mult + input;
		  out = adder2 + mult;
		  adder2 = next_adder2;
		  return(out);
		}
};                                               
} // namespace SPUC 
#endif

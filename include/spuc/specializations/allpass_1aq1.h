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
#ifndef ALLPASS_1AQ1
#define ALLPASS_1AQ1
namespace SPUC {
//! \ingroup allpass examples
//! \brief   The filter is a "first-order" Allpass section of the form
//!  <p>\f$  G(z) =  \frac{a + z^{-1}}{(1+a*z^{-1})} \f$ with A = 0.25
//!  \author Tony Kirke,  Copyright(c) 2001 
//!  
class allpass_1aq1 
{
    protected:   
		long adder2;
        
    public:
        allpass_1aq1() { adder2 = 0;}

void init() { adder2 = 0;}
long clock(long input) {
// Shift inputs by one time sample and place new sample into array
	long adder1,next_adder2,mult,out;

// Adder1 needs 1 more bit than input	
	adder1 = adder2 - input;

	mult = -((adder1+2)>>2); // Multiplier = 1/4

// Next adder2 needs 1 more bit than input
	next_adder2 = mult + input;

// output adder needs 1 more bit than input
	out = adder2 + mult;
	adder2 = next_adder2;
	return(out);
	}
};                                               
} // namespace SPUC 
#endif


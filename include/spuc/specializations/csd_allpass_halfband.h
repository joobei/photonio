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
#ifndef CSD_ALLPASS_HALFBAND
#define CSD_ALLPASS_HALFBAND
#include <allpass_1aq1.h>
#include <allpass_1aq2.h>
namespace SPUC {
//! \ingroup iir examples
//! \brief This filter is a combination of 2 fixed allpass filters allpass_1aq1 and allpass_1aq2
// so that the overall H(z) is \f$ H(z) = G(z,a0)*z^{-1} + G(z,a1) \f$
// <p>The combination of these two allpass functions result
// in a lowpass/highpass complementary pair. This class only
// uses the low pass filter.
//!  \author Tony Kirke,  Copyright(c) 2001 
//!  
class csd_allpass_halfband
{
	public:
		char ready;
    protected:   
		allpass_1aq1 A0;
		allpass_1aq2 A1;
		long out0,out1;
      
    public:
        csd_allpass_halfband()	{ ready = 1;	}

           
long clock(long input) {
// Shift inputs by one time sample and place new sample into array

	ready = !ready;
	if (ready) {
		out0 = A0.clock(input);
	} else {
		out1 = A1.clock(input);
	}
	if (ready) return(out0 + out1);
	else return(0); // to indicate that this sample is not calculated
	// Complimentary filter return(0.5*(out0 - out1));
	}
};     
} // namespace SPUC 

#endif
                                          

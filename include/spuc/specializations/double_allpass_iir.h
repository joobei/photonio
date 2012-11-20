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
#include "allpass_1.h"
namespace SPUC {
//! \brief Similar to a specific instantiation of allpass_iir.
// This one uses the double type and default delay 1
//!  \author Tony Kirke,  Copyright(c) 2001 
//!  
class double_allpass_iir
{
 protected:   
	allpass_1<double> A0,A1;
	delay<double> dly;
	long delay_size;
       
 public:
	double_allpass_iir(double c0=0.25, double c1=0.75, long delay=1) 
		: A0(c0,delay), A1(c1,delay), delay_size(delay)	{ 
		dly.set_size(delay);
	}

	
	double clock(double input) {
		// Shift inputs by one time sample and place new sample into array
		double out0,out1;
		dly.input(input);
		out0 = A0.clock(input);
		out1 = A1.clock(dly.check(delay_size/2));
		return(0.5*(out0 + out1));
		// Complimentary filter return(0.5*(out0 - out1));
	}
};                                            
} // namespace SPUC 

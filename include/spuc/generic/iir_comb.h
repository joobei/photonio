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
#ifndef IIR_COMB
#define IIR_COMB
#include <delay.h>
namespace SPUC {
/*! 
  \brief  Template for IIR comb type filter with programmable delay and gain
  \ingroup iir
*/
template <class Numeric> class iir_comb	{
 protected:   
	double in_gain;                    
	double acc_gain;                    
	Numeric out;
	delay<Numeric> dly;
	long delay_size;
	Numeric previous_out;
	Numeric previous_in;
        
 public:
	iir_comb(double A=0, long delay=2) : acc_gain(A), in_gain(1-A),
		delay_size(delay) {
		dly.set_size(delay);
		previous_in = previous_out = out = 0 ; 
	}
	void init(double A, long delay) { 
		in_gain=1-A; acc_gain = A;
		delay_size = delay;
		dly.set_size(delay);
	}

	void set_coeff(double A) { in_gain=1-A; acc_gain = A;}
	//! Input new sample and calculate output
	Numeric clock(Numeric input) {
		// Shift previous outputs and calculate new output
		out = acc_gain*previous_out + in_gain*input;
		previous_out = dly.input(out);
		return(out);
	}
	//! Reset
	void reset() {
		previous_in = previous_out = out = 0;
		dly.reset();
	}
};                                               
} // namespace SPUC 
#endif

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
#ifndef IIR_SHELF
#define IIR_SHELF
namespace SPUC {
//! \brief  Template Class for 1st Order iir filter
//! \ingroup iir
//
//! 
//!   Template for shelving filter
//!   <p>The filter is assumed of the form
//!   \f$ H(s) = \frac{s+A}{s+B} \f$
//!   which becomes
//!   \f$ H(z) =  \frac{(A+1)+(A-1)*z^-1}{(B+1)+(B-1)*z^-1} \f$
template <class Numeric> class iir_shelf
{
    protected:   
	double a;
	double b;
	Numeric out;
	Numeric previous_out;
	Numeric previous_in;
        
 public:
	iir_shelf(double A=0,double B=0) : a(A),b(B) {
		previous_in = previous_out = out = 0 ; }
		//! Input new sample and calculate output
		Numeric clock(Numeric input) {
			// Shift previous outputs and calculate new output */
			out = ((1-b)*previous_out + (a+1)*input + (a-1)*previous_in)/(1+b);
			previous_out = out;
			previous_in = input;
			return(out);
		}
		//! Reset
		void reset() {
			previous_in = previous_out = out = 0;
		}
 };                                               
} // namespace SPUC 
#endif

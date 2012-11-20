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
#ifndef IIR_2NDHPF
#define IIR_2NDHPF
namespace SPUC {
//! \brief  Template Class for 2nd Order high-pass iir filter
//! \ingroup iir IIR filters
//
//!  The filter is assumed to be a high pass 2nd order digital filter 
//!   of the form
//!   <p>\f$ G(s) = \frac{s^2}{(s^2 + B*s + A)} in the "s" domain \f$
//!   which becomes the form 
//!   <p>\f$ G(z) =  \frac{1 - 2*z + z^2}{(1-B+A) + (2*A-2)*z + (1+A+B)*z^2} \f$ 
//!   <p>in the "z" domain
//! \image html iir_2nd.gif
//! \image latex iir_2nd.eps
template <class Numeric> class iir_2ndhpf
{
    protected:   
    	Numeric b0,b1,b2;                    
    	Numeric a1,a2;
        Numeric in[3];
        Numeric out[3]; 
        
    public:
        iir_2ndhpf(Numeric A, Numeric B) {
			double g = 1.0/(1.0+A+B);
			b0 = g;
			b1 = -2*g;
			b2 = g;
			a1 = 2*(A-1)*g;
			a2 = (1-B+A)*g;
        	in[0] = in[1] = in[2] = out[2] = out[1] = out[0] = 0 ; }
        void reset() {
        	in[0] = in[1] = in[2] = out[2] = out[1] = out[0] = 0 ; } 
//! Print out coefficients
void print() {
    printf("IIR Coefficients B0 = %lf, B1 =  %lf, B2 = %lf",b0,b1,b2);
    printf(" A0 = 1, A1 = %lf, A2 = %lf\n",a1,a2);
}
//! Input new sample and calculate output
Numeric clock(Numeric input) {
	// Shift inputs by one time sample and place new sample into array
	in[0] = in[1];
	in[1] = in[2];
	in[2] = input;
	// Shift previous outputs and calculate new output */
	out[0] = out[1];
	out[1] = out[2];
	out[2] = b0*in[2] + b1*in[1] + b2*in[0] - a1*out[1] - a2*out[0];
	return(out[2]);
}
};                                               
} // namespace SPUC 
#endif

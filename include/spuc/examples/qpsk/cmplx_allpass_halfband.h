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
#ifndef CMPLX_ALLPASS_HALFBAND
#define CMPLX_ALLPASS_HALFBAND
#include <allpass_halfband.h>
namespace SPUC {
//! \author Tony Kirke,  Copyright(c) 2001 
//! 
//! \brief Complex version of allpass halfband
//! \ingroup examples
class cmplx_allpass_halfband
{
	public:
		allpass_halfband<double> A_i,A_q;
        
		cmplx_allpass_halfband() : A_i(0.25,0.75), A_q(0.25,0.75) { }

		complex<long> clock(complex<long> input) {
			return( round(complex<double>(A_i.clock(input.re),A_q.clock(input.im)),0) );
		}
		char ready(void) { return(A_i.even); };

};                                    
} // namespace SPUC 
#endif

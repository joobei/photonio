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
#ifndef complex_CSD_ALLPASS_HALFBAND
#define complex_CSD_ALLPASS_HALFBAND
#include <csd_allpass_halfband.h>
namespace SPUC {
//! \brief Complex version of csd_allpass_halfband
//! \author Tony Kirke,  Copyright(c) 2001 
//! 
class cmplx_csd_allpass_halfband
{
	public:
		csd_allpass_halfband A_i,A_q;
            
		complex<long> clock(complex<long> input) {
			return( complex<long>(A_i.clock(input.re),A_q.clock(input.im)) );
		}
		char ready(void) { return(A_i.ready); };

};
                                          
} // namespace SPUC 
#endif

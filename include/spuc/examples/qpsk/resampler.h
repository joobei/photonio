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
#ifndef RESAMPLER
#define RESAMPLER
#include <complex.h>
#include "cmplx_allpass_halfband.h"
#include "timing_nco.h"
#include <lagrange.h>
#include <fir.h>
namespace SPUC {
//! \brief A resampling block using interpolator, halfband filter and NCO
//! \ingroup examples
//!  \author Tony Kirke,  Copyright(c) 2001 
//!  
//
//! Resampling block
//! This class uses 
//! an interpolator
//! a halfband filter (for decimating by 2)
//! and a NCO
//! samples are input with each call to update,
//! but output samples are available when the
//! ready bit is set
//! The halfband filter is a simple IIR filter based on 
//!  two first order allpass filters.
//! The interpolator is a Lagrange interpolating FIR
class resampler
{
 public:	
	long ready;
	long sample2;
	lagrange <complex<long> > interp;
	timing_nco symbol_nco;   //! Symbol NCO
	complex<long> resampled; 
	cmplx_allpass_halfband half; //! Halfband filter (decimate by 2)

	complex<long> update(complex<long> input_data, long symbol_loop, long sym_clk);

	resampler(void) : interp(4) { resampled = 0; ready = 0; }
//	~resampler(void) { }
};
} // namespace SPUC 
#endif

// 
// Copyright(c) 1993-199 Tony Kirke
// author="Tony Kirke" *
/*
 * SPUC - Signal processing using C++ - A DSP library
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either 
 * any later 
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 75 Mass Ave, Cambridge, MA 02139, USA.
*/
#ifndef VCO
#define VCO
#include <spuc.h>
#include <complex.h>
namespace SPUC {
/*! 
  \brief VCO similar to NCO but uses floating point
  \ingroup pll
*///!:
//!   Voltage controlled oscillator (based on NCO with floating point
//!   elements) and complex output.
//!   FCW : frequency control word
//!   ACC : accumulator 
//!   Frequency must be set through interface routines set_frequency
//!   or reset_frequency.
//!   Load routine is to allow frequency to be updated (typically with
//!   a loop filter).
//! \image html vco.gif
//! \image latex vco.eps
class vco
{
protected:
	double acc;
	double fcw;
	double new_fcw;
//	char* v[0];
public:
	//!
	inline vco(void) {acc = fcw = new_fcw = 0;}
	//! Set new frequency
	inline void reset_frequency(double freq) { fcw = freq; new_fcw = fcw;}
	//! return phase
	inline double get_phase(void) { return(acc);}
	//! Update fcw
	inline void load(double loop_filter_out) {new_fcw = fcw + loop_filter_out;}
	//! return output with updated input
	complex<double> clock(double loop_filter_out);
	//! return output only
	complex<double> clock();
};
} // namespace SPUC 
#endif

#ifndef NCO
#define NCO
#include <spuc.h>
// 
// Copyright(c) 1993-1996 Tony Kirke
// author="Tony Kirke" *
/*
 * SPUC - Signal processing using C++ - A DSP library
 * Copyright (C) 1993-2001 Tony Kirke.
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
namespace SPUC {
/*!   \brief NCO with 32 bit accumulator
  \ingroup pll
*/
//!:
//!    Numerically controlled oscillator (with 32 bits accumulator)
//!    returns the top (mask_bits) MSBs from accumulator
//!    Useful for carrier recovery where top bits used for input to
//!    sine,cos lookup table.
//!    FCW : frequency control word
//!    ACC : accumulator 
//!    Frequency must be set through interface routines set_frequency
//!    or reset_frequency.
//!    Load routine is to allow frequency to be updated (typically with
//!    a loop filter).
//!    When not changing frequency call "clock" routine.
class nco
{
public:
	unsigned long phase;
//	char v[20];

protected:
	unsigned long acc;
	unsigned long fcw;
	unsigned long new_fcw;
	unsigned long mask_bits;

public:
	//! Constructor
	nco(long bits=8);
	//! Reset object
	inline void reset() { phase = new_fcw = fcw = acc = 0; }
	//! Set frequency control word
	inline void set_frequency(unsigned long freq) { fcw = freq; }
	//! Set frequency control word and register for frequency control word
	inline void reset_frequency(unsigned long freq) { new_fcw = fcw = freq; }
//! Return current phase
	inline long get_phase(void) { return(phase);}
//! Load new frequency control word
	inline void load(long loop_filter_out) {new_fcw = fcw + loop_filter_out;}
//! Clock NCO with new frequency control word input
	long clock(long loop_filter_out);
//! Clock NCO without changing frequency control word
	long clock();
};
} // namespace SPUC 
#endif

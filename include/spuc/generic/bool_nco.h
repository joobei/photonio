// 
// Copyright(c) 1993-1996 Tony Kirke
// author="Tony Kirke"
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
#ifndef BNCO
#define BNCO
#include <spuc.h>
#include <complex.h>
namespace SPUC {
/*! 
  \brief  NCO with single bit output
  \ingroup pll
*/
//!
//! NCO that returns a single bit indicating whether the
//! internal accumulator has overflowed.
//! Internal accumulator is 31 bits.
class bool_nco
{
 public:
  bool phase;

 protected:
  unsigned long acc;
  unsigned long fcw;
  unsigned long new_fcw;

 public:
  bool_nco() {acc = fcw = new_fcw = phase = 0;}
  inline void reset() { phase = 0; new_fcw = fcw = acc = 0; }
  inline void set_frequency(unsigned long freq) { fcw = freq; }
  inline void reset_frequency(unsigned long freq) { new_fcw = fcw = freq; }
  inline bool get_phase(void) { return(phase);}
  inline void load(long loop_filter_out) {new_fcw = fcw + loop_filter_out;}
  bool clock(long loop_filter_out);
  bool clock();
};
} // namespace SPUC 
#endif

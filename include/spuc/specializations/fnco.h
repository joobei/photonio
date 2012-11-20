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
#ifndef FNCO
#define FNCO
#include <spuc.h>
#include <uint.h>
namespace SPUC {
//! \ingroup sim
//! \brief Template class for a NCO based on template unsigned int class
//!  \author Tony Kirke,  Copyright(c) 2001 
//!  
template <int M> class fnco
{
 public:
  uint<M> phase;
//  char v[20];

protected:
  uint<M> fcw;
  uint<M> new_fcw;

public:
  fnco() {
	  fcw = new_fcw = phase = 0;
  }
  inline void set_frequency(uint<M> freq) { fcw = freq; }
  inline void reset_frequency(uint<M> freq) { new_fcw = fcw = freq; }
  inline uint<M> get_phase(void) { return(phase);}
  inline void load(uint<M> loop_filter_out) {new_fcw = fcw + loop_filter_out;}
  uint<M> clock(uint<M> loop_filter_out);
  uint<M> clock();
};
} // namespace SPUC 
#endif

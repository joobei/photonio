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
#include "resampler.h"
namespace SPUC {
complex<long> resampler::update(complex<long> input_data, long symbol_loop, long sym_clk) 
{
  double time1,time2;
  complex<long> resampled1,resampled2;
  complex<long> half_out;

  // Sampling NCO
  // Determine if 2nd sample should be processed
  sample2 = symbol_nco.run(symbol_loop,sym_clk);

  time1 = symbol_nco.get_off1();
  time2 = symbol_nco.get_off2();
  resampled1 = interp.update(input_data,time1);
  resampled2 = interp.rephase(time2);

  half_out = half.clock(resampled1);
  if (half.ready()) {
	ready = 1;
	resampled = half_out;
  } else {
	ready = 0;
	resampled = 0;
  }
  
  // if 2nd sample should be used, send it through
  // the decimating halfband filter,
  // then check is output is ready
  if (sample2) {
	half_out = half.clock(resampled2);
	if (half.ready()) {
	  ready = 1;
	  resampled = half_out;
	} 
  }

  return(resampled);
}
} // namespace SPUC 

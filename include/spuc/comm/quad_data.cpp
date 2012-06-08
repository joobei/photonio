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
#include "quad_data.h"
namespace SPUC {
quad_data::quad_data(int total_over) : rcfir(12*total_over+1), pn_i(0x006d, 63, -1),
  pn_q(0x074d, 1023, -1), data(1,1), interp(4) {
	void root_raised_cosine(fir<complex<double> >& rcfir, double alpha, int rate);

  over = total_over;
  rcfir.set_rate(over);
  rcfir.set_automatic();
  root_raised_cosine(rcfir, 0.35, total_over);
  prev_timing_offset = 0.0;
}
complex<double> quad_data::get_fir_output(void)	{
  if (rcfir.phase==0) {
#ifndef NO_QUAD_TX_DATA
	data = complex<double>(pn_i.out(),pn_i.out());
#endif
	rcfir.input(data);
  }
  return(rcfir.clock());
}
complex<double> quad_data::get_sample(double timing_inc) 
{
  // timing inc is in units of total_over oversampling rate
  // i.e 1 corresponds to 1/total_over of a symbol
  if (timing_inc < 0) timing_inc = 0; // Timing_inc should
  // not be negative in the first place!
  double next_timing_offset = prev_timing_offset + timing_inc;
  while (next_timing_offset >= 1.0) {
	next_timing_offset -=  1.0;
	interp.input(get_fir_output());
  };

  prev_timing_offset = next_timing_offset;
  return(interp.rephase(next_timing_offset));
}

} // namespace SPUC 

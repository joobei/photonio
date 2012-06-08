// 
// author="Tony Kirke"
// Copyright (c) 1993-1996 Tony Kirke
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
#include <lms_dfe.h>
//:
// LMS Decision Feedback equalizer
//
namespace SPUC {
lms_dfe::lms_dfe(char inf, char inb, double gain) : u(gain) {
  ff.set_size(inf);
  ff.set_gain(gain);
  if (inb > 0) {
	  fb.set_size(inb);
	  fb.set_gain(gain);
	  ff.coeff[inf-1] = 1;
  } else {
	ff.coeff[inf/2] = 1;
  }
}
void lms_dfe::reset() {
  ff.reset();
  if (ff.num_taps > 0) {
	if (fb.num_taps > 0) {
	  fb.reset();
	  ff.settap(ff.num_taps-1,1);
	} else {
	  ff.settap(ff.num_taps/2,1);
	}
  }
}
// Perform convolutions
complex<double> lms_dfe::update(complex<double> in, complex<double> slicer_output) {    
  complex<double> sff=0;
  sff = ff.update(in);
  if (fb.num_taps > 0) sff += fb.update(slicer_output);
  return(sff);
}
// Update coefficients
void lms_dfe::update_taps_lms(complex<double> err) {
  ff.update_lms(err);
  if (fb.num_taps > 0) fb.update_lms(err);
}
} // namespace SPUC 

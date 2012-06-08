// 
// author="Tony Kirke"
// Copyright (c) 1993-1996 Tony Kirke
//
/* 
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
#ifndef LMS_DFE
#define LMS_DFE
#include <complex.h>
#include <fir_adapt.h>
namespace SPUC {
/*! 
  \addtogroup equalizers Equalization Classes
 */
//! \ingroup equalizers comm
//! \brief LMS Decision Feedback equalizer
//! \image html lms_dfe.gif
//! \image latex lms_dfe.eps
class lms_dfe {
 public:
  //! Feedforward FIR section
  fir_adapt < complex<double> >  ff;
  //! Feedback FIR section
  fir_adapt < complex<double> >  fb;
  //! Adaptation Gain
  double u;

 public:
  //! Constructor with feedforward size, feedback size and gain
  lms_dfe(char inf=16, char inb=0, double gain=0.01);
  ~lms_dfe() { }
  void reset();
  void set_ff_tap(long i,complex<double> x) { ff.coeff[i] = x; }
  complex<double> get_ff_tap(long x) { return (ff.coeff[x]); }
  complex<double> get_fb_tap(long x) { return (fb.coeff[x]); }
  long ff_size() { return (ff.num_taps); }
  long fb_size() { return (fb.num_taps); }
  void set_gain(double g) {
	  u = g;
	  ff.set_gain(u);
	  fb.set_gain(u);
  }
  //! Perform convolutions
  complex<double> update(complex<double> in, complex<double> slicer_output); 
  //! Update coefficients
  void update_taps_lms(complex<double> err);
};
} // namespace SPUC 
#endif

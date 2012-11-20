// 
// author="Tony Kirke"
// Copyright (c) 2001 Tony Kirke
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
#ifndef MLSD
#define MLSD
#include <complex.h>
#include <delay.h>
#include <fir_adapt.h>
namespace SPUC {
/*! 
  \addtogroup equalizers Equalization Classes
 */
//! \ingroup equalizers comm
//! \brief Maximum Likelihood Sequence Detection
//! assuming binary alphabet [1,-1]
class mlsd {
 public:
  fir_adapt < complex<double> >  cfir;  /// Adaptive FIR section
  long num_taps; /// Number of taps
  long n_states; /// Number of states
  long* path;    /// Path history
  double* weight; /// Statemetrics
  double* tmp_weight; /// temporary
  long* tmp_path;     /// temporary

 public:
  /// Constructor with feedforward size
  mlsd(char inf=3);
  /// Destructor
  ~mlsd() {
	  delete [] tmp_path;
	  delete [] tmp_weight;
	  delete [] path;
	  delete [] weight;
  }
  /// Reset
  void reset() {
	cfir.reset();
  }
  /// Update coefficients
  void update_taps_lms(complex<double> err) {
	cfir.update_lms(err);
  }
  complex<double> tap0(void) { return(cfir.coeff[0]); }
  /// Convolve CFIR with branch sequence for estimate
  complex<double> estimate(long seq);
  /// Do sequence estimation
  long seq_estimation(complex<double> y);
};
} // namespace SPUC 
#endif

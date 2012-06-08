// Copyright (c) 1993-1996 Tony Kirke
// 
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
#ifndef FIRA
#define FIRA
#include <fir.h>
namespace SPUC {
/*! 
  \addtogroup fir FIR filters
*/
//! \brief template class fir_adapt Based on FIR class, created to support LMS adaptive filtering
//! \ingroup fir equalizers
//! \image html fir_adapt.gif
//! \image latex fir_adapt.eps
template <class Numeric> class fir_adapt : public fir<Numeric>
{
 public: 
  //! Adaptation gain
  double u;
    
 public: 
  //! Constructor
  fir_adapt<Numeric>(void) { u=0.01;	};
  //! Constructor with size
  fir_adapt<Numeric>(long n, double gain=0.01) : u(gain), fir<Numeric>(n) {	};
  //! Reset both history and coefficients!
  void reset() {
	fir<Numeric>::reset();
	for (int i=0;i<fir<Numeric>::num_taps;i++) fir<Numeric>::coeff[i] = 0;
  }
  //! Set adaptation gain
  void set_gain(double gain) { u = gain;}
  //! Calculate updated taps using LMS algorithm
  void update_lms(Numeric err) {
	for (int i=0;i<fir<Numeric>::num_taps;i++) fir<Numeric>::coeff[i] += u*err*conj(fir<Numeric>::z[i]);
  }
  //! Calculate updated taps using LMS algorithm and sign of input
  void update_lms_sign_coeff(Numeric err) {
	for (int i=0;i<fir<Numeric>::num_taps;i++) fir<Numeric>::coeff[i] += u*err*signbit(conj(fir<Numeric>::z[i]));
  }
};	
} // namespace SPUC 
#endif

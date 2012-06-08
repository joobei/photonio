// 
// author="Tony Kirke"
// Copyright(c) 1993-1996 Tony Kirke
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
#include <math.h>
#include <spuc.h>
#include <complex.h>
#include <fir.h>
#include <fir_interp.h>
namespace SPUC {
//:
/*! 
  \addtogroup fir FIR filters
*/
//! \brief calculates the sampled Gaussian filter impulse response
//! \ingroup fir
void gaussian_fir(fir<double>& gaussf, double bt, double spb)
{
  // generate the sampled Gaussian filter impulse response
  // and transfer coefficients to "gaussf".
  // spb   = samples per bit (at full rate)
  // BT    = filter bandwidth
  int i;
  double dt, s, t0, ts, gsum;
  int nir = gaussf.num_taps;
  double gf[nir];
  dt = 1.0/spb;
  s = 1.0/(sqrt(log(2.0)) / (TWOPI*bt));
  t0 = -0.5 * nir;
  gsum = 0.0;	// normalize to unit sum
  for (i=0; i<nir; i++) {
    t0++;
    ts = s*t0;
    gf[i] = ::exp(-0.5*ts*ts);
    gsum += gf[i];
  }
  // Unity DC gain
  gsum = 1.0/gsum;
  for (i=0; i<nir; i++) gaussf.coeff[i] = gf[i]*gsum;
}
} // namespace SPUC 

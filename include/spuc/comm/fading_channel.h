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
#include <complex.h>
#include <fir.h>
#include <noise.h>
#ifndef FADECH
#define FADECH
namespace SPUC {
/*!  
  \addtogroup sim Simulation Classes
 */
/*!  \brief  A Class for simulating a FIR channel model
  \author Tony Kirke,  Copyright(c) 2001 
  
  \ingroup sim comm
*/
//!  FIR Channel Model.
//!  Exponential decay delay line channel model
//!  Mean channel power is normalized
//!  Profiles are generated using gaussian pdf
class fading_channel 
{
 public:
	fir<complex<double> >  exp_decay;
	long taps;
	double delay_spread;
	noise tap_gain;

	// constructor
	fading_channel(long paths=1, double norm_delay_spread=1) {
		taps=paths;
		delay_spread = norm_delay_spread;
		exp_decay.set_size(taps);
		generate_channel();
	}
	void setup(double norm_delay_spread);
	void generate_channel();
	complex<double> update(const complex<double> s);
};
}
#endif

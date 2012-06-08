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
#ifndef TN
#define TN
namespace SPUC {
#define MAX_STEP 4
#define STEP_BITS 3
#define MASK_HI 65535
#define MASK_HIx 63535
#define MASK_LO  65536
#define MASK_LOd2  32768
#define BITS_USED 16
//! \brief A NCO for symbol recovery in a variable rate QPSK receiver
//! \ingroup sim examples
//!  \author Tony Kirke,  Copyright(c) 2001 
//!  
//
//! Since interpolation is being done, either one or two
//! samples must be calculated on each update.
//! When sample2 is true, there are 2 samples available.
class timing_nco
{
 protected:
	signed long resid1;
	signed long acc;
	unsigned long fcw;
	unsigned long new_fcw;
	signed long phase1;
	signed long phase2;
	signed long new_phase;
	long sample2;
public:
	inline timing_nco(void) {
		resid1 = 0;
		acc = MASK_LOd2;
		fcw = new_fcw = phase1 = phase2 = 0; 
		sample2 = 1;}
	inline void set_frequency(unsigned long freq) { fcw = freq; }
	inline void reset_frequency(unsigned long freq) { new_fcw = fcw = freq; }
	inline double get_off1(void) { return((double)phase1/MASK_LO);}
	inline double get_off2(void) { return((double)phase2/MASK_LO);}
	inline long get_phase1(void) { return(-MAX_STEP+phase1);}
	inline long get_phase2(void) { return(-MAX_STEP+phase2);}
	long run(long loop_filter_out=0, int load=1);
	signed long residual_phase(signed long acc_value);
};
#endif
} // namespace SPUC 

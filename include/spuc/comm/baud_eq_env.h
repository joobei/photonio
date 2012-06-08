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
// 
//#include <stdio.h>
//#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <max_pn.h>   	
#include <noise.h>     
#include "bpsk_ber_test.h"
#include <fading_channel.h>
#include <mle.h>
namespace SPUC {
/*! 
  \addtogroup sim Simulation Classes
 */
/*!  \brief  Class for doing a simulation of an equalizer running at 1 sample/symbol
  \author Tony Kirke,  Copyright(c) 2001 
  
  \ingroup sim
 */
class baud_eq_env
{
	public:
  
		bpsk_ber_test* BER_mon;
		max_pn* tx_data_source;
		noise* n;
		mle<complex<double> >* DUT;
		fading_channel* multipaths;

		long num;
		double var;
		double snr;
		long data; // output of equalizer
		long output_delay; // Equalizer output delay (for paths to merge)
		long eq_type; // type of equalizer used
		// 0 MLSE, 1 DDFSE

		complex<double> base; 
		complex<double> main; 
		complex<double> b_noise;  // Noise
		long rcv_symbols;       // Number of symbols decoded
		long count;  			// index of sample number at input rate
		long paths; // Number of multipaths in channel model
		long phase_acc;

		baud_eq_env(long paths=1);
		void loop_init(long eq_type, long data_delay, double dly_spread);
		void step(void);
		void loop_end(void);
};	                 
}

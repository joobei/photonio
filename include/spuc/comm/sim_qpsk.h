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
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <max_pn.h>   	
#include <noise.h>     
#include <vco.h>   	
#include <fir_interp.h> 
#include "quad_data.h"
#include "qpsk_ber_test.h"
#include "qpsk.h"

namespace SPUC {
/*! 
	  \addtogroup sim Simulation Classes
*/
/*! \brief  A Class for simulating a QPSK system 
  \author Tony Kirke,  Copyright(c) 2001 
  
  \ingroup sim
*/
//!
//! A Class for simulating a QPSK system that includes
//! transmitters, receivers, A/D, frequency offsets,
//! gaussian noise, and a BER tester
class sim_qpsk
{
	public:
  
		qpsk_ber_test* BER_mon;
		quad_data* tx_data_source;
		vco* freq_offset;
		noise* n;
		qpsk* RECEIVER;      
		a_d* ADC;

		long num;
		double var;
		double snr;
		double timing_offset;
		long total_over;

		complex<double> base; 
		complex<double> main; 
		complex<double> b_noise;  // Noise
		complex<long> adc_out;
		long rcv_symbols;       //! Number of symbols decoded
		long count;  			//! index of sample number at input rate
		double resample_over ;
		double nominal_scale;
		double actual_over;
		double tx_time_inc;
		int rc_delay;
		long symbol_nco_word;

		sim_qpsk(void);
		void loop_init(double actual_over,  double time_offset=0, long adj = 0);
		void step(void);
		void loop_end(void);
};	                 

} // namespace SPUC 

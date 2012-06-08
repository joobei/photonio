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
#include "qpsk_variable.h"
namespace SPUC {
/*!  
  \addtogroup sim Simulation Classes
*/
//! \brief  A Class for simulating a variable rate QPSK system 
//!  \ingroup sim examples
//
//! \detailed A Class for simulating a variable rate QPSK system 
//! that includes
//! transmitters, receivers, frequency offsets,
//! gaussian noise, and a BER tester
//! Based on sim_qpsk with some minor changes.
class sim_qpsk_variable
{
 public:
	qpsk_ber_test* BER_mon;
	quad_data* tx_data_source;
	vco* freq_offset;
	noise* n;
	qpsk_variable* RECEIVER;      

	long num;
	double var;
	double snr;
	double timing_offset;
	long total_over;

	complex<double> data; 
	complex<double> base; 
	complex<double> main; 
	complex<double> b_noise;                // Noise
	
	double sum_s;                       // Signal sum for Es/No estimation
	double sum_n;                       // Noise sum for Es/No estimation
	long rcv_symbols;         		  // Number of symbols decoded
	long count;  					  // index of sample number at input rate
	int dec_rate_log;
	double resample_over ;
	// AGC stuff
	double agc_scale;
	double nominal_scale;
	double analog_agc;
	double analog_filter_gain;
	double analog_agc_gain;
	//

	double actual_over;
	double tx_time_inc;
	int rc_delay;
	long symbol_nco_word;

	sim_qpsk_variable(void);
	void loop_init(double actual_over,  double time_offset=0);
	void step(void);
	void loop_end(void);
};	                 

} // namespace SPUC 

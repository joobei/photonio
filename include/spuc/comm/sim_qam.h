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
#include <fading_channel.h>
#include <fir_interp.h>
#include <lagrange.h>
//#include "qam_demap.h"
#include "qam_tx.h"
#include "base_demod.h"
namespace SPUC {
/*! 
  \addtogroup sim Simulation Classes
*/
/*! \brief  A Class for simulating a QAM system 
  \author Tony Kirke,  Copyright(c) 2001 
  
  \ingroup sim
*/
//!
//! A Class for simulating a QAM system that includes
//! transmitters, receivers, A/D, frequency offsets,
//! gaussian noise, and a BER tester
class sim_qam {
	public:
  
	qam_tx TX;
	noise* n;
	base_demod* DUT;
	vco* freq_offset;
	fir<complex<double> > rx_filter;
	lagrange<complex<double> > interp;
	double var;
	double snr;
	double channel_pwr;
	complex<double> tx_data; 
	complex<long> data; 
	long output_delay; // Equalizer output delay (for paths to merge)
	long rate;

	bool enable_freq_offset;
	bool enable_time_offset;
	double carrier_offset_rate;
	double time_inc;
	double time_offset;


	complex<double> base; 
	complex<double> main; 
	complex<double> main1, base1;
	
	complex<double> b_noise;  // Noise
	long rcv_symbols;       // Number of symbols decoded
	long tx_symbols;       // Counter for transmitted symbols
	long count;  			// index of sample number at input rate
	double phase_inc;
	double phase_acc;
	const long over; // Oversampling rate
	double alpha;

	sim_qam(double tx_filter_bw=0.25);
	void loop_init(long rate, long conv_rate, 
				   double carrier_off=0, double time_off=0);
	complex<long> step();
	~sim_qam() {
		if (n) delete n;
	}
	void loop_end(void);
};	                 
}


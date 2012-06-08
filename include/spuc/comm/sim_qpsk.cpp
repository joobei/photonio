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
// $Id: sim_qpsk.cpp,v 1.3 2005/09/16 17:00:45 spuc Exp $
#include "sim_qpsk.h"
namespace SPUC {
sim_qpsk::sim_qpsk(void) 
{
	snr = 6.0;
	timing_offset = 0.0;
	base = complex<double>(0,0);
	rcv_symbols = 0;
	count = 0;
	resample_over = 0;
	tx_time_inc =0;
	rc_delay = 0;
	symbol_nco_word=0;
#ifdef NEWNOISE
	n = new noise;
#endif
}
void sim_qpsk::loop_init(double actual, double time_offset, long adj_chan)
{
	void root_raised_cosine(fir<double> rcfir, double alpha, int rate);
	actual_over = actual;
	total_over = (int)actual_over;	 // Nearest integer oversampling rate
	// Timing Increment (in 1/total_over samples) for tx
	tx_time_inc = total_over/actual_over;
	resample_over = actual_over;
	var = sqrt(0.5*actual_over)*pow(10.0,-0.05*snr);   // Unfiltered noise std dev	
	BER_mon = new qpsk_ber_test;
	tx_data_source = new quad_data(total_over);
	freq_offset = new vco;
	RECEIVER = new qpsk;
	ADC = new a_d(6);
#ifndef NEWNOISE
	n = new noise;
#endif	
	tx_data_source->set_initial_offset(time_offset);
	freq_offset->reset_frequency(-TWOPI/(actual_over*4000.0));

	rcv_symbols=0;  		  // Number of symbols decoded
	count=0;    			 // index of sample number at input rate

	// QPSK Receiver Setup
#ifndef NOTIME
	resample_over *= 1.0001; // 100 ppm timing error (internal clock faster than reference)
#endif
	// This should be related to total_over + offset
	symbol_nco_word = (long)floor(resample_over*(1 << 14));
	//	RECEIVER->rate_change.symbol_nco.reset_frequency(symbol_nco_word);
}
// STEP
void sim_qpsk::step(void) 
{
  count++;  
  // Get new sample from transmitter
  base = tx_data_source->get_sample(tx_time_inc);
  // Analog signal + noise + Up conversion
#ifndef NOFREQ
  base *= freq_offset->clock();
#endif
  // Noise term
  b_noise = var*n->Cgauss();
  // Add noise
  base += b_noise;
  // AGC
  base *= 20;			
  // A/D
  adc_out = ADC->sample(base);
  // Clock IC
  RECEIVER->clock(adc_out);
  if (RECEIVER->symclk()) rcv_symbols++;
}	
void sim_qpsk::loop_end(void)
{
  delete BER_mon;
  delete tx_data_source;
  delete freq_offset;
  delete ADC;
#ifndef NEWNOISE
  delete n;    
#endif
  delete RECEIVER;
}                                      

} // namespace SPUC 

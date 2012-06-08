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
#include "sim_qpsk_variable.h"
namespace SPUC {
sim_qpsk_variable::sim_qpsk_variable(void) 
{
	snr = 6.0;
	timing_offset = 0.0;
	data = complex<double>(1,1);
	base = complex<double>(0,0);
	rcv_symbols = 0;
	count = 0;
	dec_rate_log=0;
	resample_over = 0;
	tx_time_inc =0;
	rc_delay = 0;
	symbol_nco_word=0;
	sum_s=sum_n=0;
#ifdef NEWNOISE
	n = new noise;
#endif
}
void sim_qpsk_variable::loop_init(double actual, double time_offset)
{
  void root_raised_cosine(fir<double> rcfir, double alpha, int rate);
  actual_over = actual;
  total_over = (int)actual_over;		 // Nearest integer oversampling rate
  tx_time_inc = total_over/actual_over;  // Timing Inc (in 1/total_over samples) for tx
  dec_rate_log = (int)floor(log(actual_over/2.0)/log(2.0));
  resample_over = actual_over/(1 << dec_rate_log);
  var = sqrt(0.5*actual_over)*pow(10.0,-0.05*snr);   // Unfiltered noise std dev	
  BER_mon = new qpsk_ber_test;
  tx_data_source = new quad_data(total_over);
  freq_offset = new vco;
  RECEIVER = new qpsk_variable;
#ifndef NEWNOISE
  n = new noise;
#endif	
  tx_data_source->set_initial_offset(time_offset);
  
  freq_offset->reset_frequency(-TWOPI/(actual_over*1000.0));
  /////freq_offset->acc = -0.2783;
  // nominal_scale = 20.0*pow(actual_over,-0.2);
  nominal_scale = 20; ///////////////////////////////
  agc_scale = nominal_scale; // initialization
  analog_agc = 0;
  analog_agc_gain = 0.0002;
  analog_filter_gain = 1-analog_agc_gain;

#ifdef _DEBUG
  sum_s=0;                      // Signal sum for Es/No estimation
  sum_n=0;                      // Noise sum for Es/No estimation
#endif
  rcv_symbols=0;     		  // Number of symbols decoded
  count=0;  						 // index of sample number at input rate

  
  // QPSK Receiver Setup
#ifndef NOTIME
  resample_over *= 1.0001; // 100 ppm timing error (internal clock faster than reference)
#endif
  symbol_nco_word = (long)floor(resample_over*(1 << 14)); // This should be related to total_over + offset
  RECEIVER->rate_change.symbol_nco.reset_frequency(symbol_nco_word);
  // Change Carrier loop gain (from default) based on oversampling rate.
  RECEIVER->carrier_loop_filter.k0 -= dec_rate_log;
  RECEIVER->carrier_loop_filter.k1 -= dec_rate_log;
}
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void sim_qpsk_variable::step(void) 
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
  // Statistics
#ifdef _DEBUG
  sum_s += magsq(base);   
  sum_n += magsq(b_noise);
#endif
  // Add noise
  base += b_noise;
  // AGC
  base *= agc_scale;			
  // Clock IC
  RECEIVER->clock(base);
  if (RECEIVER->symclk()) rcv_symbols++;
  
  // Analog AGC circuitry
//  analog_agc = analog_filter_gain*analog_agc + analog_agc_gain*(2*RECEIVER->agc_out()-1); 
//  agc_scale += 0.01*analog_agc; // integrator!
}	
void sim_qpsk_variable::loop_end(void)
{
  //	BER_mon->final(rcv_symbols);
  delete BER_mon;
  delete tx_data_source;
  delete freq_offset;
#ifndef NEWNOISE
  delete n;    
#endif
  delete RECEIVER;
}                                     

} // namespace SPUC 

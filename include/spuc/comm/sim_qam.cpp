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
// $Id: sim_qam.cpp,v 1.2 2005/09/16 17:00:44 spuc Exp $
#include "sim_qam.h"
#include <spuc.h>
using namespace SPUC;
//----------------------------------------------------------------------------
// Constructor!
//---------------------------------------------------------------------------
sim_qam::sim_qam(double rc_alpha)
  :  over(4),
	 alpha(rc_alpha),
	 interp(4),
	 rx_filter(12*4+1),
	 TX(12, 4, 0, 0, rc_alpha)
{
  void root_raised_cosine(fir<complex<double> >& rcfir, double alpha, int rate);
  snr = 10.0;
  base = complex<double>(0,0);
  count = 0;
  output_delay = 0;
  n = new noise;

  enable_freq_offset = 0;
  enable_time_offset = 0;
  rate = 0; // default
  rcv_symbols=0; // Number of symbols decoded
  root_raised_cosine(rx_filter,alpha,over);
  double scale  = (1.0/double(over));
  for (int j=0;j<rx_filter.num_taps;j++) { rx_filter.coeff[j] *= scale; }
  channel_pwr = 1.0;
}
//---------------------------------------------------------------------------
// Initialize pointers
//---------------------------------------------------------------------------
void sim_qam::loop_init(long rate, long conv_rate,
						double carrier_off,  double time_off)
{
  void root_raised_cosine(fir<complex<double> >& rcfir, double alpha, int rate);
  output_delay = 0;
  var = sqrt(0.5*(double)over)*pow(10.0,-0.05*snr);   // Unfiltered noise std dev	

  TX.loop_init(rate, conv_rate);

  carrier_offset_rate = carrier_off;
  time_inc = 1.0 + (double)(time_off/1000000.0);
  if (carrier_off) enable_freq_offset=1;
  else enable_freq_offset = 0;
  if (time_off) enable_time_offset=1;
  else enable_time_offset = 0;

  freq_offset = new vco;
  freq_offset->reset_frequency(carrier_offset_rate);
  
  time_offset = 0;
  rcv_symbols=0;  		  // Number of symbols decoded
  count=0;    			 // index of sample number at input rate

  rx_filter.reset();
}
//---------------------------------------------------------------------------
// STEP
//---------------------------------------------------------------------------
complex<long> sim_qam::step() 
{

  // Get new sample from transmitter
  if (enable_time_offset) {
	time_offset += time_inc;
	while (time_offset >= 1.0) {
	  time_offset -= 1.0; 
	  interp.input(TX.clock());
	}
	base = interp.rephase(time_offset);
  } else {
	base = TX.clock();
  }
	// Apply Frequency offset
	if (enable_freq_offset) {
		complex<double> rot = freq_offset->clock();
		base *= rot;
	}
	// Noise term
	b_noise = var*n->Cgauss(); 
	base1 = base + b_noise;
	main1 = rx_filter.update(base1);
	data = DUT->step(main1);
	if (DUT->sym_pulse()) rcv_symbols++;
	return(data);
}	
//---------------------------------------------------------------------------
// Delete pointers
//----------------------------------------------------------------------------
void sim_qam::loop_end(void)
{
//  delete multipaths;
  delete freq_offset;
  interp.reset();
  rx_filter.reset();
}     


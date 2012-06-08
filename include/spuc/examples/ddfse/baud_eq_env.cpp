// This file is modified example of baud_eq_env.cpp in /spuc/comm directory
// baud_eq_env.h is the same

// version="$Revision: 1.1 $"  
// author="Tony Kirke" *
// Copyright(c) 2001 Tony Kirke
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
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;
#include "baud_eq_env.h"
#include <spuc.h>
using namespace SPUC;
baud_eq_env::baud_eq_env(long npaths) :  paths(npaths)
{
	snr = 10.0;
	base = complex<double>(0,0);
	rcv_symbols = 0;
	count = 0;
	output_delay = 0;
	eq_type = 0;
#ifdef NEWNOISE
	n = new noise;
#endif
}
void baud_eq_env::loop_init(long equalizer_type, long data_delay,
							double delay_spread)
{
	long mlse_len;
	output_delay = data_delay;
	eq_type = equalizer_type;
	var = sqrt(2.0)*pow(10.0,-0.05*snr);   // Unfiltered noise std dev	
	BER_mon = new bpsk_ber_test;
	tx_data_source = new max_pn;
	multipaths = new fading_channel(paths,delay_spread);

	// PICK EQUALIZER Type == DDFSE
	if (eq_type == 0) {
	  mlse_len = paths-1;
	  DUT = new mle<complex<double> >((char)(mlse_len));
	} else if (eq_type == 1) {
	  int dfe_size = 3;
	  mlse_len = paths-1 - dfe_size;
	  DUT = new mle<complex<double> >((char)(mlse_len),dfe_size);
	}
	multipaths->exp_decay.coeff[0] = 0.9;
	multipaths->exp_decay.coeff[1] = 0.1;

#ifndef NEWNOISE
	n = new noise;
#endif	
	// initialise CIR from Channel CIR
	DUT->set_cir(multipaths->exp_decay);
	rcv_symbols=0;  		  // Number of symbols decoded
	count=0;    			 // index of sample number at input rate
	phase_acc=0;
}
// STEP
void baud_eq_env::step(void) 
{
  count++;  
  // Get new sample from transmitter
    phase_acc += tx_data_source->out();
  //phase_acc++;
  base = expj(phase_acc*PI/2);
  // Pass through Channel model
  main = multipaths->update(base);
  // Noise term
  b_noise = var*n->Cgauss();
  // Add noise
  main += b_noise;
  // Clock equalizer
  if (eq_type == 0) {
	if (count>1) data = DUT->mlsd(main);
  } else if (eq_type == 1) {
	if (count>1) data = DUT->ddfse(main);
  }
  data = (data & MASK_BIT(output_delay)) ? 1 : -1;
  rcv_symbols++;
}	
void baud_eq_env::loop_end(void)
{
  delete BER_mon;
  delete tx_data_source;
  delete multipaths;
#ifndef NEWNOISE
  delete n;    
#endif
  delete DUT;
}                                      


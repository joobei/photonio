//! 
//! author="Tony Kirke" *
//! Copyright(c) 2001 Tony Kirke
// SPUC - Signal processing using C++ - A DSP library
/* 
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
// $Id: qam_tx.cpp,v 1.2 2005/09/16 17:00:44 spuc Exp $
#include <spuc.h>
#include "qam_tx.h"
using namespace SPUC;
//----------------------------------------------------------------------------
// Constructor!
//---------------------------------------------------------------------------
qam_tx::qam_tx(long sym_sp, long over_sam, long mod_rate, long conv_rate, double rc_alpha)
  :  
	over(over_sam), 
	alpha(rc_alpha),
	preamble_pn(63*4),
	training_source(0x074d,1023,-1),
    training_interval(2048),
	ENC(3),
	tx_filter(sym_sp*over_sam+1),
	rate(mod_rate)
{
  loop_init(mod_rate, conv_rate);
}
void qam_tx::loop_init(long mod_rate, long conv_rate)
{
  void root_raised_cosine(fir<complex<double> >& rcfir, double alpha, int rate);
  tx_data = 0;
  tx_symbols=0; // Counter for transmitted symbols
  rate = mod_rate;

  preamble_source.reset();
  ENC.rate_index = rate;
  if (rate == 0) {
	data_level = 1; //BPSK
	training_scale = 1;
  } else if (rate == 1) {
	data_level = sqrt(0.5); // QPSK
	training_scale = 1;
  } else if (rate == 2) {
	data_level = sqrt(0.1);
	training_scale = 3;
  } else if (rate == 3) {
	data_level = sqrt(1.0/42.0);
	training_scale = 5; ///////5;
  } else {
	data_level = 1;
	training_scale = 1;
  }

#ifdef NORM
  data_level = 1;
#endif
	
  tx_filter.reset();
  tx_filter.set_rate(over);
  tx_filter.set_automatic();
  root_raised_cosine(tx_filter,alpha,over);
//  double scale  = sqrt(1.0); ///double(over));
//  for (int j=0;j<tx_filter.num_taps;j++) { tx_filter.coeff[j] *= scale; }
	
  tx_symbols=0;
  count=0;   
  training_source.reset();
}
//---------------------------------------------------------------------------
// STEP
//---------------------------------------------------------------------------
complex<double> qam_tx::clock() 
{
  // Get new sample from transmitter
  if (count++%over == 0) {
	if (tx_symbols < preamble_pn) {
	  // Preamble is not scaled!!!
	  tx_data = 
		data_level*complex<double>(preamble_source.out(),0);
	} else if (tx_symbols < training_interval + preamble_pn) {
	  tx_data = training_scale*
		data_level*complex<double>(training_source.out(),0);
	} else {
	  tx_data = data_level*rational(ENC.data_map(rate,0));
	}
	tx_symbols++;
	tx_filter.input(tx_data);
  }
  return(tx_filter.clock());
}


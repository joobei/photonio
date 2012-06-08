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
// $Id: qpsk.cpp,v 1.4 2005/09/16 17:00:44 spuc Exp $
// Description: fixed rate QPSK running at 2 samples/symbol with
// data clock already synchronized
//
#include <raised_cosine.h>
#include "qpsk.h"
namespace SPUC {
qpsk::qpsk(void)  : rcv_sqrt_rc(9), final_baseband_delay(2),
	 hard_decision_delay(2), timing_disc_delay(3)
{
    void root_raised_cosine(fir<complex<long> >& rcfir, double alpha, int rate, int bits, double scale);
	//! alpha = 0.35 root raised cosine fir
	root_raised_cosine(rcv_sqrt_rc,0.35,2,8, -0.2);

	c_nco.reset_frequency(0);
	carrier_error = 0;
	timing_error = 0;
	carrier_loop_out = 0;
	carrier_loop_filter.k0 = 1 << 15; 
	carrier_loop_filter.k1 = 1 << 8;

	carrier_loop_filter.k0_en = 0;
	carrier_loop_filter.k1_en = 0;
	symbol_loop_out = 0;
	symbol_loop_filter.k0 = 1 << 6;
	symbol_loop_filter.k1 = 1;
	
	symbol_loop_filter.k0_en = 0;
	symbol_loop_filter.k1_en = 0;
	symbol_clk = 0;
	symbol_clk_pls = 0;
	symbol_x2_clk_pls = 0;
	sample_clk = 0;
	nda_timing_error = 0;
  }
//***********************************************************************
// MAIN ROUTINE
//***********************************************************************
void qpsk::clock(complex<long>adc_out)  {
  complex<long> carrier_phase;
  long nda=0; //! Don't use NDA timing discriminator

  //! Down conversion 
  if (symbol_clk_pls)	carrier_phase = c_nco.clock(carrier_loop_out);
  else 	carrier_phase = c_nco.clock();
  baseband = adc_out*carrier_phase;
  baseband = round(baseband,7);

  symbol_clk_pls = 0;
  symbol_x2_clk_pls = 0;
  resampled = baseband; //! Digital Interpolation/Resampling would go here
  symbol_x2_clk = 1;

  //! Processing at 2 times the symbol rate
  if (symbol_x2_clk) {
	symbol_x2_clk_pls = 1;
	//! input to matched filter & does calculation
	mf_in = resampled;
	mf_out = rcv_sqrt_rc.update(mf_in); 
	mf_out = round(mf_out, 6);
	//! Slicer - get sign bit prior to rounding!
	hard_decision = signbit(mf_out);
	symbol_clk = !symbol_clk;
	prev_sym = timing_disc_delay.input(mf_out);
	prev_sam = timing_disc_delay.checkback(1);
	decision = hard_decision;
	if (nda) timing_error = nda_symbol(prev_sym,mf_out);
	//! Symbol rate processing	
	if (symbol_clk) {
	  hard_decision_prev = hard_decision_delay.input(decision);
	  symbol_clk_pls = 1;
	  //! Matched Filter out
	  final_baseband = mf_out; 
	  //! Symbol discriminator
	  if (!nda) timing_error = dd_symbol(prev_sym,mf_out,
										 hard_decision_prev,
										 decision);
	  //! Carrier discriminator
	  carrier_error = qpsk_dd_phase(mf_out,decision);
	  //! Symbol + timing loop filters
	  symbol_loop_out = symbol_loop_filter.update(timing_error); 
	  carrier_loop_out = carrier_loop_filter.update(carrier_error);
	}	
  }
  return;
}
} // namespace SPUC 

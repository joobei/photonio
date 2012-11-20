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
#ifndef QPSK
#define QPSK
#include <spuc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <math.h>
//#include <limits.h>
#include <time.h>
#include <iostream>
#include <complex.h>
#include <delay.h>
#include <loop_filter.h>
#include <nco.h>
#include <a_d.h>
#include <fir.h>
#include <fir_interp.h>
#include <lagrange.h>
#include <discriminators.h>
//#include <raised_cosine.h>
#include "carrier_nco.h"
namespace SPUC {
/*! \brief A QPSK receiver that can operate at exactly 2 samples/symbol
  \author Tony Kirke,  Copyright(c) 2001 
  
  \ingroup comm examples
*/
//! A QPSK receiver that can operate at exactly 2 samples/symbol
//! to illustrate carrier phase locked loop and demod process
//! see qpsk_variable for more comprehensive example.
//! \image html qpsk.gif
//! \image latex qpsk.eps
class qpsk
{
  public:

  a_d	ADC;
  loop_filter<long> carrier_loop_filter;
  loop_filter<long> symbol_loop_filter;

  complex<long> prev_sam,prev_sym;
  complex<long> decision;

  long carrier_loop_out,symbol_loop_out;
  long symbol_clk;
  long symbol_clk_pls;
  long sample_clk;
  long symbol_x2_clk;
  long symbol_x2_clk_pls;

  carrier_nco c_nco;

  fir< complex<long> > rcv_sqrt_rc;
  
  delay < complex<long> > hard_decision_delay,final_baseband_delay;
  delay < complex<long> > timing_disc_delay;

  long bpsk;
  long dec_rate_log;
  long carrier_error;
  long symbol_nco_out;
  complex<long> hard_decision_prev,final_baseband_prev;
  complex<long> baseband;
  complex<long> resampled;
  complex<long> carrier_in;
  complex<long> carrier_nco_out;
  complex<long> mf_in;
  complex<long> mf_out;
  complex<long> final_baseband;
  long timing_error;
  long nda_timing_error;
  complex<long> hard_decision;


  long I_data() { return(re(hard_decision)); }
  long Q_data() { return(im(hard_decision)); }
  complex<long> data() { return(hard_decision); };
  long carrier_loop() { return(carrier_loop_out); }
  long symbol_loop() { return(symbol_loop_out); }
  long symclk(void) { return(symbol_clk_pls); }
  void clock(complex<long>adc_out);
  qpsk(void);

};
} // namespace SPUC 
#endif

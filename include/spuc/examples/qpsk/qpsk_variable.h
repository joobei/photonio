// 
// Copyright(c) 1993-1996 Tony Kirke
// author="Tony Kirke" *
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
#ifndef QPSKV
#define QPSKV
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <math.h>
//#include <limits.h>
#include <time.h>
#include <iostream>
#include <spuc.h>
#include <complex.h>
#include <delay.h>
#include <loop_filter.h>
#include "carrier_nco.h"
#include <a_d.h>
#include <fir.h>
#include <fir_interp.h>
#include <lagrange.h>
#include <cordic.h>
#include <qpsk_discriminators.h>
#include "sum_and_dump.h"
#include "resampler.h"
#include "agc.h"
namespace SPUC {
/*! 
  \addtogroup comm Communication Classes
*/
/*!  \brief A QPSK receiver that can operate over a range of non-integer sampling rates
  \ingroup comm examples
*/
//
// A QPSK receiver that can operate over a range of non-integer sampling rates
// Symbol timing, frequency control and carrier phase locked loops
// are included. Also contains root-raised cosine matched filter,
// A/D and agc function.
//
class qpsk_variable
{
  public:

  a_d	ADC;
  agc   sigma_delta;
  loop_filter<long> afc_loop_filter;
  loop_filter<long> carrier_loop_filter;
  loop_filter<long> symbol_loop_filter;
  sum_and_dump symbol_lock_detect;
  qpsk_discriminators discriminators;

  long agc_bit;
  long carrier_loop_out,symbol_loop_out;
  long symbol_clk;
  long symbol_clk_pls;
  long sample_clk;
  long symbol_x2_clk;
  long symbol_x2_clk_pls;

  lagrange <complex<double> > interp;
  carrier_nco carrier__nco;
  cordic cordic_mult;

  resampler rate_change;

  fir< complex<long> > rcv_sqrt_rc;
  fir< complex<long> > fmf;
  
  delay< complex<long> > hard_decision_delay,final_baseband_delay;
  delay< complex<long> > timing_disc_delay;

  long bpsk;
  long resampler_round;
  long dec_rate_log;
  int sym_dec;
  long carrier_error;
  long symbol_nco_out;
  complex<long> hard_decision_prev,final_baseband_prev;
  complex<long> adc_out;
  complex<long> baseband;
  complex<long> decimated;
  complex<long> decimated_baseband;
  complex<long> resampled;
  complex<long> carrier_in;
  complex<long> carrier_nco_out;
  complex<long> mf_in;
  complex<long> mf_out;
  complex<long> fmf_out;
  complex<long> final_baseband;
  long timing_error;
  long nda_timing_error;
  long sym_lock; // from lock discriminator
  long symbol_locked; // averaged value;
  long sym_lock_thres;
  long cp_afc,quad_afc;
  long afc;
  complex<long> hard_decision;

  long qpsk_bpsk_reg;
  long lock_rate_reg;
  long invert_q_reg; 

  long I_data() { return(re(hard_decision)); }
  long Q_data() { return(im(hard_decision)); }
  complex<long> data() { return(hard_decision); };
  long carrier_loop() { return(carrier_loop_out); }
  long symbol_loop() { return(symbol_loop_out); }
  long symclk(void) { return(symbol_clk_pls); }
  long agc_out(void) { return(agc_bit); }
  void symbol_lock_average(void);
  void clock(complex<double>adc_in);
  qpsk_variable(void);

};
} // namespace SPUC 
#endif

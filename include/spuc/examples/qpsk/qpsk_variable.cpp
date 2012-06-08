// 
// author="Tony Kirke"
// Copyright(c) 1993-1996 Tony Kirke
// Description: variable rate qpsk receiver
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
#include "qpsk_variable.h"
namespace SPUC {
qpsk_variable::qpsk_variable(void)  : ADC(6) , rcv_sqrt_rc(9), interp(4), final_baseband_delay(2),
	 hard_decision_delay(2), timing_disc_delay(3),
	 symbol_lock_detect(12), fmf(9), sigma_delta(32)

{
	// CSD Coefficients for alpha = 0.35 root raised cosine fir
	rcv_sqrt_rc.coeff[4] = 14;
	rcv_sqrt_rc.coeff[3] = rcv_sqrt_rc.coeff[5] = 8;
	rcv_sqrt_rc.coeff[2] = rcv_sqrt_rc.coeff[6] = -1;
	rcv_sqrt_rc.coeff[1] = rcv_sqrt_rc.coeff[7] = -2;
	rcv_sqrt_rc.coeff[0] = rcv_sqrt_rc.coeff[8] = 1;

	fmf.coeff[3] = rcv_sqrt_rc.coeff[5];
	fmf.coeff[2] = rcv_sqrt_rc.coeff[6];
	fmf.coeff[1] = rcv_sqrt_rc.coeff[7];
	fmf.coeff[0] = rcv_sqrt_rc.coeff[8];
	fmf.coeff[4] = 0;
	fmf.coeff[5] = -rcv_sqrt_rc.coeff[5];
	fmf.coeff[6] = -rcv_sqrt_rc.coeff[6];
	fmf.coeff[7] = -rcv_sqrt_rc.coeff[7];
	fmf.coeff[8] = -rcv_sqrt_rc.coeff[8];


//	unsigned long fcw = 1 << (25); for testing
	carrier__nco.reset_frequency(0);
	carrier_error = 0;
	timing_error = 0;
	carrier_loop_out = 0;
	carrier_loop_filter.k0 = 1 << 15; 
	carrier_loop_filter.k1 = 1 << 8;

	carrier_loop_filter.k0_en = 0;
	carrier_loop_filter.k1_en = 0;
	symbol_loop_out = 0;
	symbol_loop_filter.k0 = 1 << 7;
	symbol_loop_filter.k1 = 1;
	
	symbol_loop_filter.k0_en = 0;
	symbol_loop_filter.k1_en = 0;

	resampler_round = 2; // default
	bpsk = 0;
	afc = 0;
	discriminators.set_mode(bpsk);
	sym_dec = 1;
	symbol_clk = 0;
	symbol_clk_pls = 0;
	symbol_x2_clk_pls = 0;
	sample_clk = 0;
	nda_timing_error = 0;
	sym_lock = 0;
	symbol_locked = 0;
	agc_bit = 0;
	sym_lock_thres = 4;
	symbol_lock_detect.set_exp(12);
  }
//***********************************************************************
// MAIN ROUTINE
//***********************************************************************
void qpsk_variable::clock(complex<double>adc_in)
  {
	long rcfd = 1; // FMF frequency discriminator
#ifdef NDA
	long nda=1;
#else
	long nda=0; // Don't use NDA timing discriminator
#endif

	// A/D input (assume valid every time step called!
    adc_out = ADC.sample(adc_in);
	agc_bit = sigma_delta.run(adc_out);

	// Down conversion 
	carrier__nco.clock(carrier_loop_out,symbol_clk_pls);
	long carrier_phase = carrier__nco.get_phase();
	//	long carrier_phase = carrier_nco.run(carrier_loop_out,symbol_x2_clk_pls);

	baseband = cordic_mult.rotate(adc_out, (unsigned long)(carrier_phase));
#ifdef NODC
	baseband = adc_out;
#endif

	symbol_clk_pls = 0;

	// Decimation
	decimated = baseband; // decim.update(baseband);
	sample_clk = 1; // decim.ready; // Output at decimated rate
	symbol_x2_clk_pls = 0;

	// Processing at Decimated Rate
	if (sample_clk) {
		resampled = rate_change.update(decimated,symbol_loop_out, symbol_clk);
		resampled = round(resampled, resampler_round);
		symbol_x2_clk = rate_change.ready;

		// Processing at 2 times the symbol rate
		if (symbol_x2_clk) {
			symbol_x2_clk_pls = 1;
			// input to matched filter & does calculation
			mf_in = saturate(resampled,6);
			mf_out = rcv_sqrt_rc.update(mf_in); 
			fmf_out = fmf.update(mf_in); 
			// Slicer - get sign bit prior to rounding!
			hard_decision = signbit(mf_out);
			mf_out = round(mf_out,7);  
			mf_out = saturate(mf_out,4);
			fmf_out = round(fmf_out,7);  

			symbol_clk = !symbol_clk;
			discriminators.sample(fmf_out,mf_out, hard_decision, symbol_clk);
			if (nda) timing_error = discriminators.nda_timing_disc();
			sym_lock = discriminators.symbol_lock_out();
			if (rcfd && afc) {
			  // carrier_error = discriminators.rcfd();
			  carrier_error = discriminators.quad();
			  carrier_loop_out = carrier_loop_filter.update(carrier_error);
			}
			
			// Symbol rate processing	
			if (symbol_clk) {
			  symbol_clk_pls = 1;
			  // lock detector
			  symbol_lock_detect.input(sym_lock);
			  if ((symbol_lock_detect.output() >> 10) > sym_lock_thres)
				symbol_locked = 1;
			  else symbol_locked = 0;
			  // Matched Filter out
			  final_baseband = mf_out; 
			  // Symbol discriminator
			  if (!nda) timing_error = discriminators.dd_timing_disc();
			  // Carrier discriminator
			  if (afc) carrier_error = discriminators.cross_prod_afc();
			  else 	carrier_error = discriminators.pll_disc();
			  // Symbol + timing loop filters
			  symbol_loop_out = symbol_loop_filter.update(timing_error); 
			  if (!rcfd || !afc) carrier_loop_out = carrier_loop_filter.update(carrier_error);
			}
		}
	}
    return;
}


} // namespace SPUC 

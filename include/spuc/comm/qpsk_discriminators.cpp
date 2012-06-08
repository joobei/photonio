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
#include <discriminators.h>
#include <qpsk_discriminators.h>
namespace SPUC {
void qpsk_discriminators::update(complex<long> data_in, complex<long> decision_in, int sym_pls)
{
	data = data_in;
	prev_sym = timing_disc_delay.input(data);
	prev_sam = timing_disc_delay.checkback(1);
	decision = decision_in;
	if (sym_pls) {
		hard_decision_prev = hard_decision_delay.input(decision);
	}
}
void qpsk_discriminators::sample(complex<long> fmf_in, complex<long> data_in, complex<long> decision_in, int sym_pls)
{
	fmf = fmf_in;
	update(data_in,decision_in,sym_pls);
}
long qpsk_discriminators::rcfd()
{
	// QPSK only!!!
	return(qpsk_rcfd(fmf, data));
}
long qpsk_discriminators::quad()
{
	// QPSK only!!!
	return(qpsk_quadricorrelator(decision, prev_sam));
}
long qpsk_discriminators::cross_prod_afc()
{
	// QPSK only!!!
	long sample = 0;
	complex<long> prev = (sample) ? prev_sam : prev_sym;
	return(cp_afc(prev, data));
}
//*******************************************************************************
// NDA Timing Discriminator
//*******************************************************************************
long qpsk_discriminators::nda_timing_disc()
{
	long terror = nda_symbol(prev_sam,data);
	terror = (terror+1)>>1;
	return(terror);
}
//*******************************************************************************
// Decision directed symbol timing Discriminator
//*******************************************************************************
long qpsk_discriminators::dd_timing_disc(void)
{
	return(dd_symbol(prev_sym,data,hard_decision_prev,decision));
}
//*******************************************************************************
// Symbol lock detector
//*******************************************************************************
long qpsk_discriminators::symbol_lock_out()
{
	long sym_lock;
	sym_lock =  magsq(data) - magsq(prev_sam);
	return(sym_lock);
}
//*******************************************************************************
// PLL Discriminator
//*******************************************************************************
long qpsk_discriminators::pll_disc()
{
	complex<long> ldata = saturate(data,4); 
	if (bpsk_mode) {
		return(bpsk_dd_phase(ldata, decision));
	} else {
		return(qpsk_dd_phase(ldata, decision));
	}
}
} // namespace SPUC 

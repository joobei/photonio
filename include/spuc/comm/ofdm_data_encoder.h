#ifndef OFDMDETX
#define OFDMDETX
#include <math.h>
#include <complex.h>
#include <bit_scrambler.h>
#include "data_conv_encoder.h"
#include "qam_mod.h"
namespace SPUC {
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
//! \ingroup comm
//! \brief  OFDM/802.11A/G Data Encoder 
//! \author Tony Kirke, Copyright(c) 2004
//! \
//!   Uses data_conv_encoder and qam_mod classes
class ofdm_data_encoder 
{
public:
	data_conv_encoder CONV;
	qam_mod QAM;
	long rate_index; // 0 BPSK, Max for highest QAM, etc
	long enc_rate;
	long tx_bits_per_symbol;
	long total_bits;
	long number_symbols;
	long frame;
	long serial;
	long sample;
	long data_index;
	bool raw_bit; // current input data bit
	bool* raw_data;
	bool* interleaver_in;
	bool* interleaved;
	bool* interleaver_out;
	long* pre_mod;
	
 public:
	const long Carriers;
	int coded_bits_per_frame;
	int raw_bits_this_frame;
	bool no_conv;
	
	// Constructor (with default data rate)
	ofdm_data_encoder(int index, int T_fft, int D_carriers,
					  int max_range) : Carriers(D_carriers),
					  CONV(index, T_fft), QAM(index)
		{
			rate_index = index;
			raw_data = new bool[T_fft*16];
			interleaver_in = new bool[T_fft*max_range];
			interleaved = new bool[T_fft*max_range];
			pre_mod = new long[Carriers];
			reset();
		}
	// Determine number of OFDM symbol (must be called AFTER set_rate())
	long tx_burst_size(long bytes) {
		number_symbols = (8*bytes+6+coded_bits_per_frame+1)/coded_bits_per_frame;
		total_bits = number_symbols*coded_bits_per_frame;
	}
	void reset() { // clear variables for next burst
		serial = 0;
		sample = 0;
		frame = 0;
		raw_bit=0;
		coded_bits_per_frame=0;
		raw_bits_this_frame=0;
		tx_bits_per_symbol = 0;
		data_index = 0;
	}
	~ofdm_data_encoder() {
		delete [] raw_data;
		delete [] interleaver_in;
		delete [] interleaved;
		delete [] pre_mod;
	} 
	void set_rate(int mod, int conv_rate) {
		if (conv_rate == 0) no_conv = 1;
		else no_conv = 0;
		enc_rate = conv_rate;
		if (mod>0) tx_bits_per_symbol = 2*mod;
		else tx_bits_per_symbol = 1;
	}
	bool* interleave(bool* data_in); 
	bool get_data(void);

	complex<long> data_map(long rate);
	
	void get_data_frame();
	void serial_to_word_input(bool in);
	long serial_to_word_output(void);
};
}
#endif



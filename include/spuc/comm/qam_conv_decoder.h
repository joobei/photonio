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
#ifndef QAMCONVD
#define QAMCONVD
#include <math.h>
#include <complex.h>
#include <viterbi.h>
#include <delay.h>
#include <fir.h>
namespace SPUC {
//! \ingroup comm
//! \brief QAM convolutional decoder
// uses qam_data_deamp for soft decisions then inputs these into 
// viterbi decoding using puncturing if necessary
// if output of data_decode is true, use get_bit for data bit output
//! \author Tony Kirke, Copyright(c) 2004
//! \
class qam_conv_decoder {
 public:
	viterbi viterbi_decoder;
	long* viterbi_input;
	
	const long soft_decision_level;
	const long Max_bits_per_symbol;
	long enc_rate;
	long rate_index;
	long soft_bit_counter;
	long rx_bits_per_symbol;
	long raw_bits_this_frame;
	long coded_bits_per_frame;
	long raw_bits_out;
	long sample;
	long serial;
	
	bool frame_ready;
	bool viterbi_ready;
	bool viterbi_data;
	bool output_buffer_empty;
	bool no_conv; // Bypass Viterbi when true
	
 public:
	qam_conv_decoder(int index) :	soft_decision_level(64),
		Max_bits_per_symbol(8) {
		no_conv=0;
		rate_index = index;
		reset();
		set_rate(0,1);
		viterbi_input = new long[Max_bits_per_symbol];
	}
	~qam_conv_decoder() {	
		delete [] viterbi_input;
	}
	void reset() {
		sample = 0; //16;
		raw_bits_this_frame=0;
		coded_bits_per_frame=0;
		soft_bit_counter=0;
		raw_bits_out = 0;
		output_buffer_empty = 1;
		viterbi_ready = 0;
		viterbi_data = 0;
		viterbi_decoder.reset();
		frame_ready = 0;
	}
	//! set modulation and punctured coding rate
	void set_rate(int mod, int conv_rate) {
		if (conv_rate == 0) no_conv = 1;
		else no_conv = 0;
		enc_rate = conv_rate;
		if (mod>0) rx_bits_per_symbol = 2*mod;
		else rx_bits_per_symbol = 1;
		rate_index = mod;
	}
	//! return true when output is available
	bool data_decode(complex<long> data_in);
	//! call when viterbi_ready is true
	bool get_bit(void) { return(viterbi_data); };
	//! clear soft decision array
	long* clear_soft_decision(long rate);
};
} // namespace SPUC 
#endif

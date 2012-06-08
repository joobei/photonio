#ifndef DCONVE
#define DCONVE
#include <math.h>
#include <complex.h>
#include <max_pn.h>
namespace SPUC {
// Copyright(c) 2004 Tony Kirke
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
//!	  \ingroup comm
//! \brief Convolutional encoder for punctured encoding
//!  \author Tony Kirke,  Copyright(c) 2001 
//!  
//
//!   Convolutional Encoder for punctured encoding of various rates
//!   Uses common G1/G2 0x6d/0x4f generator polynomials
//!  Primarily designed for Encoder in an 802.11a system
class data_conv_encoder 
{
 public:
	max_pn test_pn;
	const long g1;
	const long g2;
	bool conv_enc_phase;
	int  conv_enc_get_bit;
	long conv_enc_u;
	long conv_bit_number;
	long rate_index;
	long enc_rate;
	long total_bits;
	long number_symbols;
	long frame;
	long serial;
	long sample;
	long data_index;
	bool raw_bit; // current input data bit
	bool* raw_data;
	long* pre_mod;

 public:
	int raw_bits_this_frame;
	bool no_conv;
	
	// Constructor (with default data rate)
	data_conv_encoder(int index, int T_fft) : 
		g1(0x6d), g2(0x4f),
		test_pn(0x0013,32767,-1)
		{
			rate_index = index;
			raw_data = new bool[T_fft*16];
			pre_mod = new long[T_fft];
			reset();
		}
	void reset() { // clear variables for next burst
		conv_enc_u = 0;
		conv_enc_phase = 1;
		conv_enc_get_bit = 1;
		conv_bit_number = 0;
		serial = 0;
		sample = 0;
		frame = 0;
		raw_bit=0;
		raw_bits_this_frame=0;
		test_pn.reset();
		data_index = 0;
	}
	~data_conv_encoder() {
		delete [] raw_data;
		delete [] pre_mod;
	} 
	void set_rate(int mod, int conv_rate) {
		if (conv_rate == 0) no_conv = 1;
		else no_conv = 0;
		enc_rate = conv_rate;
	}
	bool conv_encoder(const long enc_rate);
	//	bool get_data(void);
	void serial_to_word_input(bool in);
	long serial_to_word_output(void);
	bool get_data(void);
};
}
#endif



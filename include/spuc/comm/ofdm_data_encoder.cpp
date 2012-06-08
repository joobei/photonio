// 
// author="Tony Kirke"
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
#include <spuc.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include "ofdm_data_encoder.h"
using namespace SPUC;
//--------------------------------------------------------------------------
// Interleave the coded bits
//--------------------------------------------------------------------------
bool* ofdm_data_encoder::interleave(bool* data_in) 
{
  int k;
  for (int i=0;i<coded_bits_per_frame;i++) {
	k = 16*i-(coded_bits_per_frame-1)*floor(16*i/coded_bits_per_frame);
#ifdef NO_INT
	interleaved[i] = data_in[i];
#else
	interleaved[i] = data_in[k];
#endif
  }
  return(interleaved);
}
//--------------------------------------------------------------------------
complex<long> ofdm_data_encoder::data_map(long rate)
{
  if (data_index == Carriers) data_index = 0; // re-wrap
  if (data_index == 0) get_data_frame();
  return(QAM.data_map(rate, data_index++));
}
//--------------------------------------------------------------------------
void ofdm_data_encoder::get_data_frame()
{

  bool data_enc;
  int i,j;
  coded_bits_per_frame = Carriers*tx_bits_per_symbol;
  
  raw_bits_this_frame = 0;
  for (j=0;j<coded_bits_per_frame;j++) {
	if (no_conv) {
	  data_enc = CONV.get_data();
	} else {
	  data_enc = CONV.conv_encoder(enc_rate);
	}
	interleaver_in[j] = data_enc;
  }

  // interleave the block!
  interleaver_out = interleave(interleaver_in);
  
  int bitc=0;
  for (j=0;j<Carriers;j++) {
	for (i=0;i<tx_bits_per_symbol;i++) {
	  serial_to_word_input(interleaver_out[bitc++]);
	}
	pre_mod[j] = serial_to_word_output();  
  }
}
void ofdm_data_encoder::serial_to_word_input(bool in) {
  serial = (serial << 1) | in;
}
long ofdm_data_encoder::serial_to_word_output(void) {
  return(serial);
}

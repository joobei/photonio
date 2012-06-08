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
//!	  \ingroup comm
//!
//!   Convolutional Encoder for punctured encoding of various rates
//   Uses common G1/G2 0x6d/0x4f generator polynomials
//  Primarily designed for Encoder in an 802.11a system
#include <spuc.h>
#include "data_conv_encoder.h"
using namespace SPUC;
//--------------------------------------------------------------------------
// Convolutional encoding
//--------------------------------------------------------------------------
bool data_conv_encoder::conv_encoder(const long steal)
{
  bool reduce(long x, long y);
  bool a;

  if (steal>1) { // ! rate 1/2
	  if (conv_enc_get_bit) {
		  conv_enc_u <<= 1;
		  conv_enc_u |= (long)get_data();
		  if (conv_bit_number++ % steal == 0) conv_enc_get_bit = 0;
		  else conv_enc_get_bit = 1;
	  } else {
		  conv_enc_get_bit = 1;
	  }
	  if (conv_enc_phase) a = reduce(conv_enc_u&g1,7);
	  else a = reduce(conv_enc_u&g2,7);
  } else {
	  if (conv_enc_phase) {
		  conv_enc_u <<= 1;
		  //OR u with the next new bit to get the contents of the encoder
		  conv_enc_u |= (long)get_data();
		  conv_bit_number++;
		  //XOR the masked register contents to get output data A
		  a = reduce(conv_enc_u&g1,7);
	  } else {
		  a = reduce(conv_enc_u&g2,7);
	  }
  }
  conv_enc_phase = !conv_enc_phase;
  if (conv_enc_get_bit && steal == 2) conv_enc_phase = 1;
  return(a);
}  
//--------------------------------------------------------------------------
void data_conv_encoder::serial_to_word_input(bool in)
{
  serial <<= 1;
  serial += in;
}
//--------------------------------------------------------------------------
long data_conv_encoder::serial_to_word_output(void) {
  long x = serial;
  serial = 0;
  return(x);
}
//--------------------------------------------------------------------------
// Get data for transmission
//--------------------------------------------------------------------------
bool data_conv_encoder::get_data(void) {
  raw_bit = test_pn.get_bit();
  raw_data[raw_bits_this_frame++] = raw_bit;
  return(raw_bit);
}

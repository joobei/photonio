// 
// author="Tony Kirke"
// Copyright (c) 2001 Tony Kirke
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
#include <stdlib.h>
#include "dvb_conv_encoder.h"
using namespace SPUC;
//--------------------------------------------------------------------------
// Convolutional encoding
//--------------------------------------------------------------------------
// only handles rate 1/2 encoding
bool dvb_conv_encoder::clock()
{
  bool a;
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
  conv_enc_phase = !conv_enc_phase;
  return(a);
}
bool dvb_conv_encoder::reduce(long x, long n)
{
	bool c=0;
	for (int i =0;i<n;i++) {
		c ^= (x & 0x01);
		x >>= 1;
	}
	return(c);
}


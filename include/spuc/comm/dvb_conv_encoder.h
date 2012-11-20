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
#ifndef DVBCONVENC
#define DVBCONVENC
#include <math.h>
#include <max_pn.h>
namespace SPUC {
/*! 
	  \addtogroup comm Communication Classes
*/
/*! \brief DVB Convolution encode for rate 1/2
  \author Tony Kirke,  Copyright(c) 2001 
  
  \ingroup comm
  \ingroup fec
*/
//! 
//! Convolutional Encoder for rate 1/2 encoding with DVB 
//! Generators. Uses PN sequence for data generation
class dvb_conv_encoder 
{
 public:
  max_pn test_pn;
  const long g1;
  const long g2;
  bool conv_enc_phase;
  int  conv_enc_get_bit;
  long conv_enc_u;
  long conv_bit_number;
  bool raw_bit;

 public:
  //! Constructor
  dvb_conv_encoder() : g1(0x6d), g2(0x4f) {
	  reset();
  }
  void reset() { // clear variables for next burst
	  conv_enc_u = 0;
	  conv_enc_phase = 1;
	  conv_enc_get_bit = 1;
	  conv_bit_number = 0;
	  test_pn.reset();
  }
  ~dvb_conv_encoder() {  }
  bool clock();
  bool get_data(void) {
	raw_bit = test_pn.get_bit();
	return(raw_bit);
  }
  bool reduce(long x, long y);

};
} // namespace SPUC 
#endif

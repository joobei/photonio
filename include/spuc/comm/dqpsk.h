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
#include <complex.h>
namespace SPUC {
/*! 
	  \addtogroup comm Communication Classes
*/
/*! \brief Differential QPSK encoder/decoder
  \author Tony Kirke,  Copyright(c) 2001 
  
  \ingroup comm
*/
//!
//! Differential encoding/decoding of QPSK
//! Encode/Decode the input bits into quaternary qpsk format
class dqpsk {
 public:
  int datbase[4][4];
  int previous_encoded_symbol;
  int previous_decoded_symbol;

  dqpsk() {
	//  Initialize previous symbol to zero 
	//  Set up encoding data base as a two element array with arguments of
	//   i) current input symbol, and ii) previous encoded symbol 

	//	static int datbase[4][4] = {0,1,2,3,1,3,0,2,2,0,3,1,3,2,1,0};
	datbase[0][0] = 0;
	datbase[0][1] = 1;
	datbase[0][2] = 2;
	datbase[0][3] = 3;

	datbase[1][0] = 1;
	datbase[1][1] = 3;
	datbase[1][2] = 0;
	datbase[1][3] = 2;

	datbase[2][0] = 2;
	datbase[2][1] = 0;
	datbase[2][2] = 3;
	datbase[2][3] = 1;

	datbase[3][0] = 3;
	datbase[3][1] = 2;
	datbase[3][2] = 1;
	datbase[3][3] = 0;
	previous_encoded_symbol = 0;
	previous_decoded_symbol = 0;
  }
  //!  Call qpsk_sym to get equivalent symbol value (0-3) of current 
  //!   input bit pair
  //!   Get current output symbol (0-3) from encoding 
  //!    data base[curr symbol][prev encoded symbol]
  //!   Call convbits to convert current encoded symbol to bit pair
  //!   Previous encoded symbol = current encoded symbol 
  complex<long> encode(complex<long> c)
	{
	  int insym,outsym; 
	  insym = qpsk_sym(c);
	  outsym = datbase[insym][previous_encoded_symbol];
	  previous_encoded_symbol = outsym;
	  return(convbits(outsym));
	}
  //! 
  //! Differential decoder
  complex<long> decode(complex<long> c)
	{
	  int insym,outsym; 
	  insym = qpsk_sym(c);
	  outsym = datbase[insym][previous_decoded_symbol];
	  previous_decoded_symbol = insym;
	  return(convbits(outsym));
	}
  //!
  //! Convert integer symbol (0-3) back to binary pair
  complex<long> convbits(int sym)
	{
	  if(sym==0) return(complex<long>(-1,-1));
	  else if(sym==1) return(complex<long>(-1,1));
	  else if(sym==2) return(complex<long>(1,-1));
	  else return(complex<long>(1,1));
	}
  //! 
  //! Encode bit pair into integer value from 0 to 3 and return symbol
  int qpsk_sym(complex<long> c)
	{
	  int insym;
	  if(c.real()==-1.) {
		if(c.imag()==-1) insym = 0;            
		else insym = 1;
	  }
	  else  {
		if(c.imag()==-1) insym = 2;            
		else insym = 3;
	  }
	  return(insym);
	}
};
} // namespace SPUC 

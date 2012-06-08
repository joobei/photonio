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
#ifndef BSCRAM
#define BSCRAM
namespace SPUC {
/*! 
	  \addtogroup comm Communication Classes
*/
/*! \brief Data scrambler
  \author Tony Kirke,  Copyright(c) 2001 
  
  \ingroup comm
*/
//! Does scrambling function on input bits
class bit_scrambler {
 public:
	unsigned long g;
	long span;

 public:
	unsigned long u;
	bit_scrambler(long gen=0x48, long bits=7, long uinit=-1) 
		: g(gen), span(bits), u(uinit) {;}
	void reset() { u=~0; }
	bool scramble(bool data_in) {
		bool res = xor_bits(u&g);
		u <<= 1;
		u += res;
		return(data_in ^ res);
	}
	//! Exclusive or reduction
	bool xor_bits(long x) {
		bool c=0;
		for (int i =0;i<span;i++) {
			c ^= (x & 0x01);
			x >>= 1;
		}
		return(c);
	}
};
} // namespace SPUC
#endif

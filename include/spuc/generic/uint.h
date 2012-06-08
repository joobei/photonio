// Copyright(c) 1993-1996 Tony Kirke
// author="Tony Kirke" *
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
#ifndef UINT
#define UINT
#include <int_u.h>
namespace SPUC {
//! \brief Template class for unsigned integers (<33 bits).
//! \ingroup base
//
//! The template parameter 
//!   is the number of bits in the integer (default 32)
//!   Derived from base class int_u.  
template <long Bits=32> class uint : public int_u
{
	public:
	bool overflow;

	uint<Bits>() { 
	  bits=Bits;
	  mask = -1 << Bits;
	  overflow = 0;
	}
	uint<Bits>(long y) { 
	  q = y;
	  bits=Bits;
	  mask = -1 << Bits;
	  overflow = 0;
	}
	inline uint<Bits> operator =(const int_u& y) {   
	  overflow = 0;
	  q = y.q;
	  if (  ((mask&q)!=mask) || ((mask&q)!=0) ) {
		q &= ~mask;
		overflow = 1;
	  }
	  return *this; 
	} 
	inline uint<Bits> operator =(const long& y) {   
	  overflow = 0;
	  if (bitpos) {
		if (y) q |= MASK_BIT(bitpos);
		else q &= ~MASK_BIT(bitpos);
		bitpos = 0;
	  } else {
		q = y;	
		if (  ((mask&q)!=mask) || ((mask&q)!=0) ) {
		  q &= ~mask;
		  overflow = 1;
		}
	  }
	  return *this; 
	} 
	inline uint<Bits> operator ()(long i) { 
	  bitpos = i;
	  return *this;
	}
};
} // namespace SPUC 
#endif

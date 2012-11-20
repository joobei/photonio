#ifndef SINT
#define SINT
#include <int_s.h>
// 
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
namespace SPUC {
//! \brief Template class for signed integers (<33 bits).
//! \ingroup base
//
//!   Template class for signed integers (<33 bits). The template parameter 
//!   is the number of bits in the integer (default 32)
//!   Derived from base class int_s.  
template <long Bits=32> class sint : public int_s
{
	public:
	  sint<Bits>() { 
		  bits=Bits-1;
		  mask = -1 << bits;
	  }
	  sint<Bits>(long y) { 
		  q = y;
		  bits=Bits-1;
		  mask = -1 << bits;
	  }
	  inline sint<Bits> operator =(const int_s& y) {   
		q = y.q;
		sign = (y.q & (1 << bits));
		if (  ((mask&q)!=mask) || ((mask&q)!=0) ) {
			q &= ~mask;
			if (sign) q |= mask;
		}
       return *this; 
	  } 
	  inline sint<Bits> operator =(const long& y) { 
		  if (bitpos) {
  			  if (y) q |= MASK_BIT(bitpos);
			  else q &= ~MASK_BIT(bitpos);
			  bitpos = 0;
		  } else {
			q = y;	
			sign = (y & (1 << bits));
			if (  ((mask&q)!=mask) || ((mask&q)!=0) ) {
			q &= ~mask;
			if (sign) q |= mask;
			}	
		  }
       return *this; 
	  } 
	  //! Dangerous : Can only be used on LHS!!!!
	  inline sint<Bits> operator ()(long i) { 
		  bitpos = i;
		  return *this;
	  }

};
//template <class m, class m1> inline sint<m>::(const sint<m1>& y) : q(y.q) {}
} // namespace SPUC 
#endif

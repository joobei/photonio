// 
// Copyright(c) 1993-1998 Tony Kirke
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
#ifndef INTU
#define INTU
#include <spuc.h>
#include <math.h>
#ifndef DOXYGEN_SHOULD_SKIP_THIS
namespace SPUC {
//! \brief Base class for unsigned integers (<33 bits).
//! \addtogroup base Base classes
//
//!   Base class for unsigned integers with less than 33 bits
//!  similar to 32 bit integers with appropriate overflows and
//!  extra functions
class int_u
{
	typedef long natural;

	public:
	  natural q;
	  natural mask;
	  natural bits;
	  natural bitpos;
      bool oveflow;


//  protected:
	  int_u() { 
		  q = 0;
		  bits = 32;
		  mask = 0;
		  bitpos=0;
	  }
	  int_u(natural y) { 
		  q = y;
		  bits = 32;
		  mask = 0;
		  bitpos=0;
	  }

  public:
  inline int_u operator =(const int_u& y) {   
    q = ~mask & y.q;
	return *this; 
  } 
  inline int_u operator =(const natural& y) {   
    q = ~mask & y;
	return *this; 
  } 
//  friend int_u operator ,(int_u r, int_u l);
  friend int_u operator %(int_u r, int_u l);
  friend int_u operator %(int_u r, natural l);
  friend int_u operator %(natural r, int_u l);
  friend int_u operator +(int_u r, int_u l);
  friend int_u operator +(int_u r, natural l);
  friend int_u operator +(natural r, int_u l);
  friend int_u operator -(int_u r, int_u l);
  friend int_u operator -(int_u r, natural l);
  friend int_u operator -(natural r, int_u l);
  friend int_u operator &(int_u r, int_u l);
  friend int_u operator &(int_u r, natural l);
  friend int_u operator &(natural r, int_u l);
  friend int_u operator ^(int_u r, int_u l);
  friend int_u operator ^(int_u r, natural l);
  friend int_u operator ^(natural r, int_u l);
  friend int_u operator |(int_u r, int_u l);
  friend int_u operator |(int_u r, natural l);
  friend int_u operator |(natural r, int_u l);
  friend int_u operator *(int_u r, int_u l);
  friend int_u operator *(int_u r, natural l);
  friend int_u operator *(natural r, int_u l);
  friend int_u operator /(int_u r, int_u l);
  friend int_u operator /(natural r, int_u l);
  friend int_u operator /(int_u r, natural l);
  friend int_u operator <<(int_u r, const natural shift) ;
  friend int_u operator >>(int_u r, const natural shift) ;

  int_u operator ++() {
  q++;
  q &= ~mask;
  return *this;  
  };
  int_u operator ++(int) {
  q++;
  q &= ~mask;
  return *this;  
  };
  int_u operator --() {
  q--;
  q &= ~mask;
  return *this;  
  };
  int_u operator --(int) {
  q--;
  q &= ~mask;
  return *this;  
  };
  int_u operator +=(int_u r) {
  q += r.q;
  q &= ~mask;
  return *this;  
  };

  int_u operator -=(int_u r){
  q -= r.q;
  q &= ~mask;
//  if (  ((mask&q)!=mask) || ((mask&q)!=0) ) q &= ~mask;
  return *this;  
  };
  int_u operator *=(int_u r) {
  q *= r.q;
  q &= ~mask;
  return *this;  
  };
  int_u operator /=(int_u r){
  if (r.q != 0) q = q/r.q;
  else    q = 0;
  return *this;  
  };

  int_u operator <<=(const natural shift)  {
	  q = q << shift;
	  q &= ~mask;
	  return *this;
  };
  int_u  operator >>=(const natural shift) {
	  q = q >> shift;
	  return *this;
  }

  int_u operator ^=(int_u r) {
  q ^= r.q;
  q &= ~mask;
  return *this;  
  };
  int_u operator &=(int_u r) {
  q &= r.q;
  q &= ~mask;
  return *this;  
  };
  int_u operator |=(int_u r) {
  q |= r.q;
  q &= ~mask;
  return *this;  
  };

  inline natural operator ~() {return(~q);}
  inline bool operator !() {return(!q);}
  inline bool operator ==(int_u r) {  return ((q == r.q));  };
  inline bool operator ==(natural r) {  return ((q == r));  };
  inline bool operator !=(int_u r) {  return ((q != r.q));  };
  inline bool operator !=(natural r) {  return ((q != r));  };
  inline bool operator >(int_u r) {  return ((q > r.q));  };
  inline bool operator >(natural r) {  return ((q > r));  };
  inline bool operator <(int_u r) {  return ((q < r.q));  };
  inline bool operator <(natural r) {  return ((q < r));  };
  inline int_u magsq(int_u y) { 
	  int_u x;
	  x.q = y.q*y.q;
	  x.mask = -1 << (y.bits * 2);
	  return(x); 
  };

  operator const natural () const {	  return(q);  }
  operator const double () const {	  return((double)q);  }
  //! Round to rbits
  int_u round(int_u in, natural rbits) {
  double scale = 1.0/(double)MASK_BIT(rbits);
  return(int_u((natural)floor((double)(scale*in.q)+0.5)));
  };

  //!  Saturate to rbits, only positive overflow
  int_u saturate(int_u in, natural rbits) {
	int_u out;
	natural low_mask = MASK_LOW(rbits);
	if (in.q > low_mask) out.q = low_mask;
	else out.q = in.q;
	out.mask = ~low_mask; // ?
    return(out);
  }
  //! Uses CLIP macro
  int_u clip(int_u in, int_u min_clp, int_u max_clp) {
  int_u out = CLIP(in, max_clp, min_clp);
  return out;
  }
  //! Return i'th bit
  inline bool bit(natural i) { return (q & MASK_BIT(i)) ? 1 : 0; }
  // OR reduction of number
  bool or_bits(int_u r) {
	  natural x=r.q;
	  bool s=0;
	  for (int i=0;i<bits;i++) { 
		s |= x & 0x1;
		x >>= 1;
	  }
	  return(s);
  }
};
   int_u operator %(int_u r, int_u l) {
	int_u x;
	x.q = r.q % l.q;
	return(x);
  }
   int_u operator %(int_u r, natural l) {
	int_u x;
	x.q = r.q % l;
	return(x);
  }
   int_u operator %(natural r, int_u l) {
	int_u x;
	x.q = r % l.q;
	return(x);
  }
   int_u operator +(int_u r, int_u l) {
	int_u x;
	x.q = r.q + l.q;
	return(x);
  }
   int_u operator +(int_u r, natural l) {
	int_u x;
	x.q = r.q + l;
	return(x);
  }
   int_u operator +(natural r, int_u l) {
	int_u x;
	x.q = r + l.q;
	return(x);
  }
   int_u operator -(int_u r, int_u l) {
	int_u x;
	x.q = r.q - l.q;
	return(x);
  }
   int_u operator -(int_u r, natural l) {
	int_u x;
	x.q = r.q - l;
	return(x);
  }
   int_u operator -(natural r, int_u l) {
	int_u x;
	x.q = r - l.q;
	return(x);
  }
   int_u operator &(int_u r, int_u l) {
	int_u x;
	x.q = r.q & l.q;
	return(x);
  }
   int_u operator &(int_u r, natural l) {
	int_u x;
	x.q = r.q & l;
	return(x);
  }
   int_u operator &(natural r, int_u l) {
	int_u x;
	x.q = r & l.q;
	return(x);
  }
   int_u operator ^(int_u r, int_u l) {
	int_u x;
	x.q = r.q ^ l.q;
	return(x);
  }
   int_u operator ^(int_u r, natural l) {
	int_u x;
	x.q = r.q ^ l;
	return(x);
  }
   int_u operator ^(natural r, int_u l) {
	int_u x;
	x.q = r ^ l.q;
	return(x);
  }
   int_u operator |(int_u r, int_u l) {
	int_u x;
	x.q = r.q | l.q;
	return(x);
  }
   int_u operator |(int_u r, natural l) {
	int_u x;
	x.q = r.q | l;
	return(x);
  }
   int_u operator |(natural r, int_u l) {
	int_u x;
	x.q = r | l.q;
	return(x);
  }
   int_u operator *(int_u r, int_u l) {
	int_u x;
	x.q = r.q*l.q;
	return(x);  
  };
   int_u operator *(int_u r, natural l) {
	int_u x;
	x.q = r.q*l;
	return(x);  
  };
   int_u operator *(natural r, int_u l) {
	int_u x;
	x.q = r*l.q;
	return(x);  
  };
   int_u operator /(int_u r, int_u l) {
	int_u x;
	x.q = r.q/l.q;
	return(x);  
  };
   int_u operator /(natural r, int_u l) {
	int_u x;
	x.q = r/l.q;
	return(x);  
  };
   int_u operator /(int_u r, natural l) {
	int_u x;
	x.q = r.q/l;
	return(x);  
  };

   int_u operator <<(int_u r, const natural shift)  {
	  int_u x;
	  x.q = r.q << shift;
	  x.mask = r.mask << shift;
	  x.bits = r.bits + shift;
	  return(x);
  };
   int_u operator >>(int_u r, const natural shift)  {
	  int_u x;
	  x.q = r.q >> shift;
	  x.mask = r.mask >> shift;
	  x.bits = r.bits - shift;
	  return(x);
  };
  int_u operator ,(int_u r, int_u l) {
	int_u x;
	x.q = (r.q << l.bits) | l.q;
	return(x);
  }
} // namespace SPUC 
#endif
#endif

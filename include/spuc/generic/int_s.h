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
#ifndef INTS
#define INTS
#include <spuc.h>
#include <stdlib.h>
#include <math.h>
namespace SPUC {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
/*! 
  \defgroup base Base classes
 */
//! \brief Base class for signed integers (<33 bits).
//! \addtogroup base Base classes
//
//!:
//!   Base class for signed integers with less than 33 bits
//!  similar to 32 bit integers with appropriate overflows and
//!  extra functions
class int_s
{
	typedef long natural;

  public:
	  natural q;
	  natural mask;
	  natural bits;
	  natural sign;
	  natural bitpos;

// protected:
	  int_s() { 
		  q = 0;
		  bits=32;
		  mask = 0;
		  sign = 0;
		  bitpos=0;
	  }
	  int_s(natural y) { 
		  q = y;
		  bits=32;
		  mask = 0;
		  sign = 0;
		  bitpos=0;
	  }

  public:
  inline int_s operator =(const int_s& y) {   
    q = y.q;
	sign = (y.q & MASK_BIT(bits));
	if (  ((mask&q)!=mask) || ((mask&q)!=0) ) {
		q &= ~mask;
		if (sign) q |= mask;
	}
	return *this; 
  } 
  inline int_s operator =(const natural& y) {   
    q = y;
	sign = (y & MASK_BIT(bits));
	if (  ((mask&q)!=mask) || ((mask&q)!=0) ) {
		q &= ~mask;
		if (sign) q |= mask;
	}
	return *this; 
  } 
  friend int_s operator ,(int_s r, int_s l);
  friend int_s operator %(int_s r, int_s l);
  friend int_s operator %(int_s r, natural l);
  friend int_s operator %(natural r, int_s l);
  friend int_s operator +(int_s r, int_s l);
  friend int_s operator +(int_s r, natural l);
  friend int_s operator +(natural r, int_s l);
  friend int_s operator -(int_s r, int_s l);
  friend int_s operator -(natural r, int_s l);
  friend int_s operator -(int_s r, natural l);
  friend int_s operator -(int_s r);
  friend int_s operator &(int_s r, int_s l);
  friend int_s operator &(int_s r, natural l);
  friend int_s operator &(natural r, int_s l);
  friend int_s operator ^(int_s r, int_s l);
  friend int_s operator ^(int_s r, natural l);
  friend int_s operator ^(natural r, int_s l);
  friend int_s operator |(int_s r, int_s l);
  friend int_s operator |(int_s r, natural l);
  friend int_s operator |(natural r, int_s l);
  friend int_s operator *(int_s r, int_s l);
  friend int_s operator *(natural r, int_s l);
  friend int_s operator *(int_s r, natural l);
  friend int_s operator /(int_s r, int_s l);
  friend int_s operator /(int_s r, natural l);
  friend int_s operator /(natural r, int_s l);
  friend int_s operator <<(int_s r, const natural shift) ;
  friend int_s operator >>(int_s r, const natural shift) ;

  int_s operator ++() {
  if (q==~mask) q=mask;
  else q++;
  return *this;  
  };

  int_s operator --() {
  if (q==mask) q=~mask;
  else q--;
  return *this;  
  };

  int_s operator ++(int) {
  if (q==~mask) q=mask;
  else q++;
  return *this;  
  };

  int_s operator --(int) {
  if (q==mask) q=~mask;
  else q--;
  return *this;  
  };

  int_s operator +=(int_s r) {
  q += r.q;
  if (  ((mask&q)!=mask) || ((mask&q)!=0) ) q &= ~mask;
  return *this;  
  };

  int_s operator -=(int_s r){
  q -= r.q;
  if (  ((mask&q)!=mask) || ((mask&q)!=0) ) q &= ~mask;
  return *this;  
  };

  int_s operator *=(int_s r) {
  q *= r.q;
  if (  ((mask&q)!=mask) || ((mask&q)!=0) ) q &= ~mask;
  return *this;  
  };

  int_s operator /=(int_s r){
  if (r.q != 0) q = q/r.q;
  else    q = 0;
  return *this;  
  };

  int_s operator <<=(const natural shift)  {
	  q = q << shift;
	  return *this;
  };

  int_s  operator >>=(const natural shift) {
	  q = q >> shift;
	  return *this;
  }

  
  int_s operator ^=(int_s r) {
  q ^= r.q;
  return *this;  
  };

  int_s operator &=(int_s r) {
  q &= r.q;
  return *this;  
  };

  int_s operator |=(int_s r) {
	  if (r.q<0) q |= r.q;
	  else q |= (r.q & ~mask);
	  return *this;  
  };

  inline natural operator ~() {return(~q);}
  inline bool operator !() {return(!q);}
  inline bool operator ==(int_s r) {  return ((q == r.q));  };
  inline bool operator ==(natural r) {  return ((q == r));  };
  inline bool operator !=(int_s r) {  return ((q != r.q));  };
  inline bool operator !=(natural r) {  return ((q != r));  };
  inline bool operator >(int_s r) {  return ((q > r.q));  };
  inline bool operator >(natural r) {  return ((q > r));  };
  inline bool operator <(int_s r) {  return ((q < r.q));  };
  inline bool operator <(natural r) {  return ((q < r));  };
  inline int_s magsq(int_s y) { 
	  int_s x;
	  x.q = y.q*y.q;
	  x.mask = -1 << (y.bits * 2);
	  return(x); 
  };

  operator const natural () const {	  return(q);  }
  operator const double () const {	  return((double)q);  }

  int_s round(int_s in, natural rbits) {
  double scale = 1.0/(double)MASK_BIT(rbits);
  return(int_s((natural)floor((double)(scale*in.q)+0.5)));
  };

  int_s saturate(int_s in, natural rbits) {
	int_s out;
	natural low_mask = MASK_LOW(rbits);
	if (labs(in.q) > low_mask) out.q = (in.q>0) ? low_mask : ~low_mask;
	else out.q = in.q;
    return(out);
  }

  int_s clip(int_s in, int_s min_clp, int_s max_clp) {
  int_s out = CLIP(in, max_clp, min_clp);
  return out;
  }

  inline bool bit(natural i) { return (q & MASK_BIT(i)) ? 1 : 0; }
  bool or_bits(int_s r) {
	  natural x=r.q;
	  bool s=0;
	  for (int i=0;i<bits;i++) { 
		s |= x & 0x1;
		x >>= 1;
	  }
	  return(s);
  }


};

   int_s operator %(int_s r, int_s l) {
	int_s x;
	x.q = r.q % l.q;
	return(x);
  }
   int_s operator %(int_s r, natural l) {
	int_s x;
	x.q = r.q % l;
	return(x);
  }
   int_s operator %(natural r, int_s l) {
	int_s x;
	x.q = r % l.q;
	return(x);
  }

   int_s operator +(int_s r, int_s l) {
	int_s x;
	x.q = r.q + l.q;
	return(x);
  }
   int_s operator +(int_s r, natural l) {
	int_s x;
	x.q = r.q + l;
	return(x);
  }
   int_s operator +(natural r, int_s l) {
	int_s x;
	x.q = r + l.q;
	return(x);
  }
   int_s operator -(int_s r, int_s l) {
	int_s x;
	x.q = r.q - l.q;
	return(x);
  }
   int_s operator -(natural r, int_s l) {
	int_s x;
	x.q = r - l.q;
	return(x);
  }
   int_s operator -(int_s r, natural l) {
	int_s x;
	x.q = r.q - l;
	return(x);
  }
   int_s operator -(int_s r) {
	int_s x;
	x.q = -r.q;
	return(x);
  }
   int_s operator &(int_s r, int_s l) {
	int_s x;
	x.q = r.q & l.q;
	return(x);
  }
   int_s operator &(int_s r, natural l) {
	int_s x;
	x.q = r.q & l;
	return(x);
  }

   int_s operator &(natural r, int_s l) {
	int_s x;
	x.q = r & l.q;
	return(x);
  }

   int_s operator ^(int_s r, int_s l) {
	int_s x;
	x.q = r.q ^ l.q;
	return(x);
  }
   int_s operator ^(int_s r, natural l) {
	int_s x;
	x.q = r.q ^ l;
	return(x);
  }
   int_s operator ^(natural r, int_s l) {
	int_s x;
	x.q = r ^ l.q;
	return(x);
  }

   int_s operator |(int_s r, int_s l) {
	int_s x;
	x.q = r.q | l.q;
	return(x);
  }
   int_s operator |(int_s r, natural l) {
	int_s x;
	x.q = r.q | l;
	return(x);
  }
   int_s operator |(natural r, int_s l) {
	int_s x;
	x.q = r | l.q;
	return(x);
  }

   int_s operator *(int_s r, int_s l) {
	int_s x;
	x.q = r.q*l.q;
	return(x);  
  };
   int_s operator *(natural r, int_s l) {
	int_s x;
	x.q = r*l.q;
	return(x);  
  };
   int_s operator *(int_s r, natural l) {
	int_s x;
	x.q = r.q*l;
	return(x);  
  };

   int_s operator /(int_s r, int_s l) {
	int_s x;
	x.q = r.q/l.q;
	return(x);  
  };
   int_s operator /(int_s r, natural l) {
	int_s x;
	x.q = r.q/l;
	return(x);  
  };
   int_s operator /(natural r, int_s l) {
	int_s x;
	x.q = r/l.q;
	return(x);  
  };

   int_s operator <<(int_s r, const natural shift)  {
	  int_s x;
	  x.q = r.q << shift;
	  x.mask = r.mask << shift;
	  return(x);
  };

   int_s operator >>(int_s r, const natural shift)  {
	  int_s x;
	  x.q = r.q >> shift;
	  x.mask = r.mask >> shift;
	  return(x);
  };
  int_s operator ,(int_s r, int_s l) {
	int_s x;
	if (l.q > 0) x.q = (r.q << l.bits) | l.q;
	else x.q = (r.q << l.bits) | (l.q & ~l.mask);
	return(x);
  }
} // namespace SPUC 
#endif
#endif

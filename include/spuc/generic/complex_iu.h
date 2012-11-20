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
#ifndef CINTU
#define CINTU
#include <spuc.h>
#include <stdlib.h>
#include <math.h>
namespace SPUC {
#define cnatural complex<long>
//! \brief Base class for complex fixed width unsigned integers.
//!  Base class for complex fixed width unsigned integers
//!	Basically a combination of complex<long> with int_u.
//!	Needed because complex<T> template type would not also 
//!	support unsigned int template type.
class complex_iu
{
	typedef long natural;
//	typedef complex<long> cnatural;

	public:
	  cnatural q;
	  natural mask;
	  natural bits;
	  natural bitpos;

//  protected:
	  complex_iu() { 
		  q = 0;
		  bits = 32;
		  mask = 0;
		  bitpos=0;
	  }
	  complex_iu(natural r, natural i=0) {
		  q = complex<long>(r,i); 
		  bits = 32;
		  mask = 0;
		  bitpos=0;
	  }

  public:
  inline complex_iu operator =(const complex_iu& y) {   
    q =  y.q & ~mask;
	return *this; 
  } 
  inline complex_iu operator =(const natural& y) {   
    q = ~mask & y;
	return *this; 
  } 
  friend complex_iu operator %(complex_iu r, natural l);
  friend complex_iu operator +(complex_iu r, complex_iu l);
  friend complex_iu operator +(complex_iu r, natural l);
  friend complex_iu operator +(cnatural r, complex_iu l);
  friend complex_iu operator -(complex_iu r, complex_iu l);
  friend complex_iu operator -(complex_iu r, cnatural l);
  friend complex_iu operator -(cnatural r, complex_iu l);
  friend complex_iu operator &(complex_iu r, natural l);
  friend complex_iu operator &(natural r, complex_iu l);
  friend complex_iu operator ^(complex_iu r, natural l);
  friend complex_iu operator ^(natural r, complex_iu l);
  friend complex_iu operator |(complex_iu r, natural l);
  friend complex_iu operator |(natural r, complex_iu l);
  friend complex_iu operator *(complex_iu r, complex_iu l);
  friend complex_iu operator *(complex_iu r, cnatural l);
  friend complex_iu operator *(complex_iu r, natural l);
  friend complex_iu operator *(natural r, complex_iu l);
  friend complex_iu operator *(cnatural r, complex_iu l);
  friend complex_iu operator /(complex_iu r, complex_iu l);
  friend complex_iu operator /(cnatural r, complex_iu l);
  friend complex_iu operator /(complex_iu r, cnatural l);
  friend complex_iu operator /(complex_iu r, natural l);
  friend complex_iu operator <<(complex_iu r, const natural shift) ;
  friend complex_iu operator >>(complex_iu r, const natural shift) ;

  complex_iu operator +=(complex_iu r) {
  q += r.q;
  q = q & ~mask;
  return *this;  
  };

  complex_iu operator -=(complex_iu r){
  q -= r.q;
  q = q & ~mask;
//  if (  ((mask&q)!=mask) || ((mask&q)!=0) ) q &= ~mask;
  return *this;  
  };
  complex_iu operator *=(complex_iu r) {
  q *= r.q;
  q = q & ~mask;
  return *this;  
  };
  complex_iu operator /=(complex_iu r){
  if (r.q.real() != 0 && r.q.imag() != 0) q = q/r.q;
  else    q = 0;
  return *this;  
  };

  complex_iu operator <<=(const natural shift)  {
	  q = q << shift;
	  q = q & ~mask;
	  return *this;
  };
  complex_iu  operator >>=(const natural shift) {
	  q = q >> shift;
	  return *this;
  }

  complex_iu operator ^=(long r) {
  q = q ^ r;
  q = q & ~mask;
  return *this;  
  };
  complex_iu operator &=(long r) {
  q = q & r;
  q = q & ~mask;
  return *this;  
  };
  complex_iu operator |=(long r) {
  q = q | r;
  q = q & ~mask;
  return *this;  
  };

  inline bool operator !=(complex_iu r) { return (bool(q != r.q));  };
  inline bool operator ==(complex_iu r) { return (bool(q == r.q));  };
  inline long magsq(complex_iu y) { 
	  long x;
	  x = y.q.real()*y.q.real() + y.q.imag()*y.q.imag();
//	  x.mask = -1 << (y.bits * 2);
	  return(x); 
  };

  operator const cnatural () const { return(q); }
//  operator const complex<double> () const { 
//	  return(complex<double>((double)q.real(),(double)q.imag())); }

  complex_iu round(complex_iu in, natural rbits) {
	  double scale = 1.0/(double)MASK_BIT(rbits);
	  return(complex_iu(
		  (long)floor((double)(scale*in.q.real())+0.5),
		  (long)floor((double)(scale*in.q.imag())+0.5)));
//	  return(complex_iu(round(complex<long>(in), rbits)));
  };

  // Only positive overflow
  complex_iu saturate(complex_iu in, natural rbits) {
	    long r,i;
		long low_mask = ((1<<(bits-1)) - 1);
		if (labs(in.q.real()) > low_mask) 
			r = (in.q.real()>0) ? low_mask : ~low_mask;
		else r = in.q.real();
		if (labs(in.q.imag()) > low_mask) 
			i = (in.q.imag()>0) ? low_mask : ~low_mask;
		else i = in.q.imag();
		return(complex_iu(r,i));
  }


};
 complex_iu operator %(complex_iu r, natural l) {
	 complex_iu x;
	 x.q = r.q % l;
	 return(x);
 }
   complex_iu operator +(complex_iu r, complex_iu l) {
	complex_iu x;
	x.q = r.q + l.q;
	return(x);
  }
   complex_iu operator +(complex_iu r, natural l) {
	complex_iu x;
	x.q = r.q + l;
	return(x);
  }
   complex_iu operator +(cnatural r, complex_iu l) {
	complex_iu x;
	x.q = r + l.q;
	return(x);
  }
   complex_iu operator -(complex_iu r, complex_iu l) {
	complex_iu x;
	x.q = r.q - l.q;
	return(x);
  }
   complex_iu operator -(complex_iu r, cnatural l) {
	complex_iu x;
	x.q = r.q - l;
	return(x);
  }
   complex_iu operator -(cnatural r, complex_iu l) {
	complex_iu x;
	x.q = r - l.q;
	return(x);
  }
   complex_iu operator &(complex_iu r, natural l) {
	complex_iu x;
	x.q = r.q & l;
	return(x);
  }
   complex_iu operator &(natural r, complex_iu l) {
	complex_iu x;
	x.q = l.q & r;
	return(x);
  }
   complex_iu operator ^(complex_iu r, natural l) {
	complex_iu x;
	x.q = r.q ^ l;
	return(x);
  }
   complex_iu operator ^(natural r, complex_iu l) {
	complex_iu x;
	x.q = l.q ^ r;
	return(x);
  }
   complex_iu operator |(complex_iu r, natural l) {
	complex_iu x;
	x.q = r.q | l;
	return(x);
  }
   complex_iu operator |(natural r, complex_iu l) {
	complex_iu x;
	x.q = l.q | r;
	return(x);
  }
   complex_iu operator *(complex_iu r, complex_iu l) {
	complex_iu x;
	x.q = r.q*l.q;
	return(x);  
  };
   complex_iu operator *(complex_iu r, cnatural l) {
	complex_iu x;
	x.q = r.q*l;
	return(x);  
  };
   complex_iu operator *(complex_iu r, natural l) {
	complex_iu x;
	x.q = r.q*l;
	return(x);  
  };
   complex_iu operator *(natural r, complex_iu l) {
	complex_iu x;
	x.q = r*l.q;
	return(x);  
  };
   complex_iu operator *(cnatural r, complex_iu l) {
	complex_iu x;
	x.q = r*l.q;
	return(x);  
  };
   complex_iu operator /(complex_iu r, complex_iu l) {
	complex_iu x;
	x.q = r.q/l.q;
	return(x);  
  };
   complex_iu operator /(cnatural r, complex_iu l) {
	complex_iu x;
	x.q = r/l.q;
	return(x);  
  };
   complex_iu operator /(complex_iu r, cnatural l) {
	complex_iu x;
	x.q = r.q/l;
	return(x);  
  };
   complex_iu operator /(complex_iu r, natural l) {
	complex_iu x;
	x.q = r.q/l;
	return(x);  
  };

   complex_iu operator <<(complex_iu r, const natural shift)  {
	  complex_iu x;
	  x.q = r.q << shift;
	  x.mask = r.mask << shift;
	  x.bits = r.bits + shift;
	  return(x);
  };
   complex_iu operator >>(complex_iu r, const natural shift)  {
	  complex_iu x;
	  x.q = r.q >> shift;
	  x.mask = r.mask >> shift;
	  x.bits = r.bits - shift;
	  return(x);
  };
} // namespace SPUC 
#endif

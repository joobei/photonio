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
#ifndef UINTC
#define UINTC
#include <complex_iu.h>
namespace SPUC {
//! \brief Template class for complex fixed width unsigned integers.
//! \ingroup base
template <long Bits=32> class complex_u : public complexiu
{
	public:
	  complex_u() { 
		  bits=Bits;
		  mask = -1 << Bits;
	  }
	  complex_u(long r, long i=0) {
		  q = complex<long>(r,i); 
		  bits=Bits;
		  mask = -1 << Bits;
	  }
	  complex_u(complex<long>& y) {
		  q = y; 
		  bits=Bits;
		  mask = -1 << Bits;
	  }
	  complex_u(complex_iu& y) {
		  q = y.q; 
		  bits=Bits;
		  mask = -1 << Bits;
		  long r,i;
		  if (  ((mask&q.real())!=mask) || ((mask&q.real())!=0) ) {
			   r = q.real() & ~mask;}
		  if (  ((mask&q.imag())!=mask) || ((mask&q.imag())!=0) ) {
			   i = q.imag() & ~mask;}
		  q = complex<long>(r,i);
	  }
//	  complex_u(long y) { 
//		  q = y;
//		  bits=Bits;
//		  mask = -1 << Bits;
//	  }
	  inline complex_u operator =(const complex_iu& y) {   
	   q = y.q;
	   long r,i;
	   if (  ((mask&q.real())!=mask) || ((mask&q.real())!=0) ) {
		   r = q.real() & ~mask;}
	   if (  ((mask&q.imag())!=mask) || ((mask&q.imag())!=0) ) {
		   i = q.imag() & ~mask;}
	   q = complex<long>(r,i);
       return *this; 
	  } 
	  inline complex_u operator =(const long& y) {   
		q = y;
		if ( ((mask&y)!=mask) || ((mask&y)!=0) ) q = (y& ~mask);
		return *this; 
	  } 

 
};
} // namespace SPUC 
#endif

// SPUC - Signal processing using C++ - A DSP library
/* 
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
#ifndef DEBUGF
#define DEBUGF
#include <iostream.h>
#include <fstream.h>
#include <complex.h>
namespace SPUC {
//! \brief Debug function
//!  \author Tony Kirke,  Copyright(c) 2001 
//!  
class debugf {

 private:
  ofstream outf;
 public:
  debugf(const char* name) :  outf(name) { 	;   }
  ~debugf() {}
  void out(double x) { outf << x << " "; }
  void out(long x) { outf << x << " "; }
  void newline() { outf << "\n"; }
  void close() { outf.close(); }
  template <class T> void out(cmplx<T> x) { 
	outf << real(x) << " " << imag(x) << " ";
  }
  template <class T> void outn(T x) {
	out(x);
	outf << "\n";
  }
  template <class T> friend void operator <<(debugf r, const T x)  {
	  r.out(x);
  }
};
}
#endif

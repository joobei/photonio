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
#ifndef RUNAV
#define RUNAV
#include <spuc/generic/delay.h>
namespace SPUC {
/*! 
  \addtogroup fir FIR filters
*/
//! \brief template class running average filter consisting of a delay line, adder and subtractor
//! \ingroup fir
//! \image html running_average.gif
//! \image latex running_average.eps
template <class Numeric> class running_average
{
    protected:
	Numeric result;
    delay<Numeric>* z; 
	long size;
      
public: 
//!/ Constructor
	running_average(long n=0) : z(NULL) {
	  int i=0;
	  size = n;
	  if (n>1) {
		z = new delay<Numeric>(n-1);
		z->reset();
	  }
	  result = 0;
	}
	//! Assignment
	running_average& operator=(const running_average &rhs) {
	  z = rhs.z;
	  result = rhs.result;
	  return(*this);
	}
	//!
	void set_size(long n) {
	  if (z) delete z;
	  else {
		z = new delay<Numeric>(n-1);
		result = 0;
		size = n;
	  }
	}
	//! destructor
	~running_average(void) { if (z) delete z;}
	//! Reset/clear
	void reset(void) { z->reset(); result = 0;}
	//! return result
	Numeric get_result() { return(result);}
	//! Clock in new input sample  
	Numeric update(Numeric in) {
		result -= z->last();
		z->input(in);
		result += in;
		return(result);
	}
};
} // namespace SPUC 
#endif

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
#ifndef FIRD
#define FIRD
#include <fir.h>
namespace SPUC {
/*! 
  \addtogroup fir FIR filters
*/
//! \brief template class fir_decim based on FIR class, created to support polyphase FIR decimation
//! \ingroup fir
template <class Numeric> class fir_decim : public fir<Numeric>
{
 public: 
	//! Constructor
	fir_decim<Numeric>(void) { }
	fir_decim<Numeric>(long n) : fir<Numeric>(n) {	}
	fir_decim<Numeric>(const char* file) { fir<Numeric>::read_taps(file); }
	
	void input(Numeric in) {
		int i;                                           
		// Update history of inputs
		for (i=fir<Numeric>::num_taps-1;i>0;i--) fir<Numeric>::z[i] = fir<Numeric>::z[i-1];  
		// Add new input
		fir<Numeric>::z[0] = in;   
	}
	//! Phase increments when in automatic mode
	//! Otherwise phase does not change
	Numeric decim(void) {
		// Perform FIR
		Numeric output;
		int i;
		for (output=0,i=0;i<fir<Numeric>::num_taps;i++) output += fir<Numeric>::coeff[i]*fir<Numeric>::z[i];
		return(output); 
	}

};	
} // namespace SPUC 
#endif

#ifndef FIRI
#define FIRI
#include <fir.h>
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
/*! 
  \addtogroup fir FIR filters
*/
//! \brief template class fir_decim based on FIR class, created to support polyphase FIR interpolation
//! \ingroup fir interpolation
template <class Numeric> class fir_interp : public fir<Numeric>
{
	public: 
	long num_low;
    long rate;    //! upsampling rate       
    long phase;   //! current polyphase phase
	long auto_mode; //! if set, automaticaly increment phase
    
    public: 
    //! Skip output sample but increment phase
    void skip() { phase++; phase = phase%rate; }; 
	//! Set interpolation rate
	void set_rate(long r) { rate = r; num_low = (fir<Numeric>::num_taps/r - 1);}
	void set_automatic(void) { auto_mode = 1;};
	void set_manual(int def_phase=0) { auto_mode = 0; phase=def_phase;};


	//! Constructor
    fir_interp<Numeric>(const char* i) : phase(0), auto_mode(1), fir<Numeric>(i) {	};
    fir_interp<Numeric>(void) : phase(0), auto_mode(1) {	};
	fir_interp<Numeric>(long n) : phase(0), auto_mode(1),
		fir<Numeric>(n) {	};
	void reset() {
		fir<Numeric>::reset();
		phase = 0;
	}
	Numeric coeff_sum() { return(fir<Numeric>::coeff_sum()); }
    void input(Numeric in) {
		int i;                                           
		// Update history of inputs
		for (i=num_low;i>0;i--) fir<Numeric>::z[i] = fir<Numeric>::z[i-1];  
		// Add new input
		fir<Numeric>::z[0] = in;   
	}
	//! Explicitly set the phase
	Numeric clock(long set_phase) {
		phase = set_phase;
		return(clock());
	}
	//! Phase increments when in automatic mode
	//! Otherwise phase does not change
	Numeric clock(void) {
		// Perform FIR
		Numeric output;
		int i;
		for (output=0,i=0;i<num_low;i++) {
			output += fir<Numeric>::coeff[(i+1)*rate+phase]*fir<Numeric>::z[i];
		}
		// Increment to next polyphase filter
		if (auto_mode) phase++;                                       
		phase = phase%rate;
		return(output); 
	}

};	
} // namespace SPUC 
#endif

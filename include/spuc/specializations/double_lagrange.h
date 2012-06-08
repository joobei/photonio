//! 
//! author="Tony Kirke"
//! Copyright(c) 1993-1996 Tony Kirke
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
//
#ifndef DLAGRANGE
#define DLAGRANGE
#include <complex.h>
namespace SPUC {
/*! \file
  \brief Lagrange FIR for double type (non-template)
 */
class double_lagrange
{
      public: 
		long num_taps;
      	double* coeff;
      protected:
      	double* z; 
      	double output;
      
      public: 
      // Get current output
      double out() { return(output); }
      // Clock in new sample & compute current output
      //double clock(double in) { return(update(in)); }    
      double check(long i) { return(z[i]); }
       	
	  double_lagrange(void) { };
	  double_lagrange(long n) : num_taps(n) {
		int i;
		coeff = new double[n];
		z = new double[n];
		for (i=0;i<n;i++) z[i] = coeff[i] = 0;  
	} 
	  void set_size(long n) {
		int i;
		num_taps = n;
		coeff = new double[n];
		z = new double[n];
		for (i=0;i<n;i++) z[i] = coeff[i] = 0;  
	} 
	// Update
	double update(double in, double offset) {
		int i;                                                       
		// Update history of inputs
		for (i=num_taps;i>0;i--) z[i] = z[i-1];  
		// Add new input
		z[0] = in;   
		// Calculate coefficients
		for (k=0;k<n;k++) {
			coeff[k] = 1;
			for (i=0;i<n;i++) {
				if (k!=i) coeff[i] *= (offset-i)/(k-i);
			}
		// Perform FIR
		for (output=0,i=0;i<num_taps;i++) output += coeff[i]*z[i];
		return(output);
	}
};
} // namespace SPUC 
#endif      

#ifndef LAGRANGE
#define LAGRANGE
#include <iostream>
#include <fstream>
#include <complex.h>
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
using namespace std;
namespace SPUC {
/*! 
  \addtogroup fir FIR filters
  \addtogroup interpolation Interpolation filters
*/

/*!   \brief  Template Class for Lagrange interpolation using a FIR  filter. 
  \ingroup fir interpolation
*/
//!:
//!    Template Lagrange interpolation via FIR Fitler
//!    This works best for double or complex<double>
//!    Coefficients are always real (i.e. no imaginary parts).
//! \image html lagrange.gif
//! \image latex lagrange.eps
template <class Numeric> class lagrange
{
      public: 
		long num_taps;
      	double* coeff;
      protected:
      	Numeric* z; 
      	Numeric output;
      
      public: 
	  //!
	  void reset() { 	
			  for (int i=0;i<=num_taps;i++) z[i] = 0;
			  output = 0;
	  }
      //! Get current output
      Numeric out() { return(output); }
      //! Clock in new sample & compute current output
      Numeric check(long i) { return(z[i]); }
       	
	  lagrange(void) { };
	  //! Constructor
	  lagrange(long n) : num_taps(n) {
		int i;
		coeff = new double[n+1];
		z = new Numeric[n+1];
		for (i=0;i<=n;i++) {
			z[i] = 0;
			coeff[i] = 0;  
		}
	  } 
	  //! Destructor
		~lagrange(void) {
			  if (num_taps > 0) {
				delete [] coeff;
				delete [] z;
			  }
		}
//! Set order of polynomial
void set_size(long n)  {
	int i;
	num_taps = n;
	coeff = new double[n+1];
	z = new Numeric[n+1];
	for (i=0;i<=n;i++) {
		z[i] = 0;
		coeff[i] = 0;  
	}
} 
//! Input new sample (do nothing else)
void input(Numeric in) {
	int i; 
	// Update history of inputs
	for (i=num_taps;i>0;i--) z[i] = z[i-1];  
	// Add new input
	z[0] = in;   
}
/**
    Update => Clock in new input sample, recalculate coefficients and
    determine output
*/
Numeric update(Numeric in, double offset) {
	int i; 
	// Update history of inputs
	for (i=num_taps;i>0;i--) z[i] = z[i-1];  
	// Add new input
	z[0] = in;   
	// Calculate coefficients
	calculate_coeff(offset);
	// Perform FIR
	return(fir());
}
//! Calculate output for current coefficients
Numeric fir(void) {
	int i;                                                       
	// Perform FIR
	for (output=0,i=0;i<=num_taps;i++) output += coeff[i]*z[i];
	return(output);
}
//! Rephase => recalculate coefficients and output  for new offset (for upsampling)
Numeric rephase(double offset) {                                                  
	// Calculate coefficients
	calculate_coeff(offset);
	// Perform FIR
	return(fir());
}
//! Calculate coefficients given an offset
void calculate_coeff(double offset) {
	int i,k;                                                       
	// Calculate coefficients
	double off = offset + num_taps/2;
	for (k=0;k<=num_taps;k++) {
		coeff[num_taps-k] = 1;
		for (i=0;i<=num_taps;i++) if (k!=i) coeff[num_taps-k] *= double(i-off)/double(i-k);
	}
}
//!  Print out coefficients
void print() {
	cout << "Lagrange coefficients ";
	for (long i=0;i<=num_taps;i++) {
		cout << coeff[i] << ' ';
		if ((i+1)%6 == 0) cout << '\n';
	}
	cout << '\n';
	cout.flush();
}    
      
};
     
} // namespace SPUC 
#endif      

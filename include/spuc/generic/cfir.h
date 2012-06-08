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
#ifndef RFIR
#define RFIR
#include <iostream.h>
#include <fstream.h>
#include <complex.h>
namespace SPUC {
/*! 
  \addtogroup fir FIR filters
*/
//! \brief template class rfir FIR filter implementation with complex input and real coefficients
//! \ingroup fir
//! \image html rfir.gif
//! \image latex rfir.eps
template <class Numeric> class rfir
{
 public: 
  long num_taps;
  Numeric* coeff;
//      protected:
  complex<Numeric>* z; 
  complex<Numeric> output;
      
 public: 
  // Set tap weights
  void settap(long i, Numeric tap) { coeff[i] = tap; }  
  // Get current output
  complex<Numeric> out() { return(output); }
  // Clock in new sample & compute current output
  complex<Numeric> check(long i) { return(z[i]); }
  rfir(void) { ;};
rfir(long n) : num_taps(n)
{
	int i;
	coeff = new Numeric[n];
	z = new complex<Numeric>[n];
	for (i=0;i<n;i++) z[i] = 0;
	for (i=0;i<n;i++) coeff[i] = 0;  
} 
void set_size(long n) 
{
	int i;
	num_taps = n;
	coeff = new Numeric[n];
	z = new cmlx<Numeric>[n];
	for (i=0;i<n;i++) z[i] = 0;
	for (i=0;i<n;i++) coeff[i] = 0;  
} 
rfir(const char* file) { read_taps(file); };               
// Update
complex<Numeric> update(complex<Numeric> in)
{
	int i;                                                       
	// Update history of inputs
	for (i=num_taps;i>0;i--) z[i] = z[i-1];  
	// Add new input
	z[0] = in;   
	// Perform FIR
	for (output=0,i=0;i<num_taps;i++) output += coeff[i]*z[i];
	return(output);
}
void read_taps(const char* file);
void print(void);

};
} // namespace SPUC 
#endif

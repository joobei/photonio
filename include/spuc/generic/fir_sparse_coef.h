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
#ifndef FIRSPARSE
#define FIRSPARSE
#include <fir.h>
namespace SPUC {
/*! 
  \addtogroup fir FIR filters
*/
//! \brief template class fir_decim based on FIR class, created to support spare coefficients (zero padded)
//! \ingroup fir
template <class Numeric> class fir_sparse_coef : public fir<Numeric>
{
 public: 
  //! Constructor
  fir_sparse_coef<Numeric>(void) { }
  fir_sparse_coef<Numeric>(long n) : fir<Numeric>(n) {	}
  fir_sparse_coef<Numeric>(const char* file) { read_taps(file); };
  long rate;

  void set_rate(long r) { rate = r; }
  void input(Numeric in) {
	  int i;                                           
	  //! Update history of inputs
	  for (i=num_taps-1;i>0;i--) z[i] = z[i-1];  
	  //! Add new input
	  z[0] = in;   
  }
  //! Phase increments when in automatic mode
  //! Otherwise phase does not change
  Numeric update(Numeric in) {
	  input(in);
	  // Perform FIR
	  Numeric output;
	  int i;
	  for (output=0,i=0;i<num_taps/rate;i++) output += coeff[i]*z[i*rate];
	  return(output); 
  }

};	
} // namespace SPUC 
#endif

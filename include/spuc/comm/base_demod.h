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
// $Id: base_demod.h,v 1.5 2005/09/16 17:00:44 spuc Exp $ // Base class
#ifndef BASEDEMOD
#define BASEDEMOD
namespace SPUC {
/*! 
  \addtogroup sim Simulation Classes
 */
/*!  \brief  base class for a demodulator (not currently used?)
  \author Tony Kirke,  Copyright(c) 2001 
  
  \ingroup sim
 */
class base_demod {
 public:
  virtual complex<long> step(complex<double> x) { return(complex<long>(0,0)); }
  virtual bool sym_pulse() { return(0); };
  virtual bool buffer_empty() { return(0); }
  virtual bool get_bits() { return(0); }
  virtual complex<double> get_taps(int j) { return(complex<double>(0.0,0.0)); }
  virtual int eq_size() { return(0); }
  bool data_ready; 
  complex<double> sample_value;
  complex<double> symbol_value;
  char* vers;
};         
}
#endif

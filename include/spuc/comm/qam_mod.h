#ifndef spucqammodh
#define spucqammodh
#include <math.h>
#include <complex.h>
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
// SPUC - Signal processing using C++ - A DSP library
namespace SPUC {
//! \ingroup comm modulators
//! \brief   QAM Modulator for BPSK - 64 QAM for 802.11A Data modulation
//!  \author Tony Kirke,  Copyright(c) 2001 
//!  
class qam_mod
{
public:
  long rate_index; // 0 BPSK, Max for highest QAM, etc

 public:
  // Constructor (with default data rate)
  qam_mod(int index) {
	  rate_index = index;
  }
  ~qam_mod() {  } 
  complex<long> data_map(long r_ind, long data_in);
  complex<long> bpsk_map(long data_in);
  complex<long> qpsk_map(long data_in);
  complex<long> qam16_map(long data_in);
  complex<long> qam64_map(long data_in);
  
};
}
#endif

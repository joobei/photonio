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

#include <complex.h>
#include <max_pn.h>
#include <noise.h>
#include <fir.h>
#include <fir_interp.h>
#include <raised_cosine.h>
#include <lagrange.h>
namespace SPUC {
/*! 
  \addtogroup comm Communication Classes
*/

/*! \brief  Class for QPSK data using a root raised cosine transmit filter
  \author Tony Kirke,  Copyright(c) 2001 
  
  \ingroup comm
*/
//
// Samples are generated by a combination of polyphase FIR and lagrange
// interpolation (to allow for a wide range of sampling rates)
class quad_data
{
 public:
  int symbols;
  int over;
  max_pn pn_i;
  max_pn pn_q;
  lagrange <complex<double> > interp;
  fir_interp <complex<double> > rcfir;
  complex<double> data;
  double prev_timing_offset;

  quad_data(int total_over);
  complex<double> get_fir_output(void);
  complex<double> get_sample(double timing_inc);
  void set_initial_offset(double timing_init) { 
	prev_timing_offset=timing_init;
  }	
};
} // namespace SPUC 

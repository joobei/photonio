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
#ifndef CIC
#define CIC
namespace SPUC {
/*! 
  \addtogroup fir FIR filters
*/

/*!   \brief   class for CIC digital filter
  \ingroup fir interpolation
*/
//!
//!  Implementation for cascaded integrator comb filters
//!  This implementation provides for both decimation and
//!  interpolation filtering. Registers are signed long and
//!  the default number of stages is 3.
//! \image html cic.gif
//! \image latex cic.eps
class cic
{
 protected:
  signed long* nacc; //! Accumulators
  signed long* diff; //! Differentiators
  signed long* prev; //! Previous values
  char stages; //! Number of stages
 public:                         
  //! Constructor
  cic(char n=3);
  //! Reset
  void reset() { 	
	for (int i=0;i<stages;i++) nacc[i] = diff[i] = prev[i] = 0;
  }
  //! For CIC interpolation, non-zero dump implies new input
  signed long interpolate(signed long in=0, signed char dump=0);
  //! For CIC decimation, non-zero dump implies output required
  signed long decimate(signed long in, signed char dump);
  //! To change the number of stages dynamically
  void num_stages(char n);
};
} // namespace SPUC 
#endif

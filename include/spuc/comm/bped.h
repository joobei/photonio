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

#include <spuc.h>
#include <complex.h>
namespace SPUC {
/*! 
	  \addtogroup comm Communication Classes
*/
/*!   \brief Block Phase estimator
  \author Tony Kirke,  Copyright(c) 2001 
  
  \ingroup comm
*/
//!   Block Phase Estimator
//!    Unquantized BPE Calculate phase angle using a moving window
//! 
//!  <I>Notes:</I>
//!   Note that the BPE only cares about the phase of a vector, and not on the
//!    actual vector magnitude.  This results in a degradation of the average bpe
//!    vector magnitude as Eb/No decreases. 
class bped {

   complex<double>* bit;      //! array containing previous inputs 
   int quad_prev;      //! Previous quadrant
   int oqtstate;       //! quadrant cross-over indicator
   double ang;         //! current averaged angle
   char nbpe;          //! length of BPE
	                                            
 public:
   double angle() { return(ang); };            //! current averaged angle
   //! Constructor 
   bped(char len=16); 			
   //! Perform BPE but don't calculate output
   void clock(const complex<double>& in);      
   //! Rotate vector into first quadrant
   double fq_angle(const complex<double>& pid); 
   //! Reference Vector 
   complex<double> refvect(void) {return( polar(1.0, -ang) );}
   complex<double> output(const complex<double>& in);    //! Perform BPE and return transformed input
   complex<double> output(const complex<long>& in);   //! Perform BPE and return transformed input
};
} // namespace SPUC 

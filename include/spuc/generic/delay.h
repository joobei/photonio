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
#ifndef DLYL
#define DLYL
namespace SPUC {
/*! 
  \addtogroup miscfunc  Miscellaneous DSP classes
*/
/*!   \brief  Template class for Delay line
  \ingroup miscfunc
*///!
//! Template class for a Delay line (primitive used in other classes)
//!  Allows user to check at various points in delay line,
//!  but default use is a pure delay.
//! \image html delay.gif
//! \image latex delay.eps
template <class Numeric> class delay 
{
 public: 
  long num_taps;
 protected:
  Numeric* z; 
      
 public: 
  //! Constructor
  delay(long n=0) : num_taps(n+1) {
	int i=0;
	z = new Numeric[num_taps];
	for (i=0;i<num_taps;i++) z[i] = 0;
  }
  //! Assignment
  delay& operator=(const delay &rhs) {
	num_taps = rhs.num_taps;
	for (int i=0;i<num_taps;i++) z[i] = rhs.z[i];
	return(*this);
  }
  //! Destructor
  ~delay(void) {	if (num_taps>0) delete [] z;}
  void reset(void) { for (int i=0;i<num_taps;i++) z[i] = 0; }
  //! Get delay at tap i
  Numeric check(long i) { return(z[i]); }
  //! Look back in delay line by i samples
  Numeric checkback(long i) { return(z[num_taps-1-i]); }
  //! Get last tap
  Numeric last() { return(z[num_taps-1]);}
  //! Set size of delay
  void set_size(long n=2) {
	int i=0;
	if (num_taps>0) delete [] z;
	num_taps = n+1;
	z = new Numeric[num_taps];
	for (i=0;i<num_taps;i++) z[i] = 0;
  }  
  //! Clock in new input sample  
  Numeric input(Numeric in) {
	int i;                                           
	// Update history of inputs
	for (i=num_taps-1;i>0;i--) z[i] = z[i-1];  
	// Add new input
	z[0] = in;   
	return(z[num_taps-1]);
  }
  //! Clock in new sample and get output from delay line
  inline Numeric update(Numeric in) {
	input(in);
	return(z[num_taps-1]);
  }	
};
} // namespace SPUC 
#endif

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
//
#ifndef A_D
#define A_D
#include <math.h>
#include <complex.h>
namespace SPUC {
/*! 
  \addtogroup sim Simulation Classes
*/
/*!   \brief An A/D conversion class
  \ingroup misc
*///
//
//   Class for A/D conversion.
//   Bit width is specified in constructor. Default is 8 bits.
//   Handles real and complex samples
// \image html a_d.gif
// \image latex a_d.eps
class a_d
{
protected:
	char size;              //! Number of bits <= 8
	long max;
	long min;

	public:
	complex<long> sample(complex<double> x) {
		return( complex<long>( sample(x.real()),sample(x.imag()) ) ); 
	}

	//! Constructor
	a_d(char h=8) : size(h) {   
		max =  ((1<<(size-1)) - 1 );
		min = ~max;
	}
	//! Quantize and limit signal to size bits  
	long sample(double x) {
		long quant;
		quant = (long)floor(x+0.5);
		if (quant>max) quant = max;
		if (quant<min) quant = min;
		return(quant);
	} 
};
} // namespace SPUC 
#endif

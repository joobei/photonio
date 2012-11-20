// 
// author="Tony Kirke"
// Copyright (c) 1993-1998 Tony Kirke
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
#ifndef CRDC
#define CRDC
#include <math.h>
#include <complex.h>
namespace SPUC {
/*! 
  \addtogroup miscfunc  Miscellaneous DSP classes
*/

/*!  
  \brief  Cordic rotator
  \ingroup miscfunc
*/
//!
//!  Performs CORDIC rotations
//!  To rotate a vector through an angle of theta, we calculate:
//!
//!	x' = x cos(theta) - y sin(theta)
//!	y' = x sin(theta) + y cos(theta)
//! Can be easily modified for hyperbolic and other functions
//! \image html cordic.gif
class cordic
{
public:
	double* arctan_lut;
	complex<long>* stage;
	int stages;
	double vector_angle;
	//!  initializes tables and constants for the CORDIC algorithm
	cordic(int n=7);
	//! Returns magnitude through CORDIC vectoring
	long vector_mag(complex <long> in) {
	  vector(in);
	}
	//! Returns arg through CORDIC vectoring
	double vector_arg(complex <long> in) {
	  vector(in);
	  return(vector_angle);
	}
	complex <long> rotate(complex <long> in, double angle);
 protected:
	//! Cordic vectoring calculates arg and magnitude
	complex<long> vector(complex <long> in);
};
} // namespace SPUC 
#endif

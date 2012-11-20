// version="$Revision: 1.1 $"
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
#include <iostream>
#include <fstream>
#include <complex.h>
#include <vector.h>
#include <fir.h>
namespace SPUC {

  /// Dummy functions for library instantations
  int dummy_cl(complex<long> z) {
	int a;
	a = real(z) + imag(z);
	return(a);
  }
  int dummy_vf(void) {
	double a;
	fir<double> z(32);
	Vector<double> tmp;

	tmp = Vector_taps(z);
	
	a = tmp(0);
	return((int)a);
  }
} // namespace SPUC 

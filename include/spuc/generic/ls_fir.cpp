// 
// author="Tony Kirke"
// Copyright(c) 1993-1996 Tony Kirke
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
#include <math.h>
#include <spuc.h>
#include <complex.h>
#include <fir.h>
#include <matrix.h>
namespace SPUC {
//:
/*! 
  \addtogroup fir FIR filters
*/
//! \brief calculates the least square filter impulse response
//! \ingroup fir
void ls_fir(fir<double> fil, double fc, double spb)
{
  int i,j;
  double f = 0.01;
  int nir = fil.num_taps;
  int N =  4*nir;

  Matrix<double> U(N,N);
  double* gf = new double[nir];

  for (i=0;i<N;i++) {
	for (j=0;j<N;j++) {
	  U(i,j) = cos(i*j*f);
	}
  }
}
}

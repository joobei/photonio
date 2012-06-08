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
#include "remez_fir.h"
namespace SPUC {
//: <font color="red"><i>Under construction!</i></font>
// \brief Template remez function where the desired response and
// weighting functions are passed as template functions,
// thus allowing a simpler interface
//!  \author Tony Kirke,  Copyright(c) 2001 
template <double Des_function(double), double Weight_function(double)>
double* remez_function(int numtaps, int numband, int r, double bands[],int type) {

  double floor(double x);
  int j=0;
  int k;
  double lowf, highf;
  double delf = 0.5/(GRIDDENSITY*r);
  double* r_fir = new double[numtaps];
  // Predict dense grid size in advance for array sizes
  int gridSize = 0;
  for (int i=0; i<numband; i++) {
	gridSize += (int)floor(0.5+2*r*GRIDDENSITY*(bands[2*i+1] - bands[2*i]));
  }
  int symmetry = (type == BANDPASS)? POSITIVE : NEGATIVE;
  if (symmetry == NEGATIVE) gridSize--;
  
  double* des = new double[gridSize];
  double* weight  = new double[gridSize];
  
  for (int band=0; band < numband; band++) {
	lowf = bands[2*band];
	highf = bands[2*band + 1];
	k = (int)floor((highf - lowf)/delf+0.5);
	for (int i=0; i<k; i++) {
	  des[band] = Des_function(lowf);
	  weight[band] = Weight_function(lowf);
	  lowf += delf;
	  j++;
	}
  }

  r_fir = remez_fir::remez(numtaps,numband,bands,des,weight,type);
  return(r_fir);
}
}

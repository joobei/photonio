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
#include <spuc.h> 
#include <cordic.h>
namespace SPUC {
//  allocates memory and determine arctans for use in the CORDIC algorithm
cordic::cordic(int n) {
	int i;
	stages = n;
	arctan_lut = new double[stages+2]; 
	stage = new complex<long>[stages+2];
	for (i=0;i<=stages;i++) arctan_lut[i] = atan(1.0/(double)(1<<i));
}
// This routine performs multiple rotations of the form:
//	x[i+1] = cos(angle[i]) { x[i] - y[i] tan(angle[i]) }
//	y[i+1] = cos(angle[i]) { y[i] + x[i] tan(angle[i]) }
// where tan(angle[i]) = 2^-i, with i being an integer. Thus allowing
// implementation by shifting and adding (or subtracting). 
// Each stage shifts by either a positive or negative angle.
complex <long> cordic::rotate(complex <long> in, double angle) 
{
  int i;
  // Initial rotation, with left shift 
  if (angle > PI) {
	stage[0].re = (-in.re << stages);
	stage[0].im = (-in.im << stages);
	angle -= PI;
  } else {
	stage[0] = in << stages;
  }
  if (angle > PI/2) {
	stage[1].re = -stage[0].im;
	stage[1].im =  stage[0].re;
	angle -= PI/2;
  } else {
	stage[1] = stage[0];
  }
	// Subsequent rotations, with right shifts 
  for (i=0;i<=stages;i++) {
	long sign = (angle < 0) ? -1 : 1;
	stage[i+2].re = stage[i+1].re - sign*(stage[i+1].im >>i);
	stage[i+2].im = stage[i+1].im + sign*(stage[i+1].re >>i);
	angle -= sign*arctan_lut[i];
  }
  return(stage[stages]);
}
// This routine performs multiple rotations of the form:
//	x[i+1].re = { x[i].re -/+ x[i].im * tan(angle[i]) }
//	x[i+1].im = { x[i].im +/- x[i].re * tan(angle[i]) }
// where tan(angle[i]) = 2^-i, with i being an integer.
// The -/+ is determined by the current SIGN of x[i].im
// in such a way that the angle of x[i] is reduced in 
// each step. The final step will hold the approximation
// of the magnitude of x in the real part of x[stages].
complex<long> cordic::vector(complex<long> in)
{
  int i;
  // Rotate into 1st Quadrant
  if (in.real()<0) {
	if (in.imag()<0) {
	  stage[0] = -in << stages;
	  vector_angle = -PI;
	} else {
	  stage[0].re = in.im << stages;
	  stage[0].im = -in.re << stages;
	  vector_angle = -PI/2;
	}
  } else {
	if (in.imag()<0) {
	  stage[0].re = -in.im << stages;
	  stage[0].im = in.re << stages;
	  vector_angle = -3*PI/2;
	} else {
	  stage[0] = in << stages;
	  vector_angle = 0;
	}
  }
	
  // Subsequent rotations, with right shifts 
  for (i=0;i<stages;i++) {
	long sign = (stage[i].im < 0) ? -1 : 1;
	stage[i+1].re = stage[i].re - sign*(stage[i].im >>i);
	stage[i+1].im = stage[i].im + sign*(stage[i].re >>i);
	vector_angle -= sign*arctan_lut[i];
  }
  return(stage[stages].re);
}


} // namespace SPUC 

// 
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
// Gaussian noise routine
// Can create (double) gaussian white noise
//            complex<double> gaussian white noise
//        or  (double) uniform noise
#include <math.h> 
#include <complex.h>
#include "noise.h"
namespace SPUC {
noise::noise(double i) : idum((long)i), s(0) {;}	// Constructor
complex<double> noise::Cgauss(void)
{
	double fac, r, v1, v2;
		do {
			v1 = (2.0 * uniform()) - 1.0;
			v2 = (2.0 * uniform()) - 1.0;
			r = (v1*v1) + (v2*v2);
		} while (r >= 1.0);
		fac = sqrt(-2.0 * log(r) / r);
		return( complex<double>( v2*fac, v1*fac ) );
}  
double noise::gauss()
	{
	double fac, r, v1, v2;

		if (s == 0) {
			do {
				v1 = (2.0 * uni()) - 1.0;
				v2 = (2.0 * uni()) - 1.0;
				r = (v1*v1) + (v2*v2);
			} while (r >= 1.0);
			fac = sqrt(-2.0 * log(r) / r);
			x = v1 * fac;
			s = 1;
			return (v2*fac);
		} else {
			s = 0;
			return (x);
		}
	}       
double noise::uni(void)
//--------------------------------------------------------------------
//       Returns uniform deviate between 0.0 and 1.0.
//       Used to generate PN data
//---------------------------------------------------------------------

	{
	double rm,r1;
	rm  = 1./M;
	idum = (long)fmod(IA*idum+IC,M);
	r1 = idum*rm;
	return(r1);
}
double noise::uniform(void)
//--------------------------------------------------------------------
//       Returns uniform deviate between 0.0 and 1.0.
//       Used to generate PN data
//---------------------------------------------------------------------

	{
	double r1;
	long hi;
	hi = idum/LQ;
	idum = LA*(idum-hi*LQ) - LR*hi;
	if (idum < 0) idum += LM;
	r1  = LAM*idum;
	return(r1);
}
} // namespace SPUC 

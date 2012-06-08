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
// See vco.h for description
#include <spuc.h>
#include <complex.h>
#include <vco.h>
namespace SPUC {
complex<double> vco::clock() {
		acc += new_fcw;
		if (acc > TWOPI) acc -= TWOPI;
		if (acc < -TWOPI) acc += TWOPI;
		return(expj(acc));
}
complex<double> vco::clock(double loop_filter_out) {
	load(loop_filter_out);
	return(clock());
}

} // namespace SPUC 

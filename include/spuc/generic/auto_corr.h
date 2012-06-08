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
#ifndef AUTOC
#define AUTOC
namespace SPUC {
// \brief Compute the autocorrelation of the Vector
//!  \author Tony Kirke,  Copyright(c) 2001 
//! \ingroup miscfunc
//: <font color="red"><i>Under construction!</i></font>
template <class T> Vector<T> auto_corr(Vector<T> x) {
    int i,j;
	T d;
	long N = x.size();
	Vector<T> autoc(N);
	
	for (j=0;j<N;j++) {
		for (i = j, d = 0; i < N; i++) d += x[i] * x[i-j];
		autoc[j] = d;
	}
	return(autoc);
}
}
#endif

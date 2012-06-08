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
#include <math.h>
#include <max_pn.h>
namespace SPUC {
signed char max_pn::out() {
		u <<= 1;
		if ((u&lenp1)==0) return(-1);
		else {
			u ^= gen;
			return(1);
		}
}
bool max_pn::get_bit() {
		u <<= 1;
		if ((u&lenp1)==0) return(0);
		else {
			u ^= gen;
			return(1);
		}
}

signed char max_pn::out1() {
	    int i,ii,is;
	    char data;  
	    char n=(char)(log(lenp1)/log(2));

		u <<= 1;
		if ((u&lenp1) != 0) data = 1; 
		else data = -1;
		i = u&gen;
		for (ii=is=0;ii<n;ii++) {is = (is + (i>>ii)&1)%2;} 
		u += is;  
		return(data);
}
} // namespace SPUC 

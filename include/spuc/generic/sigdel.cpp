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
#include <sigdel.h>
namespace SPUC {
sigma_delta::sigma_delta(char nin, char nout) {
	nbit_in = 31-nin;
	nbit_out = 32-nout;
	acc = 0;
	mask = (0xffffffff>>nout);
}               
signed char sigma_delta::single(signed long in)
{          
	signed long tmp = (in<<nbit_in); 
	acc += tmp;
	tmp = acc&(~mask);
	signed char out = (char)(tmp>>nbit_out);
	acc -= tmp;
	return(out);
}
} // namespace SPUC 

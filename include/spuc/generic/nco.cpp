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
// Set nco.h for description.
#include <spuc.h>
#include <nco.h>
namespace SPUC {
nco::nco(long bits) {
			acc = fcw = new_fcw = phase = 0;
			mask_bits = MASK_NEG_HI(bits);
}
long nco::clock() {
		acc += new_fcw;
		phase = acc & mask_bits; // truncate 
		return(phase);
}
long nco::clock(long loop_filter_out) {
		new_fcw = fcw + loop_filter_out;
		return(clock());
}
} // namespace SPUC 

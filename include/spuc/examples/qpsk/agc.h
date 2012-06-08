namespace SPUC {

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
#ifndef QPSKAGC
#define QPSKAGC
#endif
//:
//! \brief This is a simple sigma-delta type AGC for the variable rate QPSK example
//! \ingroup examples
//! \author Tony Kirke, Copyright(c) 1993-1996
//! 
class agc {
 public:	
	long agc_acc;
	long agc_bit;
	long agc_thres;

	agc(long thres=32) : agc_thres(thres) {
		agc_acc = 0;
		agc_bit = 0;
	}
	long run(complex<long> adc) {
		long	abs_level = abs(adc.re) + abs(adc.im);
		long agc_diff = abs_level - agc_thres + 32;
		if (agc_diff > 64) agc_diff = 64;
		if (agc_diff < 0) agc_diff = 0;
		agc_acc += agc_diff;
		agc_bit = (agc_acc & 0x40) ? 0 : 1;
		agc_acc &= 0x3f; // Clear overflow bit
		return(agc_bit);
	}
};
} // namespace SPUC 

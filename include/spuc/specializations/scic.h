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
#ifndef SCIC
#define SCIC
#include <delay.h>
#include <cic.h>
namespace SPUC {
//!  \ingroup fir
//! \brief Implementation for sharped cascaded integrator comb filter
//!  \author Tony Kirke,  Copyright(c) 2001 
//!  
//
//! Registers are signed long and default number of stages is 2.
class scic
{
 protected:
	cic main;
	cic sub;
	delay<long> dly;
	char stages;
	long max_rate;
 public:                         
	// Constructor
	scic(char n=2, long r=4);
	// For SCIC decimation
	signed long decimate(signed long in, long rate, signed char dump);
	// To change the number of stages dynamically
	void num_stages(char n, long r);
};
} // namespace SPUC 
#endif

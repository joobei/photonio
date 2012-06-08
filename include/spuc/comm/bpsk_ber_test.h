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
#include <time.h>	// For time functions
#ifdef KBHITD
#include <conio.h>	// For kbhit
#endif
#include <max_pn.h>   	// Maximal length PN generator
#include <iostream>
#include <fstream>
#include <iomanip>
namespace SPUC {
/*! 
  \addtogroup sim Simulation Classes
*/
/*! \brief  A Class for simulating a BPSK system 
  \author Tony Kirke,  Copyright(c) 2001 
  
  \ingroup sim
*/
class bpsk_ber_test
{
	public:
	long modc;
	time_t start_time;
	max_pn ref;
	long corr_sum;
	long errors;
	signed char synced;
	long interval;
	long prev_errors;

	bpsk_ber_test(void) : ref(0x006d,63,-1) {
		modc = 1;
		start_time = time(NULL);
		errors = 0;
		corr_sum = 0;
		synced = 0;
		interval = 0;
		prev_errors = 0;
	}

	signed char found_sync(void) { return(synced); }
	void ber_results(long received);
	long synchronize(long* received, long data);
	void final_results(long received);
	void correlate(long* received, long data) {
	  synchronize(received, data);
	  ber_results(*received);
	}
	inline void print_running_ber(void) {
	  long sym_int = interval;
	  cout << "Symbol interval = " << setw(8) << sym_int;
	  cout << "BER = " << running_ber() << '\n';
	}
    double running_ber(void);
	double ber(long received) { return(errors/(double)received); }
	
};
} // namespace SPUC

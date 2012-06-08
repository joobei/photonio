// 
// author="Tony Kirke" *
// Copyright(c) 2001 Tony Kirke
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
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;
#include "bpsk_ber_test.h"
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//	Determine BER result
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
using namespace SPUC;
void bpsk_ber_test::ber_results(long received)
{
  // For measuring simulation time
	
  if ((received%modc == 0) && synced ){
	if (received>512) {
	  cout << "Symbols = " << received;
	  cout << " Errors = ", errors;
	  cout << " BER = " << (double)errors/(double)received;
	  cout << " Elpsd Time = " << difftime(time(NULL),start_time)/60.0 << "(min)\n";
	  cout.flush();
	}
	modc <<= 1;
  }
#ifdef KBHITD
  if(kbhit()) {
	if (received>512) {
	  cout << "Symbols = " << received;
	  cout << " Errors = ", errors;
	  cout << " BER = ", (double)errors/(double)received;
	  cout << " Elpsd Time = " << difftime(time(NULL),start_time)/60.0 << "(min)\n";
	}
	getch();
//	if (getch() == 's') exit(1);  
	}
#endif
}
//***********************************************************************************************
// Correlate received signal with reference PN and indicate when 
// Synchronization is found by setting sync to +1 or -1.
// When synchronization is first found output is 2,
// otherwise it is the received bit multiplied by the
// reference PN bit.
//*********************************************************************************************          
long bpsk_ber_test::synchronize(long* received, long data)
{
	const char thres = 55;               // Threshold for sync
	const char pnlen = 63;
	signed char out=0;
		
// Correlate with reference PN 
	signed char tmp = ref.out();
	out = (data>=0) ? tmp : -tmp;
	corr_sum += out;
	if ((*received%pnlen==0) && (synced==0)) {
			if (corr_sum>thres) {
					synced = 1;    	// Synchronization achieved
					*received = 1;  // Reset number of symbols
					interval = 1;
			} else if (-corr_sum>thres) {
					synced = -1; 
					*received = 1;
					interval = 1;
			} else {
				 	corr_sum = 0;	// Reset sum 
					ref.out();  // Clock extra PN sample
			}
			
	}
	// End correlation block
	// Count Errors
	if (synced!=0) errors += (out==synced) ? 0 : 1;
	interval++;
	return(errors);
}
// 
// Final result
//
void bpsk_ber_test::final_results(long received)
{

	if (synced!=0) {
		cout << "Symbols = " << received;
		cout << " Errors = " << errors;
		cout << " BER = ", (double)errors/(double)received;
		cout << " Elpsd Time = " << difftime(time(NULL),start_time)/60.0 << "(min)\n";
	} else {
	  cout << "Synchronization with reference PN not found!\n";
	}
}
//
// Tracks the number of errors since last invocation and
// returns the BER over the interval
// 
double bpsk_ber_test::running_ber(void) 
{
			long new_errors = errors - prev_errors;
			prev_errors = new_errors;
			if (interval == 0) return(0); // Error condition
			double rber = new_errors/(double)interval;
			interval = 0; // reset interval counter
			return(rber);
}


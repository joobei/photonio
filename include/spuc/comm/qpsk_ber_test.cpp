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
#include "qpsk_ber_test.h"
namespace SPUC {
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//	Determine BER result
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void qpsk_ber_test::ber_results(long received)
{
  // For measuring simulation time
  double elapsed_time=0;
  if ((received%modc == 0) && sync ){
	elapsed_time = difftime(time(NULL),start_time); 
	if (received>512) {
	  cout << "Symbols = " << received;
	  cout << " Errors = " << errors;
	  cout << " BER = " << (double)errors/(double)received;
	  cout << " Elpsd Time = " << elapsed_time/60.0 << "(min)\n";
	  cout.flush();
	}
	modc <<= 1;
  }
#ifdef KBHITD
  if(kbhit()) {
	elapsed_time = difftime(time(NULL),start_time);
	if (received>512) {
	  cout << "Symbols = " << received;
	  cout << " Errors = " << errors;
	  cout << " BER = " << (double)errors/(double)received;
	  cout << " Elpsd Time = " << elapsed_time/60.0 << "(min)\n";
	  cout.flush();
	}
	getch();
	}
#endif
}
//**********************************************************************************
// Correlate received signal with reference PN and indicate when 
// Synchronization is found by setting sync to +1 or -1.
// When synchronization is first found output is 2,
// otherwise it is the received bit multiplied by the
// reference PN bit.
//
long qpsk_ber_test::synchronize(long* received, complex<long> data)
{
	const char thres = 57; // Threshold for sync
	const char pnlen = 63;
	complex<long> out=0;
	
// Correlate with reference PN 
	signed char tmp = ref.out();
	out = tmp*data;
	corr_sum += out;
	if ((*received%pnlen==0) && (sync==0)) {
	  
	  if (corr_sum.real()>thres) {
		rotate = complex<long>(1,0); 
		*received = 1; 
		interval = 1;
		sync = 1;
	  } else if (-corr_sum.real()>thres) {
		rotate = complex<long>(-1,0); 
		*received = 1;
		interval = 1;
		sync = 1;
	  } else if (corr_sum.imag()>thres) {
		rotate = complex<long>(0,-1); 
		*received = 1;
		interval = 1;
		sync = 1;
	  } else if (-corr_sum.imag()>thres) {
		rotate = complex<long>(0,1); 
		*received = 1;
		interval = 1;
		sync = 1;
	  } else {
		sync = 0;
		corr_sum = 0;	// Reset sum 
		ref.out();  // Clock extra PN sample
		rotate = complex<long>(0,0);
	  }	
		
	}
	// End correlation block
	// Count Errors
	if (sync!=0) errors += (re(out*rotate)==1) ? 0 : 1;
	interval++;
	return(errors);
}
// 
// Final result
//
void qpsk_ber_test::final_results(long received)
{
  double elapsed_time=0;
  if (sync!=0) {
	elapsed_time = difftime(time(NULL),start_time);
	cout << "Symbols = " << received;
	cout << " Errors = " << errors;
	cout << " Elpsd Time = " << elapsed_time/60.0 << "(min)\n";
	cout << " BER = " << (double)errors/(double)received << "\n";
    cout.flush();
  } else {
	cout << "Synchronization with reference PN not found!\n";
    cout.flush();
  }
}
// Tracks the number of errors since last call
// and returns the BER over the interval
// 
double qpsk_ber_test::running_ber(void) 
{
  long new_errors = errors - prev_errors;
  prev_errors = new_errors;
  if (interval == 0) return(0); // Error condition
  double rber = new_errors/(double)interval;
  interval = 0; // reset interval counter
  return(rber);
}

} // namespace SPUC 

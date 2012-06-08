// SPUC - Signal processing using C++ - A DSP library
/* 
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
#ifndef DEBUGFC
#define DEBUGFC
#include <iostream.h>
#include <fstream.h>
#include <cmplx.h>
#include <debugf.h>
typedef debugf* dbf;
#define MSTREAMS 10
namespace SPUC {
//! \brief Debug function
//!  \author Tony Kirke,  Copyright(c) 2001 
//!  
class debugfc {

 private:
	dbf fc[MSTREAMS];
	bool on[MSTREAMS];
	int streams;
 public:
	inline bool is_on(int x) { return(on[x]); }
	debugfc() { 
		streams=0;
		for (int i=0;i<MSTREAMS;i++) on[i]=0;
	}
	~debugfc() {
	}
	void add_stream_number(const int num, const char* n) {
		// If not added already
		if (!on[num]) {
			on[num] = 1;
			debugf* ptr = new debugf(n);
			fc[num] = ptr;
			streams++;
		}
	}
	void add_stream(const char* n) {
		if (!on[streams]) {
			on[streams] = 1;
			debugf* ptr = new debugf(n);
			fc[streams++] = ptr;
		}
	}
	void close_stream(const int num) {
		if (on[num]) {
			fc[num]->close();
			on[num] = 0;
		}
	}
	void close_all_streams() {
		for (int i=0;i<MSTREAMS;i++) {
			if (on[i]) {
				fc[i]->close();
				on[i] = 0;
			}
		}
	}
	void newline(const int n) { fc[n]->newline(); }
	template <class T> void out(int stream_num, T x) {
		fc[stream_num]->out(x);
	}
	template <class T> void outn(int stream_num, T x) {
		fc[stream_num]->outn(x);
	}
};
}
#endif

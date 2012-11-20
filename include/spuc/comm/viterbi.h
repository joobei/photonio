// SPUC - Signal processing using C++ - A DSP library
/* Copyright 1999 Phil Karn, KA9Q
 * Converted to C++ and modified by Tony Kirke Feb 2003
*/
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
/* The two generator polynomials for the NASA Standard K=7 code.
 * Since these polynomials are known to be optimal for this constraint
 * length there is not much point in changing them. But if you do, you
 * will have to regenerate the BUTTERFLY macro calls in viterbi()
 */
#ifndef VITERBIS
#define VITERBIS
#define	POLYA	0x6d
#define	POLYB	0x4f
namespace SPUC {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
class viterbi_state
{
 public:
  unsigned long path;	/* Decoded path to this state */
  long metric;		/* Cumulative metric to this state */
};
#endif
/*! \brief A Viterbi decoder (for DVB)
  \author Tony Kirke,  Copyright(c) 2001 
  
  \ingroup comm fec
*/
class viterbi
{
 public:
  bool decoded;
  bool enable_output;
  bool output_ready;
  long prev_value;
  viterbi_state state0[64],state1[64],*state,*next;
  int bitcnt;
  int beststate;
  long depuncture_bit_number;
  bool phase;

  viterbi() {
	  reset();
	  state = state0;
	  next = state1;
	  for(int i=0;i<64;i++)	state[i].metric = -999999;
	  state[0].path = 0;
  }
  void reset() {
	phase=0;
	depuncture_bit_number=0;
	decoded=1;
	output_ready=0;
	enable_output=0;
	bitcnt=0;
	prev_value=0;
	beststate=0;
	state = state0;
	next = state1;
	// Initialize starting metrics
	// ...no longer prefer 0 state
	for(int i=0;i<64;i++) {
		state0[i].metric = -999999;
		state1[i].metric = -999999;
		state0[i].path = 0;
		state1[i].path = 0;
	}
  }
  bool clock(long value) {
	bool z;
	decoded = !decoded;
	if (decoded) z = decode(prev_value,value);
	prev_value = value;
	if (enable_output) output_ready = decoded;
	return(z);
  }
  bool decode(long s0, long s1);
  void minimize_metrics() {
	long bestmetric = state[beststate].metric;
	for(int i=0;i<64;i++){
	  state[i].metric -=  bestmetric;
	}
  }
  bool depuncture(const long steal, long soft_in);
};
}
#endif

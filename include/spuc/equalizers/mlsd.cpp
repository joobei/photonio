// 
// author="Tony Kirke"
// Copyright (c) 2001 Tony Kirke
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
#include "mlsd.h"
//:
// Maximum Likelihood Sequence Detection
// Assuming binary alphabet [1,-1]
//
// Constructor with feedforward size, feedback size and gain
namespace SPUC {
mlsd::mlsd(char inf) : num_taps(inf) {
  cfir.set_size(inf);
  //	cfir[nf/2] = 1;
  n_states = 1 << (inf-1); 
  path = new long[n_states];
  tmp_path = new long[n_states];
  weight = new double[n_states];
  tmp_weight = new double[n_states];
	for (int i=0;i<n_states;i++) {
	  path[i] = 0;
	  weight[i] = 0;
	}
}
complex<double> mlsd::estimate(long seq) {
  int j, i;
  complex<double> pred = (0,0);
  j = (0x1 << num_taps);
  for(i = 0; i < num_taps; i++) {
	j >>= 1;
	if((seq & j) != 0) pred += cfir.coeff[i];
	else pred -= cfir.coeff[i];
  }
  return(pred);
}
long mlsd::seq_estimation(complex<double> y) {
  double temp0,temp1;
  long ex_state0, ex_state1;
  long old_state0, old_state1;
  double metric0,metric1;
  double min_weight;
  long path_state;
  int i;
  for (i=0;i<n_states;i++) {
	// 0 transition
	ex_state0 = (i<<1);
	// previous state assuming 0 transition
	metric0 = magsq(y-estimate(ex_state0));
	old_state0 = ex_state0 % n_states;
	temp0 = weight[old_state0] + metric0;
	// 1 transition
	ex_state1 = ex_state0 + 1;
	// previous state assuming 1 transition
	metric1 = magsq(y-estimate(ex_state1));
	old_state1 = ex_state1 % n_states;
	temp1 = weight[old_state1] + metric1;
	if (temp0 < temp1) {
	  tmp_weight[i] = temp0;
	  tmp_path[i] = (path[old_state0] << 1);
	} else {
	  tmp_weight[i] = temp1;
	  tmp_path[i] = (path[old_state1] << 1) | 0x1;
	}
  }
  // get minimum weight for all the
  // states and return the path data
  // for this minimum weight
  // Also, store new weights and paths
  // for all states
  min_weight = tmp_weight[0];
  path_state = 0;
  for (i=0;i<n_states;i++) {
	if (tmp_weight[i] < min_weight) {
	  min_weight = tmp_weight[i];
	  path_state = i;
	}
	// store for next iteration
	
	weight[i] = tmp_weight[i];
	path[i] = tmp_path[i];
  }
  return(path[path_state]);
}
} // namespace SPUC 

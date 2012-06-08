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
#ifndef TOPLTZ
#define TOPLTZ
namespace SPUC {
//! \brief Get Symmetric Toeplitz matrix from Vector
//!  \author Tony Kirke,  Copyright(c) 2001 
//: <font color="red"><i>Under construction!</i></font>
template <class T> Matrix<T> toeplitz(Vector<T> x) {
  
  int i,j;
  int k=0;
  long N = x.size();
  Matrix<T> A(N,N);
 
  for (j=0;j<N;j++) {
	  for (i=0;i<N;i++) A[j][i] = x[(i+k)%N];
	  k++;
  }
  return(A);
}
}
#endif

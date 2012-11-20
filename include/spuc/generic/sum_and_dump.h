namespace SPUC {
// 
// author="Tony Kirke"
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
//!:
//! This modules sums the input for a fixed number of samples
//! stores the result and begins a new summation.
//! Similar to an integrate and dump function
/*! 
  \addtogroup fir FIR filters
*/
//! \brief sum and dump filter
//! \ingroup fir
class sum_and_dump {
public:
  long average;
  long count_exp;
  long count;
  long count_val;
  long result;

  sum_and_dump(long exp) : count_exp(exp) {
    count = result = average = 0;
    count_val = (1 << count_exp) - 1;
  }
  void input(long in);
  long output(void) {return(result);}
  void set_exp(long exp) { 
	  count_exp = exp; count = 0; 
	  count_val = (1 << count_exp) - 1;
  }
};
} // namespace SPUC 

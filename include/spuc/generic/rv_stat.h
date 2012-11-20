// 
// author="Tony Kirke" *
// Copyright(c) 2001 Tony Kirke
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
#ifndef STATC
#define STATC
#include <spuc.h>
namespace SPUC {
//! \brief a basic random Variable Statistics Class
template <class Numeric> class rv_stat
{
 protected:
  double count;
  Numeric av;
  Numeric sq;
  Numeric min_abs;
  Numeric max_abs;

 public:
  // Constructor
  rv_stat() {
	count = 0;
	av = 0;
	sq = 0;
	min_abs = MXLONG;
	max_abs = 0;
  }
  void update(Numeric x) {
	count += 1.0;
	av += x;
	sq += x*x;
	min_abs = MIN(ABS(min_abs),x);
	max_abs = MAX(ABS(max_abs),x);
  }  
  inline Numeric average() { if (count>0) return(av/count); else return(0);}
  inline Numeric mean_sq() { if (count>0) return(sq/count); else return(0); }
  inline Numeric rms() { return(sqrt(sq/count)); }
  inline Numeric var() { 
	if (count>0) return( sq/count - (av*av/(count*count))); 
	else	return(0); }
  inline Numeric std() { return(sqrt(var())); } 
  inline Numeric minimum() { return(min_abs); }
  inline Numeric maximum() { return(max_abs); }
};
}
#endif

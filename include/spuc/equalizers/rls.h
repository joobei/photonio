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
#ifndef RLSC
#define RLSC
#include <vector.h>
#include <matrix.h>
#include <fir.h>
namespace SPUC {
/*! 
  \addtogroup equalizers Equalization Classes
 */
//! \ingroup equalizers comm
//! \brief Recursive Least Squares Algorithm
class rls {
	private:
		int n, m;
		Matrix<double> P;
		Vector<double> k;
		Vector<double> w, u, ut, x;
		double ialpha;
		fir<double> cfir;
		
	public:
		/// Constructor
		rls(const int n_in, double a);
		/// Destructor
		~rls() {}
		/// Adapt filter
		void adapt(double x_in);
		/// Process input sample
		double update(double x_in);
	};
}
#endif

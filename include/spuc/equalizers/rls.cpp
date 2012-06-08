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
// rls.cpp		implementation of the kalman filter class
#include "rls.h"
using namespace SPUC;
rls::rls(const int n, double alpha)
	: P(n,n), k(n), w(n), u(n), ut(n) , x(n),
	cfir(n)
{
  ialpha = 1.0/alpha;
  P = 0.01;
  k = (double)0;
  w = (double)0;
  u = (double)0;
  ut= (double)0;
  x = (double)0;
}
double rls::update(double y) {
  
  double out = cfir.update(y);
  w = Vector_taps(cfir);
  u = Vector_input(cfir);
  return(out);
}
void rls::adapt(double y)
{
  //  k = ialpha*P*u/(1+ialpha*ut*P*u);
  //  w += k*conj(e);
  //  P = ialpha*(P-k*ut*P);
  Matrix<double> d;
  Vector<double> Pu = P*u;
  k = (ialpha/(1+ialpha*dot(ut,Pu)))*(Pu);
  double e = y-dot(w,u);
  w = w + e*k;
  d = k*(ut*P);
  P = ialpha*(P - d);
}
// namespace SPUC








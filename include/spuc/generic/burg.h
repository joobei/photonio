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
#include <math.h>
#include <vec.h>
namespace SPUC {
//! \brief AR model coefficients calculation using Burg algorithm 
//! \ingroup miscfunc
//!  \author Tony Kirke,  Copyright(c) 2001 
template <class T> Vector<T> burg( Vector<T>& x, int P) {
        const double EPS = 1e-30;
		Vector<T> a(P);
		int N = x.size();
		T* ef = new T[N];
		T* ef_prev = new T[N];
		T* eb = new T[N];
		T* eb_prev = new T[N];
		T*   aa = new T[P];
		T* rc = new T[N];
		
		T gamma;
		
		T num, den ;
		int t, p ;
		int i;
		
		for (i=0;i<N;i++) {	ef[i] = eb[i] = x[i];  }
		
		for (p=0;p<P;p++)  {
			num = 0.0;
			den = 0.0;
			
			for (t=p+1;t<N;t++) { 
				den += ef[t]*ef[t] + eb[t-1]*eb[t-1];
				num += ef[t] * eb[t-1]; 
			}
			
			if (magsq(den)>EPS) gamma=2.0*num/den;
			else gamma  = 0;
			
			a[p] = gamma;
			rc[p]=-a[p];
			
			for (i=0;i<N;i++) {
				ef_prev[i] = ef[i];
				eb_prev[i] = eb[i];
			}
			
			for (t=1;t<N;t++)  {
				ef[t] +=  -(conj(gamma) * eb_prev[t-1]);
				eb[t]  = eb_prev[t-1] - (gamma * ef_prev[t]);
			}
	  
			if (p>0) for(t=0;t<p;t++) a[t]= aa[t]  + rc[p]*aa[p-t-1];
			for (i=0;i<p+1;i++) aa[i] = a[i];
		}
	
		delete [] ef;
		delete [] eb;
		delete [] ef_prev;
		delete [] eb_prev;
		delete [] aa;
		delete [] rc;
		return(a);
	}
}


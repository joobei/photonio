// 
// author="Tony Kirke"
// Copyright(c) 2005 Tony Kirke
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
#include "elliptic.h"
#include <gsl/gsl_mode.h>
#include <gsl/gsl_sf_ellint.h>
#include <gsl/gsl_sf_elljac.h>
using namespace std;
namespace SPUC {
elliptic::elliptic(double fcd, long ord, double stopattn, double ripple) {
	int i;
	double stp;
	double m1,m2;
	double a,m,Kk1,Kpk1,k,wr,fstp,Kk,u;

	gain = 1;
	order = ord;
	n2 = (order+1)/2;
	odd = (order%2);
	epi = pow(10.0,(ripple/10.));
	epi = sqrt(epi-1.0);
	double wca = tan(PI*fcd);
	//! wca - pre-warped angular frequency
	roots = new complex<double>[n2];
	zeros = new complex<double>[n2];
	if (odd) iir_1 = new iir_1st< double >;

	//! if stopattn < 1 dB assume it is stopband edge instead
	if (stopattn > 1.0) {
	  a = pow(10.0,(stopattn/10.));
	  m1 = epi*epi/(a - 1.0);
	  m2 = 1 - m1;
	  Kk1 = ellpk(m1);
	  Kpk1 = ellpk(m2);
	  u =  Kpk1 / (order * Kk1);
	  k = msqrt(u);
	  wr = 1.0/k;
	  fstp = atan(wca*wr)/PI;
	} else {
	  fstp = stopattn;
	}
	wr = tan(fstp*PI)/wca;
	if( wr < 0.0 )	wr = -wr;
#ifdef DBG
	cout << "ord = " << ord << "\n";
	cout << "passband ripple = " << ripple << "\n";
	cout << "pass band = " << fcd << "\n";
	cout << "pass band wca= " << wca << "\n";
	cout << "stop band = " << fstp << " " << atan(wca*wr)/PI << "\n";
#endif
	k = 1.0/wr;
	m = k*k;
	Kk = ellpk(m);
	u = lamda_plane(k,m,ord,epi);
	wca = 1.0/tan(fcd*PI/2.0); 
	s_plane(ord,u,m,k,Kk,1.0/wca);
#ifdef DBG
	for(i=0; i<n2; i++ ) {
	  cout << "zero = " 
		   << zeros[i].real() << " "
		   << zeros[i].imag() << " "
		   << " pole = " 
		   << roots[i].real() << " "
		   << roots[i].imag() << " "
		   << "\n";
	}
#endif
	bilinear(n2,roots);
	bilinear(n2,zeros);
#ifdef DBG
	for(i=0; i<n2; i++ ) {
	  cout << "Z zeros = " 
		   << zeros[i].real() << " +/- "
		   << zeros[i].imag() << " "
		   << "Z poles = " 
		   << roots[i].real() << " +/- "
		   << roots[i].imag() << " "
		   << "\n";
	}
#endif
	iir = new iir_2nd< double >[order/2];
	get_coeff(n2); // gain not yet set...
  }
  //! Reset history
  void elliptic::reset() {
	for (int j=odd;j<n2;j++) { 
	  iir[j-odd].reset();
	  if (odd) iir_1->reset();
	}
  } 
  //! print coefficients
  void elliptic::print() {
	int j;
	for (j=odd;j<n2;j++) iir[j-odd].print();
	if (odd) iir_1->print();
  } 
  //! Clock in sample and get output.
  double elliptic::clock(double in) {
	double tmp = in;
	for (int i=odd;i<n2;i++) {	tmp = iir[i-odd].clock(tmp); }
	if (odd) tmp = iir_1->clock(tmp);
	return(gain*tmp);
  }
  //! Do bilinear transformation
  void elliptic::bilinear(long n2, complex<double>* r) {
	int j;
	const double a = 2.0;
	if (odd) {
	  //! For s-domain zero (or pole) at infinity we
	  //! get z-domain zero (or pole) at z = -1
	  double tmp = (r[0].real()-a)/(a+r[0].real());
	  iir_1->set_coeff(-tmp);
	  gain *= 0.5*(1+tmp);
	}
	for (j=odd;j<n2;j++) r[j] = (1.0+r[j])/(1.0-r[j]);
  }
  //! Get 2nd order IIR coefficients
  void elliptic::get_coeff(long n2) {
	for (int j=odd;j<n2;j++) { 
	  iir[j-odd].set_a(-2*roots[j].real(),magsq(roots[j]));
	  iir[j-odd].set_b(-2*zeros[j].real(),magsq(zeros[j]));
	  gain *= (magsq(roots[j]) - 2*roots[j].real() + 1.0)/(4*magsq(roots[j])); 
	}
  }
  //! get roots in Lamda plane
  double elliptic::lamda_plane(double k, double m, int n, double eps) {
	double m1;
	double Kk;
	double Kpk;
	double Kk1, Kpk1;
	double u;
	double phi;

	Kk  = ellpk(m);
	Kpk = ellpk(1-m);
	u = (  n * Kpk / Kk );
	m1 = msqrt(u); 
	m1 *= m1;
	Kk1 = ellpk(m1);
	phi = atan(1.0/eps);
	u = Kk*ellik(phi, 1.0-m1)/(n*Kk1);
	return u;
  }
  //! calculate s plane poles and zeros
  void elliptic::s_plane(int n, double u, double m, double k,
						 double Kk,	double wc) {
	double a,b;
	double sn1,cn1,dn1;
	double sn,cn,dn;
	double r;
	int i,j;
	double kon = Kk/(double)n;
	ellpj( u, 1.0-m, &sn1, &cn1, &dn1 );
	for( i=0; i<(n+1)/2; i++ ) {
	  b = (n-1-2*i)*kon;
	  ellpj( b, m, &sn, &cn, &dn );
	  a = wc/(k*sn);
	  if (i<=n/2)	zeros[i] = complex<double>(0.0,a);
	  r = k * sn * sn1;
	  r = 1.0/(cn1*cn1 + r*r);
	  roots[i] = complex<double>(-wc*cn*dn*sn1*cn1*r,wc*sn*dn1*r);
	}
  }
  double elliptic::ellik(double phi,double k) {
	return(gsl_sf_ellint_E(phi,k, GSL_PREC_DOUBLE));
  }
  double elliptic::ellpk(double k) {
	return(gsl_sf_ellint_Kcomp(k, GSL_PREC_DOUBLE));
  }
  int elliptic::ellpj( double u, double m, double* sn,
			 double* cn, double* dn) {
	return(gsl_sf_elljac_e(u,m,sn,cn,dn));
  }
  //! modulus from ratio of K/K'
  double elliptic::msqrt(double u) {
	double dt1, dt2;
	double a = 1.0;
	double b = 1.0;
	double r = 1.0;
	//! see http://www.physik-astro.uni-bonn.de/~dieckman/InfProd/InfProd.html
	//! calculate  theta2 / theta3
	//! m = (theta2/theta3) ^ 4
	double q = ::exp( -PI*u );
	double p = q;

	do {
	  r *= p;
	  a += 2.0 * r;
	  dt1 = r/a;
	  r *= p;
	  b += r;
	  p *= q;
	} while ( dt1 > 1.0e-7 );
	
	a = b/a;
	a = 4.0 * sqrt(q) * a * a;	
	return(a);
  }
} // namespace SPUC 

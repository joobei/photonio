// 
// Copyright(c) 1993-1996 Tony Kirke
// author="Tony Kirke" *
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
#include <iostream>
#include <fstream>
#include <complex.h>
#include <fir.h>
namespace SPUC {
//! FIR filter implementation
template <> int fir< complex<double> >::read_complex_taps(const char* file)
{
// Assumes coeficients are complex.
	int i=0;
	double tmp;
	double tmpi,tmpr;
	num_taps = 0;

	ifstream firf(file);
	if (!firf) {
	  cout << "Could not open file " << file << "\n";
	  return(-1);
	}
	while (!firf.eof()) {
			firf >>	tmp;
			firf >> tmp;
			num_taps++;
	}
	firf.close();

	coeff = new complex<double>[num_taps];
	z = new complex<double>[num_taps];

	firf.open(file);
	while (!firf.eof()) {
			firf >> tmpr;
			firf >> tmpi;
			coeff[i++] = complex<double>(tmpr,tmpi);
	}							
	firf.close();
   
	for (i=0;i<num_taps;i++) z[i] = 0;  
	return(0);
}
template <> int fir< complex<long> >::read_taps(const char* file)
{
// Assumes coeficients are real ONLY.
	int i=0;
	long tmp;
	num_taps = 0;

	ifstream firf(file);
	if (!firf) {
	  cout << "Could not open file " << file << "\n";
	  return(-1);
	}
	while (!firf.eof()) {
	  firf >>	tmp;
	  num_taps++;
	}
	firf.close();

	coeff = new complex<long>[num_taps];
	z = new complex<long>[num_taps];

	firf.open(file);
	while (!firf.eof()) {
	  firf >> tmp;
	  coeff[i++] = tmp;
	}							
	firf.close();
   
	for (i=0;i<num_taps;i++) z[i] = 0;  
	return(0);
}
template <> int fir< complex<double> >::read_taps(const char* file)
{
// Assumes coeficients are real ONLY.
	int i=0;
	double tmp;
	num_taps = 0;

	ifstream firf(file);
	if (!firf) {
	  cout << "Could not open file " << file << "\n";
	  return(-1);
	}
	while (!firf.eof()) {
	  firf >>	tmp;
	  num_taps++;
	}
	firf.close();

	coeff = new complex<double>[num_taps];
	z = new complex<double>[num_taps];

	ifstream firfx(file);
	while (!firfx.eof()) {
	  firfx >> tmp;
	  coeff[i++] = tmp;
	}							
	firfx.close();
	
	for (i=0;i<num_taps;i++) z[i] = 0;  
	return(0);
}
template <> int fir<long>::read_taps(const char* file)
{
	int i=0;
	long tmp;
	num_taps = 0;

	ifstream firf(file);
	if (!firf) {
	  cout << "Could not open file " << file << "\n";
	  return(-1);
	}
	while (!firf.eof()) {
			firf >>	tmp;
			num_taps++;
	}
	firf.close();

	coeff = new long[num_taps];
	z = new long[num_taps];

	firf.open(file);
	while (!firf.eof()) firf >> coeff[i++];
	firf.close();
   
	for (i=0;i<num_taps;i++) z[i] = 0;  
	return(0);
}
template <> int fir<double>::read_taps(const char* file)
{
	int i=0;
	double tmp;
	num_taps = 0;

	ifstream firf(file);
	if (! firf) {
	  cout << "Error opening file " << file << "\n";
	  return(-1);
	}

	while (!firf.eof()) {
	  firf >>	tmp;
	  num_taps++;
	}
	firf.close();
	
	coeff = new double[num_taps];
	z = new double[num_taps];

	ifstream firfx(file);
	if (! firfx) {
	  cout << "Error opening file " << file << "\n";
	}
	while (!firfx.eof()) {
	  firfx >> coeff[i++];
	}
	firfx.close();
   
	for (i=0;i<num_taps;i++) z[i] = 0;  
	return(0);
}   
template <> void fir<double>::print() {
	cout << "FIR filter coefficients" << '\n';
	for (long i=0;i<num_taps;i++) {
		cout << coeff[i] << cout.width(10) << ' ';
		if ((i+1)%6 == 0) cout << '\n';
	}
	cout << '\n';
	cout.flush();
}
template <> void fir<complex<double> >::print() {
	long i;
	cout << "Real FIR filter coefficients" << '\n';
	for (i=0;i<num_taps;i++) {
		cout << re(coeff[i]) << cout.width(10) << ' ';
		if ((i+1)%6 == 0) cout << '\n';
	}
	cout << '\n';
	cout << "Imaginary FIR filter coefficients" << '\n';
	for (i=0;i<num_taps;i++) {
		cout << im(coeff[i]) << cout.width(10) << ' ';
		if ((i+1)%6 == 0) cout << '\n';
	}	
	cout << '\n';
	cout.flush();
}    
template <> void fir<complex<long> >::print() {
	long i;
	cout << "Real FIR filter coefficients" << '\n';
	for (i=0;i<num_taps;i++) {
		cout << (long)re(coeff[i]) << cout.width(10) << ' ';
		if ((i+1)%6 == 0) cout << '\n';
	}
	cout << '\n';
	cout << "Imaginary FIR filter coefficients" << '\n';
	for (i=0;i<num_taps;i++) {
		cout << (long)im(coeff[i]) << cout.width(10) << ' ';
		if ((i+1)%6 == 0) cout << '\n';
	}	
	cout << '\n';
	cout.flush();
}
} // namespace SPUC 

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
// See cic.h for description
#include <cic.h>
namespace SPUC {
cic::cic(char n) : stages(n)
{
	int i;
	nacc = new signed long[n];  
	diff = new signed long[n]; 
	prev = new signed long[n]; 
	for (i=0;i<stages;i++) nacc[i] = diff[i] = prev[i] = 0;
}
void cic::num_stages(char n)
{
	int i;
	stages = n; 
	delete nacc;
	delete diff;
	delete prev; 
	nacc = new signed long[n];  
	diff = new signed long[n]; 
	prev = new signed long[n]; 
	for (i=0;i<stages;i++) nacc[i] = diff[i] = prev[i] = 0;
}
signed long cic::interpolate(signed long in, signed char dump)
{
	char i;
	if (dump) {
		diff[0] = in - prev[0];
		prev[0] = in;
		for (i=1;i<stages;i++) {
			diff[i] = diff[i-1] - prev[i];
			prev[i] = diff[i-1];
		}
		nacc[0] += diff[stages-1];
	}  
	for (i=0;i<(stages-1);i++) nacc[i+1] += nacc[i];
	return(nacc[stages-1]);
} 
signed long cic::decimate(signed long in, signed char dump)
{
	char i;
	nacc[0] += in;
	for (i=0;i<(stages-1);i++) nacc[i+1] += nacc[i];
	if (dump) {
		diff[0] = nacc[stages-1] - prev[0];
		prev[0] = nacc[stages-1];
		for (i=1;i<stages;i++) {
			diff[i] = diff[i-1] - prev[i];
			prev[i] = diff[i-1];
		}
	}
	return(diff[stages-1]);
}


} // namespace SPUC 

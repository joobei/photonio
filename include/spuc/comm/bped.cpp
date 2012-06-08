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
#include <bped.h>
// Constructor
using namespace SPUC;
bped::bped(char len) : nbpe(len) { 
  oqtstate = 0; 
  quad_prev = 0;
  bit = new complex<double>[len];
  for (char i=0;i<nbpe;i++) bit[i] = complex<double>(0,0);
}
// Sample input
void bped::clock(const complex<double>& in)
{
	double out_angle, out_angle4;
	int quad_now;
	char i;

	for (i=nbpe;i>0;i--) bit[i] = bit[i-1];
	bit[0] = in;

 	complex<double> vx(0,0);
            
	for(i=0; i<nbpe; i++) vx += polar(1/(double)nbpe,fq_angle(bit[i])); 
	out_angle4 = arg(vx);      
	if(out_angle4 <= 0.) out_angle4 += TWOPI;  
                                                     
	// take out bias due to inacurracy in RTD since out_angle4 is always < 2*PI 
	if(out_angle4 >=TWOPI) out_angle4 = TWOPI-0.000001;  

	// output quadrant translator
	quad_now=(int)(out_angle4*RTD/90);
	quad_now = quad_now%4;  
          
	// keep track of quadrant cross-overs 
	if((quad_prev==3)&&(quad_now==0)) {
	  oqtstate += 1;
	  oqtstate = oqtstate%4;  
	}
	else if((quad_prev==0)&&(quad_now==3)) {
	  oqtstate += 3;        
	  oqtstate = oqtstate%4;
	}     
	// end output quadrant translator

	out_angle = (out_angle4/4.0 - PI/4.0);
	
	/*  compensate for quadrant cross-overs */
	out_angle += oqtstate*PI/2;                        
	if(out_angle <= 0.) out_angle += TWOPI;
	/*  keep track of the phase introduced */
	ang = out_angle;
 	/*  store the current quadrant number */  
  	quad_prev = quad_now;
}
//
// Put vector into first quadrant so that it can be quadrupled 
// return angle value in first quadrant                                        
//
double bped::fq_angle(const complex<double>& pid)
{
	complex<double> pid1;
	double xangle;

	if ((pid.re>=0.)&(pid.im>=0.)) pid1 = pid;
	else if((pid.re>=0.)&(pid.im<0.)) pid1 = pid*complex<double>(0,1);
	else if((pid.re<0.)&(pid.im>=0.)) pid1 = pid*complex<double>(0,-1);
	else pid1 = -pid;
                       
	xangle = 4.0*arg(pid1);
	while (fabs(xangle) > TWOPI) {
		if (xangle>0) xangle -= TWOPI;
    	else xangle += TWOPI;
	}
	if (xangle<0.) xangle += 2*PI;
	return(xangle);
}                              
//
//  Phase rotation
//  Rotate phase angle of I and Q data relative to the bpe phase estimate
//  Get output for Complex double input 
complex<double> bped::output(const complex<double>& in)
{       
		clock(in);              
		return(in*refvect());
}
// Get output for Complex long input 
complex<double> bped::output(const complex<long>& in)
{                                  
		complex<double> temp = complex<double>((double)in.re,(double)in.im);
		clock(temp);              
		return(temp*refvect());
}

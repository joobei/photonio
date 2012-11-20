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
namespace SPUC {
/*! 
  \addtogroup miscfunc  Miscellaneous DSP classes
*/
/*!   \brief Simple 1st order All-digital Sigma Delta converter
  \ingroup miscfunc
*///!:
//!    All-digital sigma delta converter
//!    Performs Sigma Delta function in digital domain
//!    for use when output will go to a (nbit_out) D/A converter.
//!    Default is 8 bits in, 1 bit out.
//!    After construction, call single() to get output.
//!    Multirate performed by calling single() with same input
//!    at the higher sampling rate.
class sigma_delta
{
	protected:
		signed long in;
		signed long acc;
		unsigned long mask;
	public:                         
		unsigned char nbit_out; 
		unsigned char nbit_in;
		
	//! Constructor
	sigma_delta(char nin=8, char nout=1);
	//! Output <= 8 bits
	signed char single(signed long in);
} ;
} // namespace SPUC 

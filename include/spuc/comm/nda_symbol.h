namespace SPUC {
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
//!	  \ingroup comm
//! \brief   Non-decision aided timing discriminator (Gardiner's algorithm)
//!  \author Tony Kirke,  Copyright(c) 2001 
//!  
//
//!   Runs at twice the symbol rate
//!   Curr - current sample
//!   Prev - Previous sample
//! \image html nda_timing.gif
//! \image latex nda_timing.eps
template <class T> T nda_symbol(complex<T> prev, complex<T> curr) 
{
	return(prev.re*(prev.re-curr.re)+prev.im*(prev.im-curr.im));
}
} // namespace SPUC 

namespace SPUC {
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
//!	  \ingroup comm
//!
//! \brief Cross-Product frequency discriminator
//! \author Tony Kirke
//! \
//
//! Prev - previous symbol
//! Curr - current symbol
template <class T> T cp_afc(complex<T> prev, complex<T> curr) 
{ 
	complex<T> rot(prev.re+prev.im,prev.im-prev.re);
	T dot = curr.re*rot.re + curr.im*rot.im;
	T cross = curr.im*rot.re - curr.re*rot.im;
	T afc = ((dot>=0) ? cross : -cross) - ((cross>=0) ? dot : -dot);
	return(afc);
}
} // namespace SPUC 

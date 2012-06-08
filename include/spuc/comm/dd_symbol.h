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
//!  \brief Decision directed timing discriminator (QPSK)
//!  \author Tony Kirke,  Copyright(c) 2001 
//!  
//
//!   Runs at the symbol rate
//!   Curr - current symbol
//!   Hard_data - Hard decision for current symbol
//!   Prev - Previous symbol
//!   Prev_Hard_data - Hard decision for previous symbol
template <class T> T dd_symbol(complex<T> prev, complex<T> curr,
							   complex<T> prev_hard_data, complex<T> hard_data) 
{
	return(re(curr*conj(prev_hard_data) - prev*conj(hard_data)));
}
} // namespace SPUC 

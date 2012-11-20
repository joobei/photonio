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
//!	\ingroup comm
//! \brief   QPSK Reduced complexity frequency discriminator
//! \author Tony Kirke,  Copyright(c) 2001 
//! 
//
//!  using the
//!   "frequency matched filter" approach derived from the 
//!   Maximum likelihood principle. see<p>
//!   A Reduced-complexity frequency discriminator derived from
//!   the maximum likelihood principle<p>  IEEE Transactions on
//!   Communications Oct. 1995<p>.
//!   FMF - output from frequency matched filter (must be
//!   done externally)
//!   Curr - current sample
template <class T> T qpsk_rcfd(complex<T> fmf, complex<T> curr) 
{
	return(curr.im*fmf.re - fmf.im*curr.re);
}
} // namespace SPUC 

// SPUC - Signal processing using C++ - A DSP library
/* 
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
#ifndef MODTYPE
#define MODTYPE
namespace SPUC {
//! \ingroup comm
//! \brief Slicer to data decisions
//!  \author Tony Kirke,  Copyright(c) 2001 
//!  
class slicer_type {
 public:
  virtual cmplx<double> slicer(cmplx<double> in);
};
//! \ingroup comm
//! \brief BPSK slicer
class bpsk_slicer : public slicer_type {
 public:
  cmplx<double> slicer(cmplx<double> in) {return(SGN(in.real())); }
};
//! \ingroup comm
//! \brief QPSK slicer
class qpsk_slicer : public slicer_type {
 public:
  cmplx<double> slicer(cmplx<double> in) {return(signbit(in)); }
};
}
#endif


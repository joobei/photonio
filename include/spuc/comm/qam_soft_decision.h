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
#ifndef QAMSD
#define QAMSD
#include <math.h>
#include <complex.h>
namespace SPUC {
/*! \brief  Soft-decision QAM Demapper (used in 802.11A)
  Returns an array of soft decision bits, array size is dependant on modulation type
  \ingroup comm
*/
void qam_data_demap(long rate_index, complex<long> data_in,
					long soft_decision_level, long* viterbi_input);
/*! \brief BPSK data demapping
  \ingroup comm
*/
void bpsk_soft_decision(complex<long> data_in, long* viterbi_input);
/*! \brief QPSK data demapping
  \ingroup comm
*/
void qpsk_soft_decision(complex<long> data_in, long* viterbi_input);
/*! \brief 16-QAM data demapping
  \ingroup comm
*/
void qam16_soft_decision(complex<long> data_in, long soft_decision_level, long* viterbi_input);
/*! \brief 64-QAM data demapping
  \ingroup comm
*/
void qam64_soft_decision(complex<long> data_in, long soft_decision_level, long* viterbi_input);
}
#endif

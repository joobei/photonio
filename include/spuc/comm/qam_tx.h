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
// $Id: qam_tx.h,v 1.2 2005/09/16 17:00:44 spuc Exp $
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <max_pn.h>   	
#include <fading_channel.h>
#include <fir_interp.h>
#include "qam_mod.h"
namespace SPUC {
/*! \brief  Class for QAM transmitter using a root raised cosine transmit filter
  \author Tony Kirke,  Copyright(c) 2001 
  
  \ingroup comm
  \ingroup modulators
*/
//
class qam_tx
{
	public:
  
	qam_mod ENC;
	max_pn preamble_source;
	max_pn training_source;
	fir_interp< complex<double> > tx_filter;
	complex<double> tx_data; 
	double data_level;
	const long preamble_pn; // Number of symbols used for pre-amble PN
	const long training_interval;
	long tx_symbols;       // Counter for transmitted symbols
	const long over; // Oversampling rate
	double training_scale;
	double alpha;
	long count;
	long rate;
	
	qam_tx(long sym_span=5, long over=8, long rate=0, long c=0, double tx_filter_bw=0.25);
	void loop_init(long x, long y);
	complex<double> clock();
};	                 
}


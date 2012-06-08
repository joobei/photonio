//! author="Tony Kirke" *
//! Copyright(c) 2001 Tony Kirke
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
#ifndef DEBUGH
#define DEBUGH
#ifdef DEBUG_STREAMS
#include <debugfc.h>
#define DEBUGS(n,f) STREAMS.add_stream_number(n,f)
#define DOUT(n,x)  if (STREAMS.is_on(n)) STREAMS.out(n,x) 
#define DOUTN(n,x) if (STREAMS.is_on(n)) STREAMS.outn(n,x) 
#define DOUTF(n) if (STREAMS.is_on(n)) STREAMS.newline(n)
#define DCLOSE(n) STREAMS.close_stream(n)
#define DCLOSEALL STREAMS.close_all_streams()
#else
#define DEBUGS(n,f) 
#define DOUT(n,f) 
#define DOUTN(n,f) 
#define DOUTF(n) 
#define DCLOSE(n) 
#define DCLOSEALL
#endif
#endif

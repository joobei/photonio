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
#include <scic.h>
namespace SPUC {
scic::scic(char n, long r) : max_rate(r), stages(3*n), main(3*n), sub(2*n)
{
  dly.set_size(max_rate);
}
void scic::num_stages(char n,long r)
{
  stages = 3*n; 
  max_rate = r;
  main.num_stages(3*n);
  sub.num_stages(2*n);
  dly.set_size(max_rate);
}
signed long scic::decimate(signed long in, long rate, signed char dump)
{
  signed long result;
  
  dly.input(in);
  result = 2*main.decimate(in,dump) - 3*sub.decimate(dly.check(rate-2),dump);
  return(result);
}


} // namespace SPUC 

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
//! \brief Template class for Levinson-Durbin algorithm
//! \author Tony Kirke,  Copyright(c) 2001 
//
//! \ingroup miscfunc
//: <font color="red"><i>Under construction!</i></font>
template <class T> T levdur(T* R)
{
  T a(R);
  T Pe;
  T q, at;
  int N = R.size();
  int j,k,l;

  a[0]=1.0;
  a[1]=-R[1]/R[0];
  Pe=R[0]*(1-a[1]*a[1]);
  for (j=2;j<=N;j++){
    for (q=R[j],l=1;l<=j-1;l++) q += a[l]*R[j-1];
    q=-q/Pe;
    for (k=1;k<=j/2;k++){
      at=a[k]+q*a[j-k];
      a[j-k]+=q*a[k];
      a[k]=at;
    }
    a[j]=q;
    Pe*=(1-q*q);
  }
  return(a);
}
}


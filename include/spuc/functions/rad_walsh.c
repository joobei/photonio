// 
/*
//! Copyright(c) 1996 Tony Kirke
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
 *----------------------------------------------------------------------------
 * Generate RADEMACHER-WALSH functions

 * CREATE (M X M) ARRAY
 * Each row contains an orthogonal Rademacher-walsh function
 * (Array indexes valid from 1 to 2^m + 1)
 *----------------------------------------------------------------------------*/
void rad_walsh(a,m)
int a[MAXSIZE][MAXSIZE];
int m;
{
	int row,col;
	int n=1;
	int i;

	//  Outer loop generates a larger array size each time */

	for (i=0;i<m;i++) {

	if (n==1) { a[1][1]=1;}
	
	for (col=1;col<=n;col++) {

		for (row=1;row<=n;row++) {
			a[row][col+n] = a[row][col];
			a[row+n][col] = a[row][col];
			a[row+n][col+n] = -a[row][col];
		}
	}
	n *= 2;
	}
}

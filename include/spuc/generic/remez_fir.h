// author = "Jake Janovetz, Iain A Robin, and Tony Kirke"
namespace SPUC {
#ifndef RMZFIR
#define RMZFIR
#define  BANDPASS 1
#define DIFFERENTIATOR 2
#define HILBERT 3
#define NEGATIVE 0
#define POSITIVE 1
#define GRIDDENSITY 16
#define MAXITERATIONS 40
/*-------------------------------------------------
 *  Copyright (C) 1995  Jake Janovetz (janovetz@coewl.cen.uiuc.edu)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 ***********************************************************************/
//! Remez_fir class
//! <p>Original code from Jake Janovetz
//! <p>Converted to Java by Iain A Robin, June 1998
//! <p>Converted to C++ by Tony Kirke July 2001
//! <p>Calculates the optimal (in the Chebyshev/minimax sense)
//! FIR filter impulse response given a set of band edges,
//! the desired reponse on those bands, and the weight given to
//! the error in those bands.
//!
//! <p><b>Inputs:</b>
//! <p>int      numtaps - Number of filter coefficients</p>
//! <p>int      numband - Number of bands in filter specification
//! <p>double[] bands   - User-specified band edges [2 * numband]
//! <p>double[] des     - User-specified band responses [numband]
//! <p>double[] weight  - User-specified error weights [numband]
//! <p>int      type    - Type of filter
//!
//! <p><b>Output:</b>
//! <p>double[] h       - Impulse response of final filter [numtaps]
/*! 
  \addtogroup fir FIR filters
*/
//! \brief template remez FIR class
//! \ingroup fir
class remez_fir {

 public:
	remez_fir() {}  

 private:
	static void createDenseGrid(int r, int numtaps, int numband, double bands[],
                       double des[], double weight[], int gridSize,
                       double grid[], double d[], double w[],
                       int symmetry);
	static void initialGuess(int r, int ext[], int gridSize);
	static void calcParms(int r, int ext[], double grid[], double d[], double w[],
				 double ad[], double x[], double y[]);
	static double computeA(double freq, int r, double ad[], double x[], double y[]);
	static void calcError(int r, double ad[], double x[], double y[],
				 int gridSize, double grid[],
				 double d[], double w[], double e[]);
	static void search(int r, int ext[], int gridSize, double e[]);
	static double* freqSample(double A[],int numtaps, int symm);
	static bool isDone(int r, int ext[], double e[]);
 public:
	static double* remez(int n, int numband,
						 double bands[], double des[], double weight[], int type);
};
#endif
} // namespace SPUC 

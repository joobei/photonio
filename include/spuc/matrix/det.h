/*---------------------------------------------------------------------------*
 *                                   IT++			             *
 *---------------------------------------------------------------------------*
 * Copyright (c) 1995-2002 by Tony Ottosson, Thomas Eriksson, Pål Frenger,   *
 * Tobias Ringström, and Jonas Samuelsson.                                   *
 *                                                                           *
 * Permission to use, copy, modify, and distribute this software and its     *
 * documentation under the terms of the GNU General Public License is hereby *
 * granted. No representations are made about the suitability of this        *
 * software for any purpose. It is provided "as is" without expressed or     *
 * implied warranty. See the GNU General Public License for more details.    *
 *---------------------------------------------------------------------------*/
//! 
//! Original code modified by Tony Kirke Feb 1, 2003
//! author="Tony Kirke" *
//  SPUC - Signal processing using C++ - A DSP library

/*! 
  \file 
  \brief Determinant calculation of square matrices
  \author Tony Ottosson

  1.4

  2002/12/19 23:56:44
*/


#ifndef __det_h
#define __det_h

#include "matrix.h"
namespace SPUC {

/*!
  \defgroup determinant Determinant
*/

/*! 
  \brief Determinant of real square matrix.
  \ingroup determinant

  Calculate determinant of the real matrix \f$\mathbf{X}\f$

  Uses LU-factorisation.
  \f[
     \det(\mathbf{X}) = \det(\mathbf{P}^T \mathbf{L}) \det(\mathbf{U}) = \det(\mathbf{P}^T) \prod(\mathrm{diag}(\mathbf{U}))
  \f]
  and the determinant of the permuation matrix is \f$ \pm 1\f$ dependening on the number of row permuations
*/
double det(const mat &X);


#ifdef HAVE_LAPACK

/*! 
  \brief Determinant of complex square matrix.
  \ingroup determinant

  Calculate determinant of the complex matrix \f$\mathbf{X}\f$

  Uses LU-factorisation.
  \f[
     \det(\mathbf{X}) = \det(\mathbf{P}^T \mathbf{L}) \det(\mathbf{U}) = \det(\mathbf{P}^T) \prod(\mathrm{diag}(\mathbf{U}))
  \f]
  and the determinant of the permuation matrix is \f$ \pm 1\f$ dependening on the number of row permuations
*/
double_complex det(const cmat &X);

#endif // HAVE_LAPACK
}
#endif // __det_h

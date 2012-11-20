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
  \brief Definitions of Singular Value Decompositions 
  \author Tony Ottosson and Tobias Ringström

  1.13

  2002/03/07 10:28:12
*/

#ifndef __svd_h
#define __svd_h

#include <vector.h>
#include <matrix.h>

namespace SPUC {

/*! 
  \ingroup matrixdecomp
  \brief Singular Value Decomposition (SVD)  

  The svd-algorithm computes the decomposition of a real \f$m \times n\f$ matrix \f$\mathbf{A}\f$
  so that
  \f[
  \mathbf{U}^T \mathbf{A} \mathbf{V} = \mathrm{diag}(\mathbf{s}) = \mathrm{diag}(\sigma_1, \ldots, \sigma_p) 
  \f]
  where the elements of \f$\mathbf{s}\f$, \f$\sigma_1 \geq \sigma_2 \geq \ldots \sigma_p \geq 0\f$
  are the singular values of \f$\mathbf{A}\f$. Or put differently
  \f[
  \mathbf{A} = \mathbf{U} \mathbf{S} \mathbf{V}^H
  \f]
*/
bool svd(const mat &A, vec &S);

/*! 
  \ingroup matrixdecomp
  \brief Singular Value Decomposition (SVD)  

  The svd-algorithm computes the decomposition of a real \f$m \times n\f$ matrix \f$\mathbf{A}\f$
  so that
  \f[
  \mathbf{U}^T \mathbf{A} \mathbf{V} = \mathrm{diag}(\mathbf{s}) = \mathrm{diag}(\sigma_1, \ldots, \sigma_p) 
  \f]
  where the elements of \f$\mathbf{s}\f$, \f$\sigma_1 \geq \sigma_2 \geq \ldots \sigma_p \geq 0\f$
  are the singular values of \f$\mathbf{A}\f$. Or put differently
  \f[
  \mathbf{A} = \mathbf{U} \mathbf{S} \mathbf{V}^H
  \f]
*/
bool svd(const cmat &A, vec &S);

/*! 
  \ingroup matrixdecomp
  \brief Singular Value Decomposition (SVD)

  The svd-algorithm computes the decomposition of a real \f$m \times n\f$ matrix \f$\mathbf{A}\f$
  so that
  \f[
  \mathbf{U}^T \mathbf{A} \mathbf{V} = \mathrm{diag}(\mathbf{s}) = \mathrm{diag}(\sigma_1, \ldots, \sigma_p) 
  \f]
  where the elements of \f$\mathbf{s}\f$, \f$\sigma_1 \geq \sigma_2 \geq \ldots \sigma_p \geq 0\f$
  are the singular values of \f$\mathbf{A}\f$. Or put differently
  \f[
  \mathbf{A} = \mathbf{U} \mathbf{S} \mathbf{V}^H
  \f]
*/
vec svd(const mat &A);

/*! 
  \ingroup matrixdecomp
  \brief Singular Value Decomposition (SVD)

  The svd-algorithm computes the decomposition of a real \f$m \times n\f$ matrix \f$\mathbf{A}\f$
  so that
  \f[
  \mathbf{U}^T \mathbf{A} \mathbf{V} = \mathrm{diag}(\mathbf{s}) = \mathrm{diag}(\sigma_1, \ldots, \sigma_p) 
  \f]
  where the elements of \f$\mathbf{s}\f$, \f$\sigma_1 \geq \sigma_2 \geq \ldots \sigma_p \geq 0\f$
  are the singular values of \f$\mathbf{A}\f$. Or put differently
  \f[
  \mathbf{A} = \mathbf{U} \mathbf{S} \mathbf{V}^H
  \f]
*/
vec svd(const cmat &A);

/*! 
  \ingroup matrixdecomp
  \brief Singular Value Decomposition (SVD)

  The svd-algorithm computes the decomposition of a real \f$m \times n\f$ matrix \f$\mathbf{A}\f$
  so that
  \f[
  \mathbf{U}^T \mathbf{A} \mathbf{V} = \mathrm{diag}(\mathbf{s}) = \mathrm{diag}(\sigma_1, \ldots, \sigma_p) 
  \f]
  where the elements of \f$\mathbf{s}\f$, \f$\sigma_1 \geq \sigma_2 \geq \ldots \sigma_p \geq 0\f$
  are the singular values of \f$\mathbf{A}\f$. Or put differently
  \f[
  \mathbf{A} = \mathbf{U} \mathbf{S} \mathbf{V}^H
  \f]
*/
bool svd(const mat &A, mat &U, vec &S, mat &V);

/*! 
  \ingroup matrixdecomp
  \brief Singular Value Decomposition (SVD)

  The svd-algorithm computes the decomposition of a real \f$m \times n\f$ matrix \f$\mathbf{A}\f$
  so that
  \f[
  \mathbf{U}^T \mathbf{A} \mathbf{V} = \mathrm{diag}(\mathbf{s}) = \mathrm{diag}(\sigma_1, \ldots, \sigma_p) 
  \f]
  where the elements of \f$\mathbf{s}\f$, \f$\sigma_1 \geq \sigma_2 \geq \ldots \sigma_p \geq 0\f$
  are the singular values of \f$\mathbf{A}\f$. Or put differently
  \f[
  \mathbf{A} = \mathbf{U} \mathbf{S} \mathbf{V}^H
  \f]
*/
bool svd(const cmat &A, cmat &U, vec &S, cmat &V);
}
#endif // __svd_h

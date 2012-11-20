// From IT++ although no copyright info in original file
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
#ifndef _MATLAB_ENGINE
#define _MATLAB_ENGINE

#include "scalfunc.h"
#include "matrix.h"
#include "matfunc.h"
#include "specmat.h"

#include "converters.h"
#include "fastmath.h"

#include <mat.h>
#include <engine.h>
namespace SPUC {
//! \brief Matlab engine interface
/* Example code: */

/* vec x, y; */
/* x=linspace(0, 2*3.14, 200); */
/* y=sin(4*x); */

/* Matlab_Engine e; */
/* e.put(x, "x"); */
/* e.put(y, "y"); */
/* e.command("plot(x, y);"); */
/* e.command("xlabel('x');"); */
/* e.command("ylabel('sin(4x)');"); */

/* It++ does not differ between row or column vectors. When a length N vector is 'put' to  */
/* the Matlab workspace it is stored as a N x 1 column vector. If you are concerned about the  */
/* dimension of the variables you 'get' from the Matlab workspace to your it++ program, you should use  */
/* the matrix versions of the 'get' command. The complex versions of the 'get' command work */
/* also on a real valued variable. */
class Matlab_Engine{
 public:
  
  /* Give the command to start Matlab on your system or try the default one. */
  Matlab_Engine(const char *start_command = "matlab -nojvm");
  ~Matlab_Engine();

  /* Transfer the it++ vector 'v' to the Matlab workspace and give it the name 'name'. */
  void put(const vec &v, const char *name);
  void put(const ivec &v, const char *name);
  void put(const bvec &v, const char *name);
  void put(const cvec &v, const char *name);

  /* Transfer the it++ matrix 'm' to the Matlab workspace and give it the name 'name'. */
  void put(const mat &m, const char *name);
  void put(const imat &m, const char *name);
  void put(const bmat &m, const char *name);
  void put(const cmat &m, const char *name);

  /* Transfer the Matlab vector with name 'name' to the it++ program. */
  void get(vec &v, const char *name);
  void get(cvec &m, const char *name);

  /* Transfer Matlab matrix with name 'name' to the it++ program. */
  void get(mat &m, const char *name);
  void get(cmat &m, const char *name);

  /* Same as above but returns via the stack instead. */
  vec get_vec(const char *name);
  mat get_mat(const char *name);
  cvec get_cvec(const char *name);
  cmat get_cmat(const char *name);

  /* Execute the Matlab command given in 'c'. */
  void command(const char *c);

 private:
  Engine *e;
};
}
#endif // _MATLAB_ENGINE

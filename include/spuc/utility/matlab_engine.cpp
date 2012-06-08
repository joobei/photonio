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
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;
#include "matlab_engine.h"
using namespace SPUC;
Matlab_Engine::Matlab_Engine(const char *start_command){
  if((e = engOpen(start_command)) == NULL)   
	cout << "Unable to open the Matlab engine!\n";
}

Matlab_Engine::~Matlab_Engine(){
  if(engClose(e))  
	cout << "Unable to close the Matlab engine!\n";
}

void Matlab_Engine::put(const vec &v, const char *name){
  const int N = v.length();
  mxArray *T;
  if((T = mxCreateDoubleMatrix(N, 1, mxREAL)) == NULL)
    cout << "Unable to allocate Matlab vector. Out of memory?\n";
  mxSetName(T, name);
  double *pt = (double*) mxGetPr(T);
  for(int k=0; k<N; *pt++ = v(k++));
  if(engPutArray(e, T))
    cout << "Unable to put it++ vector to Matlab workspace!\n";
}

void Matlab_Engine::put(const cvec &v, const char *name){
  const int N = v.length();
  mxArray *T;
  if((T = mxCreateDoubleMatrix(N, 1, mxCOMPLEX)) == NULL)
    cout << "Unable to allocate Matlab vector. Out of memory?\n";
  mxSetName(T, name);
  double *pt_r = (double*) mxGetPr(T);
  double *pt_i = (double*) mxGetPi(T);
  for(int k=0; k<N; k++){
    *pt_r++ = real(v(k));
    *pt_i++ = imag(v(k));
  }
  if(engPutArray(e, T))
    cout << "Unable to put it++ vector to Matlab workspace!\n";
}

void Matlab_Engine::put(const mat &m, const char *name){
  const int rows = m.rows();
  const int cols = m.cols();
  mxArray *T;
  if((T = mxCreateDoubleMatrix(rows, cols, mxREAL)) == NULL)
    cout << "Unable to allocate Matlab matrix. Out of memory?\n";
  mxSetName(T, name);
  double *pt = (double*) mxGetPr(T);
  for(int row=0; row<rows; row++)
    for(int col=0; col<cols; *pt++ = m(row, col++));
  if(engPutArray(e, T))
    cout << "Unable to put it++ matrix to Matlab workspace!\n";
}

void Matlab_Engine::put(const cmat &m, const char *name){
  const int rows = m.rows();
  const int cols = m.cols();
  mxArray *T;
  if((T = mxCreateDoubleMatrix(rows, cols, mxCOMPLEX)) == NULL)
    cout << "Unable to allocate Matlab matrix. Out of memory?\n";
  mxSetName(T, name);
  double *pt_r = (double*) mxGetPr(T);
  double *pt_i = (double*) mxGetPi(T);
  for(int row=0; row<rows; row++){ 
    for(int col=0; col<cols; col++){
      *pt_r++ = real(m(row, col));
      *pt_i++ = imag(m(row, col));
    }
  }
  if(engPutArray(e, T))
    cout << "Unable to put it++ matrix to Matlab workspace!\n";
}

void Matlab_Engine::get(vec &v, const char *name){
  mxArray *T;
  if((T = engGetArray(e, name)) == NULL)
    cout << "No variable with the given name exists in the Matlab workspace!\n";  
  else{
    const int *dims = mxGetDimensions(T);
    const int ndims = mxGetNumberOfDimensions(T);
    if((ndims>2) || !((dims[0]==1)||(dims[1]==1)))
      cout << "The requested variable is not a vector!\n";    
    if(mxIsComplex(T))
      cout << "The requested variable is complex-valued!\n";    
    double *pt = (double*) mxGetPr(T);
    const int N = mxGetNumberOfElements(T);
    v.set_size(N, false);
    for(int k=0; k<N; v(k++) = *pt++);
  }
}

void Matlab_Engine::get(cvec &v, const char *name){
  mxArray *T;
  if((T = engGetArray(e, name)) == NULL)
    cout << "No variable with the given name exists in the Matlab workspace!\n";  
  else{
    const int *dims = mxGetDimensions(T);
    const int ndims = mxGetNumberOfDimensions(T);
    if((ndims>2) || !((dims[0]==1)||(dims[1]==1)))
      cout << "The requested variable is not a vector!\n";    
    if(!mxIsComplex(T))
      cout << "The requested variable is real-valued!\n";    
    double *pt_r = (double*) mxGetPr(T);
    double *pt_i = (double*) mxGetPi(T);
    const int N = mxGetNumberOfElements(T);
    v.set_size(N, false);
    if(mxIsComplex(T)) // Copy both real and imaginary part.
      for(int k=0; k<N; k++){
	complex<double> value(*pt_r++, *pt_i++);
	v(k) = value;
      }
    else // Copy only the real part.
      for(int k=0; k<N; k++){
	complex<double> value(*pt_r++, 0);
	v(k) = value;
      }         
  }
}

void Matlab_Engine::get(mat &m, const char *name){
  mxArray *T;
  if((T = engGetArray(e, name)) == NULL)
    cout << "No variable with the given name exists in the Matlab workspace!\n";
  else{
    if(mxGetNumberOfDimensions(T)==2){
      double *pt = (double*) mxGetPr(T);
      if(mxIsComplex(T))
	cout << "The requested variable is complex-valued!\n";
      const int *dims = mxGetDimensions(T);
      m.set_size(dims[0], dims[1], false);
      for(int row=0; row<dims[0]; row++)
	for(int col=0; col<dims[1]; m(row, col++)=*pt++);      
    }
    else
      cout << "The requested variable is not a matrix!\n";
  }
}

void Matlab_Engine::get(cmat &m, const char *name){
  mxArray *T;
  if((T = engGetArray(e, name)) == NULL)
    cout << "No variable with the given name exists in the Matlab workspace!\n";
  else{
    if(mxGetNumberOfDimensions(T)==2){
      double *pt_r = (double*) mxGetPr(T);
      double *pt_i = (double*) mxGetPi(T);   
      const int *dims = mxGetDimensions(T);      
      m.set_size(dims[0], dims[1], false);
      if(mxIsComplex(T)) // Copy both real and imaginary part.
	for(int row=0; row<dims[0]; row++){ 
	  for(int col=0; col<dims[1]; col++){
	    complex<double> value(*pt_r++, *pt_i++);
	    m(row, col) = value;
	  }
	}          
      else // Copy only the real part.
	for(int row=0; row<dims[0]; row++){ 
	  for(int col=0; col<dims[1]; col++){
	    complex<double> value(*pt_r++, 0);
	    m(row, col) = value;
	  }
	}      	
    }
    else
      cout << "The requested variable is not a matrix!\n";
  }
}

void Matlab_Engine::put(const ivec &v, const char *name){
  put(to_vec(v), name);
}

void Matlab_Engine::put(const bvec &v, const char *name){
  put(to_vec(v), name);
}

void Matlab_Engine::put(const imat &m, const char *name){
  put(to_mat(m), name);
}

void Matlab_Engine::put(const bmat &m, const char *name){
  put(to_mat(m), name);
}

vec Matlab_Engine::get_vec(const char *name){
  vec v;
  get(v, name);
  return v;
}

cvec Matlab_Engine::get_cvec(const char *name){
  cvec v;
  get(v, name);
  return v;
}

mat Matlab_Engine::get_mat(const char *name){
  mat m;
  get(m, name);
  return m;
}

cmat Matlab_Engine::get_cmat(const char *name){
  cmat m;
  get(m, name);
  return m;
}

void Matlab_Engine::command(const char *c){
  engEvalString(e, c);
}

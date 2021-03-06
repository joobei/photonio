// SPUC - Signal processing using C++ - A DSP library
/*---------------------------------------------------------------------------*
 *                                   IT++			             *
 *---------------------------------------------------------------------------*
 * Copyright (c) 1995-2001 by Tony Ottosson, Thomas Eriksson, P�l Frenger,   *
 * Tobias Ringstr�m, and Jonas Samuelsson.                                   *
 *                                                                           *
 * Permission to use, copy, modify, and distribute this software and its     *
 * documentation under the terms of the GNU General Public License is hereby *
 * granted. No representations are made about the suitability of this        *
 * software for any purpose. It is provided "as is" without expressed or     *
 * implied warranty. See the GNU General Public License for more details.    *
 *---------------------------------------------------------------------------*/

/*! 
  \file 
  \brief Conversion routines between IT++ and Matlab
  \author Tony Ottosson and P�l Frenger
  1.5
  2002/12/19 23:56:44
*/


#ifndef __itmex_h
#define __itmex_h

#include <mex.h>
#include "mbase.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#ifndef VERSION

#ifdef V4_COMPAT
#define VERSION "MATLAB 4 compatible"
#else
#define VERSION "MATLAB R11 native"
#endif /* V4_COMPAT */

#endif /* VERSION */

#ifdef ARRAY_ACCESS_INLINING
#define INLINE " (inlined)"
#else
#define INLINE
#endif /* ARRAY_ACCESS_INLINING */

static const char *version = VERSION INLINE;

#ifdef __cplusplus
extern "C" {
#endif

const char *mexVersion () {
/* mex version information */
return version;
}

#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef __cplusplus
}
#endif


//--------------------------------------------------------
// mex -> it++
//--------------------------------------------------------

/*! 
  \defgroup mexfiles Writing Matlab Mex-files using IT++
  \brief Conversions between IT++ and Matlab for writing mex-files.
  \author Tony Ottosson and P�l Frenger

  These routines are used to help writng mex-files for speeding up matlab simulations.

  A simple mex-file that performs QPSK modulation is given below.

  \code
  #include <itcomm.h>
  #include <itmex.h>

  void mexFunction(int n_output, mxArray *output[], int n_input, const mxArray *input[])
  {
    // Check the number of inputs and output arguments
    if(n_output!=1) mexErrMsgTxt("Wrong number of output variables!");
    if(n_input!=1) mexErrMsgTxt("Wrong number of input variables!");

    // Convert input variables to IT++ format
    bvec input_bits = mxArray2bvec(input[0]);
    
    // ------------------ Start of routine ---------------------------
    cvec output_symbols;
    QPSK_45 qpsk;

    output_symbols = qpsk.modulate_bits(input_bits);
    // ------------------ End of routine -----------------------------

    // Create output vectors
    output[0] = mxCreateDoubleMatrix(1,output_symbols.size(), mxCOMPLEX);

    // Convert the IT++ format to Matlab format for output
    cvec2mxArray(output_symbols, output[0]);
  }
  \endcode
*/

//!\addtogroup mexfiles
//!@{

//! Convert the matlab-format mxArray to bin
bin mxArray2bin(const mxArray *in);
//! Convert the matlab-format mxArray to short
short mxArray2short(const mxArray *in);
//! Convert the matlab-format mxArray to int
int mxArray2int(const mxArray *in);
//! Convert the matlab-format mxArray to double
double mxArray2double(const mxArray *in);
//! Convert the matlab-format mxArray to double_complex
double_complex mxArray2double_complex(const mxArray *in);
//string mxArray2string(const mxArray *in);

//! Convert the matlab-format mxArray to bvec
bvec mxArray2bvec(const mxArray *in);
//! Convert the matlab-format mxArray to svec
svec mxArray2svec(const mxArray *in);
//! Convert the matlab-format mxArray to ivec
ivec mxArray2ivec(const mxArray *in);
//! Convert the matlab-format mxArray to vec
vec mxArray2vec(const mxArray *in);
//! Convert the matlab-format mxArray to cvec
cvec mxArray2cvec(const mxArray *in);

//! Convert the matlab-format mxArray to bmat
bmat mxArray2bmat(const mxArray *in);
//! Convert the matlab-format mxArray to smat
smat mxArray2smat(const mxArray *in);
//! Convert the matlab-format mxArray to imat
imat mxArray2imat(const mxArray *in);
//! Convert the matlab-format mxArray to mat
mat mxArray2mat(const mxArray *in);
//! Convert the matlab-format mxArray to cmat
cmat mxArray2cmat(const mxArray *in);

//--------------------------------------------------------
// it++ -> mex
//--------------------------------------------------------
//! Convert bin to the matlab-format mxArray
void bin2mxArray(const bin &in, mxArray *out);
//! Convert short to the matlab-format mxArray
void short2mxArray(const short &in, mxArray *out);
//! Convert int to the matlab-format mxArray
void int2mxArray(const int &in, mxArray *out);
//! Convert double to the matlab-format mxArray
void double2mxArray(const double &in, mxArray *out);
//! Convert double_complex to the matlab-format mxArray
void double_complex2mxArray(const double_complex &in, mxArray *out);
//void string2mxArray(const string &in, mxArray *out);

//! Convert bvec to the matlab-format mxArray
void bvec2mxArray(const bvec &in, mxArray *out);
//! Convert svec to the matlab-format mxArray
void svec2mxArray(const svec &in, mxArray *out);
//! Convert ivec to the matlab-format mxArray
void ivec2mxArray(const ivec &in, mxArray *out);
//! Convert vec to the matlab-format mxArray
void vec2mxArray(const vec &in, mxArray *out);
//! Convert cvec to the matlab-format mxArray
void cvec2mxArray(const cvec &in, mxArray *out);

//! Convert bmat to the matlab-format mxArray
void bmat2mxArray(const bmat &in, mxArray *out);
//! Convert smat to the matlab-format mxArray
void smat2mxArray(const smat &in, mxArray *out);
//! Convert imat to the matlab-format mxArray
void imat2mxArray(const imat &in, mxArray *out);
//! Convert mat to the matlab-format mxArray
void mat2mxArray(const mat &in, mxArray *out);
//! Convert cmat to the matlab-format mxArray
void cmat2mxArray(const cmat &in, mxArray *out);

//--------------------------------------------------------
// mex -> C
//--------------------------------------------------------
//! Convert the matlab-format mxArray to C-format pointer to short
void mxArray2Csvec(const mxArray *in, short *out);
//! Convert the matlab-format mxArray to C-format pointer to int
void mxArray2Civec(const mxArray *in, int *out);
//! Convert the matlab-format mxArray to C-format pointer to double
void mxArray2Cvec(const mxArray *in, double *out);
//! Convert the matlab-format mxArray to C-format pointers to double (real and imaginary parts)
void mxArray2Ccvec(const mxArray *in, double *out_real, double *out_imag);

//! Convert the matlab-format mxArray to C-format pointer to pointer to short
void mxArray2Csmat(const mxArray *in, short **out);
//! Convert the matlab-format mxArray to C-format pointer to pointer to int
void mxArray2Cimat(const mxArray *in, int **out);
//! Convert the matlab-format mxArray to C-format pointer to pointer to double
void mxArray2Cmat(const mxArray *in, double **out);
//! Convert the matlab-format mxArray to C-format pointer to pointer to double (real and imaginary parts)
void mxArray2Ccmat(const mxArray *in, double **out_real, double **out_imag);

//--------------------------------------------------------
// C -> mex
//--------------------------------------------------------
//! Convert C-format pointer to short to matlab-format mxArray
void Csvec2mxArray(short *in, mxArray *out);
//! Convert C-format pointer to int to matlab-format mxArray
void Civec2mxArray(int *in, mxArray *out);
//! Convert C-format pointer to double to matlab-format mxArray
void Cvec2mxArray(double *in, mxArray *out);
//! Convert C-format pointers to double (real and imaginary parts) to matlab-format mxArray
void Ccvec2mxArray(double *in_real, double *in_imag, mxArray *out);

//! Convert C-format pointer to pointer to short to matlab-format mxArray
void Csmat2mxArray(short **in, mxArray *out);
//! Convert C-format pointer to pointer to int to matlab-format mxArray
void Cimat2mxArray(int **in, mxArray *out);
//! Convert C-format pointer to pointer to double to matlab-format mxArray
void Cmat2mxArray(double **in, mxArray *out);
//! Convert C-format pointer to pointer to double (real and imaginary parts) to matlab-format mxArray
void Ccmat2mxArray(double **in_real, double **in_imag, mxArray *out);

//!@}

//---------------------------------------------------------------
bin mxArray2bin(const mxArray *in)
{
  int size;
  double* temp = (double*) mxGetPr(in);
  if (temp==0) mexErrMsgTxt("mxArray2bin: Pointer to data is NULL");
  size = mxGetNumberOfElements(in);
  if (size!=1) mexErrMsgTxt("mxArray2bin: Size of data is not equal to one");

  return ( ( (*temp) > 0.0 ) ? bin(1) : bin(0) );
}

short mxArray2short(const mxArray *in)
{
  int size;
  double* temp = (double*) mxGetPr(in);
  if (temp==0) mexErrMsgTxt("mxArray2short: Pointer to data is NULL");
  size = mxGetNumberOfElements(in);
  if (size!=1) mexErrMsgTxt("mxArray2short: Size of data is not equal to one");

  return (short) (*temp);
}

int mxArray2int(const mxArray *in)
{
  int size;
  double* temp = (double*) mxGetPr(in);
  if (temp==0) mexErrMsgTxt("mxArray2int: Pointer to data is NULL");
  size = mxGetNumberOfElements(in);
  if (size!=1) mexErrMsgTxt("mxArray2int: Size of data is not equal to one");

  return (int) (*temp);
}

double mxArray2double(const mxArray *in)
{
  int size;
  double* temp = (double*) mxGetPr(in);
  if (temp==0) mexErrMsgTxt("mxArray2double: Pointer to data is NULL");
  size = mxGetNumberOfElements(in);
  if (size!=1) mexErrMsgTxt("mxArray2double: Size of data is not equal to one");

  return (*temp);
}

double_complex mxArray2double_complex(const mxArray *in)
{
  int size;
  double* tempR = (double*) mxGetPr(in);
  double* tempI = (double*) mxGetPi(in);

  if ((tempR==0)&&(tempI==0)) mexErrMsgTxt("mxArray2double_complex: Pointer to data is NULL");

  size = mxGetNumberOfElements(in);
  if (size!=1) mexErrMsgTxt("mxArray2double_complex: Size of data is not equal to one");

  if (tempR==0) {
    return double_complex( 0.0 , (*tempI) );
  } else if (tempI==0) {
    return double_complex( (*tempR), 0.0 );
  } else {
    return double_complex( (*tempR), (*tempI) );
  }

}

bvec mxArray2bvec(const mxArray *in)
{
  bvec out;
  int i, size;
  double* temp = (double*) mxGetPr(in);
  if (temp==0) mexErrMsgTxt("mxArray2bvec: Pointer to data is NULL");

  size = mxGetNumberOfElements(in);
  if (size==0) mexErrMsgTxt("mxArray2bvec: Size of data is zero");

  out.set_size(size,false);

  for (i=0; i<size; i++) {
    out(i) = ( ((*temp++)>1e-5) ? bin(1) : bin(0) );
  }

  return out;

}

svec mxArray2svec(const mxArray *in)
{
  svec out;
  int i, size;
  double* temp = (double*) mxGetPr(in);
  if (temp==0) mexErrMsgTxt("mxArray2svec: Pointer to data is NULL");

  size = mxGetNumberOfElements(in);
  if (size==0) mexErrMsgTxt("mxArray2svec: Size of data is zero");

  out.set_size(size,false);

  for (i=0; i<size; i++) {
    out(i) = (short) (*temp++);    
  }

  return out;

}

ivec mxArray2ivec(const mxArray *in)
{
  ivec out;
  int i, size;
  double* temp = (double*) mxGetPr(in);
  if (temp==0) mexErrMsgTxt("mxArray2ivec: Pointer to data is NULL");

  size = mxGetNumberOfElements(in);
  if (size==0) mexErrMsgTxt("mxArray2ivec: Size of data is zero");

  out.set_size(size,false);

  for (i=0; i<size; i++) {
    out(i) = (int) (*temp++);    
  }

  return out;

}

vec mxArray2vec(const mxArray *in)
{
  vec out;
  int i, size;
  double* temp = (double*) mxGetPr(in);
  if (temp==0) mexErrMsgTxt("mxArray2vec: Pointer to data is NULL");

  size = mxGetNumberOfElements(in);
  if (size==0) mexErrMsgTxt("mxArray2vec: Size of data is zero");

  out.set_size(size,false);

  for (i=0; i<size; i++) {
    out(i) = (*temp++);    
  }

  return out;

}

cvec mxArray2cvec(const mxArray *in)
{
  cvec out;
  int i, size;
  double* tempR = (double*) mxGetPr(in);
  double* tempI = (double*) mxGetPi(in);

  if ((tempR==0)&&(tempI==0)) mexErrMsgTxt("mxArray2cvec: Pointer data is NULL");

  size = mxGetNumberOfElements(in);
  if (size==0) mexErrMsgTxt("mxArray2cvec: Size of data is zero");

  out.set_size(size,false);

  if (tempR==0) {
    for (i=0; i<size; i++) { out(i) = double_complex( 0.0, (*tempI++)); }
  } else if (tempI==0) {
    for (i=0; i<size; i++) { out(i) = double_complex((*tempR++), 0.0 ); }
  } else {
    for (i=0; i<size; i++) { out(i) = double_complex((*tempR++), (*tempI++)); }
  }

  return out;

}

bmat mxArray2bmat(const mxArray *in)
{
  bmat out;
  int r, c, rows, cols;
  double* temp = (double*) mxGetPr(in);
  if (temp==0) mexErrMsgTxt("mxArray2bmat: Pointer to data is NULL");

  rows = mxGetM(in);
  if (rows==0) mexErrMsgTxt("mxArray2bmat: Data has zero rows");
  cols = mxGetN(in);
  if (cols==0) mexErrMsgTxt("mxArray2bmat: Data has zero columns");

  out.set_size(rows,cols,false);

  for (c=0; c<cols; c++) {
    for (r=0; r<rows; r++) {
      out(r,c) = ( ((*temp++) > 0.0) ? bin(1) : bin(0) );
    }
  }

  return out;

}

smat mxArray2smat(const mxArray *in)
{
  smat out;
  int r, c, rows, cols;
  double* temp = (double*) mxGetPr(in);
  if (temp==0) mexErrMsgTxt("mxArray2smat: Pointer to data is NULL");

  rows = mxGetM(in);
  if (rows==0) mexErrMsgTxt("mxArray2smat: Data has zero rows");
  cols = mxGetN(in);
  if (cols==0) mexErrMsgTxt("mxArray2smat: Data has zero columns");

  out.set_size(rows,cols,false);

  for (c=0; c<cols; c++) {
    for (r=0; r<rows; r++) {
      out(r,c) = (short) (*temp++);    
    }
  }

  return out;

}

imat mxArray2imat(const mxArray *in)
{
  imat out;
  int r, c, rows, cols;
  double* temp = (double*) mxGetPr(in);
  if (temp==0) mexErrMsgTxt("mxArray2imat: Pointer to data is NULL");

  rows = mxGetM(in);
  if (rows==0) mexErrMsgTxt("mxArray2imat: Data has zero rows");
  cols = mxGetN(in);
  if (cols==0) mexErrMsgTxt("mxArray2imat: Data has zero columns");
  out.set_size(rows,cols,false);

  for (c=0; c<cols; c++) {
    for (r=0; r<rows; r++) {
      out(r,c) = (int) (*temp++);    
    }
  }

  return out;

}

mat mxArray2mat(const mxArray *in)
{
  mat out;
  int r, c, rows, cols;
  double* temp = (double*) mxGetPr(in);
  if (temp==0) mexErrMsgTxt("mxArray2mat: Pointer to data is NULL");

  rows = mxGetM(in);
  if (rows==0) mexErrMsgTxt("mxArray2mat: Data has zero rows");
  cols = mxGetN(in);
  if (cols==0) mexErrMsgTxt("mxArray2mat: Data has zero columns");
  out.set_size(rows,cols,false);

  for (c=0; c<cols; c++) {
    for (r=0; r<rows; r++) {
      out(r,c) = (*temp++);    
    }
  }

  return out;

}

cmat mxArray2cmat(const mxArray *in)
{
  cmat out;
  int r, c, rows, cols;
  double* tempR = (double*) mxGetPr(in);
  double* tempI = (double*) mxGetPi(in);

  if ((tempR==0)&&(tempI==0)) mexErrMsgTxt("mxArray2cmat: Pointer to data is NULL");

  rows = mxGetM(in);
  if (rows==0) mexErrMsgTxt("mxArray2cmat: Data has zero rows");
  cols = mxGetN(in);
  if (cols==0) mexErrMsgTxt("mxArray2cmat: Data has zero columns");
  out.set_size(rows,cols,false);

  if (tempR==0) {
    for (c=0; c<cols; c++) { for (r=0; r<rows; r++) { out(r,c) = double_complex( 0.0 ,(*tempI++) ); } }
  } else if (tempI==0) {
    for (c=0; c<cols; c++) { for (r=0; r<rows; r++) { out(r,c) = double_complex( (*tempR++), 0.0 ); } }
  } else {
    for (c=0; c<cols; c++) { for (r=0; r<rows; r++) { out(r,c) = double_complex( (*tempR++),(*tempI++) ); } }
  }

  return out;

}

void bvec2mxArray(const bvec &in, mxArray *out)
{
  double* temp = (double *) mxGetPr(out);
  if (temp==0) mexErrMsgTxt("bvec2mxArray: Pointer to data is NULL");
  if (in.size()==0) mexErrMsgTxt("bvec2mxArray: Size of data is zero");
  for(int i=0; i<in.size(); i++) {
    if(in(i))
      *temp++= 1.0;
    else
      *temp++= 0.0;
  }
}

void ivec2mxArray(const ivec &in, mxArray *out)
{
  double* temp = (double *) mxGetPr(out);
  if (temp==0) mexErrMsgTxt("ivec2mxArray: Pointer to data is NULL");
  if (in.size()==0) mexErrMsgTxt("ivec2mxArray: Size of data is zero");

  for(int i=0; i<in.size(); i++) {
    *temp++= (double) in(i);
  }
}

void vec2mxArray(const vec &in, mxArray *out)
{
  double* temp = (double *) mxGetPr(out);
  if (temp==0) mexErrMsgTxt("vec2mxArray: Pointer to data is NULL");
  if (in.size()==0) mexErrMsgTxt("vec2mxArray: Size of data is zero");

  for(int i=0; i<in.size(); i++) {
    *temp++= (double) in(i);
  }
}

void cvec2mxArray(const cvec &in, mxArray *out)
{
  double* tempR = (double *) mxGetPr(out);
  double* tempI = (double *) mxGetPi(out);
  if (tempR==0) mexErrMsgTxt("cvec2mxArray: Pointer to real valued part is NULL");
  if (tempI==0) mexErrMsgTxt("cvec2mxArray: Pointer to imaginary valued part is NULL");
  if (in.size()==0) mexErrMsgTxt("cvec2mxArray: Size of data is zero");

  for(int i=0; i<in.size(); i++) {
    *tempR++= (double) in(i).real();
    *tempI++= (double) in(i).imag();
  }
}

void bmat2mxArray(const bmat &in, mxArray *out)
{
  int rows, cols, r, c;

  double* temp = (double *) mxGetPr(out);
  if (temp==0) mexErrMsgTxt("bmat2mxArray: Pointer to data is NULL");

  rows = in.rows();
  cols = in.cols();
  if (rows==0) mexErrMsgTxt("bmat2mxArray: Data has zero rows");
  if (cols==0) mexErrMsgTxt("bmat2mxArray: Data has zero columns");

  for(c=0; c<cols; c++) {
    for(r=0; r<rows; r++) {
      if(in(r,c))
        *temp++= 1.0;
      else
        *temp++= 0.0;    
    }
  }

}

void smat2mxArray(const smat &in, mxArray *out)
{
  int rows, cols, r, c;

  double* temp = (double *) mxGetPr(out);
  if (temp==0) mexErrMsgTxt("smat2mxArray: Pointer to data is NULL");

  rows = in.rows();
  cols = in.cols();
  if (rows==0) mexErrMsgTxt("smat2mxArray: Data has zero rows");
  if (cols==0) mexErrMsgTxt("smat2mxArray: Data has zero columns");

  for(c=0; c<cols; c++) {
    for(r=0; r<rows; r++) {
      *temp++= (double) in(r,c);
    }
  }

}

void imat2mxArray(const imat &in, mxArray *out)
{
  int rows, cols, r, c;

  double* temp = (double *) mxGetPr(out);
  if (temp==0) mexErrMsgTxt("imat2mxArray: Pointer to data is NULL");

  rows = in.rows();
  cols = in.cols();
  if (rows==0) mexErrMsgTxt("imat2mxArray: Data has zero rows");
  if (cols==0) mexErrMsgTxt("imat2mxArray: Data has zero columns");

  for(c=0; c<cols; c++) {
    for(r=0; r<rows; r++) {
      *temp++= (double) in(r,c);
    }
  }

}

void mat2mxArray(const mat &in, mxArray *out)
{
  int rows, cols, r, c;

  double* temp = (double *) mxGetPr(out);
  if (temp==0) mexErrMsgTxt("mat2mxArray: Pointer to data is NULL");

  rows = in.rows();
  cols = in.cols();
  if (rows==0) mexErrMsgTxt("mat2mxArray: Data has zero rows");
  if (cols==0) mexErrMsgTxt("mat2mxArray: Data has zero columns");

  for(c=0; c<cols; c++) {
    for(r=0; r<rows; r++) {
      *temp++= in(r,c);
    }
  }

}

void cmat2mxArray(const cmat &in, mxArray *out)
{
  int rows, cols, r, c;

  double* tempR = (double *) mxGetPr(out);
  double* tempI = (double *) mxGetPi(out);
  if (tempR==0) mexErrMsgTxt("cvec2mxArray: Pointer to real valued part is NULL");
  if (tempI==0) mexErrMsgTxt("cvec2mxArray: Pointer to imaginary valued part is NULL");

  rows = in.rows();
  cols = in.cols();
  if (rows==0) mexErrMsgTxt("cvec2mxArray: Data has zero rows");
  if (cols==0) mexErrMsgTxt("cvec2mxArray: Data has zero columns");

  for(c=0; c<cols; c++) {
    for(r=0; r<rows; r++) {
      *tempR++= (double) in(r,c).real();
      *tempI++= (double) in(r,c).imag();
    }
  }

}

void mxArray2Csvec(const mxArray *in, short *out)
{
  double* temp = (double*) mxGetPr(in);
  if (temp==0) mexErrMsgTxt("mxArray2Csvec: Pointer to data is NULL");
  int size = mxGetNumberOfElements(in);
  if (size==0) mexErrMsgTxt("mxArray2Csvec: Size of data is zero");
  for (int i=0; i<size; i++) { out[i] = (short) (*temp++); }
}

void mxArray2Civec(const mxArray *in, int *out)
{
  double* temp = (double*) mxGetPr(in);
  if (temp==0) mexErrMsgTxt("mxArray2Civec: Pointer to data is NULL");
  int size = mxGetNumberOfElements(in);
  if (size==0) mexErrMsgTxt("mxArray2Civec: Size of data is zero");
  for (int i=0; i<size; i++) { out[i] = (int) (*temp++); }
}

void mxArray2Cvec(const mxArray *in, double *out)
{
  double* temp = (double*) mxGetPr(in);
  if (temp==0) mexErrMsgTxt("mxArray2Cvec: Pointer to data is NULL");
  int size = mxGetNumberOfElements(in);
  if (size==0) mexErrMsgTxt("mxArray2Cvec: Size of data is zero");
  for (int i=0; i<size; i++) { out[i] = (*temp++); }
}

void mxArray2Ccvec(const mxArray *in, double *out_real, double *out_imag)
{
  double* tempR = (double*) mxGetPr(in);
  double* tempI = (double*) mxGetPi(in);
  if (tempR==0) mexErrMsgTxt("mxArray2Ccvec: Pointer to real valued part is NULL");
  if (tempI==0) mexErrMsgTxt("mxArray2Ccvec: Pointer to imaginary valued part is NULL");
  int size = mxGetNumberOfElements(in);
  if (size==0) mexErrMsgTxt("mxArray2Ccvec: Size of data is zero");
  for (int i=0; i<size; i++) { out_real[i] = (*tempR++); out_imag[i] = (*tempI++); }
}

void mxArray2Csmat(const mxArray *in, short **out)
{
  int r, c;
  double* temp = (double*) mxGetPr(in);
  if (temp==0) mexErrMsgTxt("mxArray2Csmat: Pointer to data is NULL");
  int rows = mxGetM(in);
  if (rows==0) mexErrMsgTxt("mxArray2Csmat: Data has zero rows");
  int cols = mxGetN(in);
  if (cols==0) mexErrMsgTxt("mxArray2Csmat: Data has zero columns");
  for (c=0; c<cols; c++) {
    for (r=0; r<rows; r++) {
      out[r][c] = (short) (*temp++);    
    }
  }
}

void mxArray2Cimat(const mxArray *in, int **out)
{
  int r, c;
  double* temp = (double*) mxGetPr(in);
  if (temp==0) mexErrMsgTxt("mxArray2Cimat: Pointer to data is NULL");
  int rows = mxGetM(in);
  if (rows==0) mexErrMsgTxt("mxArray2Cimat: Data has zero rows");
  int cols = mxGetN(in);
  if (cols==0) mexErrMsgTxt("mxArray2Cimat: Data has zero columns");
  for (c=0; c<cols; c++) {
    for (r=0; r<rows; r++) {
      out[r][c] = (int) (*temp++);    
    }
  }
}

void mxArray2Cmat(const mxArray *in, double **out)
{
  int r, c;
  double* temp = (double*) mxGetPr(in);
  if (temp==0) mexErrMsgTxt("mxArray2Cmat: Pointer to data is NULL");
  int rows = mxGetM(in);
  if (rows==0) mexErrMsgTxt("mxArray2Cmat: Data has zero rows");
  int cols = mxGetN(in);
  if (cols==0) mexErrMsgTxt("mxArray2Cmat: Data has zero columns");
  for (c=0; c<cols; c++) {
    for (r=0; r<rows; r++) {
      out[r][c] = (*temp++);    
    }
  }
}

void mxArray2Ccmat(const mxArray *in, double **out_real, double **out_imag)
{
  int r, c;
  double* tempR = (double*) mxGetPr(in);
  double* tempI = (double*) mxGetPi(in);
  if (tempR==0) mexErrMsgTxt("mxArray2Cmat: Pointer to real valued part is NULL");
  if (tempI==0) mexErrMsgTxt("mxArray2Cmat: Pointer to imaginary valued part is NULL");
  int rows = mxGetM(in);
  if (rows==0) mexErrMsgTxt("mxArray2Cmat: Data has zero rows");
  int cols = mxGetN(in);
  if (cols==0) mexErrMsgTxt("mxArray2Cmat: Data has zero columns");
  for (c=0; c<cols; c++) {
    for (r=0; r<rows; r++) {
      out_real[r][c] = (*tempR++);    
      out_imag[r][c] = (*tempI++);    
    }
  }
}

void Csvec2mxArray(short *in, mxArray *out)
{
  double* temp = (double *) mxGetPr(out);
  if (temp==0) mexErrMsgTxt("Csvec2mxArray: Pointer to data is NULL");
  int size = mxGetNumberOfElements(out);
  if (size==0) mexErrMsgTxt("Csvec2mxArray: Size of data is zero");
  for(int i=0; i<size; i++) { *temp++= (double) in[i]; }
}

void Civec2mxArray(int *in, mxArray *out)
{
  double* temp = (double *) mxGetPr(out);
  if (temp==0) mexErrMsgTxt("Civec2mxArray: Pointer to data is NULL");
  int size = mxGetNumberOfElements(out);
  if (size==0) mexErrMsgTxt("Civec2mxArray: Size of data is zero");
  for(int i=0; i<size; i++) { *temp++= (double) in[i]; }
}

void Cvec2mxArray(double *in, mxArray *out)
{
  double* temp = (double *) mxGetPr(out);
  if (temp==0) mexErrMsgTxt("Cvec2mxArray: Pointer to data is NULL");
  int size = mxGetNumberOfElements(out);
  if (size==0) mexErrMsgTxt("Cvec2mxArray: Size of data is zero");
  for(int i=0; i<size; i++) { *temp++= in[i]; }
}

void Ccvec2mxArray(double *in_real, double *in_imag, mxArray *out)
{
  double* tempR = (double *) mxGetPr(out);
  double* tempI = (double *) mxGetPi(out);
  if (tempR==0) mexErrMsgTxt("Ccvec2mxArray: Pointer to real valued part is NULL");
  if (tempI==0) mexErrMsgTxt("Ccvec2mxArray: Pointer to imaginary valued part is NULL");
  int size = mxGetNumberOfElements(out);
  if (size==0) mexErrMsgTxt("Ccvec2mxArray: Size of data is zero");
  for(int i=0; i<size; i++) { *tempR++= in_real[i]; *tempI++= in_imag[i]; }
}

void Csmat2mxArray(short **in, mxArray *out)
{
  int r, c;
  double* temp = (double *) mxGetPr(out);
  if (temp==0) mexErrMsgTxt("Csmat2mxArray: Pointer to data is NULL");
  int rows = mxGetM(out);
  if (rows==0) mexErrMsgTxt("Csmat2mxArray: Data has zero rows");
  int cols = mxGetN(out);
  if (cols==0) mexErrMsgTxt("Csmat2mxArray: Data has zero columns");
  for(c=0; c<cols; c++) {
    for(r=0; r<rows; r++) {
      *temp++= (short) in[r][c];
    }
  }
}

void Cimat2mxArray(int **in, mxArray *out)
{
  int r, c;
  double* temp = (double *) mxGetPr(out);
  if (temp==0) mexErrMsgTxt("Cimat2mxArray: Pointer to data is NULL");
  int rows = mxGetM(out);
  if (rows==0) mexErrMsgTxt("Cimat2mxArray: Data has zero rows");
  int cols = mxGetN(out);
  if (cols==0) mexErrMsgTxt("Cimat2mxArray: Data has zero columns");
  for(c=0; c<cols; c++) {
    for(r=0; r<rows; r++) {
      *temp++= (int) in[r][c];
    }
  }
}

void Cmat2mxArray(double **in, mxArray *out)
{
  int r, c;
  double* temp = (double *) mxGetPr(out);
  if (temp==0) mexErrMsgTxt("Cmat2mxArray: Pointer to data is NULL");
  int rows = mxGetM(out);
  if (rows==0) mexErrMsgTxt("Cmat2mxArray: Data has zero rows");
  int cols = mxGetN(out);
  if (cols==0) mexErrMsgTxt("Cmat2mxArray: Data has zero columns");
  for(c=0; c<cols; c++) {
    for(r=0; r<rows; r++) {
      *temp++= in[r][c];
    }
  }
}

void Ccmat2mxArray(double **in_real, double **in_imag, mxArray *out)
{
  int r, c;
  double* tempR = (double *) mxGetPr(out);
  double* tempI = (double *) mxGetPi(out);
  if (tempR==0) mexErrMsgTxt("Ccmat2mxArray: Pointer to real valued part is NULL");
  if (tempI==0) mexErrMsgTxt("Ccmat2mxArray: Pointer to imaginary valued part is NULL");
  int rows = mxGetM(out);
  if (rows==0) mexErrMsgTxt("Ccmat2mxArray: Data has zero rows");
  int cols = mxGetN(out);
  if (cols==0) mexErrMsgTxt("Ccmat2mxArray: Data has zero columns");
  for(c=0; c<cols; c++) {
    for(r=0; r<rows; r++) {
      *tempR++= in_real[r][c];
      *tempI++= in_imag[r][c];
    }
  }
}

#endif //__itmex_h

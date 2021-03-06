// 
// SPUC - Signal processing using C++ - A DSP library
/*---------------------------------------------------------------------------*
 *                                   IT++			             *
 *---------------------------------------------------------------------------*
 * Copyright (c) 1995-2001 by Tony Ottosson, Thomas Eriksson, Pål Frenger,   *
 * Tobias Ringström, and Jonas Samuelsson.                                   *
 *                                                                           *
 * Permission to use, copy, modify, and distribute this software and its     *
 * documentation under the terms of the GNU General Public License is hereby *
 * granted. No representations are made about the suitability of this        *
 * software for any purpose. It is provided "as is" without expressed or     *
 * implied warranty. See the GNU General Public License for more details.    *
 *---------------------------------------------------------------------------*/

/*! 
  \file 
  \brief Definitions of interleaver classes
  \author Pål Frenger, Modified by Tony Kirke, Feb 1,2003

  1.7

  2002/02/18 23:15:36
*/

#ifndef __interleave_h
#define __interleave_h
#include <binary.h>
#include <matrix.h>
#include <random.h>
namespace SPUC {
template<class T> class Vec;
template<class T> class Mat;
/*! \defgroup interl Interleavers
 */

/*! \ingroup interl
  \class Block_Interleaver comm/interleave.h
  \brief Block Interleaver Class.
  
  Data is written row-wise and read column-wise when interleaving.
  <h3>Example of use:</h3>
  \code
  BPSK bpsk;
  bvec bits = "0 1 1 0 0 0 1 1 1 1 0 0 1 0 0 1";
  vec symbols = bpsk.modulate_bits(bits);

  Block_Interleaver<double> block_interleaver(4,4);
  vec interleaved_symbols = block_interleaver.interleave(symbols);
  \endcode
  
*/
template <class T>
class Block_Interleaver {
public:
    //! Block_Interleaver constructor
    Block_Interleaver(void) {rows = 0; cols = 0;};
    //! Block_Interleaver constructor
    Block_Interleaver(int in_rows, int in_cols);
    //! Function for block interleaving. May add some zeros.
    Vec<T> interleave(const Vec<T> &input);
    //! Function for block interleaving. May add some zeros.
    void interleave(const Vec<T> &input, Vec<T> &output);
    //! Function for block deinterleaving. Removes additional zeros if \a keepzeros = 0.
    Vec<T> deinterleave(const Vec<T> &input, short keepzeros = 0 );
    //! Function for block deinterleaving. Removes additional zeros if \a keepzeros = 0.
    void deinterleave(const Vec<T> &input, Vec<T> &output, short keepzeros = 0 );
    //! Set the number of \a rows for block interleaving
    void set_rows(int in_rows) {rows = in_rows;};
    //! Set the number of \a columns for block interleaving
    void set_cols(int in_cols) {cols = in_cols;};
    //! Get the number of \a rows for block interleaving
    int get_rows(void) {return rows;};
    //! Get the number of \a columns for block interleaving    
    int get_cols(void) {return cols;};
private:
    int rows, cols, input_length;
};

/*! \ingroup interl
  \class Cross_Interleaver comm/interleave.h
  \brief Cross Interleaver Class.

  <h3> Example of use: </h3>
  \code
  BPSK bpsk;
  bvec bits = "0 1 1 0 0 0 1 1 1 1 0 0 1 0 0 1";
  vec symbols = bpsk.modulate_bits(bits);

  Cross_Interleaver<double> cross_interleaver(4);
  vec interleaved_symbols = cross_interleaver.interleave(symbols);
  \endcode
  
  <ul>
  <li> See S. B. Wicker, "Error control systems for digital communications and storage,"
  Prentice Hall 1995, p. 427 for details. </li>
  </ul>
*/
template <class T>
class Cross_Interleaver {
public:
    //! Cross_Interleaver constructor
    Cross_Interleaver(void) {order = 0;};
    //! Cross_Interleaver constructor
    Cross_Interleaver(int in_order);
    //! Function for cross interleaving. Adds some zeros.
    Vec<T> interleave(const Vec<T> &input);
    //! Function for cross interleaving. Adds some zeros.
    void interleave(const Vec<T> &input, Vec<T> &output);
    //! Function for cross deinterleaving. Removes aditional zeros if \a keepzeros = 0.
    Vec<T> deinterleave(const Vec<T> &input, short keepzeros = 0);
    //! Function for cross deinterleaving. Removes aditional zeros if \a keepzeros = 0.
    void deinterleave(const Vec<T> &input, Vec<T> &output, short keepzeros = 0);
    //! Set the \a order of the Cross Interleaver
    void set_order(int in_order); 
     //! Get the \a order of the Cross Interleaver
    int get_order(void) {return order;};
private:
    int order;
    int input_length;
    Mat<T> inter_matrix;
    Vec<T> tempvec, zerostemp;
};

/*! \ingroup interl
  \class Sequence_Interleaver comm/interleave.h
  \brief Sequence Interleaver Class

  <h3> Example of use: </h3>
  \code
  BPSK bpsk;
  bvec bits = "0 1 1 0 0 0 1 1 1 1 0 0 1 0 0 1";
  vec symbols = bpsk.modulate_bits(bits);

  Sequence_Interleaver<double> sequence_interleaver(16);
  sequence_interleaver.randomize_interleaver_sequence();
  vec interleaved_symbols = sequence_snterleaver.interleave(symbols);
  \endcode
  
*/
template <class T>
class Sequence_Interleaver {
public:
    //! Sequence_Interleaver constructor.
    Sequence_Interleaver(void) {interleaver_depth = 0;};
    /*! 
      \brief Sequence_Interleaver constructor.

      Chooses a random sequence of length \a in_interleaver_depth for interleaving.
    */
    Sequence_Interleaver(int in_interleaver_depth);
    /*! 
      \brief Sequence_Interleaver constructor.

      Uses the \a in_interleaver_sequence for interleaving.
    */
    Sequence_Interleaver(ivec in_interleaver_sequence);
    //! Function for sequence interleaving. May add some zeros.
    Vec<T> interleave(const Vec<T> &input);
    //! Function for sequence interleaving. May add some zeros.
    void interleave(const Vec<T> &input, Vec<T> &output);
    //! Function for sequence deinterleaving. Removes additional zeros if \a keepzeros = 0.
    Vec<T> deinterleave(const Vec<T> &input, short keepzeros = 0 );
    //! Function for sequence deinterleaving. Removes additional zeros if \a keepzeros = 0.
    void deinterleave(const Vec<T> &input, Vec<T> &output, short keepzeros = 0 );
    //! Generate a new random sequence for interleaving.
    void randomize_interleaver_sequence();
    //! Returns the interleaver sequence presently used.
    ivec get_interleaver_sequence();
    //! Set the interleaver sequence to be used.
    void set_interleaver_sequence(ivec in_interleaver_sequence);
    //! Set the length of the interleaver sequence to be used.
    void set_interleaver_depth(int in_interleaver_depth) { interleaver_depth = in_interleaver_depth; };
    //! Get the length of the interleaver sequence presently used.
    int get_interleaver_depth(void) { return interleaver_depth; };
private:
    ivec interleaver_sequence;
    int interleaver_depth, input_length;
};
/************************************************************
 * Block Interleaver
 ************************************************************/
template<class T>
Block_Interleaver<T>::Block_Interleaver(int in_rows, int in_cols)
{
    rows = in_rows;
    cols = in_cols;
    input_length = 0;
};

template<class T>
void Block_Interleaver<T>::interleave(const Vec<T> &input, Vec<T> &output)
{
    input_length = input.length();
    long_long steps = (long_long)ceil( double(input_length)/double(rows*cols) );
    long_long output_length = steps * rows * cols;
    output.set_length(output_length,false);
    int	s, r, c;
    
    if (input_length==output_length) {
	//Block interleaver loop: All steps.
	for (s=0; s<steps; s++) {
	    for (c=0; c<cols; c++) {
		for (r=0; r<rows; r++) {
		    output(s*rows*cols+r*cols+c) = input(s*rows*cols+c*rows+r);
		}
	    }
	}
    } else {
	//Block interleaver loop: All, but the last, steps.
	for (s=0; s<steps-1; s++) {
	    for (c=0; c<cols; c++) {
		for (r=0; r<rows; r++) {
		    output(s*rows*cols+r*cols+c) = input(s*rows*cols+c*rows+r);
		}
	    }
	}
	//The last step.
	Vec<T> zerovect(output_length - input_length);
	zerovect.clear();
	Vec<T> temp_last_input = concat( input.right(rows*cols-zerovect.length()), zerovect );
	for (c=0; c<cols; c++) {
	    for (r=0; r<rows; r++) {
		output((steps-1)*rows*cols+r*cols+c) = temp_last_input(c*rows+r);
	    }
	}
    }
}

template<class T>
Vec<T> Block_Interleaver<T>::interleave(const Vec<T> &input)
{
    Vec<T> output;
    interleave(input,output);
    return output;
}

template<class T>
void Block_Interleaver<T>::deinterleave(const Vec<T> &input, Vec<T> &output, short keepzeros)
{
    long_long thisinput_length = input.length();
    long_long steps = (long_long)ceil( double(thisinput_length)/double(rows*cols) );
    long_long output_length = steps * rows * cols;
    output.set_size(output_length,false);
    long_long	s, r, c;

    if (thisinput_length==output_length) {
	//Block deinterleaver loop: All, but the last, steps.
	for (s=0; s<steps; s++) {
	    for (r=0; r<rows; r++) {
		for (c=0; c<cols; c++) {
		    output(s*rows*cols+c*rows+r) = input(s*rows*cols+r*cols+c);
		}
	    }
	}
    } else {
	//Block deinterleaver loop: All, but the last, steps.
	for (s=0; s<steps-1; s++) {
	    for (r=0; r<rows; r++) {
		for (c=0; c<cols; c++) {
		    output(s*rows*cols+c*rows+r) = input(s*rows*cols+r*cols+c);
		}
	    }
	}
	//The last step.
	Vec<T> zerovect(output_length - thisinput_length);
	zerovect.clear();
	Vec<T> temp_last_input = concat( input.right(rows*cols-zerovect.length()), zerovect );
	for (r=0; r<rows; r++) {
	    for (c=0; c<cols; c++) {
		output((steps-1)*rows*cols+c*rows+r) = temp_last_input(r*cols+c);
	    }
	}
	if (keepzeros == 0)
	    output.set_size(input_length,true);
    }
}

template<class T>
Vec<T> Block_Interleaver<T>::deinterleave(const Vec<T> &input, short keepzeros)
{
    Vec<T> output;
    deinterleave(input,output,keepzeros);
    return output;
}

/************************************************************
 * Cross Interleaver
 ************************************************************/
template<class T>
Cross_Interleaver<T>::Cross_Interleaver(int in_order)
{
    order = in_order;
    input_length = 0;
    inter_matrix.set_size(order,order,false);
    tempvec.set_size(order,false);
    zerostemp.set_size(order,false);
}

template<class T>
void Cross_Interleaver<T>::interleave(const Vec<T> &input, Vec<T> &output)
{
    input_length = input.length();
    long_long steps = (long_long)ceil( float(input_length) / order ) + order;
    long_long output_length = steps * order;
    output.set_length(output_length,false);
    long_long i, r, c;

    inter_matrix.clear();
    zerostemp.clear();
	
    //Cross interleaver loop:
    for (i=0; i< steps; i++ ){

	//Shift the matrix to the right:
	for (c=order-1; c>0; c--) 
	    inter_matrix.set_col(c, inter_matrix.get_col(c-1) );
		
	// Write the new data to the matrix
	if ((i*order+order)<input_length)
	    tempvec = input.mid(i*order,order);
	else if ((i*order)<input_length)
	    tempvec = concat( input.right(input_length-i*order), zerostemp.left(order-(input_length-i*order)) );
	else
	    tempvec.clear();
	inter_matrix.set_col(0,tempvec);
		
	//Read the matrix diagonal-wise:
	for (r=0; r<order; r++) 
	    output(i*order+r) = inter_matrix(r,r);
    }
}


template<class T>
Vec<T> Cross_Interleaver<T>::interleave(const Vec<T> &input)
{
    Vec<T> output;
    interleave(input,output);
    return output;
}

template<class T>
void Cross_Interleaver<T>::deinterleave(const Vec<T> &input, Vec<T> &output, short keepzeros)
{
    long_long thisinput_length = input.length();
    long_long	steps = (long_long)ceil( float(thisinput_length) / order ) + order;
    long_long output_length = steps * order;
    output.set_size(output_length,false);
    long_long i, r, c;
	long pow(long o, long i);

    inter_matrix.clear();
    zerostemp.clear();
	
    //Cross interleaver loop:
    for (i=0; i<steps; i++ ){

	//Shift the matrix to the right:
	for (c=order-1; c>0; c--) 
	    inter_matrix.set_col(c,inter_matrix.get_col(c-1));
		
	// Write the new data to the matrix
	if ((i*order+order)<thisinput_length)
	    tempvec = input.mid(i*order,order);
	else if ((i*order)<thisinput_length)
	    tempvec = concat( input.right(thisinput_length-i*order), zerostemp.left(order-(thisinput_length-i*order)) );
	else
	    tempvec.clear();
	inter_matrix.set_col(0,tempvec);
		
	//Read the matrix diagonal-wise:
	for (r=0; r<order; r++) 
	    output(i*order+r)  = inter_matrix(r,order-1-r);
    }
    if (keepzeros == 0) 
	output = output.mid(pow(order,2)-order,input_length);
}

template<class T>
Vec<T> Cross_Interleaver<T>::deinterleave(const Vec<T> &input, short keepzeros)
{
    Vec<T> output;
    deinterleave(input,output,keepzeros);
    return output;
}

template<class T>
void Cross_Interleaver<T>::set_order(int in_order)
{
    order = in_order;
    input_length = 0;
    inter_matrix.set_size(order,order,false);
    tempvec.set_size(order,false);
    zerostemp.set_size(order,false);
}

/************************************************************
 * Sequence Interleaver
 ************************************************************/
template<class T>
Sequence_Interleaver<T>::Sequence_Interleaver(int in_interleaver_depth)
{
	double randu(void);
    interleaver_depth = in_interleaver_depth;
    interleaver_sequence = sort_index(randu(in_interleaver_depth));
    input_length = 0;
}

template<class T>
Sequence_Interleaver<T>::Sequence_Interleaver(ivec in_interleaver_sequence)
{
    interleaver_depth = in_interleaver_sequence.length();
    interleaver_sequence = in_interleaver_sequence;
    input_length = 0;
}

template<class T>
void Sequence_Interleaver<T>::interleave(const Vec<T> &input, Vec<T> &output)
{
    input_length = input.length();
    long_long steps = (long_long)ceil( double(input_length)/double(interleaver_depth) );
    long_long output_length = steps*interleaver_depth;
    output.set_size(output_length,false);
    long_long	s, i;

    if (input_length==output_length) {
	
	//Sequence interleaver loop: All steps.
	for (s=0; s<steps; s++) {
	    for (i=0; i<interleaver_depth; i++) {
		output(s*interleaver_depth+i) = input(s*interleaver_depth+interleaver_sequence(i));
	    }
	}
	
    } else {
	
	//Sequence interleaver loop: All, but the last, steps.
	for (s=0; s<steps-1; s++) {
	    for (i=0; i<interleaver_depth; i++) {
		output(s*interleaver_depth+i) = input(s*interleaver_depth+interleaver_sequence(i));
	    }
	}
	//The last step.
	Vec<T> zerovect(output_length - input_length);
	zerovect.clear();
	Vec<T> temp_last_input = concat( input.right(interleaver_depth-zerovect.length()), zerovect );
	for (i=0; i<interleaver_depth; i++) {
	    output((steps-1)*interleaver_depth+i) = temp_last_input(interleaver_sequence(i));
	}
	
    }
}

template<class T>
Vec<T> Sequence_Interleaver<T>::interleave(const Vec<T> &input)
{
    Vec<T> output;
    interleave(input,output);
    return output;
}

template<class T>
void Sequence_Interleaver<T>::deinterleave(const Vec<T> &input, Vec<T> &output, short keepzeros)
{
    long_long thisinput_length = input.length();
    long_long steps = (long_long)ceil( double(thisinput_length)/double(interleaver_depth) );
    long_long output_length = steps*interleaver_depth;
    output.set_length(output_length,false);
    long_long	s, i;

    if (thisinput_length == output_length) {

	//Sequence interleaver loop: All steps.
	for (s=0; s<steps; s++) {
	    for (i=0; i<interleaver_depth; i++) {
		output(s*interleaver_depth+interleaver_sequence(i)) = input(s*interleaver_depth+i);
	    }
	}
	
    } else {
	//Sequence interleaver loop: All, but the last, steps.
	for (s=0; s<steps-1; s++) {
	    for (i=0; i<interleaver_depth; i++) {
		output(s*interleaver_depth+interleaver_sequence(i)) = input(s*interleaver_depth+i);
	    }
	}
	//The last step.
	Vec<T> zerovect(output_length - thisinput_length);
	zerovect.clear();
	Vec<T> temp_last_input = concat( input.right(interleaver_depth-zerovect.length()), zerovect );
	for (i=0; i<interleaver_depth; i++) {
	    output((steps-1)*interleaver_depth+interleaver_sequence(i)) = temp_last_input(i);
	}
	if (keepzeros == 0) 
	    output.set_size(input_length,true);
    }

}

template<class T>
Vec<T> Sequence_Interleaver<T>::deinterleave(const Vec<T> &input, short keepzeros)
{
    Vec<T> output;
    deinterleave(input,output,keepzeros);
    return output;
}

template<class T>
void Sequence_Interleaver<T>::randomize_interleaver_sequence()
{
	double randu(void);
    interleaver_sequence = sort_index(randu(interleaver_depth));
}

template<class T>
ivec Sequence_Interleaver<T>::get_interleaver_sequence()
{
    return interleaver_sequence;
}

template<class T>
void Sequence_Interleaver<T>::set_interleaver_sequence(ivec in_interleaver_sequence)
{
    interleaver_sequence = in_interleaver_sequence;
    interleaver_depth = interleaver_sequence.size(); //<-- Bug Fix 2000-09-29 PF
}

}
#endif

// 
// SPUC - Signal processing using C++ - A DSP library
/*! 
  \file 
  \brief Definitions of convolution interleaver class
  \author T Kirke
*/

#ifndef __cinterleave_h
#define __cinterleave_h
#include <binary.h>
#include <matrix.h>
#include <random.h>
namespace SPUC {
#define MAXINTLVR 128
	template<class T> class Vec;
	template<class T> class Mat;
	/*! \defgroup interl Interleavers
	 */
	
	/*! \ingroup interl
	  \class Convolutional_Interleaver comm/conv_interleaver.h
	  \brief Convolutional Interleaver Class.
	  
	  \endcode
	  
	*/
	template <class T>
		class Convolutional_Interleaver {
	public:
		//! Convolutional_Interleaver constructor
		Convolutional_Interleaver(void) {rows = 0; cols = 0; rd_sel = wr_sel = 0;};
		//! Convolutional_Interleaver constructor
		Convolutional_Interleaver(int in_rows, int in_cols);
		//! Function for convolutional interleaving.
		T interleave(const T input);
		void interleave_write(const T input);
		T interleave_read(void);
		//! Function for convolutional deinterleaving.
		T deinterleave(const T input);
		//! Set the number of \a rows for block interleaving
		void set_size(int in_rows, int in_cols) {
			int i;
			rows = in_rows;
			cols = in_cols;
			input_length = 0;
			if (rows > MAXINTLVR) rows = MAXINTLVR;
			for (i=0;i<rows;i++) {
				D[i].set_size(i*cols);
			}
			wr_sel = 0;
			rd_sel = 0;
		};
		//! Get the number of \a rows for block interleaving
		int get_rows(void) {return rows;};
		//! Get the number of \a columns for block interleaving    
		int get_cols(void) {return cols;};
		void reset(void)
		{
			int i;
			for (i=0;i<rows;i++) D[i].reset();
			wr_sel = 0;
			rd_sel = 0;
		}
		void resync(void) { 
			wr_sel = 0;
			rd_sel = 0;
		}
	private:
		int rows, cols, input_length;
		int wr_sel;
		int rd_sel;
		delay<T> D[MAXINTLVR];

	};
	
	//
	// Convolutional Interleaver
	//
	template<class T>
		Convolutional_Interleaver<T>::Convolutional_Interleaver(int in_rows, int in_cols)
		{
			int i;
			rows = in_rows;
			cols = in_cols;
			input_length = 0;
			if (rows > MAXINTLVR) rows = MAXINTLVR;
			for (i=0;i<rows;i++) {
				D[i].set_size(i*cols);
			}
			rd_sel = wr_sel = 0;
		}
	
	template<class T>
		T Convolutional_Interleaver<T>::interleave(const T input)
		{
			interleave_write(input);
			return(interleave_read());
		}
	template<class T>
		void Convolutional_Interleaver<T>::interleave_write(const T input)
		{
			D[wr_sel].input(input);
			wr_sel++;
			wr_sel = wr_sel % rows;
		}
	template<class T>
		T Convolutional_Interleaver<T>::interleave_read(void)
		{
			T outp = D[rd_sel].last();
			rd_sel++;
			rd_sel = rd_sel % rows;
			return(outp);
		}
	template<class T>
		T Convolutional_Interleaver<T>::deinterleave(const T input)
		{
			// not using rd_sel yet!
			T outp = D[rows-1-wr_sel].update(input);
			wr_sel++;
			wr_sel = wr_sel % rows;
			return(outp);
		}


}

#endif

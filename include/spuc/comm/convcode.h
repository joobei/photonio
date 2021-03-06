// 
// SPUC - Signal processing using C++ - A DSP library
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

/*! 
  \file 
  \brief Definition of a binary convolutional encoder class
  \author Tony Ottosson 951019, Revised completely 980205, Modified by Tony Kirke, Feb 1,2003

  1.10

  2003/01/04 00:21:55
*/

#ifndef __convcode_h
#define __convcode_h

#include <array.h>
#include <binary.h>
#include <vector.h>
#include <matrix.h>

using std::string;
namespace SPUC {
/*!
  \ingroup fec
  \brief Binary Convolutional rate 1/n class
    
  The codes are given as feedforward encoders an given in the Proakis form. That is the binary
  generators (K-tuples) are converted to octal integers. Observe that the constraint length (K)
  is defined as the number of meomory cells plus one (as in Proakis).

  Encoding is performed with the encode function. Also available is the encode_tail function
  which automatically add a tail of K-1 zeros and also assume that the encoder starts in
  the zero state. Observe that decode_tail is used for data
  encoded with encode_tail, and decode assumes that the memory truncation length either is the
  default (5*K) or set using the set_truncation_length function.

  Example of use: (rate 1/3 constraint length K=7 ODS code using BPSK over AWGN)
  \code
  Convolutional_Code code;
  ivec generator(3);
  generator(0)=0133;
  generator(1)=0165;
  generator(2)=0171;
  code.set_generator_polynomials(generator, 7);
  code.set_truncation_length(30);

  bvec bits=randb(100), encoded_bits, decoded_bits;
  vec tx_signal, rx_signal;

  code.encode(bits, encoded_bits);
  tx_signal = bpsk.modulate_bits(encoded_bits);
  rx_signal = tx_signal + sqrt(0.5)*randn(tx_signal.size());
  code.decode(rx_signal, decoded_bits);
  \endcode
  Comment: ODS-code stand for Optimum Distance Spectrum Code. For details see T. Ottosson, "Coding, Modulation
  and Multiuser Decoding for DS-CDMA Systems," Ph.d. thesis, Department of Information Theory, Scool of Electrical
  and Computer Engineering, Chalmers University of Technology, Göteborg 1997.

  It is also possible to set the generatorpolynomials directly using the builtin tables which consists of:
  Maximum Free Distance (MFD) Codes of rates R=1/2 through R=1/8 and
  Optimum Distance Spectrum (ODS) Codes of rates R=1/2 through R=1/4.
*/
class Convolutional_Code {
public:
  //! Constructor
  Convolutional_Code(void) { trunc_length = 0; K = 0; m = 0; set_start_state(0); }
  //! Destructor
  virtual ~Convolutional_Code(void) {}
  /*! 
    \brief Set the code according to built-in tables

    The \a type_of_code can be either \a MFD or \a ODS for maximum free distance codes (according to Proakis)
    or Optimum Distance Spectrum Codes accoring to Frenger, Orten and Ottosson.
  */
  void set_code(string type_of_code, int inverse_rate, int constraint_length);
  //! Set generator polynomials. Given in Proakis integer form
  void set_generator_polynomials(const ivec &gen, int constraint_length);
  //! Get generator polynomials
  ivec get_generator_polynomials(void) { return gen_pol; }
  //! Return rate of code
  double get_rate(void) { return rate; }
  
  //! Encode a binary vector of inputs starting from state set by the set_state function
  void encode(const bvec &input, bvec &output);
  //! Encode a binary vector of inputs starting from state set by the set_state function
  bvec encode(const bvec &input) { bvec output; encode(input, output); return output; }
  /*!
    \brief Encoding that strarts and ends in the zero state

    Encode a binary vector of inputs starting from zero state and also adds a tail
    of K-1 zeros to force the encoder into the zero state. Well suited for packet
    transmission.
  */
  void encode_tail(const bvec &input, bvec &output);
  //! Encode a binary vector of inputs starting from state set by the set_state function
  bvec encode_tail(const bvec &input) { bvec output; encode_tail(input, output); return output; }
  
  //! Encode a binary vector of inputs using tailbiting.
  void encode_tailbite(const bvec &input, bvec &output);
  //! Encode a binary vector of inputs using tailbiting.
  bvec encode_tailbite(const bvec &input)
    { bvec output; encode_tailbite(input, output); return output; }
  
  /*!
      \brief Encode a binary bit startinf from the internal encoder state.
      
      To initialize the encoder state use set_start_state() and init_encoder()
  */
  void encode_bit(const bin &input, bvec &output);
  //! Encode a binary bit startinf from the internal encoder state.
  bvec encode_bit(const bin &input) { bvec output; encode_bit(input, output); return output; }
  //! Set the encoder internal state in start_state (set by set_start_state()).
  void init_encoder() { encoder_state = start_state; }
  
  /*!
    \brief Decode a block of encoded data where encode_tail has been used.
    
    Thus is assumes a decoder start state of zero and that a tail of
    K-1 zeros has been added. No memory truncation.
  */
  void decode_tail(const vec &received_signal, bvec &output);
  //! Decode a block of encoded data where encode_tail has been used
  bvec decode_tail(const vec &received_signal)
    { bvec output; decode_tail(received_signal, output); return output; }
  
  //! Decode a block of encoded data where encode_tailbite has been used. Tries all start states.
  void decode_tailbite(const vec &received_signal, bvec &output);
  //! Decode a block of encoded data where encode_tailbite has been used. Tries all start states.
  bvec decode_tailbite(const vec &received_signal)
    { bvec output; decode_tail(received_signal, output); return output; }
  
  //! Viterbi decoding using truncation of memory (default = 5*K)
  void decode(const vec &received_signal, bvec &output);
  //! Viterbi decoding using truncation of memory (default = 5*K)
  bvec decode(const vec &received_signal)
    { bvec output; decode(received_signal, output); return output; }
  
  /*!
    \brief Calculate the inverse sequence
    
    Assumes that encode_tail is used in the encoding process. Returns false if there is
    an error in the coded sequence (not a valid codeword). Do not check that the tail forces
    the encoder into the zeroth state.
  */
  bool inverse_tail(const bvec coded_sequence, bvec &input);
  //! Set encoder and decoder start state. The generator polynomials must be set first.
  void set_start_state(const int state)
    { 
	  //it_error_if(state<0 || (state>=(1<<m) && m != 0), "Invalid start state"); 
	  start_state = state; }
  //!  Get the current encoder state
  int get_encoder_state(void) { return encoder_state; }
  
  //! Set memory truncation length. Must be at least K.
  void set_truncation_length(const int length) { 
	  //	  it_error_if(length<K, "Truncation length shorter than K"); 
	  trunc_length = length; }
  //! Get memory truncation length
  int get_truncation_length(void) { return trunc_length; }
  
  //! Check if catastrophic. Returns true if catastrophic
  bool catastrophic(void);
  
  //! Calculate distance profile. If reverse = true calculate for the reverse code instead.
  void distance_profile(llvec &dist_prof, int dmax = 100000, bool reverse = false);
  /*!
    \brief Calculate spectrum
    
    Calculates both the weight spectrum (Ad) and the information weight spectrum (Cd) and
    returns it as llvec:s in the 0:th and 1:st component of spectrum, respectively. Suitable
    for calculating many terms in the spectra (uses an breadth first algorithm). It is assumed
    that the code is non-catastrophic or else it is a possibility for an eternal loop.
    dmax = an upper bound on the free distance
    no_terms = no_terms including the dmax term that should be calculated
  */
  void calculate_spectrum(Array<llvec> &spectrum, int dmax, int no_terms);
  /*!
    \brief Cederwall's fast algorithm
    
    See IT No. 6, pp. 1146-1159, Nov. 1989 for details.
    Calculates both the weight spectrum (Ad) and the information weight spectrum (Cd) and
    returns it as llvec:s in the 0:th and 1:st component of spectrum, respectively. The FAST algorithm
    is good for calculating only a few terms in the spectrum. If many terms are desired, use calc_spectrum instead.
    The algorithm returns -1 if the code tested is worse that the input dfree and Cdfree.
    It returns 0 if the code MAY be catastrophic (assuming that test_catastrophic is true),
    and returns 1 if everything went right.
    
    \arg \c dfree the free distance of the code (or an upper bound)
    \arg \c no_terms including the dfree term that should be calculated
    \arg \c Cdfree is the best value of information weight spectrum found so far
  */
  int fast(Array<llvec> &spectrum, const int dfree, const int no_terms,
	   const int Cdfree = 1000000, const bool test_catastrophic = false);

 protected:
  //! Next state from instate given the input
  int next_state(const int instate, const int input) { return ( (instate >> 1) | (input << (m-1))); }
  //! The previous state from state given the input
  int previous_state(const int state, const int input) { return ( ((state << 1) | input) & ((1<<m)-1) ); }
  //! The weight of the transition from given state with the input given
  int weight(const int state, const int input);
  //! The weight of the two paths (input 0 or 1) from given state
  void weight(const int state, int &w0, int &w1);
  //! The weight (of the reverse code) of the transition from given state with the input given
  int weight_reverse(const int state, const int input);
  //! The weight (of the reverse code) of the two paths (input 0 or 1) from given state
  void weight_reverse(const int state, int &w0, int &w1);
  //! Output on transition (backwards) with input from state
  bvec output_reverse(const int state, const int input);
  //! Output on transition (backwards) with input from state
  void output_reverse(const int state, bvec &zero_output, bvec &one_output);
  //! Calculate delta metrics for 0 and 1 input branches reaching state \ state
  void calc_metric_reverse(const int state, const vec &rx_codeword, double &zero_metric, double &one_metric);

  //! Returns the input that results in state, that is the MSB of state
  int get_input(const int state) { return (state >> (m-1)); }
  
  //! Number of generators
  int n;
  //! Constraint length
  int K;
  //! Memory of the encoder
  int m;
  //! Generator polynomials
  ivec gen_pol;
  //! Generator polynomials for the reverse code
  ivec gen_pol_rev;
  //! The current encoder state
  int encoder_state;
  //! The encoder start state
  int start_state;
  //! The decoder truncation length
  int trunc_length;
  //! The rate of the code
  double rate;
  //! Auxilary table used by the codec
  bvec xor_int_table;
};

// --------------- Some other functions that maybe should be moved -----------
/*!
  \relates Convolutional_Code
  \brief Reverses the bitrepresentation of in (of size length) and converts to an integer
*/
int reverse_int(int length, int in);

/*!
  \relates Convolutional_Code
  \brief Calculate the Hamming weight of the binary representation of in of size length
*/
int weight_int(int length, int in);

/*!
  \relates Convolutional_Code
  \brief Compare two distance spectra. Return 1 if v1 is less, 0 if v2 less, and -1 if equal.
*/
int compare_spectra(llvec v1, llvec v2);

/*!
  \relates Convolutional_Code
  \brief Compare two distance spectra using a weight profile.

  Return 1 if v1 is less, 0 if v2 less, and -1 if equal.
*/
int compare_spectra(ivec v1, ivec v2, vec weight_profile);
}

#endif // __convcode_h

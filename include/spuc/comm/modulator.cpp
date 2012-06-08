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
  \brief Implementation of modulator classes
  \author ?, Modified by Tony Kirke, Feb 1,2003

  1.16

  2002/12/19 23:56:48
*/

#include <float.h>

#include <converters.h>
#include <binary.h>
#include <matrix.h>
#include <specmat.h>
#include "modulator.h"
#include "commfunc.h"
#include <complex.h>
/*
using std::abs;
using std::real;
using std::imag;
using std::conj;
using std::polar;
using std::arg;
*/

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define M_1_DIV_SQRT2 0.70710678118654752440
#endif //DOXYGEN_SHOULD_SKIP_THIS
using namespace SPUC;

//------------- class: Modulator_1d ----------------
Modulator_1d::Modulator_1d(const vec &insymbols, const ivec &inbitmap)
{
  set(insymbols, inbitmap);
}

void Modulator_1d::set(const vec &insymbols, const ivec &inbitmap)
{
  // it_assert(insymbols.size() == inbitmap.size(), "Modulator_1d: number of symbols and bitmap does not match");
  M = inbitmap.size();
  k = round_i(log2(double(M)));
  symbols = insymbols;
  bitmap = inbitmap; 
}

vec Modulator_1d::modulate(const ivec &symbolnumbers)
{
  vec temp(symbolnumbers.size());
  for (int i=0; i<symbolnumbers.size(); i++)
    temp(i) = symbols(symbolnumbers(i));
  return temp;
}

vec Modulator_1d::modulate_bits(const bvec &bits)
{
  int no_symbols = floor_i(double(bits.length())/double(k)), i, pos, symb;
  vec output = zeros(no_symbols);

  for (i=0; i<no_symbols; i++) {
    pos = 0;
    symb = bin2dec(bits.mid(i*k,k));
    while (bitmap(pos)!=symb) { pos++; }
    output(i) = symbols(pos);
  }
  return output;
}

ivec Modulator_1d::demodulate(const vec &signal)
{
  int i, j;
  double dist, mindist;
  int closest;
  ivec output(signal.size());

  for (i=0; i<signal.size(); i++) {
    mindist = ABS(double(symbols(0)) - signal(i));
    closest = 0;
    for (j=1; j<M; j++) {
      dist = ABS(double(symbols(j)) - signal(i));
      if (dist<mindist) { mindist = dist; closest = j; }
    }
    output(i) = closest;
  }
  return output;
}

bvec Modulator_1d::demodulate_bits(const vec &signal)
{
  int i, j;
  double dist, mindist;
  int closest;
  bvec output(k*signal.size());

  for (i=0; i<signal.size(); i++) {
    mindist = ABS(double(symbols(0)) - signal(i));
    closest = 0;
    for (j=1; j<M; j++) {
      dist = ABS(double(symbols(j)) - signal(i));
      if (dist<mindist){ mindist = dist; closest = j; }
    }
    output.replace_mid(i*k,dec2bin(k,bitmap(closest)));
  }
  return output;
}

vec Modulator_1d::get_symbols(void) { return symbols; }

ivec Modulator_1d::get_bitmap(void) { return bitmap; }

//------------- class: MOD_BPSK ----------------

//vec MOD_BPSK::modulate(const svec &symbolnumbers) {return 1.0-2.0*to_vec(symbolnumbers);}

void MOD_BPSK::modulate_bits(const bvec &bits, vec &out)
{
    out.set_size(bits.size(), false);
    for (int i=0;i<bits.size();i++) { out(i) = bits(i) == 0 ? 1.0 : -1.0;}
}

// output is complex but symbols in real part only
void MOD_BPSK::modulate_bits(const bvec &bits, cvec &out)
{
    out.set_size(bits.size(), false);
    for (int i=0;i<bits.size();i++) { out(i) = bits(i) == 0 ? 1.0 : -1.0;}
}

vec MOD_BPSK::modulate_bits(const bvec &bits)
{
    vec temp(bits.size());
    modulate_bits(bits, temp);
    return temp;
}

// svec MOD_BPSK::demodulate(const vec &signal) {
//     bvec temp = demodulate_bits(signal);
//     return to_svec(temp);
// }

void MOD_BPSK::demodulate_bits(const vec &signal, bvec &out)
{
    out.set_size(signal.size(), false);
    for (int i=0; i<signal.length(); i++) { out(i) = (signal(i)>0) ? bin(0) : bin(1); }
}

bvec MOD_BPSK::demodulate_bits(const vec &signal)
{
    bvec temp(signal.size());
    demodulate_bits(signal, temp);
    return temp;
}

// Symbols are in real part. That is channel estimation is already applied to the signal
void MOD_BPSK::demodulate_bits(const cvec &signal, bvec &out)
{
    out.set_size(signal.size(), false);
    for (int i=0; i<signal.length(); i++) { out(i) = (real(signal(i))>0) ? bin(0) : bin(1); }
}

// Outputs log-likelihood ratio of log (Pr(bit = 0|rx_symbols)/Pr(bit = 1|rx_symbols))
void MOD_BPSK::demodulate_soft_bits(const vec &rx_symbols, double N0, vec &soft_bits)
{
  double factor = 4/N0;
    soft_bits.set_size(rx_symbols.size(), false);

    for (int i=0; i<rx_symbols.size(); i++) {
      soft_bits(i) = factor*rx_symbols(i);
    }
}

// Outputs log-likelihood ratio for fading channels
void MOD_BPSK::demodulate_soft_bits(const cvec &rx_symbols, const cvec &channel, double N0, vec &soft_bits)
{
  double factor = 4/N0;
  soft_bits.set_size(rx_symbols.size(), false);

  for (int i=0; i<rx_symbols.size(); i++) {
    soft_bits(i) = factor*real(rx_symbols(i)*conj(channel(i)));
  }
}
//------------- class: MOD_PAM ----------------

// vec MOD_PAM::modulate(const svec &symbolnumbers) {
//     vec temp(symbolnumbers.length());
//     for (int i=0; i<symbolnumbers.length(); i++)
// 	temp(i)=symbols(symbolnumbers(i));
//     return temp;
// }

// svec MOD_PAM::demodulate(const vec &signal) {
//     int i, itterations = signal.length();
//     svec output(itterations);
//     int temp;
//     for (i=0; i<itterations; i++) {
// 	temp = (int)round((M-1)-(signal(i)+(M-1))/2.0);
// 	if (temp<0) temp = 0;
// 	else if (temp>(M-1)) temp = (M-1);
// 	output(i) = short(temp);
//     }
//     return output;
// }

void MOD_PAM::modulate_bits(const bvec &bits, vec &out)
{
    int no_symbols = floor_i(double(bits.length())/double(k)), i, symb;
    out.set_size(no_symbols, false);

    for (i=0; i<no_symbols; i++) {
	symb = bin2dec(bits.mid(i*k,k));
	out(i) = symbols(bits2symbols(symb));
    }
}

vec MOD_PAM::modulate_bits(const bvec &bits)
{
    vec temp(bits.size());
    modulate_bits(bits, temp);
    return temp;
}

void MOD_PAM::demodulate_bits(const vec &signal, bvec &out)
{
    int i, est_symbol;
    out.set_size(k*signal.size(), false);

    for (i=0; i<signal.size(); i++) {
	est_symbol = round_i((M-1)-(signal(i)*scaling_factor+(M-1))/2.0);
	if (est_symbol<0) est_symbol = 0;
	else if (est_symbol>(M-1)) est_symbol = (M-1);
	out.replace_mid(i*k,bitmap.get_row(est_symbol));
    }
}

bvec MOD_PAM::demodulate_bits(const vec &signal)
{
    bvec temp(signal.size());
    demodulate_bits(signal, temp);
    return temp;
}

void MOD_PAM::set_M(int Mary)
{
  M = Mary;
  k = round_i(log2(M));

  // it_error_if(ABS(pow2i(k)-Mary)>0.0,"M-ary PAM: M is not a power of 2");

  symbols.set_size(M, false);
  bits2symbols.set_size(M, false);
  bitmap = graycode(k);
  average_energy = double(M*M-1)/3.0;
  scaling_factor = sqrt(average_energy);
  for (int i=0; i<M; i++) {
    symbols(i) = double((M-1)-i*2) / scaling_factor;
    bits2symbols(bin2dec(bitmap.get_row(i))) = i;
  }
}

//------------- class: Modulator_2d ----------------
Modulator_2d::Modulator_2d(const cvec &insymbols, const ivec &inbitmap)
{
  set(insymbols, inbitmap);
}

void Modulator_2d::set(const cvec &insymbols, const ivec &inbitmap)
{
  // it_assert(insymbols.size() == inbitmap.size(), "Modulator_2d: number of symbols and bitmap does not match");
  symbols = insymbols;
  bitmap = inbitmap; 
  M = bitmap.size();
  k = round_i(log2(double(M)));
  soft_bit_mapping_matrices_calculated = false;
}

cvec Modulator_2d::modulate(const ivec &symbolnumbers)
{
  cvec temp(symbolnumbers.length());
  for(int i=0;i<symbolnumbers.length();i++)
    temp(i)=symbols(symbolnumbers(i));
  return temp;
}

cvec Modulator_2d::modulate_bits(const bvec &bits)
{
  int no_symbols = floor_i(double(bits.length())/double(k)), i, pos, symb;
  cvec output = zeros_c(no_symbols);

  for (i=0; i<no_symbols; i++) {
    pos = 0;
    symb = bin2dec(bits.mid(i*k,k));
    while (bitmap(pos)!=symb) { pos++; }
    output(i) = symbols(pos);
  }
  return output;
}

ivec Modulator_2d::demodulate(const cvec &signal)
{
  int i, j;
  double dist, mindist;
  int closest;
  ivec output(signal.size());

  for (i=0; i<signal.size(); i++) {
    mindist = magsq( symbols(0) - signal(i) );
    closest = 0;
    for (j=1; j<M; j++) {
      dist = magsq( symbols(j) - signal(i) );
      if (dist<mindist){ mindist = dist; closest = j; }
    }
    output(i) = closest;
  }
  return output;
}

bvec Modulator_2d::demodulate_bits(const cvec &signal)
{
  int i, j;
  double dist, mindist;
  int closest;
  bvec output(k*signal.size());

  for (i=0; i<signal.size(); i++) {
    mindist = magsq( symbols(0) - signal(i) );
    closest = 0;
    for (j=1; j<M; j++) {
      dist = magsq( symbols(j) - signal(i) );
      if (dist<mindist){ mindist = dist; closest = j; }
    }
    output.replace_mid(i*k,dec2bin(k,bitmap(closest))); 
  }
  return output;
}

cvec Modulator_2d::get_symbols() { return symbols; }

ivec Modulator_2d::get_bitmap(void) { return bitmap; }

void Modulator_2d::demodulate_soft_bits(const cvec &rx_symbols, double N0, vec &soft_bits)
{
  //Definitions of local variables
  long no_symbols = rx_symbols.length();
  long l, i, j;
  double p_z_s0, p_z_s1;
  complex<double> z, s0, s1;
  vec soft_word(k), P0(k), P1(k);

  //Check if the soft bit mapping matrices S0 and S1 needs to be calculated
  if (soft_bit_mapping_matrices_calculated==false) {
    calculate_softbit_matricies(bitmap);
  }

  //Allocate storage space for the result vector:
  soft_bits.set_size(k*no_symbols,false);

  //For each symbol l do:
  for (l=0; l<no_symbols; l++) {

    P0.clear();
    P1.clear();
    z = rx_symbols(l);

    //For each bit position i do:
    for (i=0; i<k; i++) {

      for (j=0; j<(M/2); j++) {
	s0 = symbols( S0(i,j) );
	s1 = symbols( S1(i,j) );
	p_z_s0 = (1.0/(PI*N0)) * ::exp(-(pow(magsq(z-s0),2.0))/N0);
	p_z_s1 = (1.0/(PI*N0)) * ::exp(-(pow(magsq(z-s1),2.0))/N0);
	P0(i) += p_z_s0;
	P1(i) += p_z_s1;
      }
      //The soft bits for the l-th received symbol:
      soft_word(i) = log( P0(i) / P1(i) );
    }
    //Put the results in the result vector:
    soft_bits.replace_mid(l*k,soft_word);
  }

}

void Modulator_2d::demodulate_soft_bits(const cvec &rx_symbols, const cvec &chan, double N0, vec &soft_bits)
{
  //Definitions of local variables
  long no_symbols = rx_symbols.length();
  long l, i, j;
  double p_z_s0, p_z_s1, a2;
  complex<double> z, s0, s1;
  vec soft_word(k), P0(k), P1(k);

  //Check if the soft bit mapping matrices S0 and S1 needs to be calculated
  if (soft_bit_mapping_matrices_calculated==false) {
    calculate_softbit_matricies(bitmap);
  }

  //Allocate storage space for the result vector:
  soft_bits.set_size(k*no_symbols,false);

  //For each symbol do:
  for (l=0; l<no_symbols; l++) {

    P0.clear();
    P1.clear();
    z = rx_symbols(l);
    a2 = pow(magsq(chan(l)),2.0);

    //For each bit position i do:
    for (i=0; i<k; i++) {

      for (j=0; j<(M/2); j++) {
	s0 = symbols( S0(i,j) );
	s1 = symbols( S1(i,j) );
	p_z_s0 = (1.0/(PI*N0*a2)) * ::exp(-(pow(magsq(z-a2*s0),2.0))/(N0*a2));
	p_z_s1 = (1.0/(PI*N0*a2)) * ::exp(-(pow(magsq(z-a2*s1),2.0))/(N0*a2));
	P0(i) += p_z_s0;
	P1(i) += p_z_s1;
      }
      //The soft bits for the l-th received symbol:
      soft_word(i) = log( P0(i) / P1(i) );
    }
    //Put the results in the result vector:
    soft_bits.replace_mid(l*k,soft_word);
  }

}

void Modulator_2d::demodulate_soft_bits_approx(const cvec &rx_symbols, double N0, vec &soft_bits)
{
  //Definitions of local variables
  long no_symbols = rx_symbols.length();
  long l, i, j;
  double d_0, d_1, Kf;
  vec d(M);
  Kf = (1.0/N0);

  //Check if the soft bit mapping matrices S0 and S1 needs to be calculated
  if (soft_bit_mapping_matrices_calculated==false) {
    calculate_softbit_matricies(bitmap);
  }

  //Allocate storage space for the result vector:
  soft_bits.set_size(k*no_symbols,false);

  //for each symbol l do:
  for (l=0; l<no_symbols; l++) {

    //Calculate all distances:
    for (i=0; i<M; i++) { d(i) = magsq( rx_symbols(l) - symbols(i) ); }

    //For each of the k bits do:
    for (i=0; i<k; i++) {

      //Find the closest 0-point and the closest 1-point:
      d_0 = d( S0(i,0) );
      d_1 = d( S1(i,0) );
      for (j=1; j<(M/2); j++) {
	if ( d( S0(i,j) ) < d_0) { d_0 = d( S0(i,j) ); }
	if ( d( S1(i,j) ) < d_1) { d_1 = d( S1(i,j) ); }
      }

      //calculate the approximative metric:
      soft_bits(l*k+i) = Kf * ( pow(d_1,2.0) - pow(d_0,2.0) );

    }

  }

}

void Modulator_2d::demodulate_soft_bits_approx(const cvec &rx_symbols, const cvec &chan, double N0, vec &soft_bits)
{

  //Definitions of local variables
  long no_symbols = rx_symbols.length();
  long l, i, j;
  double d_0, d_1, Kf, c2;
  vec d(M);

  //Check if the soft bit mapping matrices S0 and S1 needs to be calculated
  if (soft_bit_mapping_matrices_calculated==false) {
    calculate_softbit_matricies(bitmap);
  }

  //Allocate storage space for the result vector:
  soft_bits.set_size(k*no_symbols,false);

  //for each symbol l do:
  for (l=0; l<no_symbols; l++) {

    c2 = pow(magsq(chan(l)),2.0); 
    Kf = 1.0 / ( c2 * N0 );   

    //Calculate all distances:
    for (i=0; i<M; i++) { d(i) = magsq( rx_symbols(l) - c2*symbols(i) ); }

    //For each of the k bits do:
    for (i=0; i<k; i++) {

      //Find the closest 0-point and the closest 1-point:
      d_0 = d( S0(i,0) );
      d_1 = d( S1(i,0) );
      for (j=1; j<(M/2); j++) {
	if ( d( S0(i,j) ) < d_0) { d_0 = d( S0(i,j) ); }
	if ( d( S1(i,j) ) < d_1) { d_1 = d( S1(i,j) ); }
      }

      //calculate the approximative metric:
      soft_bits(l*k+i) = Kf * ( pow(d_1,2.0) - pow(d_0,2.0) );

    }

  }

}

void Modulator_2d::calculate_softbit_matricies(ivec inbitmap)
{
  //Definitions of local variables
  int kk, m, count0, count1;
  bvec bits(k);

  //Allocate storage space for the result matricies:
  S0.set_size(k,M/2,false);
  S1.set_size(k,M/2,false);

  for (kk=0; kk<k; kk++) {
    count0 = 0; 
    count1 = 0;
    for (m=0; m<M; m++) {
      bits = dec2bin(k,inbitmap(m));
      if (bits(kk)==bin(0)) {
	S0(kk,count0) = m;
	count0++; 
      } else {
	S1(kk,count1) = m;
	count1++;
      }
    }
  }

}


//------------- class: MOD_QPSK ----------------

void MOD_QPSK::modulate_bits(const bvec &bits, cvec &out)
{
  int i, no_symbols = (int)floor(double(bits.size())/2);
  out.set_size(no_symbols, false);
  double real_part, imag_part;
  for (i=0; i<no_symbols; i++) {
    real_part = (bits(2*i)==0) ? M_1_DIV_SQRT2 : -M_1_DIV_SQRT2;
    imag_part = (bits(2*i+1)==0) ? M_1_DIV_SQRT2 : -M_1_DIV_SQRT2;
    out(i) = complex<double>(real_part, imag_part);
  }
}

cvec MOD_QPSK::modulate_bits(const bvec &bits)
{
  cvec out;
  modulate_bits(bits, out);
  return out;
}

// cvec MOD_QPSK::modulate(const svec &symbolnumbers) {
//     cvec temp(symbolnumbers.length());
//     for(int i=0;i<symbolnumbers.length();i++)
// 	temp(i)=symbols(symbolnumbers(i));
//     return temp;
// }

// svec MOD_QPSK::demodulate(const cvec &signal) {
//     int i, itterations = signal.length();
//     svec output(itterations);
//     double ang, temp;
//     for (i=0; i<itterations; i++) {
// 	ang = arg(signal(i));
// 	temp = ( (ang < 0) ? (2*PI+ang) : ang );
// 	output(i) = short(floor(temp*2/PI)) % 4;
//     }
//     return output;
// }

void MOD_QPSK::demodulate_bits(const cvec &signal, bvec &out)
{
    int i, no_symbols = signal.size();
    out.set_size(2*no_symbols, false);
    for (i=0; i<no_symbols; i++) {
	out(2*i)   = ((real(signal(i)) > 0) ? 0 : 1);
	out(2*i+1) = ((imag(signal(i)) > 0) ? 0 : 1);
    }
}

bvec MOD_QPSK::demodulate_bits(const cvec &signal)
{
  bvec out;
  demodulate_bits(signal, out);
  return out;
}


//Outputs log-likelihood ratio of log (Pr(bit = 0|rx_symbols)/Pr(bit = 1|rx_symbols))
void MOD_QPSK::demodulate_soft_bits(const cvec &rx_symbols, double N0, vec &soft_bits)
{
    soft_bits.set_size(2*rx_symbols.size(), false);
    double factor = 2*sqrt(2.0)/N0;
    for (int i=0; i<rx_symbols.size(); i++) {
      soft_bits(2*i) = real(rx_symbols(i))*factor;
      soft_bits(2*i+1) = imag(rx_symbols(i))*factor;
    }
}

//Outputs log-likelihood ratio for fading channels
void MOD_QPSK::demodulate_soft_bits(const cvec &rx_symbols, const cvec &channel, double N0, vec &soft_bits)
{
    soft_bits.set_size(2*rx_symbols.size(), false);
    complex<double> temp;
    double factor = 2*sqrt(2.0)/N0;
    
    for (int i=0; i<rx_symbols.size(); i++) {
      temp = rx_symbols(i)*conj(channel(i));
      soft_bits(2*i) = real(temp)*factor;
      soft_bits(2*i+1) = imag(temp)*factor;
    }
}


//------------- class: MOD_PSK ----------------

// cvec MOD_PSK::modulate(const svec &symbolnumbers) {
//     cvec temp(symbolnumbers.length());
//     for(int i=0;i<symbolnumbers.length();i++)
// 	temp(i)=symbols(symbolnumbers(i));
//     return temp;
// }

// svec MOD_PSK::demodulate(const cvec &signal) {
//     int i, itterations = signal.length();
//     svec output(itterations);
//     double ang, temp;
//     for (i=0; i<itterations; i++) {
// 	ang = arg(signal(i));
// 	temp = ( (ang < 0) ? (2*PI+ang) : ang );
// 	output(i) = short(round(temp*(M/2)/PI)) % M;
//     }
//     return output;
// }

void MOD_PSK::modulate_bits(const bvec &bits, cvec &out)
{
    int no_symbols = floor_i(double(bits.length())/double(k)), i, symb;
    out.set_size(no_symbols, false);

    for (i=0; i<no_symbols; i++) {
	symb = bin2dec(bits.mid(i*k,k));
	out(i) = symbols(bits2symbols(symb));
    }
}

cvec MOD_PSK::modulate_bits(const bvec &bits)
{
    cvec temp(bits.size());
    modulate_bits(bits, temp);
    return temp;
}

void MOD_PSK::demodulate_bits(const cvec &signal, bvec &out)
{
    int i, est_symbol;
    out.set_size(k*signal.size(), false);
    double ang, temp;

    for (i=0; i<signal.size(); i++) {
	ang = arg(signal(i));
	temp = ( (ang < 0) ? (2*PI+ang) : ang );
	est_symbol = round_i(temp*(M/2)/PI) % M;
	out.replace_mid(i*k, bitmap.get_row(est_symbol));
    }
}

bvec MOD_PSK::demodulate_bits(const cvec &signal)
{
    bvec temp;
    demodulate_bits(signal, temp);
    return temp;
}

void MOD_PSK::demodulate_soft_bits(const cvec &rx_symbols, double N0, vec &soft_bits)
{
  int l, i, j;
  double P0, P1, d0, d1;
  double d0min, d0min2, d1min, d1min2;
  double treshhold = -log(DBL_EPSILON); // To be sure that any precision is left in the calculatation
  double inv_N0 = 1/N0;

  soft_bits.set_size(k*rx_symbols.size(), false);

  for (l=0; l<rx_symbols.size(); l++) {
    for (i=0; i<k; i++) {
      P0 = P1 = 0;
      d0min = d0min2 = d1min = d1min2 = 1e100;
      for (j=0; j<(M/2); j++) {
	d0 = (pow(magsq(rx_symbols(l)-symbols(S0(i,j))),2.0))*inv_N0;
	d1 = (pow(magsq(rx_symbols(l)-symbols(S1(i,j))),2.0))*inv_N0;
 	if (d0 < d0min) { d0min2 = d0min; d0min = d0; }
 	if (d1 < d1min) { d1min2 = d1min; d1min = d1; }
	P0 += ::exp(-d0);
	P1 += ::exp(-d1); 
      }
      if ( (d0min2-d0min) > treshhold && (d1min2-d1min) > treshhold )
	soft_bits(l*k+i) = -d0min + d1min;
      else
	soft_bits(l*k+i) = log(P0/P1);
    }
  }
}

void MOD_PSK::demodulate_soft_bits(const cvec &rx_symbols, const cvec &channel, double N0, vec &soft_bits)
{
  int l, i, j;
  double P0, P1, d0, d1;
  double d0min, d0min2, d1min, d1min2;
  double treshhold = -log(DBL_EPSILON); // To be sure that any precision is left in the calculatation
  double inv_N0 = 1/N0;

  soft_bits.set_size(k*rx_symbols.size(), false);

  for (l=0; l<rx_symbols.size(); l++) {
    for (i=0; i<k; i++) {
      P0 = P1 = 0;
      d0min = d0min2 = d1min = d1min2 = 1e100;
      for (j=0; j<(M/2); j++) {
	d0 = (pow(magsq(rx_symbols(l)-channel(l)*symbols(S0(i,j))),2.0))*inv_N0;
	d1 = (pow(magsq(rx_symbols(l)-channel(l)*symbols(S1(i,j))),2.0))*inv_N0;
 	if (d0 < d0min) { d0min2 = d0min; d0min = d0; }
 	if (d1 < d1min) { d1min2 = d1min; d1min = d1; }
	P0 += ::exp(-d0);
	P1 += ::exp(-d1);  
      }
      if ( (d0min2-d0min) > treshhold && (d1min2-d1min) > treshhold )
	soft_bits(l*k+i) = -d0min + d1min;
      else
	soft_bits(l*k+i) = log(P0/P1);
    }
  }
}

void MOD_PSK::set_M(int Mary)
{
  k = round_i(log2(Mary));
  M = Mary;
  // it_error_if(magsq(pow2i(k)-Mary)>0.0,"M-ary PSK: M is not a power of 2");
  symbols.set_size(M, false);
  bits2symbols.set_size(M, false);
  bitmap = graycode(k);

  int i;
  double delta = 2.0*PI/M, epsilon = delta/10000.0;
  complex<double> symb;
  for (i=0; i<M; i++) {
    symb = complex<double>(polar(1.0,delta*i));
    if (ABS(real(symb)) < epsilon) { symbols(i) = complex<double>(0.0,imag(symb)); }
    else if (ABS(imag(symb)) < epsilon) { symbols(i) = complex<double>(real(symb),0.0); }
    else { symbols(i) = symb; }

    bits2symbols(bin2dec(bitmap.get_row(i))) = i;
  }

  //Calculate the soft bit mapping matrices S0 and S1
  S0.set_size(k,M/2,false);
  S1.set_size(k,M/2,false);
  int count0, count1, kk, m;
  bvec bits;
  
  for (kk=0; kk<k; kk++) {
    count0 = 0; 
    count1 = 0;
    for (m=0; m<M; m++) {
      bits = bitmap.get_row(m);
      if (bits(kk)==bin(0)) {
	S0(kk,count0) = m;
	count0++; 
      } else {
	S1(kk,count1) = m;
	count1++;
      }
    }
  }
}

//------------- class: MOD_QAM ----------------

// cvec MOD_QAM::modulate(const svec &symbolnumbers) {
//     cvec temp(symbolnumbers.length());
//     for(int i=0;i<symbolnumbers.length();i++)
// 	temp(i)=symbols(symbolnumbers(i));
//     return temp;
// }

// ivec MOD_QAM::demodulate(const cvec &signal) {
//     int i, itterations = signal.length();
//     ivec output(itterations);
//     int temp_real, temp_imag;
//     short L = (short)round(sqrt(M));
//     for (i=0; i<itterations; i++) {
// 	temp_real = (int)round((L-1)-(real(signal(i))+(L-1))/2.0);
// 	temp_imag = (int)round((L-1)-(imag(signal(i))+(L-1))/2.0);
// 	if (temp_real<0) temp_real = 0; else if (temp_real>(L-1)) temp_real = (L-1);
// 	if (temp_imag<0) temp_imag = 0; else if (temp_imag>(L-1)) temp_imag = (L-1);
// 	output(i) = short(temp_imag*L + temp_real);
//     }
//     return output;
// }

void MOD_QAM::modulate_bits(const bvec &bits, cvec &out)
{
    int no_symbols = floor_i(double(bits.length())/double(k)), i, symb;
    out.set_size(no_symbols, false);

    for (i=0; i<no_symbols; i++) {
	symb = bin2dec(bits.mid(i*k,k));
	out(i) = symbols(bits2symbols(symb));
    }
}

cvec MOD_QAM::modulate_bits(const bvec &bits)
{
    cvec temp(bits.size());
    modulate_bits(bits, temp);
    return temp;
}

void MOD_QAM::demodulate_bits(const cvec &signal, bvec &out)
{
    int i;
    out.set_size(k*signal.size(), false);

    int temp_real, temp_imag;

    for (i=0; i<signal.size(); i++) {
	temp_real = round_i((L-1)-(real(signal(i)*scaling_factor)+(L-1))/2.0);
	temp_imag = round_i((L-1)-(imag(signal(i)*scaling_factor)+(L-1))/2.0);
	if (temp_real<0) temp_real = 0; else if (temp_real>(L-1)) temp_real = (L-1);
	if (temp_imag<0) temp_imag = 0; else if (temp_imag>(L-1)) temp_imag = (L-1);
	out.replace_mid( k*i, bitmap.get_row(temp_imag*L + temp_real) );
    }
}

bvec MOD_QAM::demodulate_bits(const cvec &signal)
{
    bvec temp;
    demodulate_bits(signal, temp);
    return temp;
}

void MOD_QAM::demodulate_soft_bits(const cvec &rx_symbols, double N0, vec &soft_bits)
{
  int l, i, j;
  double P0, P1, d0, d1;
  double d0min, d0min2, d1min, d1min2;
  double treshhold = -log(DBL_EPSILON); // To be sure that any precision is left in the calculatation
  double inv_N0 = 1/N0;

  soft_bits.set_size(k*rx_symbols.size(), false);

  for (l=0; l<rx_symbols.size(); l++) {
    for (i=0; i<k; i++) {
      P0 = P1 = 0;
      d0min = d0min2 = d1min = d1min2 = 1e100;
      for (j=0; j<(M/2); j++) {
	d0 = (pow(magsq(rx_symbols(l)-symbols(S0(i,j))),2.0))*inv_N0;
	d1 = (pow(magsq(rx_symbols(l)-symbols(S1(i,j))),2.0))*inv_N0;
 	if (d0 < d0min) { d0min2 = d0min; d0min = d0; }
 	if (d1 < d1min) { d1min2 = d1min; d1min = d1; }
	P0 += ::exp(-d0);
	P1 += ::exp(-d1);  
      }
      if ( (d0min2-d0min) > treshhold && (d1min2-d1min) > treshhold )
	soft_bits(l*k+i) = -d0min + d1min;
      else
	soft_bits(l*k+i) = log(P0/P1);
    }
  }
}

void MOD_QAM::demodulate_soft_bits(const cvec &rx_symbols, const cvec &channel, double N0, vec &soft_bits)
{
  int l, i, j;
  double P0, P1, d0, d1;
  double d0min, d0min2, d1min, d1min2;
  double treshhold = -log(DBL_EPSILON); // To be sure that any precision is left in the calculatation
  double inv_N0 = 1/N0;

  soft_bits.set_size(k*rx_symbols.size(), false);

  for (l=0; l<rx_symbols.size(); l++) {
    for (i=0; i<k; i++) {
      P0 = P1 = 0;
      d0min = d0min2 = d1min = d1min2 = 1e100;
      for (j=0; j<(M/2); j++) {
	d0 = (pow(magsq(rx_symbols(l)-channel(l)*symbols(S0(i,j))),2.0))*inv_N0;
	d1 = (pow(magsq(rx_symbols(l)-channel(l)*symbols(S1(i,j))),2.0))*inv_N0;
 	if (d0 < d0min) { d0min2 = d0min; d0min = d0; }
 	if (d1 < d1min) { d1min2 = d1min; d1min = d1; }
	P0 += ::exp(-d0);
	P1 += ::exp(-d1);  
      }
      if ( (d0min2-d0min) > treshhold && (d1min2-d1min) > treshhold )
	soft_bits(l*k+i) = -d0min + d1min;
      else
	soft_bits(l*k+i) = log(P0/P1);
    }
  }
}

void MOD_QAM::set_M(int Mary)
{
  k = round_i(log2(Mary));
  M = Mary;
  L = round_i(sqrt((double)M));
  // it_error_if(ABS(pow2i(k)-Mary)>0.01,"M-ary QAM: M is not a power of 2");

  int i, j, kk, m, count0, count1;
  bvec bits;

  symbols.set_size(M, false);
  bitmap.set_size(M, k, false);
  bits2symbols.set_size(M, false);
  bmat gray_code=graycode(round_i(log2(L)));
  average_energy = double(M-1)*2.0/3.0;
  scaling_factor = sqrt(average_energy);

  for (i=0; i<L; i++) {
    for (j=0; j<L; j++) {
      symbols(i*L+j) = complex<double>( ((L-1)-j*2)/scaling_factor ,((L-1)-i*2)/scaling_factor);
      bitmap.set_row( i*L+j, concat(gray_code.get_row(i), gray_code.get_row(j)) );
      bits2symbols( bin2dec(bitmap.get_row(i*L+j)) ) = i*L+j;
    }
  }

  //Calculate the soft bit mapping matrices S0 and S1
  S0.set_size(k,M/2,false);
  S1.set_size(k,M/2,false);
  
  for (kk=0; kk<k; kk++) {
    count0 = 0; 
    count1 = 0;
    for (m=0; m<M; m++) {
      bits = bitmap.get_row(m);
      if (bits(kk)==bin(0)) {
	S0(kk,count0) = m;
	count0++; 
      } else {
	S1(kk,count1) = m;
	count1++;
      }
    }
  }
}

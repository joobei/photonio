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
// 
//  SPUC - Signal processing using C++ - A DSP library
/*! 
  \file 
  \brief Definition of classes for random number generators
  \author Tony Ottosson, modified by Tony Kirke Feb 1, 2003

  The Random_Generator class is built on the MersenneTwister MTRand class code by
  Richard J. Wagner. See http://www-personal.engin.umich.edu/~wagnerr/MersenneTwister.html
  for details. The code is distributed under the GPL license.

  The Mersenne Twister is an algorithm for generating random numbers.  It
  was designed with consideration of the flaws in various other generators.
  The period, 2^19937-1, and the order of equidistribution, 623 dimensions,
  are far greater.  The generator is also fast; it avoids multiplication and
  division, and it benefits from caches and pipelines.  For more information
  see the inventors' web page at http://www.math.keio.ac.jp/~matumoto/emt.html
  
  Reference:
  M. Matsumoto and T. Nishimura, "Mersenne Twister: A 623-Dimensionally
  Equidistributed Uniform Pseudo-Random Number Generator", ACM Transactions on
  Modeling and Computer Simulation, Vol. 8, No. 1, January 1998, pp 3-30.

  Copyright (C) 2001  Richard J. Wagner
 
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
 
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  The original code included the following notice:
  
  Copyright (C) 1997, 1999 Makoto Matsumoto and Takuji Nishimura.
  When you use this, send an email to: matumoto@math.keio.ac.jp
  with an appropriate reference to your work.

  It would be nice to CC: rjwagner@writeme.com and Cokus@math.washington.edu
  when you write.

  1.19

  2003/01/04 00:21:54
*/

#ifndef __random_h
#define __random_h
#include <math.h>
#include <cstdlib>
#include <complex.h>
//#include "spucassert.h"
#include <binary.h>
namespace SPUC {
#ifndef DOXYGEN_SHOULD_SKIP_THIS

typedef complex<double> double_complex;
/*! 
  \defgroup randgen Random Number Generation
*/

/*! 
  \brief Base class for random (stochastic) sources.
  \ingroup randgen
  
  Uses Marsienne Twister random generator.
*/
class Random_Generator {
 public:
  //! Construct a new Random_Generator.    
  Random_Generator();
  //! Set the seed to a semi-random value (based on time and process id).
  void randomize();  
  //! Reset the source.  The same sequance will be generated as the last time.
  void reset();
  //! Reset the source after setting the seed to seed.
  void reset(unsigned int seed);
  //! Return a uniformly distributed [0,2^32-1] integer.
  unsigned int random_int()
    {
      if( left == 0 ) reload();
      --left;
		
      register unsigned int s1;
      s1 = *pNext++;
      s1 ^= (s1 >> 11);
      s1 ^= (s1 <<  7) & 0x9d2c5680U;
      s1 ^= (s1 << 15) & 0xefc60000U;
      return ( s1 ^ (s1 >> 18) );
    }
  //! Return a uniformly distributed (0,1) value. [2^¯33,1-2^-33] in steps of 2^-32.
  double random_01() { return ( (double(random_int()) + 0.5) * 2.3283064365386963e-10 ); } //*2^-32+2^-33
  //! Return a uniformly distributed [0,1] value.
  double random_01_closed() { return ( double(random_int()) * 2.3283064370807974e-10 ); } // * 1/(2^32-1)
 protected:
 private:
  //!
  unsigned long lastSeed;
  //!
  static const unsigned int MMAXINT;   // largest value from randInt()
  //!
  static const unsigned int MAGIC;    // magic constant
  
  //!
  static unsigned int state[624];     // internal state
  //!
  static unsigned int *pNext;         // next value to get from state
  //!
  static int left;                    // number of values left before reload needed
  //!
  void set_seed( unsigned int oneSeed );
  //!
  void set_seed( unsigned int *const bigSeed );
  //!
  void reload()
    {
      // Generate N new values in state
      // Made clearer and faster by Matthew Bellew (matthew.bellew@home.com)
      register unsigned int *p = state;
      register int i;
      for( i = 624 - 397; i--; ++p )
	*p = twist( p[397], p[0], p[1] );
      for( i = 397; --i; ++p )
	*p = twist( p[397-624], p[0], p[1] );
      *p = twist( p[397-624], p[0], state[0] );
      
      left = 624, pNext = state;
    }
  //!
  unsigned int hiBit( const unsigned int& u ) const { return u & 0x80000000U; }
  //!
  unsigned int loBit( const unsigned int& u ) const { return u & 0x00000001U; }
  //!
  unsigned int loBits( const unsigned int& u ) const { return u & 0x7fffffffU; }
  //!
  unsigned int mixBits( const unsigned int& u, const unsigned int& v ) const
    { return hiBit(u) | loBits(v); }
  //!
  unsigned int twist( const unsigned int& m, const unsigned int& s0, const unsigned int& s1 ) const
    { return m ^ (mixBits(s0,s1)>>1) ^ (loBit(s1) ? MAGIC : 0U); }
  //  static unsigned int hash( time_t t, clock_t c );
};

//! \addtogroup randgen
//!@{

//! Set the seed of the Global Random Number Generator
void RNG_reset(unsigned long seed);
//! Set the seed of the Global Random Number Generator to the same as last reset/init
void RNG_reset();
//! Set a random seed for the Global Random Number Generator.
void RNG_randomize();
//!@}

/*! 
  \brief Bernoulli distribution
  \ingroup randgen
*/
class Bernoulli_RNG {
 public:
    //! Binary source with probability prob for a 1
    Bernoulli_RNG(double prob) { setup(prob); }
    //! Binary source with probability prob for a 1
    Bernoulli_RNG() { p=0.5; }
    //! set the probability
    void setup(double prob)
      {
//	spuc_assert(prob>=0.0 && prob<=1.0, "The bernoulli source probability must be between 0 and 1");
	p = prob;
      }
    //! return the probability
    double get_setup() const { return p; }
    //! Get one sample.
    bin operator()() { return sample(); }
    //! Get a sample
    bin sample() { return bin( RNG.random_01() < p ? 1 : 0 ); }
protected:
private:
    //!
    double p;
    //!
    Random_Generator RNG;
};

/*! 
  \brief Integer uniform distribution
  \ingroup randgen
  
  Example: Generation of random uniformly distributed integers in the interval [0,10].
  #include "sigproc.h"
  
  int main() {
  
  I_Uniform_RNG gen(0, 10);
  
  cout << gen() << endl; // prints a random integer
  cout << gen(10) << endl; // prints 10 random integers
  }
*/
class I_Uniform_RNG {
 public:
  //! constructor. Sets min and max values.
  I_Uniform_RNG(int min=0, int max=1);
  //! set min and max values
  void setup(int min, int max);
  //! get the parameters
  void get_setup(int &min, int &max) const;
  //! Get one sample.
  int operator()() { return sample(); }
  //! Return a single value from this random generator
  int sample() { return ( (int)floor(RNG.random_01() * (hi - lo + 1)) + lo ); }
 protected:
 private:
  //!
  int lo;
  //!
  int hi;
  //!
  Random_Generator RNG;
};

/*!
  \brief Uniform distribution
  \ingroup randgen
*/
class Uniform_RNG {
 public:
  //! Constructor. Set min, max and seed.
  Uniform_RNG(double min=0, double max=1.0);
  //! set min and max
  void setup(double min, double max);
  //! get parameters
  void get_setup(double &min, double &max) const;
  //! Get one sample.
  double operator()() { return ( sample()* (hi_bound - lo_bound) + lo_bound ); }
  //! Get a Uniformly distributed (0,1) sample 
  double sample() {  return RNG.random_01(); }
 protected:
 private:
  //!
  double lo_bound, hi_bound;
  //!
  Random_Generator RNG;
};

/*!
  \brief Exponential distribution
  \ingroup randgen
*/
class Exponential_RNG {
public:
    //! constructor. Set lambda.
    Exponential_RNG(double lambda=1.0);
    //! Set lambda
    void setup(double lambda) { l=lambda; }
    //! get lambda
    double get_setup() const;
    //! Get one sample.
    double operator()() { return sample(); }
protected:
private:
    //!
    double sample() {  return ( -log(RNG.random_01()) / l ); }
    //!
    double l;
    //!
    Random_Generator RNG;
};

/*!
  \brief Normal distribution
  \ingroup randgen
*/
class Normal_RNG {
 public:
  //! Constructor. Set mean and variance.
  Normal_RNG(double meanval, double variance) { setup(meanval, variance); };
  //! Constructor. Set mean and variance.
  Normal_RNG() { mean = 0.0; sigma = 1.0; odd = true; }
  //! Set mean, and variance
  void setup(double meanval, double variance)
    { mean = meanval; sigma = sqrt(variance); odd = true; }
  //! Get mean and variance
  void get_setup(double &meanval, double &variance) const;
  //! Get one sample.
  double operator()() { return (sigma*sample()+mean); }
  //! Get a Normal distributed (0,1) sample
  double sample()
    {
      double a,b;

      if (odd) {
	a = TWOPI * RNG.random_01();
	b = sqrt(-2.0 * log(RNG.random_01()));
	s1 = b * cos(a);
	s2 = b * sin(a);
	odd = !odd;
	return s1;
      } else {
	odd = !odd;
	return s2;
      }
    }
 protected:
 private:
  //!
  double mean, sigma, s1, s2;
  //!
  bool odd;
  //!
  Random_Generator RNG;
};

/*! 
  \brief Laplacian distribution
  \ingroup randgen
*/
class Laplace_RNG {
 public:
  //! Constructor. Set mean and variance.
  Laplace_RNG(double meanval=0.0, double variance=1.0);
  //! Set mean and variance
  void setup(double meanval, double variance);
  //! Get mean and variance
  void get_setup(double &meanval, double &variance) const;
  //! Get one sample.
  double operator()() { return sample(); }
  //! Returns a single sample
  double sample()
  {
    u=RNG.random_01();
    if(u<0.5)
      l=log(2.0*u);
    else
      l=-log(2.0*(1-u));
      
    l *= sqrt(var/2.0);
    l += mean;
      
    return l;
  }
 protected:
 private:
  //!
  double mean, var, u, l;
  //!
  Random_Generator RNG;
};

/*!
  \brief A Complex Normal Source
  \ingroup randgen
*/
class Complex_Normal_RNG {
 public:
  //! Constructor. Set mean and variance.
  Complex_Normal_RNG(double_complex mean, double variance) { setup(mean, variance); }
  //! Constructor. Set mean and variance.
  Complex_Normal_RNG() { m = 0.0; sigma=1.0; }
  //! Set mean and variance
  void setup(double_complex mean, double variance) { m = mean; sigma = sqrt(variance); }
  //! Get mean and variance
  void get_setup(double_complex &mean, double &variance) { mean = m; variance = sigma*sigma; }
  //! Get one sample.
  double_complex operator()() { return sigma*sample()+m; }
  //! Get a Complex Normal (0,1) distributed sample
  double_complex sample()
    {
      // Make a const of 2.0*M_PI
      double a = TWOPI*RNG.random_01(), b = sqrt(-log(RNG.random_01()));
      return double_complex(b * cos(a), b * sin(a));
    }
 protected:
 private:
  //!
  double_complex m;
  //!
  double sigma;
  //!
  Random_Generator RNG;
};

/*!
  \brief Filtered normal distribution
  \ingroup randgen
*/
class AR1_Normal_RNG {
 public:
  //! Constructor. Set mean, variance, and correlation.
  AR1_Normal_RNG(double meanval=0.0, double variance=1.0, double rho=0.0);
  //! Set mean, variance, and correlation
  void setup(double meanval, double variance, double rho);
  //! Get mean, variance and correlation
  void get_setup(double &meanval, double &variance, double &rho) const;
  //! Set memory contents to zero
  void reset();
  //! Get a single random sample
  double operator()() { return sample(); }

 protected:
 private: 
  //!
  double sample();
  //!
  double my_mean, mem, r, factr, mean, var, r1, r2;
  //!
  bool odd;
  //!
  Random_Generator RNG;
};

/*!
  \brief Gauss_RNG is the same as Normal Source
  \ingroup randgen
*/
typedef Normal_RNG Gauss_RNG;

/*! 
  \brief AR1_Gauss_RNG is the same as AR1_Normal_RNG
  \ingroup randgen
*/
typedef AR1_Normal_RNG AR1_Gauss_RNG;

/*!
  \brief Weibull distribution
  \ingroup randgen
*/
class Weibull_RNG {
public:
    //! Constructor. Set lambda and beta.
    Weibull_RNG(double lambda=1.0, double beta=1.0);

    //! Set lambda, and beta
    void setup(double lambda, double beta);
    //! Get lambda and beta
    void get_setup(double &lambda, double &beta) { lambda=l; beta=b; }
    //! Get one sample.
    double operator()() { return sample(); }
protected:
private:
    //!
    double sample();
    //!
    double l, b;
    //!
    double mean, var;
    //!
    Random_Generator RNG;
};

/*! 
  \brief Rayleigh distribution
  \ingroup randgen
*/
class Rayleigh_RNG {
public:
    //! Constructor. Set sigma.
    Rayleigh_RNG(double sigma=1.0);

    //! Set sigma
    void setup(double sigma) { sig=sigma; }
    //! Get sigma
    double get_setup() { return sig; }
    //! Get one sample.
    double operator()() { return sample(); }
protected:
private:
    //!
    double sample();
    //!
    double sig;
    //!
    Random_Generator RNG;
};

/*! 
  \brief Rice distribution
  \ingroup randgen
*/
class Rice_RNG {
public:
    //! Constructor. Set sigma, and s.
    Rice_RNG(double sigma=1.0, double _s=1.0);
    //! Set sigma, and s
    void setup(double sigma, double _s) { sig=sigma; s=_s; }
    //! Get parameters
    void get_setup(double &sigma, double &_s) { sigma=sig; _s=s; }
    //! Get one sample.
    double operator()() { return sample(); }
protected:
private:
    //!
    double sample();
    //!
    double sig, s;
    //!
    Random_Generator RNG;
};

//! \addtogroup randgen
//!@{


//!@}

// -------------------- INLINES ----------------------------------------------

inline void Random_Generator::reset()
{
  set_seed(lastSeed);
}

inline void Random_Generator::reset(unsigned int seed)
{
  lastSeed = seed;
  set_seed(lastSeed);
}

inline void Random_Generator::set_seed( unsigned int oneSeed )
{
	// Seed the generator with a simple uint32
	register unsigned int *s;
	register int i;
	for( i = 624, s = state;
	     i--;
		 *s    = oneSeed & 0xffff0000,
		 *s++ |= ( (oneSeed *= 69069U)++ & 0xffff0000 ) >> 16,
		 (oneSeed *= 69069U)++ ) {}  // hard to read, but fast
	reload();
}

inline void Random_Generator::set_seed( unsigned int *const bigSeed )
{
	// Seed the generator with an array of 624 uint32's
	// There are 2^19937-1 possible initial states.  This function
	// allows any one of those to be chosen by providing 19937 bits.
	// Theoretically, the array can contain any values except all zeroes.
	// Just call seed() if you want to get array from /dev/urandom
	register unsigned int *s = state, *b = bigSeed;
	register int i = 624;
	for( ; i--; *s++ = *b++ & 0xffffffff ) {}
	reload();
}

inline double AR1_Normal_RNG::sample()
{
    double s;

    if (odd) {
	r1 = RNG.random_01();
	r2 = RNG.random_01();
	s = sqrt(factr * log(r2)) * cos(TWOPI * r1);
    } else
	s = sqrt(factr * log(r2)) * sin(TWOPI * r1);

    odd = !odd;
  
    mem = s + r * mem;
    s = mem + mean;
  
    return s;
}

inline double Weibull_RNG::sample()
{
  return ( pow(-log(RNG.random_01()), 1.0/b) / l );
}

inline double Rayleigh_RNG::sample()
{
    double r1, r2;
    double s1, s2, samp;
	double _hypot(double x, double y);

    r1 = RNG.random_01();
    r2 = RNG.random_01();
    s1 = sqrt(-2.0 * log(r2)) * cos(TWOPI * r1);
    s2 = sqrt(-2.0 * log(r2)) * sin(TWOPI * r1);
    // s1 and s2 are N(0,1) and independent

    samp = sig * _hypot(s1, s2);
     
    return samp;
}

inline double Rice_RNG::sample()
{
    double r1, r2;
    double s1, s2, samp;
    double m1 = 0.0;
    double m2 = sqrt(s*s - m1*m1);
	double _hypot(double x, double y);

    r1 = RNG.random_01();
    r2 = RNG.random_01();
    s1 = sqrt(-2.0 * log(r2)) * cos(TWOPI * r1);
    s2 = sqrt(-2.0 * log(r2)) * sin(TWOPI * r1);
    // s1 and s2 are N(0,1) and independent

    samp = _hypot(sig*s1+m1, sig*s2+m2);
      
    return samp;
}

// -------------------------------------------------------------------------------
bin randb(void);
double randu(void);
int randi(int low, int high);
double randn(void);
double_complex randn_c(void);
// Set the seed of the Global Random Number Generator
void RNG_reset(unsigned long seed);
void RNG_reset(void);
void RNG_randomize(void);
#endif
}
#endif // __random_h

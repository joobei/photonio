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
  \brief Implementation of classes for random number generators.
  \author Tony Ottosson

  1.10

  2002/12/19 23:56:45
*/

#include <ctime>
#ifdef _MSC_VER
#  include <process.h>
#  define getpid _getpid
#else
#  include <unistd.h>
#endif

#include "random.h"
namespace SPUC {
int Random_Generator::left = 0;
const unsigned int Random_Generator::MMAXINT = 0xffffffff;   // largest value from randInt()
const unsigned int Random_Generator::MAGIC  = 0x9908b0dfU;  // magic constant

unsigned int Random_Generator::state[624];
unsigned int *Random_Generator::pNext;


//! Variable used to ensure proper seed initialization 
static bool __Random_Generator_seed_is_initialized = false; 
}
using namespace SPUC;

///////////////////////////////////////////////
// Random_Generator
///////////////////////////////////////////////

Random_Generator::Random_Generator()
{
  if (!__Random_Generator_seed_is_initialized){
    lastSeed = 4357U;
    reset();
    __Random_Generator_seed_is_initialized = true;
  }
}

void Random_Generator::randomize()
{
  lastSeed = time(0) * getpid(); // not good enough if randomize is used within a short time-interval
  reset();
}


///////////////////////////////////////////////
// I_Uniform_RNG
///////////////////////////////////////////////
I_Uniform_RNG::I_Uniform_RNG(int min, int max)
{
    setup(min, max);
}

void I_Uniform_RNG::setup(int min, int max)
{
    if (min <= max) {
	lo = min;
	hi = max;
    }
    else {
	lo = max;
	hi = min;
    }
}

void I_Uniform_RNG::get_setup(int &min, int &max) const
{
    min = lo;
    max = hi;
}

///////////////////////////////////////////////
// Uniform_RNG
///////////////////////////////////////////////
Uniform_RNG::Uniform_RNG(double min, double max)
{
    setup(min, max);
}

void Uniform_RNG::setup(double min, double max)
{
    if (min <= max) {
	lo_bound = min;
	hi_bound = max;
    }
    else {
	lo_bound = max;
	hi_bound = min;
    }
}

void Uniform_RNG::get_setup(double &min, double &max) const
{
    min = lo_bound;
    max = hi_bound;
}


///////////////////////////////////////////////
// Exp_RNG
///////////////////////////////////////////////
Exponential_RNG::Exponential_RNG(double lambda)
{
    setup(lambda);
}


///////////////////////////////////////////////
// Normal_RNG
///////////////////////////////////////////////


void Normal_RNG::get_setup(double &meanval, double &variance) const
{
    meanval = mean;
    variance = sigma*sigma;
}


///////////////////////////////////////////////
// Laplace_RNG
///////////////////////////////////////////////
Laplace_RNG::Laplace_RNG(double meanval, double variance)
{
    setup(meanval, variance);
}

void Laplace_RNG::setup(double meanval, double variance)
{
    mean = meanval;
    var = variance;
}

void Laplace_RNG::get_setup(double &meanval, double &variance) const
{
    meanval = mean;
    variance = var;
}




///////////////////////////////////////////////
// AR1_Normal_RNG
///////////////////////////////////////////////
AR1_Normal_RNG::AR1_Normal_RNG(double meanval, double variance, double rho)
{
  mean = meanval;
  var = variance;
  r = rho;
  mem = 0.0;
  factr = -2.0 * var * (1.0 - rho*rho);
  odd = true;
}

void AR1_Normal_RNG::setup(double meanval, double variance, double rho)
{
    mean = meanval;
    var = variance;
    r = rho;
    factr = -2.0 * var * (1.0 - rho*rho);
    mem = 0.0;
    odd = true;
}

void AR1_Normal_RNG::get_setup(double &meanval, double &variance, double &rho) const
{
    meanval = mean;
    variance = var;
    rho = r;
}



void AR1_Normal_RNG::reset()
{
    mem = 0.0;
}




///////////////////////////////////////////////
// Rayleigh_RNG
///////////////////////////////////////////////
Rayleigh_RNG::Rayleigh_RNG(double sigma)
{
    setup(sigma);
}

///////////////////////////////////////////////
// Rice_RNG
///////////////////////////////////////////////
Rice_RNG::Rice_RNG(double lambda, double beta)
{
    setup(lambda, beta);
}

// -------------------------------------------------------------------------------
namespace SPUC {
bin randb(void)
{
  Bernoulli_RNG src;
  return src.sample();
}

double randu(void)
{
  Uniform_RNG src;
  return src.sample();
}

int randi(int low, int high)
{
  I_Uniform_RNG src;
  src.setup(low, high);
  return src();
}

double randn(void)
{
  Normal_RNG src; 
  return src.sample();
}

double_complex randn_c(void)
{
  Complex_Normal_RNG src; 
  return src.sample();
}

// Set the seed of the Global Random Number Generator
void RNG_reset(unsigned long seed)
{
  Random_Generator RNG;
  RNG.reset(seed);
}

// Set the seed of the Global Random Number Generator to the same as last time
void RNG_reset()
{
  Random_Generator RNG;
  RNG.reset();
}

// Set a random seed for the Global Random Number Generator
void RNG_randomize()
{
  Random_Generator RNG;
  RNG.randomize();
}
}

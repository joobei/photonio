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
//! 
//  SPUC - Signal processing using C++ - A DSP library
/*! 
  \file 
  \brief Binary class implemenations
  \author Tony Ottosson, modified by Tony Kirke Feb 1, 2003

  1.2

  2001/12/02 21:38:24
*/
 
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;
#include "binary.h"

namespace SPUC {

ostream &operator<<(ostream &output, const bin &inbin)
{
    output << static_cast<int>(inbin);
    return output;
}

istream &operator>>(istream &input, bin &outbin)
{
    int tmp;
    input >> tmp;
    outbin = tmp;
    return input;
}
}

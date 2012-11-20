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
// 
//  SPUC - Signal processing using C++ - A DSP library
/*! 
  \file 
  \brief Binary class definitions 
  \author Tony Ottosson, modified by Tony Kirke Feb 1, 2003
  \ingroup base
  1.8

  2003/01/04 00:21:54
*/

#ifndef __bitary_h
#define __bitary_h

#include <iostream>
#include <cassert>
using namespace std;
//#include "spucassert.h"
namespace SPUC {
/*! 
  \brief Binary arithmetic (boolean) class
  \author Tony Ottosson

  This class creates a binary aritmetic class, following the ordinary
  rules for binary (GF(2)) fields.

  Examples:
  \code
  bit a;         // Creation of variable
  bit a = 0;     // Creating a variable and assigning it value 0
  bit b = 1;     // Creating a variable and assigning it value 1
  bit c = a + b; // XOR operation
  c = !a;        // NOT
  c = a * b;     // AND
  c = a / b;     // OR
  \endcode
*/
class bit {
 public:
  //! Default constructor
  bit() { b=0; }
    
  //! Set the binary object equal to \c value. Either "0" or "1".
  bit(const short value) {
//    it_assert0(value==0 || value==1, "bin(value): value must be 0 or 1");
    b = (char)value;
  }

  // Copy constructor
  bit(const bit &inbin) { b=inbin.b; }

  //! Assign a value
  void operator=(const short &value) {
//    it_assert0(value==0 || value==1, "bin(value): value must be 0 or 1");
    b = (char)value;
  }

  //! Assign a value
  void operator=(const bit &inbin)   { b=inbin.b; }

  //! OR
  void operator/=(const bit &inbin) { b=b|inbin.b; }

  //! OR
  void operator|=(const bit &inbin) { b=b|inbin.b; }
  //! OR
  bit operator/(const bit &inbin) const { return bit(b|inbin.b); }
  //! OR
  bit operator|(const bit &inbin) const { return bit(b|inbin.b); }

  //! XOR
  void operator+=(const bit &inbin) { b=b^inbin.b; }
  //! XOR
  void operator^=(const bit &inbin) { b=b^inbin.b; }
  //! XOR
  bit operator+(const bit &inbin) const { return bit(b^inbin.b); }
  //! XOR
  bit operator^(const bit &inbin) const { return bit(b^inbin.b); }
  //! XOR
  void operator-=(const bit &inbin) { b=b^inbin.b; }
  //! XOR
  bit operator-(const bit &inbin) const {return bit(b^inbin.b); }
  //! Dummy definition to be able to use vec<bin>
  bit operator-() const { return bit(b); }
	
  //! AND
  void operator*=(const bit &inbin) { b=b&inbin.b; }
  //! AND
  void operator&=(const bit &inbin) { b=b&inbin.b; }
  //! AND
  bit operator*(const bit &inbin) const { return bit(b&inbin.b); }
  //! AND
  bit operator&(const bit &inbin) const { return bit(b&inbin.b); }
	
  //! NOT
  bit operator!(void) const { return bit(b^1); }
  //! NOT
  bit operator~(void) const { return bit(b^1); }

  //! Check if equal
  bool operator==(const bit &inbin) const { return b == inbin.b; }
  //! Check if equal
  bool operator==(const int &inbin) const { return b == inbin; }

  //! Check if not equal
  bool operator!=(const bit &inbin) const { return b != inbin.b; }
  //! Check if not equal
  bool operator!=(const int &inbin) const { return b != inbin; }

  //! Less than (interpret the binary values {0,1} as integers)
  bool operator<(const bit &inbin) const  { return b < inbin.b; }
  //! Less than equal (interpret the binary values {0,1} as integers)
  bool operator<=(const bit &inbin) const { return b <= inbin.b; }

  //! Greater than (interpret the binary values {0,1} as integers)
  bool operator>(const bit &inbin) const  { return b > inbin.b; }
  //! Greater than equal (interpret the binary values {0,1} as integers)
  bool operator>=(const bit &inbin) const { return b >= inbin.b; }
	
  //! Convert \c bit to \c short
  operator short() const  { return short(b); }
  //! Convert \c bit to \c int
  operator int() const    { return int(b); }
  //! Convert \c bit to \c bool
  operator bool() const   { return b!=0; }
  //! Convert \c bit to \c float
  operator float() const  { return float(b); }
  //! Convert \c bit to \c double
  operator double() const { return double(b); }

  //! Output the binary value of the object
  char value() const { return b; }

 protected:

 private:
  char b;
};

/*! 
  \relates bit
  \brief Output stream of bit
*/

ostream &operator<<(ostream &output, const bit &inbin);

/*! 
  \relates bit
  \brief Input stream of bit
*/
istream &operator>>(istream &input, bit &outbin);
} //namespace SPUC 

#endif // __bitary_h

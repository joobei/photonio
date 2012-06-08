#include "gf.h"
#include <math.h>
using namespace SPUC;
ivec gf::q="1 2 4 8 16 32 64 128 256 512 1024 2048 4096 8192 16384";
void gf::set(int qvalue, const bvec &vectorspace)
{
    int i, temp=0;
    set_size(qvalue);
	//    it_assert0(vectorspace.length() == m, "gf::set, out of range");
    int sizebvec=vectorspace.length();
	for (i=0; i<sizebvec; i++) {
		temp += (vectorspace[i]) ? ( 1 << (sizebvec-i-1) ) : 0;
	}
    value = logalpha[m][temp];
}
bvec gf::get_vectorspace() const
{
  int x;
  int i;
  bvec temp(m);
  if (value == -1)	x = 0;
  else           	x = alphapow[m][value];
  for (i=m-1; i>=0; i--) {
	temp[i] = (x & 1) ? 1 : 0;
	x = (x >> 1);
  }
  return temp;
}

int  gf::get_value() const
{
    return value;
}

int gf::operator==(const gf &ingf) const
{
  if (value == -1 && ingf.value == -1)
	return true;
  if (m==ingf.m && value==ingf.value)
	return true;
  else
	return false;
}

int gf::operator!=(const gf &ingf) const
{
  gf tmp(*this);
  return !(tmp==ingf);
}

void gf::operator=(const gf &ingf)
{
  m=ingf.m;
  value=ingf.value;
}

void gf::operator=(const int inexp)
{
  //    it_assert0(m>0 && inexp>=-1 && inexp<(q[m]-1), "gf::op=, out of range");
  value=inexp;
}

void gf::operator+=(const gf &ingf)
{
  if (value == -1) {
	value=ingf.value;
	m=ingf.m;
  }
  else if (ingf.value != -1) {
	//	it_assert0(ingf.m == m, "gf::op+=, not same field");
	value=logalpha[m][alphapow[m][value] ^ alphapow[m][ingf.value]];
  }
}

gf gf::operator+(const gf &ingf) const
{
  gf tmp(*this);
  tmp+=ingf;
  return tmp;
}

void gf::operator-=(const gf &ingf)
{
  (*this)+=ingf;
}

gf gf::operator-(const gf &ingf) const
{
  gf tmp(*this);
  tmp-=ingf;
  return tmp;
}

void gf::operator*=(const gf &ingf)
{
  if (value == -1 || ingf.value == -1)
	value=-1;
  else {
	//	it_assert0(ingf.m == m, "gf::op+=, not same field");
	value=(value+ingf.value)%(q[m]-1);
  }
}

gf gf::operator*(const gf &ingf) const
{
  gf tmp(*this);
  tmp*=ingf;
  return tmp;
}

void gf::operator/=(const gf &ingf)
{
  assert(ingf.value !=-1); // no division by the zeroth element
  if (value == -1)
	value=-1;
  else {
	//	it_assert0(ingf.m == m, "gf::op+=, not same field");
	value=(value-ingf.value+q[m]-1)%(q[m]-1);
  }
}

gf gf::operator/(const gf &ingf) const
{
  gf tmp(*this);
  tmp/=ingf;
  return tmp;
}
// set q=2^mvalue
void gf::set_size(int qvalue)
{
    int mtemp=0;

    mtemp = ::log(qvalue)/::log(2.0);
	//    it_assert((1<<mtemp)==qvalue, "gf::setsize : q is not a power of 2");
	//    it_assert(mtemp<=14, "gf::setsize : q must be less than or equal to 2^14");

    /* Construct gf(q), q=2^m. From Wicker, "Error Control Systems  for digital communication and storage" pp. 463-465 */

    int reduce, temp, n;
    const int reducetable[]={3,3,3,5,3,9,29,17,9,5,83,27,43}; // starts at m=2,..,14
	//    it_error_if(mtemp < 1 || mtemp > 14, "createfield : m out of range");
    m=mtemp;
	alphapow.newsize(m+1,qvalue);
	logalpha.newsize(m+1,qvalue);

	if (m == 1) { // gf(2), special case
	  alphapow[1][0];
	  logalpha[1][0]=-1; logalpha[1][1]=0;
	} else {
	  reduce=reducetable[m-2];
	  alphapow[m][0]=1; // alpha^0 = 1
	  for (n=1; n<(1<<m)-1; n++) {
		temp=alphapow[m][n-1];
		temp=(temp << 1); // multiply by alpha
		if (temp & (1<<m)) // contains alpha**m term
		  alphapow[m][n]=(temp & ~(1<<m))^reduce;
		else
		  alphapow[m][n]=temp; // if no alpha**m term, store as is
		  
		// create table to go in opposite direction
		logalpha[m][n]=-1; // special case, actually log(0)=-inf
	  }
	  for (n=0;n<(1<<m)-1;n++)  logalpha[m][alphapow[m][n]]=n;
	}
}

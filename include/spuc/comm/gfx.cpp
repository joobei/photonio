#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;
#include "gfx.h"
using namespace SPUC;
// ------------------ class gfx --------------------
gfx::gfx()
{
  degree=-1;
  q=0;
}
gfx::gfx(int qvalue)
{
  //    it_assert0(qvalue>=0, "gfx::gfx, out of range");
  q=qvalue;
}

void gfx::set(int qvalue, const ivec &invalues)
{
  //    it_assert0(qvalue>0, "gfx::set, out of range");
  degree=invalues.length()-1;
  coeffs.set_size(degree+1);
  for (int i=0;i<degree+1;i++)
	coeffs[i].set(qvalue,invalues[i]);
  q=qvalue;
}

void gfx::set(int qvalue, const char *invalues)
{
    set(qvalue,ivec(invalues));
}

void gfx::set(int qvalue, const string invalues)
{
  set(qvalue,invalues.c_str());
}

gfx::gfx(int qvalue, int indegree)
{
  //    it_assert0(qvalue>0 && indegree>=0, "gfx::gfx, out of range");
  q=qvalue;
  coeffs.set_size(indegree+1);
  degree=indegree;
  for (int i=0;i<degree+1;i++)
	coeffs[i].set(q,-1);
}
gfx::gfx(int qvalue, const ivec &invalues)
{
  set(qvalue,invalues);
}

gfx::gfx(int qvalue, char *invalues)
{
  set(qvalue,invalues);
}

gfx::gfx(int qvalue, string invalues)
{
  set(qvalue,invalues.c_str());
}

gfx::gfx(const gfx &ingfx)
{
    degree=ingfx.degree;
    coeffs=ingfx.coeffs;
    q=ingfx.q;
}

int gfx::get_size() const
{
  return q;
}

int gfx::get_degree() const
{
  return degree;
}

void gfx::set_degree(int indegree)
{
  //    it_assert0(indegree>=-1, "gfx::set_degree, out of range");
  coeffs.set_size(indegree+1);
  degree=indegree;
}

int gfx::get_true_degree() const
{
  int i=degree;
  while(coeffs[i].get_value()==-1) {
	i--;
	if (i==-1)
	  break;
  }
  return i;
}

void gfx::clear()
{
  //    it_assert0(degree>=0 && q>0, "gfx::clear, not set");
  for(int i=0;i<degree+1;i++)
	coeffs[i].set(q,-1);
}

void gfx::operator=(const gfx &ingfx)
{
    degree=ingfx.degree;
    coeffs=ingfx.coeffs;
    q=ingfx.q;
}

void gfx::operator+=(const gfx &ingfx)
{
	//    it_assert0(q == ingfx.q, "gfx::op+=, not same field");
    if (ingfx.degree > degree) {
	coeffs.set_size(ingfx.degree+1);
	// set new coefficients to the zeroth element
	for (int j=degree+1; j<coeffs.dim(); j++){ coeffs[j].set(q,-1); }
	degree=ingfx.degree;
    }
    for (int i=0;i<ingfx.degree+1;i++) { coeffs[i]+=ingfx.coeffs[i]; }
}

gfx gfx::operator+(const gfx &ingfx) const
{
    gfx tmp(*this);
    tmp+=ingfx;
    return tmp;
}

void gfx::operator-=(const gfx &ingfx)
{
    (*this)+=ingfx;
}

gfx gfx::operator-(const gfx &ingfx) const
{
    gfx tmp(*this);
    tmp-=ingfx;
    return tmp;
}

void gfx::operator*=(const gfx &ingfx)
{
	//    it_assert0(q == ingfx.q, "gfx::op*=, Not same field");
    int i,j;
    Array1D<gf> tempcoeffs=coeffs;
    coeffs.set_size(degree+ingfx.degree+1);
    for (j=0; j<coeffs.dim(); j++)
	coeffs[j].set(q,-1); // set coefficients to the zeroth element (log(0)=-Inf=-1)
    for (i=0;i<degree+1;i++)
	for (j=0;j<ingfx.degree+1;j++)
	    coeffs[i+j]+=tempcoeffs[i]*ingfx.coeffs[j];
    degree=coeffs.dim()-1;
}

gfx gfx::operator*(const gfx &ingfx) const
{
    gfx tmp(*this);
    tmp*=ingfx;
    return tmp;
}
gf gfx::operator()(const gf &ingf)
{
	//    it_assert0(q == ingf.get_size(), "gfx::op(), Not same field");
    gf temp(coeffs[0]), ingfpower(ingf);
    for (int i=1; i<degree+1; i++) {
	temp+=coeffs[i]*ingfpower;
	ingfpower*=ingf;
    }
    return temp;
}
//----------------- Help Functions -----------------
namespace SPUC {
gfx operator*(const gf &ingf, const gfx &ingfx)
{
	//    it_assert0(ingf.get_size() == ingfx.q, "gfx::op*, Not same field");
    gfx temp(ingfx);
    for (int i=0;i<ingfx.get_degree()+1;i++)
	temp.coeffs[i] *= ingf;
    return temp;
}

gfx  operator*( const gfx &ingfx, const gf &ingf)
{
    gfx temp(ingfx);
    for (int i=0;i<ingfx.get_degree()+1;i++)
	temp.coeffs[i] *= ingf;
    return temp;
}

gfx  operator/(const gfx &ingfx, const gf &ingf)
{
	//    it_assert0(ingf.get_size() == ingfx.q, "gfx::op/, Not same field");
    gfx temp(ingfx);
    for (int i=0;i<ingfx.get_degree()+1;i++)
	temp.coeffs[i] /= ingf;
    return temp;
}
//! Division of two gfx (local help function)
gfx divgfx(const gfx &c, const gfx &g) {
    int q = c.get_size();
    gfx temp = c;
    int tempdegree = temp.get_true_degree();
    int gdegree = g.get_true_degree();
    int degreedif = tempdegree - gdegree;
    gfx m(q,degreedif), divisor(q);

    for (int i=0; i<c.get_degree(); i++) {
	m[degreedif] = temp[tempdegree]/g[gdegree];
	divisor.set_degree(degreedif);
	divisor.clear();
	divisor[degreedif] = m[degreedif];
	temp -= divisor*g;
	tempdegree = temp.get_true_degree();
	degreedif = tempdegree - gdegree;
	if ( (degreedif<0) || (temp.get_true_degree()==0 && temp[0] == gf(q,-1) ) ) {
	    break;
	}
    }
    return m;
}

//! Modulo function of two gfx (local help function)
gfx modgfx(const gfx &a, const gfx &b) 
{
    int q = a.get_size();
    gfx temp = a;
    int tempdegree = temp.get_true_degree();
    int bdegree = b.get_true_degree();
    int degreedif = a.get_true_degree() - b.get_true_degree();
    gfx m(q,degreedif), divisor(q);

    for (int i=0; i<a.get_degree(); i++) {
	m[degreedif] = temp[tempdegree]/b[bdegree];
	divisor.set_degree(degreedif);
	divisor.clear();
	divisor[degreedif] =  m[degreedif];
	temp -= divisor*b; // Bug-fixed. Used to be: temp -= divisor*a;
	tempdegree = temp.get_true_degree();
	degreedif = temp.get_true_degree() - bdegree;
	if ( (degreedif<0) || (temp.get_true_degree()==0 && temp[0] == gf(q,-1) ) ) {
	    break;
	}
    }
    return temp;
}

//! Output stream operator for GF
ostream &operator<<(ostream &os, const gf &ingf)
{
    if (ingf.value == -1)
	os << "0";
    else
	os << "alpha^" << ingf.value;
    return os;
}

//! Output stream operator for GFX
ostream &operator<<(ostream &os, const gfx &ingfx)
{
    int terms=0;
    for (int i=0; i<ingfx.degree+1; i++) {
	if (ingfx.coeffs[i] != gf(ingfx.q,-1) ) {
	    if (terms != 0) os << " + ";
	    terms++;
	    if (ingfx.coeffs[i] == gf(ingfx.q,0) ) {// is the coefficient an one (=alpha^0=1)
		os  << "x^" << i;
	    } else {
		os  << ingfx.coeffs[i] << "*x^" << i;
	    }
	}
    }
    if (terms == 0) os << "0";
    return os;
}

}

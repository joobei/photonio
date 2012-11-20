#ifndef __galoisgf_h
#define __galoisgf_h

#include <binary.h>
#include <vector.h>
#include <matrix.h>
#include <array1d.h>
#include <array2d.h>
#include <iostream>
#include <cassert>
namespace SPUC {
/*! 
  \addtogroup fec Forward Error Correcting Codes
*/

/*!  \brief Galois Field
  \ingroup fec
*/
class gf {
 public:
    //! Constructor
    gf()  
		{
			for (int j=0;j<15;j++) q[j] = (1 << j);
			m=0;
		}
    //! Constructor
    gf(int qvalue) { 
		m=0; 
		for (int j=0;j<15;j++) q[j] = (1 << j);
		if (qvalue==0) // qvalue==0 gives the zeroth element
			value=-1; else set_size(qvalue); 
	}
    //! Constructor
    gf(int qvalue, int inexp) { 
		m=0; 
		for (int j=0;j<15;j++) q[j] = (1 << j);
		set(qvalue,inexp); 
	}
    //! Copy constructor
    gf(const gf &ingf) {
		for (int j=0;j<15;j++) q[j] = (1 << j);
		m=ingf.m; value=ingf.value; 
	}
    //! gf(q) equals \a alpha ^ \a inexp
    void set(int qvalue, int inexp) {
		set_size(qvalue); 
		value=inexp;
	}
    /*! 
      \brief gf(q) equals the element that corresponds to the given vector space.
      The format is (...,c,b,a), where the element x is given as x=...+c*alpha^2+b*alpha+a.
    */
    void set(int qvalue, const bvec &vectorspace);
    //! set q=2^mvalue
    void set_size(int qvalue);
    //! Return q.
    int get_size() const { return ( (m != 0) ? q[m] : 0 ); }

    /*! 
      \brief Returns the vector space representation of gf(q).
	  
      The format is (...,c,b,a), where the element x is given as x=...+c*alpha^2+b*alpha+a.
    */
    bvec get_vectorspace() const;
    //! Returns the alpha exponent
    int  get_value() const;
    //! Equality check
    int operator==(const gf &ingf) const;
    //! Not-equality check
    int operator!=(const gf &ingf) const;
  
    //! gf(q) equals ingf
    void operator=(const gf &ingf);
    //! gf(q) equals alpha^inexp
    void operator=(const int inexp);
    //! sum of two gf(q)
    void operator+=(const gf &ingf);
    //! sum of two gf(q)
    gf operator+(const gf &ingf) const;
    //! Difference of two gf(q), same as sum for q=2^m.
    void operator-=(const gf &ingf);
    //! Difference of two gf(q), same as sum for q=2^m.
    gf operator-(const gf &ingf) const;
    //! product of two gf(q)
    void operator*=(const gf &ingf);
    //! product of two gf(q)
    gf operator*(const gf &ingf) const;
    //! division of two gf(q)
    void operator/=(const gf &ingf);
    //! product of two gf(q)
    gf operator/(const gf &ingf) const;
    //! Output stream for gf(q)
    friend ostream &operator<<(ostream &os, const gf &ingf);

 protected:
 private:
    char m;
    int value;
    Array2D<int> alphapow,logalpha;
    static ivec q;
};

} // namespace SPUC 
#endif // __galois_h

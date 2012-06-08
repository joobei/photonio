#ifndef __galoisfx_h
#define __galoisfx_h

#include "gf.h"
namespace SPUC {
//! \ingroup fec
/*! \brief Polynomials over gf(q)[x], where q=2^m, m=1,...,14
*/
class gfx {
public:
    //! Constructor
    gfx();
    //! Constructor
    gfx(int qvalue);
    //! Constructor
    gfx(int qvalue, int indegree);
    //! Constructor
    gfx(int qvalue, const ivec &invalues);
    //! Constructor
    gfx(int qvalue, char *invalues);
    //! Constructor
    gfx(int qvalue, string invalues);
    //! Copy constructor
    gfx(const gfx &ingfx);
    //! Return q.
    int get_size() const;
    //! Return degree of gf(q)[x]
    int get_degree() const;
    /*! 
      \brief Resize the polynomial to the given indegree. If the new polynomial is bigger, then the new coefficients are set to zero.
    */
    void set_degree(int indegree);
    //! Return true degree of gf(q)[x]
    int get_true_degree() const;
    //! Set the gf(q)[x] polynomial
    void set(int qvalue, const char *invalues);
    //! Set the gf(q)[x] polynomial
    void set(int qvalue, const string invalues);
    //! Set the gf(q)[x] polynomial
    void set(int qvalue, const ivec &invalues);
    //! Set all coefficients to zero.
    void clear();
    //! Acces to individual element in the gf(q)[x] polynomial
    gf operator[](int index) const {
		// it_assert0(index<=degree, "gfx::op[], out of range"); 
		return coeffs[index]; }
    //! Acces to individual element in the gf(q)[x] polynomial
    gf &operator[](int index) {
		// it_assert0(index<=degree, "gfx::op[], out of range"); 
		return coeffs[index]; }
    //! Copy
    void operator=(const gfx &ingfx);
    //! sum of two gf(q)[x]
    void operator+=(const gfx &ingfx);
    //! sum of two gf(q)[x]
    gfx operator+(const gfx &ingfx) const;
    //! Difference of two gf(q), same as sum for q=2^m.
    void operator-=(const gfx &ingfx);
    //! Difference of two gf(q), same as sum for q=2^m.
    gfx operator-(const gfx &ingfx) const;
    //! product of two gf(q)[x]
    void operator*=(const gfx &ingfx);
    //! product of two gf(q)[x]
    gfx operator*(const gfx &ingfx) const;
    //! Evaluate polynom at alpha^inexp
    gf operator()(const gf &ingf);
    //! Multiply a gf element with a gf(q)[x]
    friend gfx  operator*(const gf &ingf, const gfx &ingfx);
    //! Multiply a gf(q)[x] with a gf element
    friend gfx  operator*( const gfx &ingfx, const gf &ingf);
    //! Divide a gf(q)[x] with a gf element
    friend gfx  operator/(const gfx &ingfx, const gf &ingf);



    //! Output stream
    friend ostream &operator<<(ostream &os, const gfx &ingfx);

    Array1D<gf> coeffs;
 
 protected:
 private:
    
	int degree, q;
};
gfx divgfx(const gfx &c, const gfx &g);

} // namespace SPUC
#endif // __galois_h

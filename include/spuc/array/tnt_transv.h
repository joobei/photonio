/*
*
* Template Numerical Toolkit (SPUC): Linear Algebra Module
*
* Mathematical and Computational Sciences Division
* National Institute of Technology,
* Gaithersburg, MD USA
*
*
* This software was developed at the National Institute of Standards and
* Technology (NIST) by employees of the Federal Government in the course
* of their official duties. Pursuant to title 17 Section 105 of the
* United States Code, this software is not subject to copyright protection
* and is in the public domain.  The Template Numerical Toolkit (SPUC) is
* an experimental system.  NIST assumes no responsibility whatsoever for
* its use by other parties, and makes no guarantees, expressed or implied,
* about its quality, reliability, or any other characteristic.
*
* BETA VERSION INCOMPLETE AND SUBJECT TO CHANGE
* see http://math.nist.gov/tnt for latest updates.
*
*/



// Matrix Transpose Views

#ifndef TRANSV_H
#define TRANSV_H

#include <iostream>
#include <cassert>

namespace SPUC {

//! Template class for Matrix Transpose Views
template <class array2D> class transpose_view
{
    protected:

        const array2D &  A_;

    public:

        typedef typename array2D::element_type T;
        typedef         T   value_type;
        typedef         T   element_type;
        typedef         T*  pointer;
        typedef         T*  iterator;
        typedef         T&  reference;
        typedef const   T*  const_iterator;
        typedef const   T&  const_reference;


        const array2D & array()  const { return A_; }
        subscript num_rows() const { return A_.num_cols();}
        subscript num_cols() const { return A_.num_rows();}
        subscript lbound() const { return A_.lbound(); }
        subscript dim(subscript i) const
        {
#ifdef SPUC_BOUNDS_CHECK
            assert( A_.lbound() <= i);
            assert( i<= A_.lbound()+1);
#endif
            if (i== A_.lbound())
                return num_rows();
            else
                return num_cols();
        }


        transpose_view(const transpose_view<array2D> &A) : A_(A.A_) {};
        transpose_view(const array2D &A) : A_(A) {};


        inline const typename array2D::element_type & operator()(
																 int i, int j) const
			{
#ifdef SPUC_BOUNDS_CHECK
				assert(lbound()<=i);
				assert(i<=A_.num_cols() + lbound() - 1);
				assert(lbound()<=j);
				assert(j<=A_.num_rows() + lbound() - 1);
#endif
				
				return A_(j,i);
			}


};
//!  Matrix Transpose Views
 template <class Matrix> transpose_view<Matrix> transpose_view(const Matrix &A)
{
    return transpose_view<Matrix>(A);
 }
template <class Matrix, class T>
	Array1D<T> matmult(
					   const transpose_view<Matrix> & A, 
					   const Array1D<T> &B)
{
    int  M = A.dim1();
    int  N = A.dim2();
	
    assert(B.dim() == N);
	
    Array1D<T> x(N);

    int i, j;
    T tmp = 0;

    for (i=1; i<=M; i++)
    {
        tmp = 0;
        for (j=1; j<=N; j++)
            tmp += A(i,j) * B(j);
        x(i) = tmp;
    }

    return x;
}

template <class Matrix, class T>
inline Array1D<T> operator*(const transpose_view<Matrix> & A, const Array1D<T> &B)
{
    return matmult(A,B);
}


template <class Matrix>
std::ostream& operator<<(std::ostream &s, const transpose_view<Matrix> &A)
{
    int M=A.dim1();
    int N=A.dim2();

    int start = A.lbound();
    int Mend = M + A.lbound() - 1;
    int Nend = N + A.lbound() - 1;

    s << M << "  " << N << endl;
    for (int i=start; i<=Mend; i++)
    {
        for (int j=start; j<=Nend; j++)
        {
            s << A(i,j) << " ";
        }
        s << endl;
    }


    return s;
}

} // namespace SPUC

#endif
// TRANSV_H

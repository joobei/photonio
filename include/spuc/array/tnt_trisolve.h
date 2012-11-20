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



// Triangular Solves

#ifndef TRISLV_H
#define TRISLV_H


#include "tnt_triang.h"

namespace SPUC
{

template <class MaTriX, class VecToR>
VecToR lower_triangular_solve(/*const*/ MaTriX &A, /*const*/ VecToR &b)
{
    int N = A.dim1();

    // make sure matrix sizes agree; A must be square

    assert(A.dim2() == N);
    assert(b.dim() == N);

    VecToR x(N);

    int i;
    for (i=1; i<=N; i++)
    {
        typename MaTriX::element_type tmp=0;

        for (int j=1; j<i; j++)
                tmp = tmp + A(i,j)*x(j);

        x(i) =  (b(i) - tmp)/ A(i,i);
    }

    return x;
}


template <class MaTriX, class VecToR>
VecToR unit_lower_triangular_solve(/*const*/ MaTriX &A, /*const*/ VecToR &b)
{
    int N = A.dim1();

    // make sure matrix sizes agree; A must be square

    assert(A.dim2() == N);
    assert(b.dim() == N);

    VecToR x(N);

    int i;
    for (i=1; i<=N; i++)
    {

        typename MaTriX::element_type tmp=0;

        for (int j=1; j<i; j++)
                tmp = tmp + A(i,j)*x(j);

        x(i) =  b(i) - tmp;
    }

    return x;
}


template <class MaTriX, class VecToR>
VecToR linear_solve(/*const*/ lowerTriangularView<MaTriX> &A, 
            /*const*/ VecToR &b)
{
    return lower_triangular_solve(A, b);
}
    
template <class MaTriX, class VecToR>
VecToR linear_solve(/*const*/ unitlowerTriangularView<MaTriX> &A, 
        /*const*/ VecToR &b)
{
    return unit_lower_triangular_solve(A, b);
}
    


//********************** upper triangular section ****************

template <class MaTriX, class VecToR>
VecToR upper_triangular_solve(/*const*/ MaTriX &A, /*const*/ VecToR &b)
{
    int N = A.dim1();

    // make sure matrix sizes agree; A must be square

    assert(A.dim2() == N);
    assert(b.dim() == N);

    VecToR x(N);

    int i;
    for (i=N; i>=1; i--)
    {

        typename MaTriX::element_type tmp=0;

        for (int j=i+1; j<=N; j++)
                tmp = tmp + A(i,j)*x(j);

        x(i) =  (b(i) - tmp)/ A(i,i);
    }

    return x;
}


template <class MaTriX, class VecToR>
VecToR unit_upper_triangular_solve(/*const*/ MaTriX &A, /*const*/ VecToR &b)
{
    int N = A.dim1();

    // make sure matrix sizes agree; A must be square

    assert(A.dim2() == N);
    assert(b.dim() == N);

    VecToR x(N);

    int i;
    for (i=N; i>=1; i--)
    {

        typename MaTriX::element_type tmp=0;

        for (int j=i+1; j<i; j++)
                tmp = tmp + A(i,j)*x(j);

        x(i) =  b(i) - tmp;
    }

    return x;
}


template <class MaTriX, class VecToR>
VecToR linear_solve(/*const*/ upperTriangularView<MaTriX> &A, 
        /*const*/ VecToR &b)
{
    return upper_triangular_solve(A, b);
}
    
template <class MaTriX, class VecToR>
VecToR linear_solve(/*const*/ unitupperTriangularView<MaTriX> &A, 
    /*const*/ VecToR &b)
{
    return unit_upper_triangular_solve(A, b);
}


} // namespace SPUC

#endif
// TRISLV_H

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



// Triangular Matrices (Views and Adpators)

#ifndef TRIANG_H
#define TRIANG_H

// default to use lower-triangular portions of arrays
// for symmetric matrices.

namespace SPUC {
//! lower Triangluar View for Matrices
template <class MaTRiX> class lowerTriangularView
{
    protected:


        const MaTRiX  &A_;
        const typename MaTRiX::element_type zero_;

    public:


    typedef typename MaTRiX::const_reference const_reference;
    typedef const typename MaTRiX::element_type element_type;
    typedef const typename MaTRiX::element_type value_type;
    typedef element_type T;

    int dim(int d) const {  return A_.dim(d); }
    int lbound() const { return A_.lbound(); }
    int dim1() const { return A_.dim1(); }
    int dim2() const { return A_.dim2(); }
    
    
    // constructors

    lowerTriangularView(/*const*/ MaTRiX &A) : A_(A),  zero_(0) {}


    inline const_reference get(int i, int j) const
    { 
#ifdef SPUC_BOUNDS_CHECK
        assert(lbound()<=i);
        assert(i<=A_.dim1() + lbound() - 1);
        assert(lbound()<=j);
        assert(j<=A_.dim2() + lbound() - 1);
#endif
        if (i<j) 
            return zero_;
        else
            return A_(i,j);
    }


    inline const_reference operator() (int i, int j) const
    {
#ifdef SPUC_BOUNDS_CHECK
        assert(lbound()<=i);
        assert(i<=A_.dim1() + lbound() - 1);
        assert(lbound()<=j);
        assert(j<=A_.dim2() + lbound() - 1);
#endif
        if (i<j) 
            return zero_;
        else
            return A_(i,j);
    }

#ifdef SPUC_USE_REGIONS 

    typedef const_region2D< lowerTriangularView<MaTRiX> > 
                    const_region;

    const_region operator()(/*const*/ index1D &I,
            /*const*/ index1D &J) const
    {
        return const_region(*this, I, J);
    }

    const_region operator()(int i1, int i2,
            int j1, int j2) const
    {
        return const_region(*this, i1, i2, j1, j2);
    }



#endif
// SPUC_USE_REGIONS

};


//! lower_triangular_view() algorithms
template <class MaTRiX, class VecToR>
VecToR matmult(/*const*/ lowerTriangularView<MaTRiX> &A, VecToR &x)
{
    int M = A.dim1();
    int N = A.dim2();

    assert(N == x.dim());

    int i, j;
    typename MaTRiX::element_type sum=0.0;
    VecToR result(M);

    int start = A.lbound();
    int Mend = M + A.lbound() -1 ;

    for (i=start; i<=Mend; i++)
    {
        sum = 0.0;
        for (j=start; j<=i; j++)
            sum = sum + A(i,j)*x(j);
        result(i) = sum;
    }

    return result;
}

template <class MaTRiX, class VecToR>
inline VecToR operator*(/*const*/ lowerTriangularView<MaTRiX> &A, VecToR &x)
{
    return matmult(A,x);
}
//!
// Unit lower Triangular View
template <class MaTRiX> class unitlowerTriangularView
{
    protected:

        const MaTRiX  &A_;
        const typename MaTRiX::element_type zero;
        const typename MaTRiX::element_type one;

    public:

    typedef typename MaTRiX::const_reference const_reference;
    typedef typename MaTRiX::element_type element_type;
    typedef typename MaTRiX::element_type value_type;
    typedef element_type T;

    int lbound() const { return 1; }
    int dim(int d) const {  return A_.dim(d); }
    int dim1() const { return A_.dim1(); }
    int dim2() const { return A_.dim2(); }

    
    // constructors

    unitlowerTriangularView(/*const*/ MaTRiX &A) : A_(A), zero(0), one(1) {}


    inline const_reference get(int i, int j) const
    { 
#ifdef SPUC_BOUNDS_CHECK
        assert(1<=i);
        assert(i<=A_.dim(1));
        assert(1<=j);
        assert(j<=A_.dim(2));
        assert(0<=i && i<A_.dim(0) && 0<=j && j<A_.dim(1));
#endif
        if (i>j)
            return A_(i,j);
        else if (i==j)
            return one;
        else 
            return zero;
    }


    inline const_reference operator() (int i, int j) const
    {
#ifdef SPUC_BOUNDS_CHECK
        assert(1<=i);
        assert(i<=A_.dim(1));
        assert(1<=j);
        assert(j<=A_.dim(2));
#endif
        if (i>j)
            return A_(i,j);
        else if (i==j)
            return one;
        else 
            return zero;
    }


#ifdef SPUC_USE_REGIONS 
  // These are the "index-aware" features

    typedef const_region2D< unitlowerTriangularView<MaTRiX> > 
                    const_region;

    const_region operator()(/*const*/ index1D &I,
            /*const*/ index1D &J) const
    {
        return const_region(*this, I, J);
    }

    const_region operator()(int i1, int i2,
            int j1, int j2) const
    {
        return const_region(*this, i1, i2, j1, j2);
    }
#endif
// SPUC_USE_REGIONS
};

template <class MaTRiX>
lowerTriangularView<MaTRiX> lower_triangular_view(
    /*const*/ MaTRiX &A)
{
    return lowerTriangularView<MaTRiX>(A);
}


template <class MaTRiX>
unitlowerTriangularView<MaTRiX> unit_lower_triangular_view(
    /*const*/ MaTRiX &A)
{
    return unitlowerTriangularView<MaTRiX>(A);
}

template <class MaTRiX, class VecToR>
VecToR matmult(/*const*/ unitlowerTriangularView<MaTRiX> &A, VecToR &x)
{
    int M = A.dim1();
    int N = A.dim2();

    assert(N == x.dim());

    int i, j;
    typename MaTRiX::element_type sum=0.0;
    VecToR result(M);

    int start = A.lbound();
    int Mend = M + A.lbound() -1 ;

    for (i=start; i<=Mend; i++)
    {
        sum = 0.0;
        for (j=start; j<i; j++)
            sum = sum + A(i,j)*x(j);
        result(i) = sum + x(i);
    }

    return result;
}

template <class MaTRiX, class VecToR>
inline VecToR operator*(/*const*/ unitlowerTriangularView<MaTRiX> &A, VecToR &x)
{
    return matmult(A,x);
}


//********************** Algorithms *************************************



template <class MaTRiX>
std::ostream& operator<<(std::ostream &s, const lowerTriangularView<MaTRiX>&A)
{
    int M=A.dim1();
    int N=A.dim2();

    s << M << " " << N << endl;

    for (int i=1; i<=M; i++)
    {
        for (int j=1; j<=N; j++)
        {
            s << A(i,j) << " ";
        }
        s << endl;
    }


    return s;
}

template <class MaTRiX>
std::ostream& operator<<(std::ostream &s, 
    const unitlowerTriangularView<MaTRiX>&A)
{
    int M=A.dim1();
    int N=A.dim2();

    s << M << " " << N << endl;

    for (int i=1; i<=M; i++)
    {
        for (int j=1; j<=N; j++)
        {
            s << A(i,j) << " ";
        }
        s << endl;
    }


    return s;
}



//! upper Triangular Section
template <class MaTRiX> class upperTriangularView
{
    protected:


        /*const*/ MaTRiX  &A_;
        /*const*/ typename MaTRiX::element_type zero_;

    public:


    typedef typename MaTRiX::const_reference const_reference;
    typedef /*const*/ typename MaTRiX::element_type element_type;
    typedef /*const*/ typename MaTRiX::element_type value_type;
    typedef element_type T;

    int dim(int d) const {  return A_.dim(d); }
    int lbound() const { return A_.lbound(); }
    int dim1() const { return A_.dim1(); }
    int dim2() const { return A_.dim2(); }
    
    
    // constructors

    upperTriangularView(/*const*/ MaTRiX &A) : A_(A),  zero_(0) {}


    inline const_reference get(int i, int j) const
    { 
#ifdef SPUC_BOUNDS_CHECK
        assert(lbound()<=i);
        assert(i<=A_.dim1() + lbound() - 1);
        assert(lbound()<=j);
        assert(j<=A_.dim2() + lbound() - 1);
#endif
        if (i>j) 
            return zero_;
        else
            return A_(i,j);
    }


    inline const_reference operator() (int i, int j) const
    {
#ifdef SPUC_BOUNDS_CHECK
        assert(lbound()<=i);
        assert(i<=A_.dim1() + lbound() - 1);
        assert(lbound()<=j);
        assert(j<=A_.dim2() + lbound() - 1);
#endif
        if (i>j) 
            return zero_;
        else
            return A_(i,j);
    }

#ifdef SPUC_USE_REGIONS 

    typedef const_region2D< upperTriangularView<MaTRiX> > 
                    const_region;

    const_region operator()(const index1D &I,
            const index1D &J) const
    {
        return const_region(*this, I, J);
    }

    const_region operator()(int i1, int i2,
            int j1, int j2) const
    {
        return const_region(*this, i1, i2, j1, j2);
    }



#endif
// SPUC_USE_REGIONS

};


//! upper_triangular_view() algorithms
template <class MaTRiX, class VecToR>
VecToR matmult(/*const*/ upperTriangularView<MaTRiX> &A, VecToR &x)
{
    int M = A.dim1();
    int N = A.dim2();

    assert(N == x.dim());

    int i, j;
    typename VecToR::element_type sum=0.0;
    VecToR result(M);

    int start = A.lbound();
    int Mend = M + A.lbound() -1 ;

    for (i=start; i<=Mend; i++)
    {
        sum = 0.0;
        for (j=i; j<=N; j++)
            sum = sum + A(i,j)*x(j);
        result(i) = sum;
    }

    return result;
}

template <class MaTRiX, class VecToR>
inline VecToR operator*(/*const*/ upperTriangularView<MaTRiX> &A, VecToR &x)
{
    return matmult(A,x);
}
// unit upper Triangular View
template <class MaTRiX> class unitupperTriangularView
{
    protected:

        const MaTRiX  &A_;
        const typename MaTRiX::element_type zero;
        const typename MaTRiX::element_type one;

    public:

    typedef typename MaTRiX::const_reference const_reference;
    typedef typename MaTRiX::element_type element_type;
    typedef typename MaTRiX::element_type value_type;
    typedef element_type T;

    int lbound() const { return 1; }
    int dim(int d) const {  return A_.dim(d); }
    int dim1() const { return A_.dim1(); }
    int dim2() const { return A_.dim2(); }

    
    // constructors

    unitupperTriangularView(/*const*/ MaTRiX &A) : A_(A), zero(0), one(1) {}


    inline const_reference get(int i, int j) const
    { 
#ifdef SPUC_BOUNDS_CHECK
        assert(1<=i);
        assert(i<=A_.dim(1));
        assert(1<=j);
        assert(j<=A_.dim(2));
        assert(0<=i && i<A_.dim(0) && 0<=j && j<A_.dim(1));
#endif
        if (i<j)
            return A_(i,j);
        else if (i==j)
            return one;
        else 
            return zero;
    }


    inline const_reference operator() (int i, int j) const
    {
#ifdef SPUC_BOUNDS_CHECK
        assert(1<=i);
        assert(i<=A_.dim(1));
        assert(1<=j);
        assert(j<=A_.dim(2));
#endif
        if (i<j)
            return A_(i,j);
        else if (i==j)
            return one;
        else 
            return zero;
    }


#ifdef SPUC_USE_REGIONS 
  // These are the "index-aware" features

    typedef const_region2D< unitupperTriangularView<MaTRiX> > 
                    const_region;

    const_region operator()(const index1D &I,
            const index1D &J) const
    {
        return const_region(*this, I, J);
    }

    const_region operator()(int i1, int i2,
            int j1, int j2) const
    {
        return const_region(*this, i1, i2, j1, j2);
    }
#endif
// SPUC_USE_REGIONS
};

template <class MaTRiX>
upperTriangularView<MaTRiX> upper_triangular_view(
    /*const*/ MaTRiX &A)
{
    return upperTriangularView<MaTRiX>(A);
}


template <class MaTRiX>
unitupperTriangularView<MaTRiX> unit_upper_triangular_view(
    /*const*/ MaTRiX &A)
{
    return unitupperTriangularView<MaTRiX>(A);
}

template <class MaTRiX, class VecToR>
VecToR matmult(/*const*/ unitupperTriangularView<MaTRiX> &A, VecToR &x)
{
    int M = A.dim1();
    int N = A.dim2();

    assert(N == x.dim());

    int i, j;
    typename VecToR::element_type sum=0.0;
    VecToR result(M);

    int start = A.lbound();
    int Mend = M + A.lbound() -1 ;

    for (i=start; i<=Mend; i++)
    {
        sum = x(i);
        for (j=i+1; j<=N; j++)
            sum = sum + A(i,j)*x(j);
        result(i) = sum + x(i);
    }

    return result;
}

template <class MaTRiX, class VecToR>
inline VecToR operator*(/*const*/ unitupperTriangularView<MaTRiX> &A, VecToR &x)
{
    return matmult(A,x);
}


//********************** Algorithms *************************************



template <class MaTRiX>
std::ostream& operator<<(std::ostream &s, 
    /*const*/ upperTriangularView<MaTRiX>&A)
{
    int M=A.dim1();
    int N=A.dim2();

    s << M << " " << N << endl;

    for (int i=1; i<=M; i++)
    {
        for (int j=1; j<=N; j++)
        {
            s << A(i,j) << " ";
        }
        s << endl;
    }


    return s;
}

template <class MaTRiX>
std::ostream& operator<<(std::ostream &s, 
        /*const*/ unitupperTriangularView<MaTRiX>&A)
{
    int M=A.dim1();
    int N=A.dim2();

    s << M << " " << N << endl;

    for (int i=1; i<=M; i++)
    {
        for (int j=1; j<=N; j++)
        {
            s << A(i,j) << " ";
        }
        s << endl;
    }


    return s;
}

} // namespace SPUC





#endif 
//TRIANG_H


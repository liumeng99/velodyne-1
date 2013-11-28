// %pacpus:license{
// This file is part of the PACPUS framework distributed under the
// CECILL-C License, Version 1.0.
// %pacpus:license}
/// @file
/// @author  Firstname Surname <firstname.surname@utc.fr>
/// @date    Month, Year
/// @version $Id: ublas.hpp 76 2013-01-10 17:05:10Z kurdejma $
/// @copyright Copyright (c) UTC/CNRS Heudiasyc 2006 - 2013. All rights reserved.
/// @brief Brief description.
///
/// Detailed description.

#ifndef __BOOST_UBLAS__
#define __BOOST_UBLAS__

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <cmath>
#include "math_exception.hpp"

namespace math {
namespace ublas {

/// Definition of a matrix using double precision
typedef boost::numeric::ublas::matrix<double> Matrix;
/// Definition of a vector using double precision
typedef boost::numeric::ublas::vector<double> Vector;
/// Definition of empty vector using double precision
typedef boost::numeric::ublas::zero_vector<double> ZeroVector;
/// Definition of empty matrix using double precision
typedef boost::numeric::ublas::zero_matrix<double> ZeroMatrix;

/// Multiplication of two matrices.
///
/// @tparam T @todo Documentation
/// @param m1 ublas matrix
/// @param m2 ublas matrix
/// @returns ublas matrix
template<class T>
inline boost::numeric::ublas::matrix<T> operator *(const boost::numeric::ublas::matrix<T> & m1, const boost::numeric::ublas::matrix<T> & m2)
{
    return prod(m1,m2);
}

/// product of a vector by a matrix
///
/// @tparam T @todo Documentation
/// @param m ublas matrix
/// @param v ublas vector
/// @returns ublas vector
template<class T>
inline boost::numeric::ublas::vector<T> operator *(const boost::numeric::ublas::matrix<T> & m, const boost::numeric::ublas::vector<T> & v)
{
    return prod(m,v);
}

/// addition of two vectors
///
/// @tparam T @todo Documentation
/// @param v1 ublas vector
/// @param v2 ublas vector
/// @returns ublas vector
template<class T>
inline boost::numeric::ublas::vector<T> operator +(const boost::numeric::ublas::vector<T> & v1, const boost::numeric::ublas::vector<T> & v2)
{
    boost::numeric::ublas::vector<T> tmp = v1;
    return tmp+=v2;
}

/// subtraction of two vectors
///
/// @tparam T @todo Documentation
/// @param v1 ublas vector
/// @param v2 ublas vector
/// @returns ublas vector
template<class T>
inline boost::numeric::ublas::vector<T> operator -(const boost::numeric::ublas::vector<T> & v1, const boost::numeric::ublas::vector<T> & v2)
{
    boost::numeric::ublas::vector<T> tmp = v1;
    return tmp-=v2;
}

/// Transposes a matrix
///
/// @tparam T @todo Documentation
/// @param m ublas matrix
/// @returns ublas matrix
template<class T>
inline boost::numeric::ublas::matrix<T> Trans(boost::numeric::ublas::matrix<T> &m)
{
    return trans(m);
}

/// Converts a vector to a matrix.
///
/// @param v ublas vector
/// @returns ublas matrix
template <class T>
inline boost::numeric::ublas::matrix<T> vector2matrix(const boost::numeric::ublas::vector<T> &v)
{
    boost::numeric::ublas::matrix<T> tmp(v.size(),1);
    for(size_t i=0;i<v.size();i++) {
        tmp(i,0)=v[i];
    }
    return tmp;
}

/// compute the norm of a vector
///
/// @param v ublas vector
/// @returns norm value
template <class T>
inline double Norm(const boost::numeric::ublas::vector<T> & v){
    double norm =0;
    for(typename boost::numeric::ublas::vector<T>::const_iterator I=v.begin(); I != v.end(); ++I) {
        norm+=(*I)*(*I);
    }
    return std::sqrt(norm);
}

/// term by term multiplication of two vectors
///
/// @param v1 : ublas vector
/// @param v2 : ublas vector
/// @returns ublas vector
template <class T>
inline boost::numeric::ublas::vector<T> Mult(const boost::numeric::ublas::vector<T> & v1, const boost::numeric::ublas::vector<T> & v2)
{
    if(v1.size()!=v2.size()) {
        throw math_error("Dot(v1,v2) : vectors must have the same size");
    }
    boost::numeric::ublas::vector<T> v(v1.size());
    for(size_t i=0;i<v1.size();i++) {
        v[i]=v1[i]*v2[i];
    }
    return v;
}

/// dot product
///
/// @param v1 ublas vector
/// @param v2 ublas vector
/// @returns dot product value
template <class T>
inline double Dot(const boost::numeric::ublas::vector<T> & v1, const boost::numeric::ublas::vector<T> & v2)
{
    if(v1.size()!=v2.size()) {
        throw math_error("Dot(v1,v2) : vectors must have the same size");
    }
    double dot=0;
    for(size_t i=0;i<v1.size();i++) {
        dot+=v1[i]*v2[i];
    }
    return dot;
}

/// matrix inversion using LU decomposition
/// @param m ublas matrix
/// @returns ublas matrix
inline Matrix InvLU(const Matrix &m)
    throw(math_error)
{
    using namespace boost::numeric::ublas;
    typedef permutation_matrix<std::size_t> pmatrix;

    if(m.size1() != m.size2()) throw math_error("Inv(m): matrix must be square");

    // create a working copy of the input
    Matrix A(m);
    // create a permutation matrix for the LU-factorization
    pmatrix pm(A.size1());
    // perform LU-factorization
    int res = lu_factorize(A,pm);
    if( res != 0 )  throw math_error("Inv(m) : singular matrix");
    // create identity matrix of "inverse"
    Matrix inverse = identity_matrix<double>(A.size1());
    // backsubstitute to get the inverse
    lu_substitute(A, pm, inverse);

    return inverse;
}

////////////////////////////////////////////////////////////////////////////////
///////////////////////// QR  DECOMPOSITION ////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/// @todo Documentation
/// @tparam T @todo Documentation
template<class T>
bool InvertMatrix (const boost::numeric::ublas::matrix<T>& input, boost::numeric::ublas::matrix<T>& inverse)
{
    using namespace boost::numeric::ublas;
    typedef permutation_matrix<std::size_t> pmatrix;
    // create a working copy of the input
    matrix<T> A(input);
    // create a permutation matrix for the LU-factorization
    pmatrix pm(A.size1());

    // perform LU-factorization
    int res = lu_factorize(A,pm);
    if( res != 0 ) return false;

    // create identity matrix of "inverse"
    inverse.assign(boost::numeric::ublas::identity_matrix<T>(A.size1()));

    // backsubstitute to get the inverse
    lu_substitute(A, pm, inverse);

    return true;
}

/// @todo Documentation
/// @tparam T @todo Documentation
template<class T>
void TransposeMultiply(const boost::numeric::ublas::vector<T>& vector,
    boost::numeric::ublas::matrix<T>& result,
    size_t size)
{
    result.resize (size,size);
    result.clear ();
    for(unsigned int row=0; row< vector.size(); ++row)
    {
        for(unsigned int col=0; col < vector.size(); ++col)
            result(row,col) = vector(col) * vector(row);

    }
}

/// @todo Documentation
/// @tparam T @todo Documentation
template<class T>
void HouseholderCornerSubstraction (boost::numeric::ublas::matrix<T>& LeftLarge,
    const boost::numeric::ublas::matrix<T>& RightSmall)
{
    using namespace boost::numeric::ublas;
    using namespace std;
    if(
        !(
        (LeftLarge.size1() >= RightSmall.size1())
        && (LeftLarge.size2() >= RightSmall.size2())
        )
        )
    {
        cerr << "invalid matrix dimensions" << endl;
        return;
    }

    size_t row_offset = LeftLarge.size2() - RightSmall.size2();
    size_t col_offset = LeftLarge.size1() - RightSmall.size1();

    for(unsigned int row = 0; row < RightSmall.size2(); ++row )
        for(unsigned int col = 0; col < RightSmall.size1(); ++col )
            LeftLarge(col_offset+col,row_offset+row) -= RightSmall(col,row);
}

/// @todo Documentation
/// @tparam T @todo Documentation
template<class T>
void QR (const boost::numeric::ublas::matrix<T>& M,
    boost::numeric::ublas::matrix<T>& Q,
    boost::numeric::ublas::matrix<T>& R)
{
    using namespace boost::numeric::ublas;
    using namespace std;

    if(
        !(
        (M.size1() == M.size2())
        )
        )
    {
        cerr << "invalid matrix dimensions" << endl;
        return;
    }
    size_t size = M.size1();

    // init Matrices
    matrix<T> H, HTemp;
    HTemp = identity_matrix<T>(size);
    Q = identity_matrix<T>(size);
    R = M;

    // find Householder reflection matrices
    for(unsigned int col = 0; col < size-1; ++col)
    {
        // create X vector
        boost::numeric::ublas::vector<T> RRowView = boost::numeric::ublas::column(R,col);
        vector_range< boost::numeric::ublas::vector<T> > X2 (RRowView, range (col, size));
        boost::numeric::ublas::vector<T> X = X2;

        // X -> U~
        if(X(0) >= 0)
            X(0) += norm_2(X);
        else
            X(0) += -1*norm_2(X);

        HTemp.resize(X.size(),X.size(),true);

        TransposeMultiply(X, HTemp, X.size());

        // HTemp = the 2UUt part of H
        HTemp *= ( 2 / inner_prod(X,X) );

        // H = I - 2UUt
        H = identity_matrix<T>(size);
        HouseholderCornerSubstraction(H,HTemp);

        // add H to Q and R
        Q = prod(Q,H);
        R = prod(H,R);
    }
}
//////////////////////////////////////////////////////////////////////////////////////////::

/// Matrix inversion using QR decomposition
/// @param m ublas matrix
/// @returns ublas matrix
inline Matrix InvQR(const Matrix &m)
    throw(math_error)
{
    using namespace boost::numeric::ublas;

    if(m.size1() != m.size2()) throw math_error("Inv(m): matrix must be square");
    Matrix Q(m), R(m), Rinv(m);
    QR (m,Q,R);
    for( int i = 0 ; i < R.size1() ; i++ )
        for( int j = 0 ; j < R.size2() ; j++ )
            if( R(i,j) < 1e-10 )
                R(i,j) = 0;
    InvertMatrix(R,Rinv);
    return Rinv*Trans(Q);
}


/// compute matrix determinant using LU decomposition
/// @param m ublas matrix
/// @returns ublas matrix
inline double DetLU(const Matrix & m)
    throw(math_error)
{
    using namespace boost::numeric::ublas;
    typedef permutation_matrix<std::size_t> pmatrix;


    if(m.size1() != m.size2()) throw math_error("Determinant(m): matrix must be square");

    // create a working copy of the input
    Matrix A(m);
    // create a permutation matrix for the LU-factorization
    pmatrix pm(m.size1());
    // perform LU-factorization
    int res = lu_factorize(A, pm);
    if( res != 0 )  throw math_error("Determinant(m) : singular matrix");
    //compute determinant
    double det = 1.0;
    for (std::size_t i=0; i < pm.size(); ++i) {
        if (pm(i) != i)
            det *= -1.0;
        det *= A(i,i);
    }
    return det;
}


/// output stream function
inline std::ostream& operator << (std::ostream& ostrm, const Matrix & m)
{
    for (size_t i=0; i < m.size1(); i++)
    {
        ostrm << '['<<'\t';
        for (size_t j=0; j < m.size2(); j++)
        {
            double x = m(i,j);
            ostrm << x << '\t';
        }
        ostrm << ']'<< std::endl;
    }
    return ostrm;
}

/// output stream function
inline std::ostream& operator << (std::ostream& ostrm, const Vector & v)
{
    for (size_t i=0; i < v.size(); i++)
    {
        ostrm << '['<<'\t';
        double x = v(i);
        ostrm << x << '\t';
        ostrm << ']'<< std::endl;
    }
    return ostrm;
}

} // namespace ublas
} // namespace math

#endif // __BOOST_UBLAS__

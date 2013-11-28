// %pacpus:license{
// This file is part of the PACPUS framework distributed under the
// CECILL-C License, Version 1.0.
// %pacpus:license}
/// @file
/// @author  Firstname Surname <firstname.surname@utc.fr>
/// @date    Month, Year
/// @version $Id: distributions.hpp 76 2013-01-10 17:05:10Z kurdejma $
/// @copyright Copyright (c) UTC/CNRS Heudiasyc 2006 - 2013. All rights reserved.
/// @brief Brief description.
///
/// Detailed description.

#ifndef __DISTRIBUTIONS_HPP__
#define __DISTRIBUTIONS_HPP__

#include "ublas.hpp"
#include <boost/math/distributions.hpp>

namespace math{
namespace distributions{


template <class RealType> 
inline RealType pdf(const boost::math::normal_distribution<RealType> & dist , boost::numeric::ublas::vector<RealType> & v){
 return pdf(dist,v[0]);
} 

template <class RealType> 
inline RealType pdf(const boost::math::uniform_distribution<RealType> & dist , boost::numeric::ublas::vector<RealType> & v){
 return pdf(dist,v[0]);
} 


/*!
 *\class multivariate_normal_distribution
 *\brief This clas describes a multivariate normal distribution
 */
template < class RealType =double >
class multivariate_normal_distribution{
public :
 
 /*!
  *\brief Default constructor
  */
  multivariate_normal_distribution(boost::numeric::ublas::vector<RealType> mean,boost::numeric::ublas::matrix<RealType> cov){

    using namespace boost::numeric::ublas;

    if(mean.size()==cov.size1() && mean.size()==cov.size2()){
      m_mean=mean;
      m_cov=cov;
    }else throw math_error("Multivariate normal distribution : the mean vector of covariance matrix must have the same size");


    typedef permutation_matrix<std::size_t> pmatrix;
    // create a working copy of the input
    matrix<RealType> A(m_cov);
    // create a permutation matrix for the LU-factorization
    pmatrix pm(A.size1());
    // perform LU-factorization
    int res = lu_factorize(A,pm);
    if( res != 0 )  throw math_error("Pdf function : covariance matrix is a singular matrix");

    // create identity matrix of "inverse"
    m_invcov = identity_matrix<double>(A.size1());
    // backsubstitute to get the inverse
   lu_substitute(A, pm, invcov);

   //compute determinant
   m_detcov = 1.0;
     for (std::size_t i=0; i < pm.size(); ++i) {
       if (pm(i) != i)
          m_detcov *= -1.0;
       m_detcov *= A(i,i);
     }

  }

 /*!
  *\brief Get the inversion of covariance matrix 
  */
 boost::numeric::ublas::matrix<RealType> invcov() const {
  return m_invcov;
 }

 /*!
  *\brief Get determinant of covariance matrix
  */
 boost::numeric::ublas::matrix<RealType> detcov() const {
  return m_detcov;
 }


 /*!
  *\brief Get mean vector
  */
 boost::numeric::ublas::vector<RealType> mean() const {
  return m_mean;
 }

 /*!
  *\brief Get covariance matrix
  */
 boost::numeric::ublas::matrix<RealType> cov() const {
  return m_cov;
 }




private :




 /*!
  *\brief mean vector
  */
 boost::numeric::ublas::vector<RealType> m_mean;

 /*!
  *\brief covariance matrix
  */
 boost::numeric::ublas::matrix<RealType> m_cov;


 /*!
  *\brief covar
  */
 boost::numeric::ublas::matrix<RealType> m_invcov;

 /*!
  *\brief cov matrix
  */
 double m_detcov;

};

typedef   multivariate_normal_distribution<double> mvnormal;


/*!
 *\fn inline RealType pdf(const multivariate_normal_distribution<RealType>& dist, const boost::numeric::ublas::vector<RealType> & x)
 *\brief Compute probability density function for a multivariate normal distribution
 */
template <class RealType>
inline RealType pdf(const multivariate_normal_distribution<RealType>& dist, const boost::numeric::ublas::vector<RealType> & x)
{

    boost::numeric::ublas::vector<RealType> mean = dist.mean();
    boost::numeric::ublas::matrix<RealType> cov= dist.cov();
    boost::numeric::ublas::matrix<RealType> invcov =dist.invcov();
    double detcov= dist.detcov;


   RealType exponent = - Dot(x-mean, invcov*(x-mean));
   exponent /= 2 ;

   RealType result = std::exp(exponent);
   result /= std::sqrt(pow(2*M_PI,mean.size())*std::abs(detcov));

   return result;
} // pdf


/*!
 *\class multivariate_uniform_distribution
 *\brief This clas describes a multivariate normal distribution
 */
template < class RealType =double >
class multivariate_uniform_distribution{
public :
 
 /*!
  *\brief Default constructor
  */
  multivariate_uniform_distribution(boost::numeric::ublas::vector<RealType> lower,boost::numeric::ublas::matrix<RealType> upper){

    if(upper.size()!= lower.size()) throw math_error("Multivariate uniform distribution : the upper vector and the loxer vector must have the same size");
    for (size_t i=0;i<upper.size();i++)
      if(lower[i]>upper[i]) throw math_error("Multivariate uniform distribution : the lower vector is not lower than upper vector");

    m_lower=lower;
    m_upper=upper;
  }
 private :

 /*!
  *\brief Lower vector
  */
 boost::numeric::ublas::vector<RealType> m_lower;

 /*!
  *\brief Upper vector 
  */
 boost::numeric::ublas::matrix<RealType> m_upper;

};

typedef multivariate_normal_distribution<double> mvuniform;


/*!
 *\fn inline RealType pdf(const multivariate_uniform_distribution<RealType>& dist, const boost::numeric::ublas::vector<RealType> & x)
 *\brief Compute probability density function for a multivariate uniform distribution
 */
template <class RealType>
inline RealType pdf(const multivariate_uniform_distribution<RealType>& dist, const boost::numeric::ublas::vector<RealType> & x)
{

   double result =1;

   boost::numeric::ublas::vector<RealType> upper = dist.upper();
   boost::numeric::ublas::matrix<RealType> lower= dist.lower();


   for(size_t i=0;i<upper.size();i++){
     if(x<upper[i] && x[i]>lower[i]) result/=(upper[i]-lower[i]);
     else {result=0; break;}
  }

  return result;
} // pdf

};
};
#endif

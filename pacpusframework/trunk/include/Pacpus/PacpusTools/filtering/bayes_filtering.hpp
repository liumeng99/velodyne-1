// %pacpus:license{
// This file is part of the PACPUS framework distributed under the
// CECILL-C License, Version 1.0.
// %pacpus:license}
/// @file
/// @author  Firstname Surname <firstname.surname@utc.fr>
/// @date    Month, Year
/// @version $Id: bayes_filtering.hpp 76 2013-01-10 17:05:10Z kurdejma $
/// @copyright Copyright (c) UTC/CNRS Heudiasyc 2006 - 2013. All rights reserved.
/// @brief Brief description.
///
/// Detailed description.

#ifndef __BAYES_FILTERING__
#define __BAYES_FILTERING__

#include "filter_exception.hpp"

namespace filter {

   /*!
    * \enum Filters
    * \brief Enumeration of bayes filters
    */
   enum Filters { KF, EKF, UKF, GSF, PF };


    /*!
    * \class BayesDynamicEquation 
    * \brief This class describes a basic dynamic equation in bayes filtering scheme \n
    * Class S describe the filter state \n
    * Class I describe middle variable stored in the filter state \n 
    * ( for exmaple in  particle scheme, the state vector (S) is a set of particle (I) ) \n   
    */
   template <class S, class I> class BayesDynamicEquation{
     public :

        /*!
        * \brief virtual method where the a priori state vector must be computed
        * \param in : the state vector at time k-1  
        * \param out : the state vector at time k 
        */  
       virtual void Predict(S *in,S *out)=0;
 
       /*!
        * \brief virtual method where parameters of the dynamic equation must be evaluated 
        * \param  s : the middle variable at time k-1 
        */
       virtual void EvaluateParameters(I *s)=0;

        
       /*!
        * \brief Destructor 
        */ 
       virtual ~BayesDynamicEquation(){}

   }; 


    /*!
    * \class BayesMeasureEquation
    * \brief This class describes a basic measure equation in bayes filtering scheme \n
    * Class S describe the filter state \n
    * Class I describe middle variable stored in the filter state  \n
    * ( for exmaple in  particle scheme, the state vector (S) is a set of particle (I) ) \n   
    */
   template <class S, class I> class BayesMeasureEquation{
       public :

       /*!
        * \brief virtual method where parameters of the measure equation must be evaluated 
        * \param s : the meedle variable at time k 
        */
        virtual void EvaluateParameters(I *s)=0;

        /*!
        * \brief virtual method where the a posteriori state vector must be computed
        * \param in : the a priori state vector at time k  
        * \param out : the a posteriori state vector at time k 
        */      
       virtual void Update(S *in,S *out)=0;

       /*!
        * \brief  Destructor 
        */ 
       virtual ~BayesMeasureEquation(){}

   };

} // namespace filter

#endif // __BAYES_FILTERING__

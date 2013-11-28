// %pacpus:license{
// This file is part of the PACPUS framework distributed under the
// CECILL-C License, Version 1.0.
// %pacpus:license}
/// @file
/// @author  Firstname Surname <firstname.surname@utc.fr>
/// @date    Month, Year
/// @version $Id: particle_filtering.hpp 76 2013-01-10 17:05:10Z kurdejma $
/// @copyright Copyright (c) UTC/CNRS Heudiasyc 2006 - 2013. All rights reserved.
/// @brief Brief description.
///
/// Detailed description.

#ifndef __PARTICLE_FILTERING_BASE__
#define __PARTICLE_FILTERING_BASE__

#include "bayes_filtering.hpp"
#include "../math/ublas.hpp"
#include "../math/rng.hpp"
#include "../math/distributions.hpp"

namespace filter{
namespace particle {

   using namespace math;
   using namespace ublas;
   using namespace rng;
   using namespace distributions;


   /*! 
   * \class Particle
   * \brief This class describe a particle \n
   *  A particle is constituted by : 1) a state vector and 2) importance weight \n
   */
  class Particle {
    public :
      /*!
       * \brief Constructor 
       * \param state_size : the size of the state vector
       * \param weight_ : the initial weight of the particle  
       */
      Particle(const size_t state_size, const double & weight_){
       X=ZeroVector(state_size);
       weight=weight_;
      }

      Vector X; /*!< state vector */

      double weight; /*!< particle weight */
  
   };

   /*! \class ParticleSet
   *   \brief This class describe a set of particles \n
   *   A set of particle is reprented by a vector of particles \n
   *   somes methods can be applied to the set of particles like : \n
   *   estimate computation, resampling scheme or normalization method \n
   */

  template<class P=Particle> class ParticleSet {
    public :
    
     std::vector<P> particles;  /*!< vector of particles */

    
     std::vector<P> particles_; /*!< temporary vector of particles used in resample scheme */


     /*!
      * \brief Allocate the set of paticles  
      * \param nparticle : number of particles
      */
     void Allocator(const size_t &nparticle);

     /*!
      *  \brief Resample the set of particles  \n
      *  \param threshold : threshold parameter  \n
      *  The resampling scheme is the reasmpling scheme proposed by Kitawaga \n 
      *  This can be overloaded to use an another resampling scheme  \n 
      */
     virtual void Resample(const double & threshold=1.0);
   
     /*!
      * \brief Compute the number of effective samples in the particle set 
      * \return number of effective samples
      */ 
     double Ness();

    /*! 
     *  \brief Get the number of particles 
     *  \return number of particles 
     */
    size_t ParticlesNo(){ return particles.size(); }

    /*!
     * \brief Compute the estimate
     * \return a vector double describing the estimate 
     */
    Vector Estimate ();   

    /*!
     * \brief Normalize the weights of each particles
     */
    void NormalizeWeights();

    /*!
     * \brief Destructor
     */
    virtual ~ParticleSet(){};

    protected :

    /*!
    * \brief Compute the cumulative sum of the weight of each particles
    * \return a vector of double describing this cumulative sum 
    */ 
      std::vector<double> WeightCumSum();

    /*!
    * \brief Create the a random cumulative sum  
    * \return a vector of double describing this cumulative sum 
    */  
    std::vector<double> RandomCumSum();

  };


   //Particle set memeber functions 
  template<class P> void ParticleSet<P>::Allocator(const size_t &nparticles){
     particles.resize(nparticles); 
     particles_.resize(nparticles);
   }

   template<class P> double ParticleSet<P>::Ness(){
     double ness =0;
     for(typename std::vector<P>::iterator I=particles.begin();I!=particles.end();I++) 
        ness+=(*I).weight*(*I).weight;

     if(ness==0)throw filter_error("In particle filter :: number effective sample computation ->  ness is equal to infinity"); 

     return 1/ness;
   }


   template<class P> std::vector<double> ParticleSet<P>::WeightCumSum(){
       double sum=0;
       std::vector<double> cumsum(particles.size());
       for(size_t i=0;i<particles.size();i++){ 
         sum+=particles[i].weight;
         cumsum[i]=sum; 
       }
       return cumsum;
   }

   template<class P> void ParticleSet<P>::NormalizeWeights(){
     
    double sum=0;
    for(typename std::vector<P>::iterator I=particles.begin();I!=particles.end();I++)
       sum+=(*I).weight;


    if(sum==0) throw filter_error("In particle filter :: normalization of weight ->  weight sum is equal to 0"); 

    for(typename std::vector<P>::iterator I=particles.begin();I!=particles.end();I++)
        (*I).weight=(*I).weight/sum;

   }

   template<class P> std::vector<double> ParticleSet<P>::RandomCumSum(){
       double sum=0;
       std::vector<double> cumsum(particles.size());
       uniform_rng UD;
       for(size_t i=0;i<particles.size();i++){ 
         sum+=UD();
         cumsum[i]=sum; 
       }
       
       //normalization 
       for(size_t i=0;i<particles.size();i++)cumsum[i]=cumsum[i]/sum;

       return cumsum;
   }


  template<class P> void ParticleSet<P>::Resample(const double & threshold){
     size_t N=ParticlesNo();
     if(Ness()<=N*threshold){ 
       // resample method by Carpenter
       std::vector<double> Q=WeightCumSum();
       std::vector<double> T=RandomCumSum();

       // select the best particle
       std::vector <size_t> ind(N);
       unsigned int j=0;
       for(unsigned int i=0; i <N; i++){
         do{
 	  if(T[i] < Q[j]){ind[i] = j;break;}
 	  j++;
         }while(j<N);
       }

       // build the new particle set  
       for(unsigned int i=0; i <N; i++){
         particles_[i]=particles[ind[i]];
         particles_[i].weight=1.0/static_cast<double>(N);
       }

       // swap the particle set
       particles.swap(particles_);
     }    
  }
 
 

   template<class P> Vector ParticleSet<P>::Estimate(){
     Vector estimate = ZeroVector(particles[0]->X.size());
     for(typename std::vector<P>::iterator I=particles.begin();I!=particles.end();I++)
       estimate+=(*I).X*(*I).weight;
     return estimate;
   }


   /*!
    * \class DynamicEquation
    * \brief This class describe a basic dynamic equation for particle filtering \n
    * where : class  S is  a set particle \n
    *       : class  I is a particle \n
    *       : class  D is a random generator used to draw input data \n 
    */ 
  template <class D, template <class> class S=ParticleSet, class P=Particle> class DynamicEquation:public BayesDynamicEquation< S<P>, P >{
   public :

       /*!
        * \brief virtual method where matrices of state system must be allocated 
        * \param state_size : the size of the state vector of each particle
        * \param data_size : the size of the input vector 
        */  
       virtual void Allocator(const size_t &state_size,const size_t &data_size)=0;

       /*!
        * \brief virtual method where parameters of the dynamic equation must be  evaluated 
        * \param s : set of particles at time k-1
        *
        */
       virtual void EvaluateParameters( P *s)=0;
  
       /*!
        * \brief virtual method where the a priori state vector must be computed
        * \param in : the set of particles at time k-1  
        * \param out : the set of particles  at time k 
        */
       virtual void Predict(S<P> *in, S<P> *out)=0;
 

       /*! 
        * \brief Destructor 
        */ 
       virtual ~DynamicEquation(){}

       /*!
        * \brief Get/Set random generator used to draw input random U 
        */ 
       D & URNG(){return _URNG;}

     protected :

       D _URNG; /*!< Random generator used to drawing input data*/
 
       Vector _Urand; /*!< Vector where random input data are stored */

   };




    /*!
    * \class LinearDynamicEquation
    * \brief This class describe a linear dynamic equation \n
    *   X(k+1)(i) =A*X(k)(i)+B*U(k)(i) \n
    *   X(.)(i) = the state of the particle i \n
    *   A = state matrix \n
    *   B = entrie matrix \n
    *   U = input random vector \n
    */
    template <class D,template <class> class S=ParticleSet, class P=Particle> class LinearDynamicEquation : public DynamicEquation<D,S,P>{
      public :
       /*!
        * \brief virtual method where matrices of state system must be allocated 
        * \param state_size : the size of the state vector of each particle
        * \param data_size : the size of the input vector 
        */ 
       void Allocator(const size_t &state_size,const size_t &data_size);
      
       /*!
        * \brief virtual method where parameters of the dynamic equation must be  evaluated 
        * \param s : set of particles at time k-1
        */
        virtual void EvaluateParameters(P *s)=0;

       /*!
        * \brief virtual method where the a priori state vector must be computed
        * \param in : the set of particles at time k-1  
        * \param out : the set of particles  at time k 
        */
        virtual void Predict(S<P> *in,S<P> *out);

        /*!
         * \brief Destructor 
         */
        virtual ~LinearDynamicEquation(){}

        /*!
         * \brief Get/Set a constant data in A matrix 
         */ 
        double & A(const int &row, const int &column){return _A(row,column);}

        /*!
         * \brief Get/Set a constant data in B matrix 
         */ 
        double & B(const int &row, const int &column){return _B(row,column);}

     protected :  

       Matrix _A; /*!< A matrix */

       Matrix _B; /*!< A matrix */

   };


   // Particle linear dynamic equation member functions
   template <class D, template <class> class S,class P> void LinearDynamicEquation<D,S,P>::Allocator(const size_t &state_size,const size_t &data_size){
      DynamicEquation<D,S,P>::_Urand=ZeroVector(data_size);
      _A=ZeroMatrix(state_size,state_size);
      _B=ZeroMatrix(state_size,data_size);
   }
      

   template <class D, template <class> class S,class P> void LinearDynamicEquation<D,S,P>::Predict(S<P> *in,S<P> *out){

          for(size_t i=0;i<in->particles.size();i++){
            EvaluateParameters(&in->particles[i]); 

            out->particles[i]->X=_A*in->particles[i]->X+_B*DynamicEquation<D,S,P>::_Urand;
            out->particles[i]->weight=in->particles[i]->weight;
          }
   }


     /*!
     * \class NonLinearDynamicEquation
     * \brief  This class describe a linear dynamic equation 
     *   \n 
     *   X(k+1)(i) =f(X(k)(i)+U(k)(i))  \n
     *   X(.)(i) = the state of the particle i \n 
     *   A = state matrix \n
     *   B = entrie matrix \n
     *   U = input random vector \n
     */
    template <class D, template <class> class S,class P> class NonLinearDynamicEquation : public DynamicEquation<D,S,P>{
       public :
       /*!
        * \brief virtual method where matrices of state system must be allocated 
        * \param state_size : the size of the state vector of each particle
        * \param data_size : the size of the input vector 
        */ 
       void Allocator(const size_t &state_size,const size_t &data_size);

       /*!
        * \brief virtual method where parameters of the dynamic equation must be  evaluated 
        * \param s : set of particles at time k-1
        * f=
        */
       virtual void EvaluateParameters(P *s)=0;
      
       /*!
        * \brief virtual method where the a priori state vector must be computed
        * \param in : the set of particles at time k-1  
        * \param out : the set of particles  at time k 
        */
      virtual void Predict(S<P> *in,S<P> *out);

      /*!
       * \brief Destructor 
       */
      virtual ~NonLinearDynamicEquation(){}
       

      protected:   

      Vector _f; /*!< Matrix f where result of (f(X,U)) is stored */  

   };

   // Particle non linear dynamic equation member functions
   template <class D,template <class> class S,class P> void NonLinearDynamicEquation<D,S,P>::Allocator(const size_t &state_size,const size_t &data_size){
      DynamicEquation<D,S,P>::_Urand=ZeroVector(data_size);
      _f=ZeroVector(state_size);
   }   

 

    template <class D,template <class> class S,class P> void NonLinearDynamicEquation<D,S,P>::Predict(S<P> *in,S<P> *out){
         for(size_t i=0;i<in->particles.size();i++){
            EvaluateParameters(&in->particles[i]); 
           
            out->particles[i].X=_f;
            out->particles[i].weight=in->particles[i].weight;

         }
   }

   /*!
    * \class MeasureEquation
    * \brief This clas describe a basic measure equation particle filtering
    * \n
    * where : class  S is  a set particle \n
    *       : class  I is a particle \n
    *       : class  D is the distribution of probability of observation data \n
    */
   template <class D , template <class> class S=ParticleSet, class P=Particle> class MeasureEquation: public BayesMeasureEquation< S<P>, P >{
   public :
 
       /*!
        * \brief virtual method where matrices of state system must be allocated 
        * \param state_size : the size of the state vector of each particle
        * \param data_size : the size of the input vector 
        */ 
       virtual void Allocator(const size_t &state_size,const size_t &data_size)=0;


       /*!
        * \brief virtual method where parameters of the measure equation must be  evaluated 
        * \param s : set of particles at time k
        */
       virtual void EvaluateParameters(P *s)=0;

      /*!
        * \brief virtual method where the a posteriori state vector must be computed
        * \param in : the a priori set of particles at time k  
        * \param out : the a posteriori set of particles at time k 
        */   
       virtual void Update(S<P> *in,S<P> *out)=0;
 
       /*!
        * \brief Destructor 
        */ 
       virtual ~MeasureEquation(){};

       /*!
        * \brief Get/Set the distribution of probability  of data observation Z 
        */ 
       D & ZDistribution(){return _ZDistribution;}
     
       protected :

       D _ZDistribution; /*!< The distribution of probability of observation data Z*/
   };



    /*!
    * \class LinearMeasureEquation
    *  \brief This class describe a linear measure equation 
    *   \n
    *   X(k)(i)=H*Z(k) \n
    *   Z(k) = observation data \n
    *   => weigth update \n
    *   w(k+1)(i)=p(Z|HX(i))w(k)(i) \n
    *   w(.)(i)= weight of particle  i \n
    */
    template <class D, template <class> class S=ParticleSet, class P=Particle> class LinearMeasureEquation : public MeasureEquation< D,S,P >{
      public :
       /*!
        * \brief virtual method where matrices of state system must be allocated 
        * \param state_size : the size of the state vector of each particle
        * \param data_size : the size of the input vector 
        */ 
       void Allocator(const size_t &state_size,const size_t &data_size);


       /*!
        * \brief virtual method where parameters of the measure equation must be  evaluated 
        * \param s : set of particles at time k
        */
       virtual void EvaluateParameters(P *s)=0;

       /*!
        * \brief virtual method where the a posteriori state vector must be computed
        * \param in : the a priori set of particles at time k  
        * \param out : the a posteriori set of particles at time k 
        */   
       virtual void Update(S<P> *in,S<P> *out);

       /*!
        * \brief Destructor 
        */
       virtual ~LinearMeasureEquation(){}

       /*!
        * \brief Get/Set a constant data in observation matrix H 
        */ 
       double & H(int row, int column){return _H(row,column);}

      protected:  

       Matrix _H;  /*!< Observation matrix */
   };


   // Particle linear measure equation member functions
   template <class D,template <class> class S, class P> void LinearMeasureEquation<D,S,P>::Allocator(const size_t &state_size,const size_t &data_size){
        _H=ZeroMatrix(data_size,state_size);
   }

   template <class D,template <class> class S, class P> void LinearMeasureEquation<D,S,P>::Update(S<P> *in,S<P> *out){
         for(size_t i=0;i<in->particles.size();i++){
            EvaluateParameters(&in->particles[i]); 
           
            out->particles[i].weight=pdf(MeasureEquation<D,S,P>::_ZDistribution, _H*in->particles[i].X)*in->particles[i].weight;

         }
        out->NormalizeWeights();
   }



    /*!
    *  \class NonLinearMeasureEquation
    *  \brief This class describe a non linear measure equation 
    *   \n 
    *   Z(k)=h(X)(i) \n
    *   Z = observation data \n
    *   => weigth update \n
    *   w(k+1)(i)=p(Z|h(X(k)(i)))w(k)(i) \n
    *   w(.)(i)= weight of particle  i \n
    */
    template <class D,template <class> class S=ParticleSet, class P=Particle> class NonLinearMeasureEquation : public MeasureEquation<D,S,P>{
      public :
       /*!
        * \brief virtual method where matrices of state system must be allocated 
        * \param state_size : the size of the state vector of each particle
        * \param data_size : the size of the input vector 
        */ 
       void Allocator(const size_t &state_size,const size_t &data_size);

       /*!
        * \brief virtual method where parameters of the measure equation must be  evaluated 
        * \param s : set of particles at time k
        * h=    
        * H=
        */
       virtual void EvaluateParameters(P *s )=0;

       /*!
        * \brief virtual method where the a posteriori state vector must be computed
        * \param in : the a priori set of particles at time k  
        * \param out : the a posteriori set of particles at time k 
        */   
       virtual void Update(S<P> *in,S<P> *out); 
       
        /*!
         * \brief Destructor 
         */
       virtual ~NonLinearMeasureEquation(){}

      protected :

       Vector _h; /*!< vector where h(X) is stored */  

   };

    // Particle non linear measure equation member functions
    template <class D,template <class> class S, class P>  void NonLinearMeasureEquation<D,S,P>::Allocator(const size_t &state_size,const size_t &data_size){
        _h=ZeroVector(data_size);
    }


   template <class D,template <class> class S, class P> void NonLinearMeasureEquation<D,S,P>::Update(S<P> *in,S<P> *out){

         for(size_t i=0;i<in->particles.size();i++){
            EvaluateParameters(&in->particles[i]); 
          
            out->particles[i].weight=pdf(MeasureEquation<D,S,P>::_ZDistribution,_h)*in->particles[i].weight;

         }

        out->NormalizeWeights();
   }

} // namespace particle
} // namespace filter


#endif // __PARTICLE_FILTERING_BASE__

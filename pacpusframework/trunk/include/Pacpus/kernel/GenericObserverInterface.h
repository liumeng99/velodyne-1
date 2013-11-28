// %pacpus:license{
// This file is part of the PACPUS framework distributed under the
// CECILL-C License, Version 1.0.
// %pacpus:license}
/// @file
/// @author  Stephane Bonnet <firstname.surname@utc.fr>
/// @date    October, 2012
/// @version $Id: GenericObserverInterface.h 76 2013-01-10 17:05:10Z kurdejma $
/// @copyright Copyright (c) UTC/CNRS Heudiasyc 2006 - 2013. All rights reserved.
/// @brief Brief description.
///
/// Detailed description.
 
#ifndef DEF_PACPUS_GENERIC_OBSERVER_INTERFACE_H
#define DEF_PACPUS_GENERIC_OBSERVER_INTERFACE_H

namespace pacpus {

/** Base class for te GenericObserverInterface template.

    The purpose of this class is to be able to take generic pointers on
    GenericObserverInterface<T> class template instances.
*/
class GenericObserverBase {
public:
  virtual ~GenericObserverBase() {}
};

/** Template interface for generic observers.
  
    This is a pure virtual class that should be used as a base to classes 
    wishing to receive notifications from GenericObservable classes.
*/
template <typename T> 
class GenericObserverInterface : public GenericObserverBase {
public:
  virtual ~GenericObserverInterface() {};

  /** Update function.

      This function is called each time the observables notifies its observers
      of a change. It must be implemented by all classes obeying the 
      GenericObserverInterface interface.

      @param observable is a pointer to the observed object that was updated.
  */
  virtual void update(T* observable) = 0;
};

} // namespace pacpus


#endif // DEF_PACPUS_GENERIC_OBSERVER_INTERFACE_H
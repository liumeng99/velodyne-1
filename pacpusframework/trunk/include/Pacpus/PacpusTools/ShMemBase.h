// %pacpus:license{
// This file is part of the PACPUS framework distributed under the
// CECILL-C License, Version 1.0.
// %pacpus:license}
/// @file
/// @author  Firstname Surname <firstname.surname@utc.fr>
/// @date    Month, Year
/// @version $Id: ShMemBase.h 76 2013-01-10 17:05:10Z kurdejma $
/// @copyright Copyright (c) UTC/CNRS Heudiasyc 2006 - 2013. All rights reserved.
/// @brief Brief description.
///
/// Detailed description.

#ifndef DEF_PACPUS_SHMEMBASE_H
#define DEF_PACPUS_SHMEMBASE_H

/// Base class for shared memory objects.
class ShMemBase
{
public:
    /// Ctor
    ShMemBase()
    {
    }

    /// Dtor
    virtual ~ShMemBase()
    {
    }

    /// Use this method to get the data of the shared memory
    virtual void *read() = 0; 

    /// Use this method to write data in shared memory
    virtual void write(void *data, int size, unsigned long offset = 0) = 0; 

    /// Use this method to wait the incoming of new data
    /// you can specify a timeout in ms to avoid infinite blocking or 0 (infinite)
    /// @returns @b true if new data available before the timeout, @b false otherwise
    virtual bool wait(unsigned long timeout = 0) = 0; 

    /// To lock the access to the memory
    virtual void lockMemory() = 0; 

    /// To unlock the access to the memory
    virtual void unlockMemory() = 0; 

#ifdef WIN32
    /// Return the event handle under Windows 
    /// @todo ... under Linux ? 
    virtual void * getEventIdentifier() = 0; 
#endif 
  
protected:
    /// @todo Documentation
    void * shMem_;

private:
};

#endif // DEF_PACPUS_SHMEMBASE_H

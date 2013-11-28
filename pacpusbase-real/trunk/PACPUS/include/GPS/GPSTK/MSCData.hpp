#pragma ident "$Id: MSCData.hpp 70 2006-08-01 18:36:21Z ehagen $"



//============================================================================
//
//  This file is part of GPSTk, the GPS Toolkit.
//
//  The GPSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 2.1 of the License, or
//  any later version.
//
//  The GPSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GPSTk; if not, write to the Free Software Foundation,
//  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//  
//  Copyright 2004, The University of Texas at Austin
//
//============================================================================

//============================================================================
//
//This software developed by Applied Research Laboratories at the University of
//Texas at Austin, under contract to an agency or agencies within the U.S. 
//Department of Defense. The U.S. Government retains all rights to use,
//duplicate, distribute, disclose, or release this software. 
//
//Pursuant to DoD Directive 523024 
//
// DISTRIBUTION STATEMENT A: This software has been approved for public 
//                           release, distribution is unlimited.
//
//=============================================================================






/**
 * @file MSCData.hpp
 * Monitor station coordinate file data
 */

#ifndef MSCDATA_HPP
#define MSCDATA_HPP

#include <string>
#include "StringUtils.hpp"
#include "Triple.hpp"
#include "ECEF.hpp"
#include "DayTime.hpp"
#include "FFData.hpp"

namespace gpstk
{
      /** @addtogroup icd211group ICD-GPS-211 Classes */
      //@{

      /** 
       * Model for a Monitor Station Coordinates File Data Record
       */
   class MSCData : public gpstk::FFData
   {
   public:
         /// constructor
      MSCData() throw() : time(gpstk::DayTime(0.L)) {}
      
         /// destructor
      virtual ~MSCData() throw() {}
      
      gpstk::DayTime time;  ///< date of coordinates release from NIMA
      unsigned long station; ///< NIMA Monitor Station number (e.g. 85408)
      std::string mnemonic;  ///< NIMA Monitor Station identifier (e.g. ARLM)
      gpstk::DayTime refepoch;  ///< epoch of coordinates and velocities
      gpstk::DayTime effepoch;  ///< earliest epoch data is valid
      gpstk::ECEF coordinates;  ///< ECEF XYZ coordinates
      gpstk::Triple velocities; ///< XYZ velocities

         /// MSCData \b is data, so this always return \c true
      virtual bool isData() const { return true; }

   protected:
         /// Write an MSCData object in the format specified by the stream
      virtual void reallyPutRecord(gpstk::FFStream& ffs) const
         throw(std::exception, gpstk::FFStreamError,
               gpstk::StringUtils::StringException);
      
         /**
          * Retrieve an MSCData record from the given gpstk::FFStream.
          * If there is a problem reading from the stream, it
          * is reset to its original position and it's failbit is set.
          * @throws StringException when a gpstk::StringUtils functions fails
          * @throws gpstk::FFStreamError when exceptions(failbit) is set and
          *  a read or formatting error occurs. This also resets the
          *  stream to it pre-read position.
          */
      virtual void reallyGetRecord(gpstk::FFStream& ffs)
         throw(std::exception, gpstk::FFStreamError,
               gpstk::StringUtils::StringException);  
   }; // class MSCData

      //@}

} // namespace gpstk

#endif   
      
#ifndef _SerialCOM_Msg_NAV_SOL_Ublox_H
#define _SerialCOM_Msg_NAV_SOL_Ublox_H


#include <vector>
#include "structureUBX.h"
#include "SerialCOM_Msg.hpp"
#include "SerialCOM_Protocol.hpp"

namespace Ublox{

/*! \class SerialCOM_Msg_NAV_SOL
 *	\brief Allows to handle NAV-SOL messages coming from a Ublox GPS receiver
 *	\author Olivier LE MARCHAND
 *	\version 1.0
 *	\date February 2007
 *	\bug None
 *	\warning None
 *	
 *  Can identify an incoming message as a NAV-SOL message and extract information from\n
 *	Protocols definitions are available in the following document : \n
 *	Title:     ANTARIS Positioning Engine  \n
 *	Subtitle:  NMEA and UBX Protocol Specification  \n
 *	Doc Type:  Specification  \n
 *	Doc ID:    GPS.G3-X-03002-D \n 
 *	Firmware:  Version 5.00  
 *
 */

	class SerialCOM_Msg_NAV_SOL: public SerialCOM_Msg, public structNAV_SOL
	{
	public:

		SerialCOM_Msg_NAV_SOL(void);

		/*! \brief reset internal variables of the message after an error or the end of a 
		 *  message */
		/*! For more information see SerialCOM_Msg::reset*/
		void reset(void);
		
		/*! \brief Read the core of the message to check if the object is able to handle this
		 *  message*/
		/*! For more information see SerialCOM_Msg::identify*/
		int identify(vector<unsigned char> *pbuffer);
		
		/*! \brief extracts the data of the message contained in the buffer vector*/
		/*! For more information see SerialCOM_Msg::formatData*/
		int formatData(vector<unsigned char> *pbuffer);

		//! return the size of the structure structNAV_SOL
		int getSizeOfStruct(){return sizeof(structNAV_SOL);};

		int fromStructFile(void *buf);
		
		//! write the Data of the message in the DByte file
		virtual int writeDByte(road_time_t currentRoadtime, road_timerange_t currentTimerange);

	};

} // END namespace Ublox

#endif
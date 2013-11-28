/********************************************************************
//  created:    2006/08/07 - 14:14
//  filename:   VectorCanDriver.h
//
//  author:     Gerald Dherbomez
//
//  version:    $Id: VectorCanDriver.h 1203 2012-08-02 11:58:15Z morasjul $
//
//  purpose:    Inherits from the CanDriver class
//              Reimplement all the virtual members of the CanDriverBase 
//              class in order to be used with the Vector driver.  
//              platform : Windows specific
//
*********************************************************************/

#ifndef _VECTORCANDRIVER_H_
#define _VECTORCANDRIVER_H_

#include <windows.h>

#include "CanDriverBase.h"
#include "VCanD.h"

class VectorCanDriver
        : public CanDriverBase
{
public:
  VectorCanDriver(void);
  VectorCanDriver(unsigned int gHwTypeT, Vaccess gChannelMaskT, unsigned int gCanIdT, unsigned int gBitRateT,int gHwChannelT);
  VectorCanDriver(int channel);
  VectorCanDriver(int channel, unsigned int bitRate);
  VectorCanDriver(char * /*dummy1*/, char * /*dummy2*/) {} // DON'T USE
  ~VectorCanDriver(void);

  Vstatus initPort (void);
  Vstatus cleanUpPort (void);

  Vstatus AcceptId(int id);
  Vstatus AcceptIdRange(int deb,int fin);
  //Vstatus sendFrame (unsigned char, unsigned char, unsigned char *, int);
  //Vstatus receiveFrame (unsigned char *, unsigned char *, unsigned char *, int *);
  Vstatus sendFrame (struct CanFrame frame);
  Vstatus receiveFrame (struct CanFrame &frame);
  void waitReceivingFrame(void);

protected:
	
private:
  VportHandle		gPortHandle;		/*!< Handle to the AC2PCI CAN Card */
  Vaccess			  gChannelMask;		/*!< Mask to select the channel of the card (channel 0 or 1) */
  Vaccess			  gPermissionMask;	/*!< Mask of the channel selected */
  unsigned int	gCanId;				/*!< Identificator of the CAN frame send by the card */
  unsigned int	gBitRate;			/*!< Variable which defined the bitrate of the card */
  unsigned int	gHwType;			/*!< Defined the type of the hardware use for CAN communication. for the moment the only possible value is HWTYPE_CANAC2PCI for the AC2PCI CAN Card */
  int				    gHwChannel;			/*!< Defined the channel of the card what we want used (channel 0 or 1) */
  HANDLE			  gEventHandle;		/*!< Handle on a windows event for known if a CAN frame is present on the frame's buffer of the card */
};

#endif

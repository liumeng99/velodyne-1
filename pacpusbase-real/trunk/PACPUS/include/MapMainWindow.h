/*********************************************************************
// created:    2006/04/28 - 11:59
// filename:   MapMainWindow.h
//
// author:     Gerald Dherbomez
// 
// version:    $Id: MapMainWindow.h 300 2007-10-24 08:30:50Z gdherbom $
// 
// purpose:    The main window that displays the map
*********************************************************************/


#ifndef _MAPMAINWINDOW_H_
#define _MAPMAINWINDOW_H_

#include <qmainwindow.h>
#include "kernel/road_time.h"

// forward declarations
class MapView;

class MapMainWindow : public QMainWindow
{
  Q_OBJECT
    
public:
  MapMainWindow();
  ~MapMainWindow();
  bool openSVS(QString fileName = QString::null, QString unlockKey = QString::null); 
  
  
public slots:
  void init(); 
  
protected:
  MapView * mapView_; 
  void customEvent( QCustomEvent * e );

	
private:

};


#endif

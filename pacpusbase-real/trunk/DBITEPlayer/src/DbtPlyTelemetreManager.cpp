/*********************************************************************
// created:    2007/04/12 - 16:30

//
// author:     Elie Al Alam & Gerald Dherbomez
// 
// version:    $Id: DbtPlyTelemetreManager.cpp 447 2008-05-12 11:41:12Z gdherbom $
//
// purpose:    Dbite Player Telemetre Manager implementation
*********************************************************************/

#include <QApplication>

#include "kernel/ComponentManager.h"
#include "DbitePlayer/DbtPlyTelemetreManager.h"

// Construction de la fabrique de composant DbtPlyTrigger
static ComponentFactory<DbtPlyTelemetreManager>* factory = new ComponentFactory<DbtPlyTelemetreManager>("DbtPlyTelemetreManager"); 


DbtPlyTelemetreManager::DbtPlyTelemetreManager(QString name):DbtPlyFileManager (name)
{
}



DbtPlyTelemetreManager::~DbtPlyTelemetreManager()
{ 
}




void DbtPlyTelemetreManager::processData(road_time_t t, road_timerange_t tr , void *buf)
{
  bufTel = (float *)buf;
  if (buf==NULL)
   emit displayTel(0);
  else
  {
    emit displayTel(bufTel[90]);
    dataTelem.time = t;
    dataTelem.tr = tr;
    dataTelem.distance = bufTel;
    dataTelem.size = 181;
    #ifdef WIN32
    sharedMemory->setData(dataTelem);
    SetEvent(hd_data);
    #endif 
  }
}



ComponentBase::COMPONENT_CONFIGURATION DbtPlyTelemetreManager::configureComponent(XmlComponentConfig config)
{
  DbtPlyFileManager::configureComponent(config);
  return ComponentBase::CONFIGURED_OK;
}



void DbtPlyTelemetreManager::startActivity(){ 
  
  DbtPlyFileManager::startActivity();
  bufTel = (float *)malloc(181*sizeof(float));
  #ifdef WIN32
  hd_data=CreateEvent(NULL,TRUE ,FALSE,"new_data");
  sharedMemory = new DbtPlyIOShMemTel();
  #endif

  // user interface 
  if (ui_)
    displayUI(); 
}



void DbtPlyTelemetreManager::stopActivity()
{ 
  free(bufTel); 
  DbtPlyFileManager::stopActivity();
}




void DbtPlyTelemetreManager::displayUI()
{
  lcd_ = new QLCDNumber ();
  lcd_->setFixedSize(50,20);
  lcd_->setWindowTitle(componentName); 
  lcd_->show();
  QApplication::connect (this, SIGNAL(displayTel(double)) , lcd_ , SLOT(display(double)));
}

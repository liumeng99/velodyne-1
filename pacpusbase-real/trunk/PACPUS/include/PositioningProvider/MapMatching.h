/*********************************************************************
//  created:    2006/10/16 - 11:17
//  filename:   MapMatching.h
//
//  author:     Pierre-Michel Bonnifait Copyright Heudiasyc UMR UTC/CNRS 6599
//
//  version:    $Id: MapMatching.h 274 2007-08-31 11:08:30Z gdherbom $
//
//  purpose:    Class which make the MapMatching with BeNomad roads.
*********************************************************************/

#ifndef _MAPMATCHING_H_
#define _MAPMATCHING_H_

#include "../../include/PositioningProvider/struct.h"

class MapMatching : public QObject, public QThread, public ComponentBase
{
  Q_OBJECT  

public:
  MapMatching (const char*);
  ~MapMatching ();
  void						configureComponent(XmlComponentConfig );
  void						startActivity ();
  void						stopActivity ();
  struct s_MatchedPosition	sendMatchedPostion (struct s_MatchedPosition , int* result);
  void            init();
  void						run ();
  
signals:
  void						errMapMatching(double );
  
private slots:
  void            unlockRoadExtract(); 
  void            copyRoads(QString str); 


private:
  TB_2D_LONGPOINT				pointPlusProche();
  double						diffTheta(double , double );
  double						diffThetaCos(double t1, double t2);
  bool						goodWay(unsigned long );
  bool						isConnex(unsigned long );
  // connexMapMatch() return 1 if succeed, 0 otherwise
  int 						connexMapMatch();
  double						calculDist();
  int							giMeTheWay(TB_2D_LONGPOINT p1, TB_2D_LONGPOINT p2);
  double						sq(double nb);
  TB_2D_LONGPOINT				calculPos(SEGMENT , TB_2D_LONGPOINT );
  bool						isEgal(TB_2D_LONGPOINT , TB_2D_LONGPOINT );
  bool						chooseRoad(hds_polyline, SEGMENT, TB_2D_LONGPOINT );
  struct s_CalcBestTheta		calcBestTheta(hds_polyline , TB_2D_LONGPOINT );
  double						calcErr(TB_2D_LONGPOINT , SEGMENT );
  double						xMatche_;
  double						yMatche_;
  double						dist_;
  struct s_MapCache			mapCache_;
  class PositioningComputing*	myEKF_;
  class MapClientSocket* myMapClient_; 
  s_MatchedPosition			m_;
  QPtrList<SEGMENT>			myList_;
  unsigned long*				connexEnd_;
  unsigned long*				connexBegin_;
  unsigned long				nbConnexEnd_;
  unsigned long				nbConnexBegin_;
  bool						reinitMapMatch_;
  hds_polyline*				curRoad_;
  double						err_;
  SEGMENT						curSegment_;
  QSemaphore*          roadExtractlock_;
  
  QMutex mapUpdateMutex_;

};

#endif /* !_MAPMATCHING_H_ */
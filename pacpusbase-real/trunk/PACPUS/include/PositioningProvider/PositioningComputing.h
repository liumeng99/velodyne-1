/*********************************************************************
//  created:    2006/10/16 - 11:48
//  filename:   PositioningComputing.h
//
//  author:     Pierre-Michel Bonnifait Copyright Heudiasyc UMR UTC/CNRS 6599
//
//  version:    $Id: PositioningComputing.h 234 2007-04-13 16:06:09Z gdherbom $
//
//  purpose:    Definition of the Class which compute infomation
				from the sensors by a kalman filter
*********************************************************************/

#ifndef _POSITIONINGCOMPUTING_H_
#define _POSITIONINGCOMPUTING_H_

#include "struct.h"

class PositioningComputing : public ComponentBase, QThread
{
public:
	PositioningComputing (const char *);
	~PositioningComputing ();
	struct s_MatchedPosition	getPosition ();	//	Renvoie une copie de la structure
												//	avec les champs x, y et theta remplis
												//	apr�s calcul.
	void						init ();		//	Initialisation de la position de
												//	d�pard et des matrices.
	void						configureComponent(XmlComponentConfig config);
	void						startActivity ();
	void						stopActivity ();
	void						run ();
	void						initEKF ();
	int							nb_Inco;

private:	
	void						initGpsDiff ();
	void						initAbsDiff ();
	void						initGyroDiff ();
	void						calcDiff (struct s_SensorsData& );
	int							time_update_EKF ();
	int							measure_update_EKF ();
	void						setQbeta (float a, float b, float phi); //////////add gst
	struct s_SensorsData		initData(struct s_SensorsData);
	struct s_SensorsData		sensorsData_;		//	Donn�es des capteurs
	matrice						X_;					//	Matrice d'�tat du filtre de kalman
	matrice						Y_;					//	Matrice de mesure GPS
	matrice						P_;					//	Matrice de covariance de l'erreur
	matrice						U_;					//	Matrice d'entr�e
	matrice						Qgamma_;			//	Matrice du bruit d'entr�e
	matrice						Qalpha_;			//	Matrice du bruit du mod�le
	matrice						Qbeta_;				//	Matrice du bruit du GPS
	road_time_t					time_;				//	Temps de fin de calcul
	struct donnees_abs_zy*		diffAbs_;
	struct donnees_gps*			diffGps_;
	struct donnees_gyro*		diffGyro_;
	class PositioningSensors*	mySensors_;
	class PositionProvider*		myBigProvider_;
  class MapClientSocket*    myMapClient_; 
};

#endif /* !_POSITIONINGCOMPUTING_H_ */

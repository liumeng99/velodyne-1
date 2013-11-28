/*********************************************************************
//  created:    2006/10/24 - 11:55
//  filename:   positioningframe.h
//
//  author:     Pierre-Michel Bonnifait Copyright Heudiasyc UMR UTC/CNRS 6599
//
//  version:    $Id: positioningframe.h 277 2007-09-04 15:47:11Z gdherbom $
//
//  purpose:    The Definition of the Main frame of the program
//				Child Class of the Positioning_Provider_frame generated by QtDesigner
*********************************************************************/

#ifndef _POSITIONINGFRAME_H_
#define _POSITIONINGFRAME_H_

#include "struct.h"
#include "../moc/positioning_provider_frame.h"

#define	ERR_PLOT_SIZE 101


class PositioningFrame : public Positioning_Provider_frame
{
	Q_OBJECT

public:
	PositioningFrame ();
	void					init();

public slots:
	void					drawGyro();
	void					drawAbs();
	void					setServerMode();
	void					startTraitment();
	void					stopTraitment();
	void					errAff(double err);

private:
	ComponentManager* mng_;
	bool started_;
	PositioningSensors* pos_;
	class PositionProvider* pro_;
	class MapMatching* map_;
	struct s_SensorsData data_;
	int cpt_;
	class LogFile* mylog_;
  double errArray_[ERR_PLOT_SIZE];
  double timeArray_[ERR_PLOT_SIZE];
  double timeThresholdArray_[2]; 
  double thresholdArray_[2]; 
};


#endif /* !_POSITIONINGFRAME_H_ */
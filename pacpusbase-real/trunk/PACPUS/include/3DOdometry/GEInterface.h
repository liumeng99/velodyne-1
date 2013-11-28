/*********************************************************************
//  created:    2009/09/01 - 16:30
//  filename:   DbtPlyGoogleEarth.h
//
//  author:     Adrien Loison & Mickael Buranello 
//  modified:	Sergio Rodriguez
//  version:    $Id: $
//
//  purpose:    Use of Google Earth 
*********************************************************************/


#ifndef _GEINTERFACE_H_
#define _GEINTERFACE_H_

#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
#include <fstream>
#include <QT/qthread.h>

#include "PacpusTools/ShMem.h"
#include "kernel/ComponentBase.h"
#include "kernel/ComponentFactory.h"
#include "PacpusTools/math/geodesy.hpp"
#include "mathFunctions.h"

//=============================================================================
//OpenCV Includes
//=============================================================================
#include "cv.h"
#include "cvaux.h"
#include "cxcore.h"

#import "C:\Program Files\Google\Google Earth\client\googleearth.exe" named_guids
//#import "C:\Program Files\Google\Google Earth\googleearth.exe" named_guids
//QString geDir = ComponentBase::param.getProperty("geDir") + "\googleearth.exe";
//#import geDir named_guids


/*************************************************************************************/
/*** Classe d�finissant un point ***/

class Point
{
	public:
		double lat; // Latitude
		double lon; // Longitude
		double alt;
		float a, b, phi; // param�tres pour l'ellipse

		Point(double latitude = 0, double longitude = 0, double altitude = 0,float _a = 0, float _b = 0, float _phi = 0) :
			lat(latitude), lon(longitude), alt(altitude), a(_a), b(_b), phi(_phi) {}
};
class VoPoint
{
	public:
		double lat;		// Latitude
		double lon;		// Longitude
		double alt;		// Altitude
		double east;	// ENU coordinates
		double north;
		double up;
		double ECEFx;   // ECEF coordinates 
		double ECEFy;
		double ECEFz;


		VoPoint(double latitude = 0, 
			    double longitude = 0, 
				double altitude=0,
				double e = 0,
				double n = 0,
				double u = 0,
				double x = 0,
				double y = 0,
				double z = 0) :
			lat(latitude), lon(longitude), alt(altitude), 
			east(e)      , north(n)      , up(u)        ,
			ECEFx(x)     , ECEFy(y)      , ECEFz(z){}
};

typedef Point*    PPoint;
typedef VoPoint*  VPoint;


/*************************************************************************************/
/*** Classe du composant GoogleEarth ***/

class GEInterface : public ComponentBase, public QThread
{

public:
	static int nb_pts_simultanes;
	EARTHLib::IApplicationGE* ge;

public:
	GEInterface(QString name);
	~GEInterface();

	void run(); // r�d�finition de la m�thode du QThread

protected:
	virtual ComponentBase::COMPONENT_CONFIGURATION configureComponent(XmlComponentConfig config);
	virtual void startActivity();
	virtual void stopActivity();

private :
	//Variables pour la trajectoire par odometrie visuelle
	CvMat*   Vect3x1;
	CvMat*   Mat3x3;
	CvMat*   rot_;
	CvMat*   trans_;
	CvMat*   curRot_;
	CvMat*   curTrans_;

	CvMat*   headingRot_; 

	CvMat*   ecef2enuRot_;
	CvMat*   ecef2enuTrans_;
	CvMat*   enuGpsPoint_;

	bool     headingInitialized_;
	double** voInitPoints_;
	double** gpsInitPoints_;
	int      pointsForHeadingInit_;
	int		 pointsCount_;

	// Lecture des points lus en m�moire partag�e
	int readSharedMem(Point * point, ShMem * shMem_);

	// Lecture des points lus en m�moire partag�e (Visual Odometry)
	int readSharedMem(Point* point, VoPoint * vopoint, ShMem * shMem_);

	//Performs a linear solution for 3D registration 
	void svdRegistration(double** visualPoints, double** gpsPoints, int noPoints, CvMat* R);
	
	// Calcule la trajectoire par odometrie visuelle (DCM computation)
	void voMotion(double* transformation, double* state);

	// Calcul de l'ellipse d'incertitude pour chaque coordonn�e
	void calculEllipse(Point * point, Point** tab, double nbPoints);

	// Importation des donn�es via la fonction LoadKMLData() -> chaine de caract�res
	void importViaLoadKmlData();

	// Importation des donn�es dans GoogleEarth via la fonction via la fonction OpenKmlFile() -> fichier KML
	void importViaOpenKmlFile();

	// Cr�ation d'un fichier KML repr�sentant le trajet parcouru
	void creerFichierTrajet(PPoint * array, const char * nomFichier, std::string * debutKml, int passage = 1);

	void importViaOpenKmlFileSnakeMode();

	void importViaLoadKmlDataSnakeMode();


	/*************************************************************************************/
	/*** Initialisation de la cam�ra ***/

	void initialiserCamera(double latitude,
		double longitude,
		double altitude = 800, 
		EARTHLib::AltitudeModeGE altitudeMode = EARTHLib::AbsoluteAltitudeGE,
		double range = 0,
		double tilt = 0,
		double azimuth = 0,
		double speed = 0.3
		);

}; // fin de la classe GoogleEarth

// D�finition de la variable static (est chang� dans le constructeur)
int GEInterface::nb_pts_simultanes = 20;



/*************************************************************************************/
/*** DEFINITION DES CONSTANTES KML ***/

// KML cr�ant le d�but du document
std::string KMLBEGIN = \
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\
<kml xmlns=\"http://earth.google.com/kml/2.2\">\n\
	<Document>\n\
		<name>Etude Exp�rimentale</name>\n\
		<open>0</open>\n\
		<description>Repr�sentation du d�placement d&apos;un v�hicule exp�rimental via GoogleEarth</description>\n";

// KML fermant le document
std::string KMLEND = "\
	</Document>\n\
</kml>";

// KML cr�ant le d�but du Placemark indiquant le d�but du trajet
std::string KMLSTARTTAGBEGIN = "\
		<Style id=\"start\">\n\
			<IconStyle id=\"ID\">\n\
				<Icon>\n\
					<href>http://maps.google.com/mapfiles/kml/paddle/D.png</href>\n\
				</Icon>\n\
			</IconStyle>\n\
		</Style>\n\
		<Placemark>\n\
			<name>D�but</name>\n\
			<description>Point de D�part du parcours</description>\n\
			<altitudeMode>relativeToGround</altitudeMode>\n\
			<Point>\n\
				<coordinates>";

// KML fermant le Placemark de d�but de trajet
std::string KMLSTARTTAGEND = "\n\
				</coordinates>\n\
			</Point>\n\
			<styleUrl>#start</styleUrl>\n\
		</Placemark>\n";

// KML cr�ant le d�but de l'itin�raire parcouru
std::string KMLROUTEBEGIN = "\
		<Style id=\"roadStyle\">\n\
			<LineStyle>\n\
				<color>f00000ff</color>\n\
				<width>5</width>\n\
			</LineStyle>\n\
		</Style>\n\
		<Placemark>\n\
			<name>Route</name>\n\
			<description>Itin�raire parcouru</description>\n\
			<styleUrl>#roadStyle</styleUrl>\n\
			<MultiGeometry>\n\
				<LineString>\n\
					<altitudeMode>relativeToGround</altitudeMode>\n\
					<coordinates>";
std::string KMLROUTEBEGINVO = "\
		<Style id=\"roadStyle\">\n\
			<LineStyle>\n\
				<color>f0ff0000</color>\n\
				<width>5</width>\n\
			</LineStyle>\n\
		</Style>\n\
		<Placemark>\n\
			<name>Route</name>\n\
			<description>Itin�raire parcouru</description>\n\
			<styleUrl>#roadStyle</styleUrl>\n\
			<MultiGeometry>\n\
				<LineString>\n\
					<altitudeMode>relativeToGround</altitudeMode>\n\
					<coordinates>";
// KML fermant l'itin�raire parcouru
std::string KMLROUTEEND = "\
					</coordinates>\n\
				</LineString>\n\
			</MultiGeometry>\n\
		</Placemark>\n ";

// KML ouvrant l'ellipse
std::string KMLELLIPSEBEGIN = "\
		<Style id=\"ellipseStyle\">\n\
			<LineStyle>\n\
				<color>ff00ffff</color>\n\
				<width>3</width>\n\
			</LineStyle>\n\
			<PolyStyle>\n\
				<fill>0</fill>\n\
				<outline>1</outline>\n\
				<width>3</width>\n\
			</PolyStyle>\n\
		</Style>\n\
		<Placemark>\n\
			<name>Ellipse</name>\n\
			<description>Ellipse incertitude</description>\n\
			<styleUrl>#ellipseStyle</styleUrl>\n\
			<MultiGeometry>\n\
				<Polygon>\n\
					<altitudeMode>relativeToGround</altitudeMode>\n\
					<outerBoundaryIs>\n\
						<LinearRing>\n\
							<coordinates>";

// KML fermant l'ellipse
std::string KMLELLIPSEEND = "\
							</coordinates>\n\
						</LinearRing>\n\
					</outerBoundaryIs>\n\
				</Polygon>\n\
			</MultiGeometry>\n\
		</Placemark>\n ";

// KML cr�ant le Placemark repr�sentant la voiture
std::string KMLCARBEGIN = "\
		<Style id=\"voiture\">\n\
			<IconStyle id=\"ID\">\n\
				<Icon>\n\
					<href>http://maps.google.com/mapfiles/kml/shapes/cabs.png</href>\n\
				</Icon>\n\
			</IconStyle>\n\
		</Style>\n\
		<Placemark>\n\
			<name>Voiture</name>\n\
			<description>V�hicule Exp�rimental: \n\
	- Pilote: G�rald Dherbomez\n\
	- Copilote: Philippe Bonnifait</description>\n\
			<styleUrl>#voiture</styleUrl>\n\
			<Point>\n\
				<coordinates>";

// KML fermant le Placemark de la voiture
std::string KMLCAREND = "\
				</coordinates>\n\
			</Point>\n\
		</Placemark>\n";


/*************************************************************************************/

#endif
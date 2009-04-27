/*
 * Rvzware based in CAPAWARE 3D
 *
 * Rvzware is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * Rvzware is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this application; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * The Rvzware development team
 */

//LatLong- UTM conversion..h
//definitions for lat/long to UTM and UTM to lat/lng conversions

//Obtenidas en http://www.gpsy.com/gpsinfo/geotoutm/

#include <string>
#include <cpw/Export.h>


#ifndef LATLONGCONV
#define LATLONGCONV

namespace cpw
{

	/** 
		\brief Comentario breve de la clase ....
		\ingroup cpw
	*/
class CPWEXPORT UTMLL
{
	public:
		void LLtoUTM(int ReferenceEllipsoid, const double Lat, const double Long, 
					 double &UTMNorthing, double &UTMEasting, char *UTMZone);
		void UTMtoLL(int ReferenceEllipsoid, const double UTMNorthing, const double UTMEasting,  const char *UTMZone,
					  double& Lat,  double& Long );
		char UTMLetterDesignator(double Lat);
		
};


	/** 
		\brief Comentario breve de la clase ....
		\ingroup cpw
	*/
class CPWEXPORT Ellipsoid
{
public:
	Ellipsoid(){};
	Ellipsoid(int Id, char* name, double radius, double ecc)
	{
		id = Id; ellipsoidName = name; 
		EquatorialRadius = radius; eccentricitySquared = ecc;
	}

	int id;
	char* ellipsoidName;
	double EquatorialRadius; 
	double eccentricitySquared;  

};


}
#endif

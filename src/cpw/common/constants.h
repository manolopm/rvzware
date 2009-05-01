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

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cpw/common/LatLong-UTMconversion.h>

using namespace cpw;

const double PI = 3.14159265;
const double FOURTHPI = PI / 4;
const double deg2rad = PI / 180;
const double rad2deg = 180.0 / PI;

static Ellipsoid ellipsoid[] = 
{//  id, Ellipsoid name, Equatorial Radius, square of eccentricity	
  Ellipsoid( -1, (char *)((std::string) "Placeholder").c_str(), 0, 0),//placeholder only, To allow array indices to match id numbers
	Ellipsoid( 1, (char *)((std::string) "Airy").c_str(), 6377563, 0.00667054),
	Ellipsoid( 2, (char *)((std::string) "Australian National").c_str(), 6378160, 0.006694542),
	Ellipsoid( 3, (char *)((std::string) "Bessel 1841").c_str(), 6377397, 0.006674372),
	Ellipsoid( 4, (char *)((std::string) "Bessel 1841 (Nambia) ").c_str(), 6377484, 0.006674372),
	Ellipsoid( 5, (char *)((std::string) "Clarke 1866").c_str(), 6378206, 0.006768658),
	Ellipsoid( 6, (char *)((std::string) "Clarke 1880").c_str(), 6378249, 0.006803511),
	Ellipsoid( 7, (char *)((std::string) "Everest").c_str(), 6377276, 0.006637847),
	Ellipsoid( 8, (char *)((std::string) "Fischer 1960 (Mercury) ").c_str(), 6378166, 0.006693422),
	Ellipsoid( 9, (char *)((std::string) "Fischer 1968").c_str(), 6378150, 0.006693422),
	Ellipsoid( 10, (char *)((std::string) "GRS 1967").c_str(), 6378160, 0.006694605),
	Ellipsoid( 11, (char *)((std::string) "GRS 1980").c_str(), 6378137, 0.00669438),
	Ellipsoid( 12, (char *)((std::string) "Helmert 1906").c_str(), 6378200, 0.006693422),
	Ellipsoid( 13, (char *)((std::string) "Hough").c_str(), 6378270, 0.00672267),
	Ellipsoid( 14, (char *)((std::string) "International").c_str(), 6378388, 0.00672267),
	Ellipsoid( 15, (char *)((std::string) "Krassovsky").c_str(), 6378245, 0.006693422),
	Ellipsoid( 16, (char *)((std::string) "Modified Airy").c_str(), 6377340, 0.00667054),
	Ellipsoid( 17, (char *)((std::string) "Modified Everest").c_str(), 6377304, 0.006637847),
	Ellipsoid( 18, (char *)((std::string) "Modified Fischer 1960").c_str(), 6378155, 0.006693422),
	Ellipsoid( 19, (char *)((std::string) "South American 1969").c_str(), 6378160, 0.006694542),
	Ellipsoid( 20, (char *)((std::string) "WGS 60").c_str(), 6378165, 0.006693422),
	Ellipsoid( 21, (char *)((std::string) "WGS 66").c_str(), 6378145, 0.006694542),
	Ellipsoid( 22, (char *)((std::string) "WGS-72").c_str(), 6378135, 0.006694318),
	Ellipsoid( 23, (char *)((std::string) "WGS-84").c_str(), 6378137, 0.00669438)
};

#endif

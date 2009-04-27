/*
 * CAPAWARE 3D Geographical Multilayer Framework Free Software
 * Copyright (C) 2009 Instituto Tecnológico de Canarias (ITC) and Universidad de Las Palmas de Gran Canaria (ULPGC)
 *
 * This file is part of CAPAWARE.
 *
 * CAPAWARE is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * CAPAWARE is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this application; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * The CAPAWARE development team
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
	Ellipsoid( -1, "Placeholder", 0, 0),//placeholder only, To allow array indices to match id numbers
	Ellipsoid( 1, "Airy", 6377563, 0.00667054),
	Ellipsoid( 2, "Australian National", 6378160, 0.006694542),
	Ellipsoid( 3, "Bessel 1841", 6377397, 0.006674372),
	Ellipsoid( 4, "Bessel 1841 (Nambia) ", 6377484, 0.006674372),
	Ellipsoid( 5, "Clarke 1866", 6378206, 0.006768658),
	Ellipsoid( 6, "Clarke 1880", 6378249, 0.006803511),
	Ellipsoid( 7, "Everest", 6377276, 0.006637847),
	Ellipsoid( 8, "Fischer 1960 (Mercury) ", 6378166, 0.006693422),
	Ellipsoid( 9, "Fischer 1968", 6378150, 0.006693422),
	Ellipsoid( 10, "GRS 1967", 6378160, 0.006694605),
	Ellipsoid( 11, "GRS 1980", 6378137, 0.00669438),
	Ellipsoid( 12, "Helmert 1906", 6378200, 0.006693422),
	Ellipsoid( 13, "Hough", 6378270, 0.00672267),
	Ellipsoid( 14, "International", 6378388, 0.00672267),
	Ellipsoid( 15, "Krassovsky", 6378245, 0.006693422),
	Ellipsoid( 16, "Modified Airy", 6377340, 0.00667054),
	Ellipsoid( 17, "Modified Everest", 6377304, 0.006637847),
	Ellipsoid( 18, "Modified Fischer 1960", 6378155, 0.006693422),
	Ellipsoid( 19, "South American 1969", 6378160, 0.006694542),
	Ellipsoid( 20, "WGS 60", 6378165, 0.006693422),
	Ellipsoid( 21, "WGS 66", 6378145, 0.006694542),
	Ellipsoid( 22, "WGS-72", 6378135, 0.006694318),
	Ellipsoid( 23, "WGS-84", 6378137, 0.00669438)
};

#endif

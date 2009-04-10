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
#ifndef _WCSLAYER_
#define _WCSLAYER_

#include <string>
#include <vector>

#include <wcs/wcsUrl.h>
#include <sstream>

#include <cpw/entity/RasterLayer.h>
#include <cpw/entity/Entity.h>

#include "Export.h"

/**
	\namespace cpw { namespace ogcwcs OGC \brief Library for the OGC standard protocols (WMS, WFS, WCS, KMZ)
	\defgroup ogcwcs OGC library (WCS protocol)
 */


namespace cpw 
{ 
	namespace ogcwcs
	{

		/** \brief Manage the information of a WCS layer 
			\ingroup ogcwcs
		*/
		class OGCWCSEXPORT WcsLayer : public cpw::RasterLayer
		{
			public:
				
				WcsLayer(const cpw::TypeId id = cpw::TypeId(), 
						 const std::string &class_name = "WcsLayer");
		
				virtual ~WcsLayer(void);

				WcsLayer(const WcsLayer &wcslayer);
				WcsLayer &operator = (const WcsLayer &wcslayer);

				virtual Entity* Clone() {return new WcsLayer(*this);}

				std::string &GetWcsName() { return wcs_name; }
				std::string &GetWcsTitle() { return wcs_title; }
				void GetLatLonBoundingBox(double &lat1, double &lat2, double &lon1, double &lon2) { lat1 = latmin; lat2 = latmax; lon1 = lonmin; lon2 = lonmax; }
				void GetUTMLatLonBoundingBox(double &lat1, double &lat2, double &lon1, double &lon2) { lat1 = utmlatmin; lat2 = utmlatmax; lon1 = utmlonmin; lon2 = utmlonmax; }
				std::string &GetWcsVersion() { return wcs_version; }
				wcsUrl &GetWcsUrl() { return wcs_url; }
				std::string &GetWcsFormat() { return wcs_image_format; }

				std::vector<std::string> &GetWcsFormats() { return wcs_image_formats; }
				std::vector<std::string> &GetWcsSRS() { return wcs_srs_list; }
				std::vector<std::string> &GetWcsUTMvector() { return wcs_utm_vector; }

				const std::string &GetWcsName() const { return wcs_name; }
				const std::string &GetWcsTitle() const { return wcs_title; }
				const void GetLatLonBoundingBox(double &lat1, double &lat2, double &lon1, double &lon2) const { lat1 = latmin; lat2 = latmax; lon1 = lonmin; lon2 = lonmax; }
				const void GetUTMLatLonBoundingBox(double &lat1, double &lat2, double &lon1, double &lon2) const { lat1 = utmlatmin; lat2 = utmlatmax; lon1 = utmlonmin; lon2 = utmlonmax; }
				const std::string &GetWcsVersion() const { return wcs_version; }
				const wcsUrl &GetWcsUrl() const { return wcs_url; }
				const std::string &GetWcsFormat() const { return wcs_image_format; }

				const std::vector<std::string> &GetWcsFormats() const { return wcs_image_formats; }
				const std::vector<std::string> &GetWcsSRS() const { return wcs_srs_list; }
				const std::vector<std::string> &GetWcsUTMvector() const { return wcs_utm_vector; }

				void SetWcsName(const std::string &name) { wcs_name = name; /*Modified("wcs_name", wcs_name);*/}
				void SetWcsTitle(const std::string &title) { wcs_title = title; /*Modified("wcs_title", wcs_title);*/}
				void SetLatLonBoundingBox(const double lat1, const double lat2, const double lon1, const double lon2) 
				{ 
					latmin = lat1; latmax = lat2; lonmin = lon1; lonmax = lon2; 
					std::ostringstream str;
					str << lat1 << " " << lat2 << " " << lon1 << " " << lon2;
					Modified("LatLonBoundingBox", str.str());
				}

				void SetUTMLatLonBoundingBox(const double lat1, const double lat2, const double lon1, const double lon2) 
				{ 
					utmlatmin = lat1; utmlatmax = lat2; utmlonmin = lon1; utmlonmax = lon2; 
					std::ostringstream str;
					str << lat1 << " " << lat2 << " " << lon1 << " " << lon2;
					Modified("UTMLatLonBoundingBox", str.str());
				}

				void SetWcsVersion(const std::string &version) { wcs_version = version; Modified("wcs_version", wcs_version);}
				void SetWcsUrl(const wcsUrl &url) { wcs_url = url; Modified("wcs_url", wcs_url.url());}
				void SetWcsFormat(const std::string &format) { wcs_image_format = format; Modified("wcs_image_format", wcs_image_format); }
				void SetWcsFormats(const std::vector<std::string> &formats) { wcs_image_formats = formats; Modified("wcs_image_formats"); }

				void SetWcsSRS(const std::vector<std::string> &_srs) { wcs_srs_list = _srs; Modified("wcs_srs_list"); }
				void SetWcsUTMvector(const std::vector<std::string> &_utmv) { wcs_utm_vector = _utmv; Modified("wcs_utm_vector"); }

				void SetSRS(const std::string &_srs) {srs = _srs;}
				std::string &GetSRS() { return srs; }
				const std::string &GetSRS() const { return srs; }

				//void SetComposedURL(std::string &cu) {s_composed_url = cu;}
				//std::string &GetComposedURL() { return s_composed_url; }
				//const std::string &GetComposedURL() const { return s_composed_url; }


				virtual void SetValue(const std::string &f, const std::string &v);

			private:

				std::string wcs_name, wcs_title;
				std::string wcs_image_format;
				std::string wcs_version;
				std::vector<std::string> wcs_image_formats;
				std::vector<std::string> wcs_srs_list;
				std::vector<std::string> wcs_utm_vector;

				std::string srs;

				wcsUrl wcs_url;

				//wcsUrl composed_url;
				//std::string s_composed_url;

				double latmin, lonmin, latmax, lonmax;
				double utmlatmin, utmlonmin, utmlatmax, utmlonmax;

			protected:
				virtual int CreatePersistence();
				virtual int CreateProperties(); 
				virtual int AdaptPersistence(cpw::Node *root);
		};

	}
}

#endif
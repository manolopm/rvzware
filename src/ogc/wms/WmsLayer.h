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

#ifndef _WMSLAYER_
#define _WMSLAYER_

#include <string>
#include <vector>

#include <wms/wmsUrl.h>
#include <sstream>

#include <cpw/entity/RasterLayer.h>
#include <cpw/entity/Entity.h>

#include <ogc/Export.h>

/**
	\namespace cpw { namespace ogc OGC \brief Library for the OGC standard protocols (WMS, WFS, WCS, KMZ)
	\defgroup ogc OGC library (WMS protocol)
 */


namespace cpw 
{ 
	namespace ogc
	{

		/** \brief Manage the information of a WMS layer 
			\ingroup ogc
		*/
		class OGCEXPORT WmsLayer : public cpw::RasterLayer
		{
			public:
				
				WmsLayer(const cpw::TypeId id = cpw::TypeId(), 
						 const std::string &class_name = "WmsLayer");
		
				virtual ~WmsLayer(void);

				WmsLayer(const WmsLayer &wmslayer);
				WmsLayer &operator = (const WmsLayer &wmslayer);

				virtual Entity* Clone() {return new WmsLayer(*this);}

				std::string &GetWmsName() { return wms_name; }
				std::string &GetWmsTitle() { return wms_title; }
				void GetLatLonBoundingBox(double &lat1, double &lat2, double &lon1, double &lon2) { lat1 = latmin; lat2 = latmax; lon1 = lonmin; lon2 = lonmax; }
				void GetUTMLatLonBoundingBox(double &lat1, double &lat2, double &lon1, double &lon2) { lat1 = utmlatmin; lat2 = utmlatmax; lon1 = utmlonmin; lon2 = utmlonmax; }
				std::string &GetWmsVersion() { return wms_version; }
				wmsUrl &GetWmsUrl() { return wms_url; }
				std::string &GetWmsFormat() { return wms_image_format; }

				std::vector<std::string> &GetWmsFormats() { return wms_image_formats; }
				std::vector<std::string> &GetWmsSRS() { return wms_srs_list; }
				std::vector<std::string> &GetWmsUTMvector() { return wms_utm_vector; }

				const std::string &GetWmsName() const { return wms_name; }
				const std::string &GetWmsTitle() const { return wms_title; }
				const void GetLatLonBoundingBox(double &lat1, double &lat2, double &lon1, double &lon2) const { lat1 = latmin; lat2 = latmax; lon1 = lonmin; lon2 = lonmax; }
				const void GetUTMLatLonBoundingBox(double &lat1, double &lat2, double &lon1, double &lon2) const { lat1 = utmlatmin; lat2 = utmlatmax; lon1 = utmlonmin; lon2 = utmlonmax; }
				const std::string &GetWmsVersion() const { return wms_version; }
				const wmsUrl &GetWmsUrl() const { return wms_url; }
				const std::string &GetWmsFormat() const { return wms_image_format; }

				const std::vector<std::string> &GetWmsFormats() const { return wms_image_formats; }
				const std::vector<std::string> &GetWmsSRS() const { return wms_srs_list; }
				const std::vector<std::string> &GetWmsUTMvector() const { return wms_utm_vector; }

				void SetWmsName(const std::string &name) { wms_name = name; Modified("wms_name", wms_name);}
				void SetWmsTitle(const std::string &title) { wms_title = title; Modified("wms_title", wms_title);}
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

				void SetWmsVersion(const std::string &version) { wms_version = version; Modified("wms_version", wms_version);}
				void SetWmsUrl(const wmsUrl &url) { wms_url = url; Modified("wms_url", wms_url.url());}
				void SetWmsFormat(const std::string &format) { wms_image_format = format; Modified("wms_image_format", wms_image_format); }
				void SetWmsFormats(const std::vector<std::string> &formats) { wms_image_formats = formats; Modified("wms_image_formats"); }

				void SetWmsSRS(const std::vector<std::string> &_srs) { wms_srs_list = _srs; Modified("wms_srs_list"); }
				void SetWmsUTMvector(const std::vector<std::string> &_utmv) { wms_utm_vector = _utmv; Modified("wms_utm_vector"); }

				void SetSRS(const std::string &_srs) {srs = _srs;}
				std::string &GetSRS() { return srs; }
				const std::string &GetSRS() const { return srs; }

				//void SetComposedURL(std::string &cu) {s_composed_url = cu;}
				//std::string &GetComposedURL() { return s_composed_url; }
				//const std::string &GetComposedURL() const { return s_composed_url; }


				virtual void SetValue(const std::string &f, const std::string &v);

			private:

				std::string wms_name, wms_title;
				std::string wms_image_format;
				std::string wms_version;
				std::vector<std::string> wms_image_formats;
				std::vector<std::string> wms_srs_list;
				std::vector<std::string> wms_utm_vector;

				std::string srs;

				wmsUrl wms_url;

				//wmsUrl composed_url;
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



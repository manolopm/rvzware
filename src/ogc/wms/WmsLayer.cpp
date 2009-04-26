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

#include <ogc/wms/WmsLayer.h>

#include <sstream>
#include <string>
#include <algorithm>
#include <cpw/debugger.h>

using namespace cpw::ogc;

WmsLayer::WmsLayer(const cpw::TypeId id, 
				   const std::string &class_name) : cpw::RasterLayer(id, class_name),srs()
{
}

WmsLayer::~WmsLayer(void)
{
}

WmsLayer::WmsLayer(const WmsLayer &wmslayer) : cpw::RasterLayer(wmslayer)
{

	wms_name = wmslayer.wms_name;
	wms_title = wmslayer.wms_title;
	wms_image_format = wmslayer.wms_image_format;
	wms_version = wmslayer.wms_version;
	
	for(std::vector<std::string>::const_iterator i= wmslayer.wms_image_formats.begin(); 
		i!=wmslayer.wms_image_formats.end(); i++)
	{
		wms_image_formats.push_back((*i));
	}		
	wms_url = wmslayer.wms_url;
			
	latmin = wmslayer.latmin;
	lonmin = wmslayer.lonmin;
	latmax = wmslayer.latmax;
	lonmax = wmslayer.lonmax;

	utmlatmin = wmslayer.utmlatmin;
	utmlonmin = wmslayer.utmlonmin;
	utmlatmax = wmslayer.utmlatmax;
	utmlonmax = wmslayer.utmlonmax;

	Modified();
}

WmsLayer &WmsLayer::operator = (const WmsLayer &wmslayer)
{
	RasterLayer::operator =(wmslayer);

	wms_name = wmslayer.wms_name;
	wms_title = wmslayer.wms_title;
	wms_image_format = wmslayer.wms_image_format;
	wms_version = wmslayer.wms_version;
	
	for(std::vector<std::string>::const_iterator i= wmslayer.wms_image_formats.begin(); 
		i!=wmslayer.wms_image_formats.end(); i++)
	{
		wms_image_formats.push_back((*i));
	}		
	wms_url = wmslayer.wms_url;
			
	latmin = wmslayer.latmin;
	lonmin = wmslayer.lonmin;
	latmax = wmslayer.latmax;
	lonmax = wmslayer.lonmax;

	utmlatmin = wmslayer.utmlatmin;
	utmlonmin = wmslayer.utmlonmin;
	utmlatmax = wmslayer.utmlatmax;
	utmlonmax = wmslayer.utmlonmax;

	Modified();
	return *this;
}

void WmsLayer::SetValue(const std::string &f, const std::string &v)
{

	if(f == "wms_name")
		SetWmsName(v);
	else if(f == "wms_title")
		SetWmsTitle(v);
	else if(f == "LatLonBoundingBox")
	{
		double lat1, lat2, lon1, lon2; 
		std::istringstream str(v);
		str >> lat1 >> lat2 >> lon1 >> lon2;
		SetLatLonBoundingBox(lat1, lat2, lon1, lon2);
	}
	else if(f == "UTMLatLonBoundingBox")
	{
		double lat1, lat2, lon1, lon2; 
		std::istringstream str(v);
		str >> lat1 >> lat2 >> lon1 >> lon2;
		SetUTMLatLonBoundingBox(lat1, lat2, lon1, lon2);
	}
	else if(f == "wms_version")
		SetWmsVersion(v);
	else if(f == "wms_image_format")
		SetWmsFormat(v);
	else if(f == "wms_url")
		SetWmsUrl(v);
	else
		RasterLayer::SetValue(f, v);
}


int WmsLayer::CreatePersistence() 
{
	RasterLayer::CreatePersistence();
	
	AddPersistence(std::string("WMS_Name"), wms_name);
	AddPersistence(std::string("WMS_Title"), wms_title);
	AddPersistence(std::string("WMS_ImageFormat"), wms_image_format);
	AddPersistence(std::string("WMS_Version"), wms_version);

	AddPersistenceLevel((std::string)"WMS_ImageFormats");
	for(std::vector<std::string>::iterator i= wms_image_formats.begin(); i!=wms_image_formats.end(); i++)
	{
		AddPersistence(std::string("WMS_Format"), *i);
	}		
	RemovePersistenceLevel();

	std::ostringstream str_latmin;
	str_latmin.precision(15);
	str_latmin << latmin;
	AddPersistence(std::string("WMS_LatMin"), str_latmin.str());

	std::ostringstream str_latmax;
	str_latmax.precision(15);
	str_latmax << latmax;
	AddPersistence(std::string("WMS_LatMax"), str_latmax.str());

	std::ostringstream str_lonmin;
	str_lonmin.precision(15);
	str_lonmin << lonmin;
	AddPersistence(std::string("WMS_LonMin"), str_lonmin.str());

	std::ostringstream str_lonmax;
	str_lonmax.precision(15);
	str_lonmax << lonmax;
	AddPersistence(std::string("WMS_LonMax"), str_lonmax.str());
//
	std::ostringstream str_utmlatmin;
	str_utmlatmin.precision(15);
	str_utmlatmin << utmlatmin;
	AddPersistence(std::string("WMS_UTMLatMin"), str_utmlatmin.str());

	std::ostringstream str_utmlatmax;
	str_utmlatmax.precision(15);
	str_utmlatmax << utmlatmax;
	AddPersistence(std::string("WMS_UTMLatMax"), str_utmlatmax.str());

	std::ostringstream str_utmlonmin;
	str_utmlonmin.precision(15);
	str_utmlonmin << utmlonmin;
	AddPersistence(std::string("WMS_UTMLonMin"), str_utmlonmin.str());

	std::ostringstream str_utmlonmax;
	str_utmlonmax.precision(15);
	str_utmlonmax << utmlonmax;
	AddPersistence(std::string("WMS_UTMLonMax"), str_utmlonmax.str());

	AddPersistence(std::string("WMS_URL"), wms_url.decode());

	return 0;
}


int WmsLayer::CreateProperties() 
{
	RasterLayer::CreateProperties();

	AddPersistenceLevel("WMS Information");

	AddPersistence(std::string("Name"), wms_name);
	AddPersistence(std::string("Title"), wms_title);
	AddPersistence(std::string("Image format"), wms_image_format);
	AddPersistence(std::string("Version"), wms_version);

	std::ostringstream str_latmin;
	str_latmin.precision(15);
	str_latmin << latmin;
	AddPersistence(std::string("LatMin"), str_latmin.str());

	std::ostringstream str_latmax;
	str_latmax.precision(15);
	str_latmax << latmax;
	AddPersistence(std::string("LatMax"), str_latmax.str());

	std::ostringstream str_lonmin;
	str_lonmin.precision(15);
	str_lonmin << lonmin;
	AddPersistence(std::string("LonMin"), str_lonmin.str());

	std::ostringstream str_lonmax;
	str_lonmax.precision(15);
	str_lonmax << lonmax;
	AddPersistence(std::string("LonMax"), str_lonmax.str());

	std::ostringstream str_utmlatmin;
	str_utmlatmin.precision(15);
	str_utmlatmin << utmlatmin;
	AddPersistence(std::string("UTMLatMin"), str_utmlatmin.str());

	std::ostringstream str_utmlatmax;
	str_utmlatmax.precision(15);
	str_utmlatmax << utmlatmax;
	AddPersistence(std::string("UTMLatMax"), str_utmlatmax.str());

	std::ostringstream str_utmlonmin;
	str_utmlonmin.precision(15);
	str_utmlonmin << utmlonmin;
	AddPersistence(std::string("UTMLonMin"), str_utmlonmin.str());

	std::ostringstream str_utmlonmax;
	str_utmlonmax.precision(15);
	str_utmlonmax << utmlonmax;
	AddPersistence(std::string("UTMLonMax"), str_utmlonmax.str());

	AddPersistence(std::string("URL"), wms_url.decode());

	RemovePersistenceLevel();

	return 0;
}


int WmsLayer::AdaptPersistence(cpw::Node *root)
{
	RasterLayer::AdaptPersistence(root);

	wms_name =  (root->GetChildValue("WMS_Name"));
	wms_title =  (root->GetChildValue("WMS_Title"));
	wms_image_format =  (root->GetChildValue("WMS_ImageFormat"));
	wms_version =  (root->GetChildValue("WMS_Version"));

	std::vector<cpw::Node *> root_children = root->GetChildren();
	std::vector<cpw::Node *>::iterator i;

	for(i = root_children.begin(); i != root_children.end(); i++)
	{
		if ((*i)->GetName() == "WMS_ImageFormats")
		{
			std::vector<cpw::Node *> format_children = (*i)->GetChildren();
			std::vector<cpw::Node *>::iterator j;
			for(j = format_children.begin(); j != format_children.end(); j++)
			{
				wms_image_formats.push_back((*j)->GetValue());
			}
		}
	}
	

	std::istringstream str_latmin (root->GetChildValue("WMS_LatMin"));
	str_latmin >> latmin;
	std::istringstream str_latmax (root->GetChildValue("WMS_LatMax"));
	str_latmax >> latmax;
	std::istringstream str_lonmin (root->GetChildValue("WMS_LonMin"));
	str_lonmin >> lonmin;
	std::istringstream str_lonmax (root->GetChildValue("WMS_LonMax"));
	str_lonmax >> lonmax;
//
	std::istringstream str_utmlatmin (root->GetChildValue("WMS_UTMLatMin"));
	str_utmlatmin >> utmlatmin;
	std::istringstream str_utmlatmax (root->GetChildValue("WMS_UTMLatMax"));
	str_utmlatmax >> utmlatmax;
	std::istringstream str_utmlonmin (root->GetChildValue("WMS_UTMLonMin"));
	str_utmlonmin >> utmlonmin;
	std::istringstream str_utmlonmax (root->GetChildValue("WMS_UTMLonMax"));
	str_utmlonmax >> utmlonmax;

	//std::istringstream str_url (root->GetChildValue("WMS_URL"));
	//str_url >> wms_url;
	wms_url = root->GetChildValue("WMS_URL");


	return 0;
}

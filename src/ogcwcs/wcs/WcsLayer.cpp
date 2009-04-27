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


#include <ogcwcs/WcsLayer.h>

#include <sstream>
#include <string>
#include <algorithm>
#include <cpw/debugger.h>

using namespace cpw::ogcwcs;

WcsLayer::WcsLayer(const cpw::TypeId id, 
				   const std::string &class_name) : cpw::RasterLayer(id, class_name),srs()
{
}

WcsLayer::~WcsLayer(void)
{
}

WcsLayer::WcsLayer(const WcsLayer &wcslayer) : cpw::RasterLayer(wcslayer)
{

	wcs_name = wcslayer.wcs_name;
	wcs_title = wcslayer.wcs_title;
	wcs_image_format = wcslayer.wcs_image_format;
	wcs_version = wcslayer.wcs_version;
	
	for(std::vector<std::string>::const_iterator i= wcslayer.wcs_image_formats.begin(); 
		i!=wcslayer.wcs_image_formats.end(); i++)
	{
		wcs_image_formats.push_back((*i));
	}		
	wcs_url = wcslayer.wcs_url;
			
	latmin = wcslayer.latmin;
	lonmin = wcslayer.lonmin;
	latmax = wcslayer.latmax;
	lonmax = wcslayer.lonmax;

	utmlatmin = wcslayer.utmlatmin;
	utmlonmin = wcslayer.utmlonmin;
	utmlatmax = wcslayer.utmlatmax;
	utmlonmax = wcslayer.utmlonmax;

	Modified();
}

WcsLayer &WcsLayer::operator = (const WcsLayer &wcslayer)
{
	RasterLayer::operator =(wcslayer);

	wcs_name = wcslayer.wcs_name;
	wcs_title = wcslayer.wcs_title;
	wcs_image_format = wcslayer.wcs_image_format;
	wcs_version = wcslayer.wcs_version;
	
	for(std::vector<std::string>::const_iterator i= wcslayer.wcs_image_formats.begin(); 
		i!=wcslayer.wcs_image_formats.end(); i++)
	{
		wcs_image_formats.push_back((*i));
	}		
	wcs_url = wcslayer.wcs_url;
			
	latmin = wcslayer.latmin;
	lonmin = wcslayer.lonmin;
	latmax = wcslayer.latmax;
	lonmax = wcslayer.lonmax;

	utmlatmin = wcslayer.utmlatmin;
	utmlonmin = wcslayer.utmlonmin;
	utmlatmax = wcslayer.utmlatmax;
	utmlonmax = wcslayer.utmlonmax;

	Modified();
	return *this;
}

void WcsLayer::SetValue(const std::string &f, const std::string &v)
{

	if(f == "wcs_name")
		SetWcsName(v);
	else if(f == "wcs_title")
		SetWcsTitle(v);
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
	else if(f == "wcs_version")
		SetWcsVersion(v);
	else if(f == "wcs_image_format")
		SetWcsFormat(v);
	else if(f == "wcs_url")
		SetWcsUrl(v);
	else
		RasterLayer::SetValue(f, v);
}


int WcsLayer::CreatePersistence() 
{
	RasterLayer::CreatePersistence();
	
	AddPersistence(std::string("WCS_Name"), wcs_name);
	AddPersistence(std::string("WCS_Title"), wcs_title);
	AddPersistence(std::string("WCS_ImageFormat"), wcs_image_format);
	AddPersistence(std::string("WCS_Version"), wcs_version);

	AddPersistenceLevel((std::string)"WCS_ImageFormats");
	for(std::vector<std::string>::iterator i= wcs_image_formats.begin(); i!=wcs_image_formats.end(); i++)
	{
		AddPersistence(std::string("WCS_Format"), *i);
	}		
	RemovePersistenceLevel();

	std::ostringstream str_latmin;
	str_latmin.precision(15);
	str_latmin << latmin;
	AddPersistence(std::string("WCS_LatMin"), str_latmin.str());

	std::ostringstream str_latmax;
	str_latmax.precision(15);
	str_latmax << latmax;
	AddPersistence(std::string("WCS_LatMax"), str_latmax.str());

	std::ostringstream str_lonmin;
	str_lonmin.precision(15);
	str_lonmin << lonmin;
	AddPersistence(std::string("WCS_LonMin"), str_lonmin.str());

	std::ostringstream str_lonmax;
	str_lonmax.precision(15);
	str_lonmax << lonmax;
	AddPersistence(std::string("WCS_LonMax"), str_lonmax.str());
//
	std::ostringstream str_utmlatmin;
	str_utmlatmin.precision(15);
	str_utmlatmin << utmlatmin;
	AddPersistence(std::string("WCS_UTMLatMin"), str_utmlatmin.str());

	std::ostringstream str_utmlatmax;
	str_utmlatmax.precision(15);
	str_utmlatmax << utmlatmax;
	AddPersistence(std::string("WCS_UTMLatMax"), str_utmlatmax.str());

	std::ostringstream str_utmlonmin;
	str_utmlonmin.precision(15);
	str_utmlonmin << utmlonmin;
	AddPersistence(std::string("WCS_UTMLonMin"), str_utmlonmin.str());

	std::ostringstream str_utmlonmax;
	str_utmlonmax.precision(15);
	str_utmlonmax << utmlonmax;
	AddPersistence(std::string("WCS_UTMLonMax"), str_utmlonmax.str());

	AddPersistence(std::string("WCS_URL"), wcs_url.decode());

	return 0;
}


int WcsLayer::CreateProperties() 
{
	RasterLayer::CreateProperties();

	AddPersistenceLevel("WCS Information");

	AddPersistence(std::string("Name"), wcs_name);
	AddPersistence(std::string("Title"), wcs_title);
	AddPersistence(std::string("Image format"), wcs_image_format);
	AddPersistence(std::string("Version"), wcs_version);

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

	AddPersistence(std::string("URL"), wcs_url.decode());

	RemovePersistenceLevel();

	return 0;
}


int WcsLayer::AdaptPersistence(cpw::Node *root)
{
	RasterLayer::AdaptPersistence(root);

	wcs_name =  (root->GetChildValue("WCS_Name"));
	wcs_title =  (root->GetChildValue("WCS_Title"));
	wcs_image_format =  (root->GetChildValue("WCS_ImageFormat"));
	wcs_version =  (root->GetChildValue("WCS_Version"));

	std::vector<cpw::Node *> root_children = root->GetChildren();
	std::vector<cpw::Node *>::iterator i;

	for(i = root_children.begin(); i != root_children.end(); i++)
	{
		if ((*i)->GetName() == "WCS_ImageFormats")
		{
			std::vector<cpw::Node *> format_children = (*i)->GetChildren();
			std::vector<cpw::Node *>::iterator j;
			for(j = format_children.begin(); j != format_children.end(); j++)
			{
				wcs_image_formats.push_back((*j)->GetValue());
			}
		}
	}
	

	std::istringstream str_latmin (root->GetChildValue("WCS_LatMin"));
	str_latmin >> latmin;
	std::istringstream str_latmax (root->GetChildValue("WCS_LatMax"));
	str_latmax >> latmax;
	std::istringstream str_lonmin (root->GetChildValue("WCS_LonMin"));
	str_lonmin >> lonmin;
	std::istringstream str_lonmax (root->GetChildValue("WCS_LonMax"));
	str_lonmax >> lonmax;
//
	std::istringstream str_utmlatmin (root->GetChildValue("WCS_UTMLatMin"));
	str_utmlatmin >> utmlatmin;
	std::istringstream str_utmlatmax (root->GetChildValue("WCS_UTMLatMax"));
	str_utmlatmax >> utmlatmax;
	std::istringstream str_utmlonmin (root->GetChildValue("WCS_UTMLonMin"));
	str_utmlonmin >> utmlonmin;
	std::istringstream str_utmlonmax (root->GetChildValue("WCS_UTMLonMax"));
	str_utmlonmax >> utmlonmax;

	//std::istringstream str_url (root->GetChildValue("WCS_URL"));
	//str_url >> wcs_url;
	wcs_url = root->GetChildValue("WCS_URL");


	return 0;
}

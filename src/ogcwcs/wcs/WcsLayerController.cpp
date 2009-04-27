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



#include <ogcwcs/WcsLayerController.h>

#include <iostream>
#include <wcs/wcsNotify.h>
#include <wcs/wcs.h>
#include <wcs/wcsUrl.h>
#include <wcs/wcsRefPtr.h>
#include <wcs/wcsMemoryStream.h>
#include <wcs/wcsCapabilitiesState.h>
#include <wcs/wcsCapabilitiesParser.h>

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cpw/debugger.h>

using namespace std;
using namespace cpw::ogcwcs;
    

std::ostream &operator <<(std::ostream &s, const Leaf &leaf) {

	s << "[" << leaf.Name() << "] " << leaf.Title() << std::endl;

	return s;
}



std::ostream &operator <<(std::ostream &s, const Folder &folder) {

	s << "[begin:" << folder.Name() << "] " << folder.Title() << std::endl;
	
	std::vector<Node *> nodes = folder.GetNodes();

	for(std::vector<Node *>::const_iterator i = nodes.begin(); i != nodes.end(); i++) {
		if((*i)->Type() == FOLDER)
			s << *((Folder*) (*i));
		else
			s << *((Leaf *)(*i));
	}

	s << "[end] " << folder.Title() << std::endl;
	return s;
}


WcsLayerController::WcsLayerController(void):cu(), wcs_crs()
{
   // initilize communication environment*/
   //wcsInitialize();
}

WcsLayerController::~WcsLayerController(void)
{

   // finalize communication environment
   //wcsFinalize();

}   


int WcsLayerController::ConnectToServer(const std::string &url) {

	wcsClient wcs_client;
	wcsRefPtr<wcsMemoryStream> memoryStream;
    
	wcs_url = url;

	if(wcs_client.getCapabilities(wcs_url))
    {
		memoryStream = wcs_client.getStream();
		std::string str1 = memoryStream->getBufferAsString();
		wcsRefPtr<wcsCapabilitiesParser> parser = new wcsCapabilitiesParser;
		wcs_root = parser->parseRawString(str1);

		if(!wcs_root.valid()) return -1;
    }
    else return -1;

    return 0;
}

void WcsLayerController::GetInformation(std::string &info) {

	if(wcs_root.valid())
    {
	}
}

void WcsLayerController::GetLayers(std::vector<std::string> &layers) {

    if(wcs_root.valid())
    {
		 vector<wcsRefPtr<wcsCapabilitiesState> > nodos;
		 wcs_root->getLeafNodes(nodos, true);
		 
		 for(vector<wcsRefPtr<wcsCapabilitiesState> >::iterator i = nodos.begin(); i != nodos.end(); i++) {

			 layers.push_back((*i)->name());

		 }
	}
}


void WcsLayerController::AddChildrenNodes(Node *layerTree, std::vector<wcsRefPtr<wcsCapabilitiesState> > &children) {

	for(std::vector<wcsRefPtr<wcsCapabilitiesState> >::iterator i = children.begin(); i != children.end(); i++) {

		//if it is a group node then add itself and its children
		if((*i)->hasChildren()) {

			Folder *folder = new Folder((*i)->name(), (*i)->title());
			layerTree->Add(folder);
			AddChildrenNodes(folder, (*i)->children());
		}
		else
			//it is a layer then add it as a leaf node
			layerTree->Add(new Leaf((*i)->name(), (*i)->title()));
	}
}


void WcsLayerController::GetLayers(Node *layerTree){

   if (wcs_root != NULL)
   {
		wcsRefPtr<wcsCapabilitiesState> theRootLayer = wcs_root->root();

	   if(!theRootLayer.valid()) return;

	   layerTree->Name() = theRootLayer->name();
	   layerTree->Title() = theRootLayer->title();

	   if(theRootLayer->hasChildren())
	   {
		   AddChildrenNodes(layerTree, theRootLayer->children());

	   }
   }
}


void WcsLayerController::GetImageFormats(std::vector<std::string> &formats) {

    if(wcs_root.valid())
    {
	}
}

void WcsLayerController::GetSRS(std::vector<std::string> &srs) {

    if(wcs_root.valid())
    {

	}
}

bool WcsLayerController::GetMap(const WcsLayer &layer, int width, int height, float latmin, float lonmin,
								float latmax, float lonmax, std::string &filename, 
								const std::string &srs) 
{
 	string options;
	wcsUrl wcs_url = layer.GetWcsUrl();
	if(wcs_url.options() != "") {
		options = wcs_url.options() + "&SERVICE=WCS&COVERAGE=" + layer.GetWcsName() ;
	}
	else options = "SERVICE=WCS&COVERAGE=" + layer.GetWcsName() ;

	wcsUrl url;
	url.mergeUrl(wcs_url.protocol(), wcs_url.server(), wcs_url.path(), options);
	
	// replace de white space for %20
	std::string wcs_image_format(layer.GetWcsFormat());
	std::string::iterator pos = std::find(wcs_image_format.begin(),wcs_image_format.end(), ' ');
	while(pos != wcs_image_format.end())
	{
		wcs_image_format.replace(pos, pos + 1, "%20");
		pos = std::find(wcs_image_format.begin(), wcs_image_format.end(), ' ');
	}

	const std::string &file_ext = wcs_image_format;

	std::string new_srs = layer.GetSRS();

	wcsClient wcs_client;
	wcsUrl wcs_cu;
	//new_srs = "EPSG:23030";
	if(!new_srs.empty())
	{
		wcs_cu = wcs_client.getMapUrl(url,width,height,latmin,lonmin,latmax,lonmax,file_ext,layer.GetWcsVersion(),new_srs);
		cu = wcs_cu.url();

	
		return wcs_client.getMap(url, width, height, latmin, lonmin, latmax, lonmax, "asciigrid", layer.GetWcsVersion(), std::string(filename+".txt"), new_srs);
		//return wcs_client.getMap(url, width, height, latmin, lonmin, latmax, lonmax, file_ext, layer.GetWcsVersion(), filename, new_srs);
	}
	else
	{
		wcs_cu = wcs_client.getMapUrl(url,width,height,latmin,lonmin,latmax,lonmax,file_ext,layer.GetWcsVersion(),srs);
		cu = wcs_cu.url();
		
		return wcs_client.getMap(url, width, height, latmin, lonmin, latmax, lonmax, "asciigrid", layer.GetWcsVersion(), std::string(filename+".txt"), srs);
		//return wcs_client.getMap(url, width, height, latmin, lonmin, latmax, lonmax, file_ext, layer.GetWcsVersion(), filename, srs);
	}
}

bool WcsLayerController::GetMap(const string &layer_name, int width, int height, float latmin, float lonmin,
								float latmax, float lonmax, std::string &filename, const std::string &image_format, 
								const std::string &version, 
								const std::string &srs) {

	//if(wcs_root.valid())
    //{

     	string options;
		if(wcs_url.options() != "") {
			options = wcs_url.options() + "&SERVICE=WCS&COVERAGE=" + layer_name ;
		}
		else options = "SERVICE=WCS&COVERAGE=" + layer_name ;

		wcsUrl url;
		url.mergeUrl(wcs_url.protocol(), wcs_url.server(), wcs_url.path(), options);
		
		wcsClient wcs_client;
		// replace de white space for %20
		std::string wcs_image_format = image_format;
		std::string::iterator pos = std::find(wcs_image_format.begin(),wcs_image_format.end(), ' ');
		while(pos != wcs_image_format.end())
		{
			wcs_image_format.replace(pos, pos + 1, "%20");
			pos = std::find(wcs_image_format.begin(), wcs_image_format.end(), ' ');
		}

		return wcs_client.getMap(url, width, height, latmin, lonmin, latmax, lonmax, wcs_image_format, version, filename, srs);
	//}
}


void WcsLayerController::CreateLayer(const std::string &layer_name, const std::string &format, WcsLayer &layer) {

	wcsRefPtr<wcsCapabilitiesState> LayerNode = wcs_root->getNodeGivenName(layer_name);

	double lat1, lat2, lon1, lon2;
	double utmlat1, utmlat2, utmlon1, utmlon2;

	LayerNode->getLatLonBoundingBox(lat1, lon1, lat2, lon2);
	LayerNode->getUTMLatLonBoundingBox(utmlat1, utmlon1, utmlat2, utmlon2);
	//vector<wcsStyle> style = LayerNode->styles();
	vector<string> formats = LayerNode->formats();
	vector<string> srs_list = LayerNode->SRS();	
	vector<string> utm_vector = LayerNode->UTM_vector();

	string version = wcs_root->wcsVersion(); 

	layer.SetWcsName(LayerNode->name());
	layer.SetWcsTitle(LayerNode->title());
	layer.SetLatLonBoundingBox(lat1, lat2, lon1, lon2);
	layer.SetUTMLatLonBoundingBox(utmlat1, utmlat2, utmlon1, utmlon2);
	layer.SetWcsVersion(version);
	layer.SetWcsUrl(wcs_url);
	layer.SetWcsFormat(format);
	layer.SetWcsFormats(formats);
	
	layer.SetWcsSRS(srs_list); //PJ
	layer.SetWcsUTMvector(utm_vector); //PJ
int m=0;
}


std::string WcsLayerController::GetMapURL(const WcsLayer &layer, int width, int height, float latmin, float lonmin,
								float latmax, float lonmax, const std::string &srs) 
{
 	string options;
	wcsUrl wcs_url = layer.GetWcsUrl();
	if(wcs_url.options() != "") {
		options = wcs_url.options() + "&SERVICE=WCS&COVERAGE=" + layer.GetWcsName() +"&STYLES=&TRANSPARENT=TRUE";
	}
	else options = "SERVICE=WCS&COVERAGE=" + layer.GetWcsName() + "&STYLES=&TRANSPARENT=TRUE";

	wcsUrl url;
	url.mergeUrl(wcs_url.protocol(), wcs_url.server(), wcs_url.path(), options);
	
	// replace de white space for %20
	std::string wcs_image_format(layer.GetWcsFormat());
	std::string::iterator pos = std::find(wcs_image_format.begin(),wcs_image_format.end(), ' ');
	while(pos != wcs_image_format.end())
	{
		wcs_image_format.replace(pos, pos + 1, "%20");
		pos = std::find(wcs_image_format.begin(), wcs_image_format.end(), ' ');
	}

	const std::string &file_ext = wcs_image_format;

	std::string new_srs = layer.GetSRS();

	wcsClient wcs_client;
	wcsUrl wcs_cu;

	if(!new_srs.empty())
	{
		wcs_cu = wcs_client.getMapUrl(url,width,height,latmin,lonmin,latmax,lonmax,file_ext,layer.GetWcsVersion(),new_srs);
		cu = wcs_cu.url();
//		return wcs_client.getMap(url, width, height, latmin, lonmin, latmax, lonmax, file_ext, layer.GetWcsVersion(), filename, new_srs);
	}
	else
	{
		wcs_cu = wcs_client.getMapUrl(url,width,height,latmin,lonmin,latmax,lonmax,file_ext,layer.GetWcsVersion(),srs);
		cu = wcs_cu.url();
//		return wcs_client.getMap(url, width, height, latmin, lonmin, latmax, lonmax, file_ext, layer.GetWcsVersion(), filename, srs);
	}
	return cu;
}

bool WcsLayerController::DescribeCoverage(const std::string &folder)
{
	std::map<std::string, std::vector<std::string> >::iterator it = wcs_crs.find(folder);
	if(it != wcs_crs.end())
		return true;

 	string options;
	options = "SERVICE=WCS&COVERAGE=" + folder;
	wcsUrl url;
	url.mergeUrl(wcs_url.protocol(), wcs_url.server(), wcs_url.path(), options);

	wcsClient wcs_client;
	wcsUrl wcs_cu;

	wcs_cu = wcs_client.getDescribeCoverageUrl(url);

	wcsRefPtr<wcsMemoryStream> memoryStream;
	if(wcs_client.getDescribeCoverage(url))
    {
		memoryStream = wcs_client.getStream();
		std::string str1 = memoryStream->getBufferAsString();

		//Procesado de la str para obtener los SRS validos
		ProcessDescribeCoverage(folder, str1);
    }
    else return false;

	return true;

}


std::vector<std::string> WcsLayerController::WCS_CRS(const std::string &folder)
{
	std::map<std::string, std::vector<std::string> >::iterator it = wcs_crs.find(folder);
	if(it != wcs_crs.end())
		return it->second;
	else
		return std::vector<std::string>();
}


void WcsLayerController::ProcessDescribeCoverage(const std::string &folder, std::string &str1)
{
	std::vector<std::string> v;
	std::string ss(str1);
	
	int pos0 = str1.find("<gml:Envelope");
	if(pos0 == -1) return;
	
	while(pos0 != -1)
	{
		int pos1 = ss.find("srsName=\"", pos0);
		if(pos1 == -1) return;

		int pos2 = ss.find("\">",pos1);
		if(pos2 == -1) return;
		
		std::string s = ss.substr(pos1+9,pos2-(pos1+9));

		v.push_back(s);

		ss = ss.substr(pos2,ss.size());
		pos0 = ss.find("<gml:Envelope");
	}

	pos0 = ss.find("<requestResponseCRSs>");
	int posf = ss.find("</requestResponseCRSs>");
	ss = ss.substr(pos0,posf-pos0);
	ss.append(" ");
	while(pos0 != -1)
	{
		int pos1 = ss.find("EPSG:",0);
		if(pos1 == -1) break;

		int pos2 = ss.find(' ',pos1);
		if(pos2 == -1) break;

		std::string s = ss.substr(pos1, pos2-pos1);
		
		std::vector<std::string>::iterator it = v.begin();
		while( (it != v.end()) && (*it!=s)) it++;

		if(it==v.end())
			v.push_back(s);

		ss = ss.substr(pos2, ss.size());
		pos0 = ss.find("EPSG:");
	}
	wcs_crs.insert(std::make_pair<std::string, std::vector<std::string> >(folder, v));

}

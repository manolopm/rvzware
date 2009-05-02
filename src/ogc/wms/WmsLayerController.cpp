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


#include <ogc/wms/WmsLayerController.h>

#include <iostream>
#include <wms/wmsNotify.h>
#include <wms/wms.h>
#include <wms/wmsUrl.h>
#include <wms/wmsRefPtr.h>
#include <wms/wmsMemoryStream.h>
#include <wms/wmsCapabilitiesState.h>
#include <wms/wmsCapabilitiesParser.h>

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cpw/debugger.h>

using namespace std;
using namespace cpw::ogc;
    

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


WmsLayerController::WmsLayerController(void):cu()
{
  // initilize communication environment*/
  //wmsInitialize();
}

WmsLayerController::~WmsLayerController(void)
{

  // finalize communication environment
  //wmsFinalize();

}   


int WmsLayerController::ConnectToServer(const std::string &url) {

  wmsClient wms_client;
  wmsRefPtr<wmsMemoryStream> memoryStream;
    
  wms_url = url;

  if(wms_client.getCapabilities(wms_url))
    {
      memoryStream = wms_client.getStream();
      std::string str1 = memoryStream->getBufferAsString();
      wmsRefPtr<wmsCapabilitiesParser> parser = new wmsCapabilitiesParser;
      wms_root = parser->parseRawString(str1);

      if(!wms_root.valid()) return -1;
    }
  else return -1;

  return 0;
}

void WmsLayerController::GetInformation(std::string &info) {

  if(wms_root.valid())
    {
    }
}

void WmsLayerController::GetLayers(std::vector<std::string> &layers) {

  if(wms_root.valid())
    {
      vector<wmsRefPtr<wmsCapabilitiesState> > nodos;
      wms_root->getLeafNodes(nodos, true);
		 
      for(vector<wmsRefPtr<wmsCapabilitiesState> >::iterator i = nodos.begin(); i != nodos.end(); i++) {

	layers.push_back((*i)->name());

      }
    }
}


void WmsLayerController::AddChildrenNodes(Node *layerTree, std::vector<wmsRefPtr<wmsCapabilitiesState> > &children) {

  for(std::vector<wmsRefPtr<wmsCapabilitiesState> >::iterator i = children.begin(); i != children.end(); i++) {

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


void WmsLayerController::GetLayers(Node *layerTree){

  if (wms_root != NULL)
    {
      wmsRefPtr<wmsCapabilitiesState> theRootLayer = wms_root->root();

      if(!theRootLayer.valid()) return;

      layerTree->Name() = theRootLayer->name();
      layerTree->Title() = theRootLayer->title();

      if(theRootLayer->hasChildren())
	{
	  AddChildrenNodes(layerTree, theRootLayer->children());

	}
    }
}


void WmsLayerController::GetImageFormats(std::vector<std::string> &formats) {

  if(wms_root.valid())
    {
    }
}

void WmsLayerController::GetSRS(std::vector<std::string> &srs) {

  if(wms_root.valid())
    {

    }
}

bool WmsLayerController::GetMap(const WmsLayer &layer, int width, int height, float latmin, float lonmin,
				float latmax, float lonmax, std::string &filename, 
				const std::string &srs) 
{
  string options;
  wmsUrl wms_url = layer.GetWmsUrl();
  if(wms_url.options() != "") {
    options = wms_url.options() + "&SERVICE=WMS&LAYERS=" + layer.GetWmsName() +"&STYLES=&TRANSPARENT=TRUE";
  }
  else options = "SERVICE=WMS&LAYERS=" + layer.GetWmsName() + "&STYLES=&TRANSPARENT=TRUE";

  wmsUrl url;
  url.mergeUrl(wms_url.protocol(), wms_url.server(), wms_url.path(), options);
	
  // replace de white space for %20
  std::string wms_image_format(layer.GetWmsFormat());
  std::string::iterator pos = std::find(wms_image_format.begin(),wms_image_format.end(), ' ');
  while(pos != wms_image_format.end())
    {
      wms_image_format.replace(pos, pos + 1, "%20");
      pos = std::find(wms_image_format.begin(), wms_image_format.end(), ' ');
    }

  const std::string &file_ext = wms_image_format;

  std::string new_srs = layer.GetSRS();

  wmsClient wms_client;
  wmsUrl wms_cu;

  if(!new_srs.empty())
    {
      wms_cu = wms_client.getMapUrl(url,width,height,latmin,lonmin,latmax,lonmax,file_ext,layer.GetWmsVersion(),new_srs);
      cu = wms_cu.url();
      return wms_client.getMap(url, width, height, latmin, lonmin, latmax, lonmax, file_ext, layer.GetWmsVersion(), filename, new_srs);
    }
  else
    {
      wms_cu = wms_client.getMapUrl(url,width,height,latmin,lonmin,latmax,lonmax,file_ext,layer.GetWmsVersion(),srs);
      cu = wms_cu.url();
      return wms_client.getMap(url, width, height, latmin, lonmin, latmax, lonmax, file_ext, layer.GetWmsVersion(), filename, srs);
    }
}

bool WmsLayerController::GetMap(const string &layer_name, int width, int height, float latmin, float lonmin,
				float latmax, float lonmax, std::string &filename, const std::string &image_format, 
				const std::string &version, 
				const std::string &srs) {

  //if(wms_root.valid())
  //{

  string options;
  if(wms_url.options() != "") {
    options = wms_url.options() + "&SERVICE=WMS&LAYERS=" + layer_name +"&STYLES=&TRANSPARENT=TRUE";
  }
  else options = "SERVICE=WMS&LAYERS=" + layer_name + "&STYLES=&TRANSPARENT=TRUE";

  wmsUrl url;
  url.mergeUrl(wms_url.protocol(), wms_url.server(), wms_url.path(), options);
		
  wmsClient wms_client;
  // replace de white space for %20
  std::string wms_image_format = image_format;
  std::string::iterator pos = std::find(wms_image_format.begin(),wms_image_format.end(), ' ');
  while(pos != wms_image_format.end())
    {
      wms_image_format.replace(pos, pos + 1, "%20");
      pos = std::find(wms_image_format.begin(), wms_image_format.end(), ' ');
    }

  return wms_client.getMap(url, width, height, latmin, lonmin, latmax, lonmax, wms_image_format, version, filename, srs);
  //}
}


void WmsLayerController::CreateLayer(const std::string &layer_name, const std::string &format, WmsLayer &layer) {

  wmsRefPtr<wmsCapabilitiesState> LayerNode = wms_root->getNodeGivenName(layer_name);

  double lat1, lat2, lon1, lon2;
  double utmlat1, utmlat2, utmlon1, utmlon2;

  LayerNode->getLatLonBoundingBox(lat1, lon1, lat2, lon2);
  LayerNode->getUTMLatLonBoundingBox(utmlat1, utmlon1, utmlat2, utmlon2);
  //vector<wmsStyle> style = LayerNode->styles();
  vector<string> formats = LayerNode->formats();
  vector<string> srs_list = LayerNode->SRS();	
  vector<string> utm_vector = LayerNode->UTM_vector();

  string version = wms_root->wmsVersion(); 

  layer.SetWmsName(LayerNode->name());
  layer.SetWmsTitle(LayerNode->title());
  layer.SetLatLonBoundingBox(lat1, lat2, lon1, lon2);
  layer.SetUTMLatLonBoundingBox(utmlat1, utmlat2, utmlon1, utmlon2);
  layer.SetWmsVersion(version);
  layer.SetWmsUrl(wms_url);
  layer.SetWmsFormat(format);
  layer.SetWmsFormats(formats);
	
  layer.SetWmsSRS(srs_list); //PJ
  layer.SetWmsUTMvector(utm_vector); //PJ
  int m=0;
}


std::string WmsLayerController::GetMapURL(const WmsLayer &layer, int width, int height, float latmin, float lonmin,
					  float latmax, float lonmax, const std::string &srs) 
{
  string options;
  wmsUrl wms_url = layer.GetWmsUrl();
  if(wms_url.options() != "") {
    options = wms_url.options() + "&SERVICE=WMS&LAYERS=" + layer.GetWmsName() +"&STYLES=&TRANSPARENT=TRUE";
  }
  else options = "SERVICE=WMS&LAYERS=" + layer.GetWmsName() + "&STYLES=&TRANSPARENT=TRUE";

  wmsUrl url;
  url.mergeUrl(wms_url.protocol(), wms_url.server(), wms_url.path(), options);
	
  // replace de white space for %20
  std::string wms_image_format(layer.GetWmsFormat());
  std::string::iterator pos = std::find(wms_image_format.begin(),wms_image_format.end(), ' ');
  while(pos != wms_image_format.end())
    {
      wms_image_format.replace(pos, pos + 1, "%20");
      pos = std::find(wms_image_format.begin(), wms_image_format.end(), ' ');
    }

  const std::string &file_ext = wms_image_format;

  std::string new_srs = layer.GetSRS();

  wmsClient wms_client;
  wmsUrl wms_cu;

  if(!new_srs.empty())
    {
      wms_cu = wms_client.getMapUrl(url,width,height,latmin,lonmin,latmax,lonmax,file_ext,layer.GetWmsVersion(),new_srs);
      cu = wms_cu.url();
      //		return wms_client.getMap(url, width, height, latmin, lonmin, latmax, lonmax, file_ext, layer.GetWmsVersion(), filename, new_srs);
    }
  else
    {
      wms_cu = wms_client.getMapUrl(url,width,height,latmin,lonmin,latmax,lonmax,file_ext,layer.GetWmsVersion(),srs);
      cu = wms_cu.url();
      //		return wms_client.getMap(url, width, height, latmin, lonmin, latmax, lonmax, file_ext, layer.GetWmsVersion(), filename, srs);
    }
  return cu;
}

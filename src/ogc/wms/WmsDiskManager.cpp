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

#include <ogc/wms/WmsDiskManager.h>

#include <ogc/wms/WmsNetManager.h>
#include <ogc/wms/WmsLayer.h>

#include <vector>
#include <string>
#include <sstream>

using namespace cpw::ogc;

WmsDiskManager::WmsDiskManager(cpw::IRequestReceiver *in_thread, int npet): RequestThread(in_thread, npet), disk()
{
	AddThread(new WmsNetManager(this, npet));
	AddThread(new WmsNetManager(this, npet));
	AddThread(new WmsNetManager(this, npet));
	AddThread(new WmsNetManager(this, npet));
	AddThread(new WmsNetManager(this, npet));
	AddThread(new WmsNetManager(this, npet));
	AddThread(new WmsNetManager(this, npet));
	AddThread(new WmsNetManager(this, npet));
	AddThread(new WmsNetManager(this, npet));
	AddThread(new WmsNetManager(this, npet));
}


WmsDiskManager::~WmsDiskManager()
{
}


void WmsDiskManager::Process(cpw::Request &request) 
{

	std::string &tile = request.tile_id;
	cpw::TypeId &layer = request.layer->GetId();

	std::map<cpw::TypeId, std::set<std::string> >::iterator i = layer_tiles.find(layer);

	if(i == layer_tiles.end()) 
	{
		AddNewLayer(layer);
		i = layer_tiles.find(layer);
	}

	if(i != layer_tiles.end())
	{
		//set image extension
		std::string ext = FindExtension(request.layer);

		std::stringstream tile_path;
		const std::string filename = tile + "." + ext;
		tile_path << layer << "/" << filename;
		request.file = tile_path.str();
		request.filename = filename;

		std::set<std::string>::iterator j = i->second.find(filename);
		
		if(j != i->second.end() && !request.force)
		{
			ReturnRequest(request);
		}
		else
		{
			SendRequest(request);
		}
	}
}

void WmsDiskManager::ProcessReturn(cpw::Request &request) 
{

	if(!request.error)
		layer_tiles[request.layer->GetId()].insert(request.filename);
}


void WmsDiskManager::AddNewLayer(cpw::TypeId &layer)
{

	std::stringstream dir;
	dir << layer;

	disk.MakeDir(dir.str());

	std::vector<std::string> files;
	
	disk.Dir(dir.str(), files);

	layer_tiles[layer] = std::set<std::string>(files.begin(), files.end());

}

void WmsDiskManager::Modify(cpw::Entity *entity)
{

	cpw::TypeId layer = entity->GetId();

	std::stringstream dir;
	dir << layer;

	disk.RemoveDir(dir.str());

	layer_tiles.erase(dir.str());

	AddNewLayer(layer);

}


const std::string WmsDiskManager::FindExtension(const cpw::Entity *layer)
{
	WmsLayer *wms_layer = (WmsLayer *) layer;

	std::string &ext = wms_layer->GetWmsFormat();

	int pos = ext.find("png");

	if(pos != std::string::npos) return "png";

	pos = ext.find("gif");

	if(pos != std::string::npos) return "gif";

	pos = ext.find("tif");

	if(pos != std::string::npos) return "tif";

	pos = ext.find("jpeg");

	if(pos != std::string::npos) return "jpg";

	pos = ext.find("PNG");

	if(pos != std::string::npos) return "png";

	pos = ext.find("GIF");

	if(pos != std::string::npos) return "gif";

	pos = ext.find("TIF");

	if(pos != std::string::npos) return "tif";

	pos = ext.find("JPEG");

	if(pos != std::string::npos) return "jpg";

	return "bmp";

}

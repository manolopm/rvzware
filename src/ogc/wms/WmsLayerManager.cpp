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

#include <ogc/wms/WmsLayerManager.h>

using namespace cpw::ogc;


WmsLayerManager::WmsLayerManager(cpw::IRequestReceiver *visual, int npet, cpw::IStatusController *status): 
	cpw::RequestManager(visual, "WmsLayer", npet, status), srs()
{
}

WmsLayerManager::~WmsLayerManager(void)
{
}


void WmsLayerManager::Process(cpw::Request &request) 
{
	std::vector<cpw::Entity *> &layers = GetLayers();

 	for(int i = 0; i < (int) layers.size(); i++)
	{
		request.layer = layers[i];

		if(srs!="")
			((WmsLayer *)request.layer)->SetSRS(srs);

		if(TestBoundingBox(request, (WmsLayer *)layers[i]))
		
			SendRequest(request);

		else

			ReturnRequest(request);//DecrementNid(request.id);

	}
}


void WmsLayerManager::ProcessReturn(cpw::Request &request)
{
	int i = 0;
}

bool WmsLayerManager::TestBoundingBox(cpw::Request &request, WmsLayer* layer)
{
	return true;   //PJ
	const float xo = request.xo;
	const float yo = request.yo;
	const float xf = request.xf;
	const float yf = request.yf;

	double x1, x2, y1, y2;

	layer->GetUTMLatLonBoundingBox(x1, x2, y1, y2);

	if     (x1 <= xo && x2 <= xo){
		return false;
	}
	else if(x1 >= xf && x2 >= xf) 
	{
		return false;
	}
	else if(y1 <= yo && y2 <= yo) 
	{
		return false;
	}
	else if(y1 >= yf && y2 >= yf) 
	{
		return false;
	}

	return true;
}

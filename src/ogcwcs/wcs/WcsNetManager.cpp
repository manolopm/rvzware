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

#include <ogcwcs/WcsNetManager.h>

#include <ogcwcs/WcsLayerController.h>

using namespace cpw::ogcwcs;

WcsNetManager::WcsNetManager(cpw::IRequestReceiver *in_thread, int npet): RequestThread(in_thread, npet)
{

}

void WcsNetManager::Process(cpw::Request &request)
{

	static int fallos = 0;

	WcsLayerController controller;

	if(controller.GetMap(*((WcsLayer *)request.layer), request.tile_width, request.tile_height, request.yo, request.xo,  request.yf,  request.xf, 
					  request.file))
	{
		request.composed_url = controller.GetComposedURL();
		ReturnRequest(request);
	}
	else 
	{
		request.file = "wcs_error.png";
		request.error = true;
		ReturnRequest(request);
		fallos++;
	}
}


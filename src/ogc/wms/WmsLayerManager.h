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

#ifndef _WMSLAYERMANAGER_
#define _WMSLAYERMANAGER_

#include <vector>

#include <ogc/wms/WmsLayer.h>

#include <cpw/thread/RequestManager.h>

#include <ogc/Export.h>


namespace cpw 
{ 
	namespace ogc 
	{

		/** \brief Thread that attends WMS petitions from the scene
			\ingroup ogc
		*/
		class OGCEXPORT WmsLayerManager: public cpw::RequestManager
		{

		public:

			WmsLayerManager(cpw::IRequestReceiver *visual, int npet = 30000, cpw::IStatusController *status = NULL);

			virtual ~WmsLayerManager(void);

			virtual void Process(cpw::Request &request);

			virtual void ProcessReturn(cpw::Request &request);

			void SetSRS(std::string _srs) {srs = _srs;}


		private:

			bool TestBoundingBox(cpw::Request &request, WmsLayer* layer);
			std::string srs;

		};


	}

}

#endif



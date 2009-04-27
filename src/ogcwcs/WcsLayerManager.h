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

#ifndef _WCSLAYERMANAGER_
#define _WCSLAYERMANAGER_

#include <vector>

#include <ogcwcs/WcsLayer.h>

#include <cpw/thread/RequestManager.h>

#include <ogcwcs/Export.h>


namespace cpw 
{ 
	namespace ogcwcs 
	{

		/** \brief Thread that attends WCS petitions from the scene
			\ingroup ogcwcs
		*/
		class OGCWCSEXPORT WcsLayerManager: public cpw::RequestManager
		{

		public:

			WcsLayerManager(cpw::IRequestReceiver *visual, int npet = 30000, cpw::IStatusController *status = NULL);

			virtual ~WcsLayerManager(void);

			virtual void Process(cpw::Request &request);

			virtual void ProcessReturn(cpw::Request &request);

			void SetSRS(std::string _srs) {srs = _srs;}

			int NumPetitions();


		private:

			bool TestBoundingBox(cpw::Request &request, WcsLayer* layer);
			std::string srs;



		};


	}

}

#endif

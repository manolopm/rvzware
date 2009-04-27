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

#ifndef _DATABASECONTROLLER_
#define _DATABASECONTROLLER_

#include <wx/wx.h>

#include <controllers/EntityController.h>
#include <cpw/entity/Entity.h>
#include <cpw/persistent/database/DataBase.h>
#include <cpw/persistent/IPersistentManager.h>
#include <cpw/common/LatLong-UTMconversion.h>

#include <libpq-fe.h>

namespace cpw
{

	namespace controllers 
	{  

		/** 
			\brief This controller is responsible for the management of DataBase and Table
			\ingroup cpw::controllers
		*/
		class DataBaseController: public EntityController
		{
			public:
				
				DataBaseController();
				
				virtual ~DataBaseController();

				void AddDataBase(wxWindow* parent, cpw::LayerTree &layer_tree); 
				void CreateTable(wxWindow* parent, cpw::LayerTree &layer_tree); 

				//Testwfs
				struct GetMapWorkingData
				{
					bool inMemory; // TRUE Read everything in a stringstream . FALSE Write the map image into a file
					std::string *membuf; // The memory buffer in case we use it
					FILE *fd; // The file descriptor in case we are writing to file
				};
				bool LoadWFS();
				bool GetMap(const std::string &url, bool ssl, std::string &memBuf, const std::string &proxy);
				static size_t writeCallback(void *buffer, size_t size, size_t nmemb, void *getMapWorkingData);
				//End test wfs
									  
			private:

				//DB access
				void DisconnectDB(cpw::DataBase &DB);
				bool ConnectDB(cpw::DataBase &DB);
				//
				void AddTablesToTree(cpw::DataBase &DB);

				

		};

	}
}

#endif

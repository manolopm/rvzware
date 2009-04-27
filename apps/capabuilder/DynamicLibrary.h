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


#ifndef _DYNAMICLYBRARY_
#define _DYNAMICLYBRARY_

#include <string>
#include <wx/wx.h>
#include "PluginInterface.h"


	/** 
		\brief This class is used to load dynamic libraries (dll) into the application during the execution
	*/
	class DynamicLibrary
	{
	public:
		DynamicLibrary(){};
		~DynamicLibrary();
		void Load(const std::string &library);
		HINSTANCE GetH() {return h;}
		void EntitiesToRegister(std::vector<cpw::Entity*> &ventityregister);
		void RegisterPluginMenu(std::string &nmenu);
		void ExecutePlugin(std::vector<cpw::Entity*> &ventity, cpw::ApplicationScene *appscene, const std::string &entity_path, const std::string &icon_path, const std::string &data_path, const std::string &language, wxWindow *parent, cpw::INavigatorManager *navigator_manager, cpw::IStatusController *status_controller = NULL);
	private:
		std::string library_url;
		HINSTANCE  h; 
	};


#endif

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


#ifndef _PLUGINITERFACE_
#define _PLUGINITERFACE_

#include "windows.h"

#include <string>
#include <vector>
#include <wx/wx.h>


#include <cpw/entity/Entity.h>
#include <cpw/graphic/ApplicationScene.h>
#include <cpw/entity/EntityFactory.h>
#include <cpw/graphic/INavigatorManager.h>
#include <cpw/common/IStatusController.h>

#ifndef IMPORT
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT __declspec(dllimport)
#endif

/** 
	\brief This is the interface for the plugins of the application
*/


//Here we declare the API for the plugins
extern "C" DLLEXPORT void EntitiesToRegister(std::vector<cpw::Entity*> &v);

extern "C" DLLEXPORT void RegisterMenu(std::string &menu);
//extern "C" DLLEXPORT void PluginEntityType(std::string &class_name);
//extern "C" DLLEXPORT void ExecPlugin(cpw::Entity **entity, cpw::EntityFactory &entity_primitive_factory); 
extern "C" DLLEXPORT void ExecPlugin(std::vector<cpw::Entity*> &ventity, cpw::ApplicationScene *appscene, const std::string &entity_path, const std::string &icon_path, const std::string &data_path, const std::string &language, wxWindow *parent, cpw::INavigatorManager *navigator_manager, cpw::IStatusController *status_controller);


typedef void (*entities_to_register)(std::vector<cpw::Entity*> &v);

typedef void (*register_menu)(std::string &);
//typedef void (*plugin_entity_type)(std::string &);
//typedef void (*exec_plugin)(cpw::Entity **, cpw::EntityFactory &entity_primitive_factory);
typedef void (*exec_plugin)(std::vector<cpw::Entity*> &ventity, cpw::ApplicationScene *appscene, const std::string &entity_path, const std::string &icon_path, const std::string &data_path, const std::string &language, wxWindow *parent, cpw::INavigatorManager *navigator_manager, cpw::IStatusController *status_controller);

//typedef void (*DLLFunctionPtr) (HWND);
//typedef void (*pf)();

#endif

/*
 * CAPAWARE 3D Geographical Multilayer Framework Free Software
 * Copyright (C) 2009 Instituto Tecnológico de Canarias (ITC) and Universidad de Las Palmas de Gran Canaria (ULPGC)
 *
 * This file is part of CAPAWARE.
 *
 * CAPAWARE is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * CAPAWARE is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this application; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * The CAPAWARE development team
*/

#include <windows.h>
#include <iostream>
#include <sstream>

#include "stdafx.h"

#include "DynamicLibrary.h"


using namespace std;  
DynamicLibrary::~DynamicLibrary()
{
	FreeLibrary((HMODULE)h);
}
void DynamicLibrary::Load(const std::string &library)
{
	library_url = library;
	h = LoadLibrary(library_url.c_str());
	if(h == NULL)
		std::cout << "Error..." << endl;	
}

void DynamicLibrary::RegisterPluginMenu(std::string &nmenu)
{
	if (h != NULL)
	{
		register_menu menu = (register_menu) GetProcAddress((HMODULE)h, "RegisterMenu");
		if (menu != NULL)
			menu(nmenu);	
	}
}

void DynamicLibrary::ExecutePlugin(std::vector<cpw::Entity*> &ventity, cpw::ApplicationScene *appscene, const std::string &entity_path, const std::string &icon_path, const std::string &data_path, const std::string &language, wxWindow *parent, cpw::INavigatorManager *navigator_manager,cpw::IStatusController* status_controller/*cwcontent::StatusController *status_controller*/)
{
	if (h != NULL)
	{
		exec_plugin exec = (exec_plugin) GetProcAddress((HMODULE)h, "ExecPlugin");
		if (exec != NULL)
			exec(ventity, appscene, entity_path, icon_path, data_path, language, parent, navigator_manager,status_controller);
	}
}

void DynamicLibrary::EntitiesToRegister(std::vector<cpw::Entity*> &ventityregister)
{
	if (h != NULL)
	{
		entities_to_register etr = (entities_to_register) GetProcAddress((HMODULE)h, "EntitiesToRegister");
		if (etr != NULL)
			etr(ventityregister);	
	}
}
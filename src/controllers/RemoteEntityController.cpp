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
#include <sstream>

#include "stdafx.h"

#include <controllers/RemoteEntityController.h>

#include <controllers/PersistentController.h>

#include <cpw/ApplicationConfiguration.h>

#include <cpw/common/types.h>
#include <cpw/entity/EntityRegistry.h>
#include <cpw/entity/EntityFactory.h>
#include <cpw/entity/ContainerLayer.h>
#include <cpw/persistent/file/PersistentFileManager.h>


using namespace cpw::controllers;


RemoteEntityController::RemoteEntityController(): ui_dialog(NULL)
{
}

RemoteEntityController::RemoteEntityController(wxWindow* parent): parent_(parent)
{
	ui_dialog = new cpw::gui::UIRemoteImportation(parent_, wxID_ANY);
}

RemoteEntityController::~RemoteEntityController(void)
{

}

void RemoteEntityController::CreateRemoteEntity(wxWindow* parent)
{
	if (ui_dialog == NULL)
		ui_dialog = new cpw::gui::UIRemoteImportation(parent, wxID_ANY);

	ui_dialog->Show();

	//std::string &project_path = ApplicationConfiguration::GetInstance()->GetDataDirectory();
	//wxFileDialog dialog (parent,_T("Open Project"),_T(project_path.c_str()),wxEmptyString,
	//						   _T("Project(*.cws)|*.cws|All files(*.*)|*.*") );


	//if(dialog.ShowModal() == wxID_OK)
	//{						
	//	std::string url = dialog.GetPath();
	//	wxMessageDialog message(parent,wxString("You need to close the application\nto load the new scene.\nDo you want to exit now?"), wxString("Warning"), wxICON_EXCLAMATION |wxYES_NO |wxCANCEL);
	//	if(message.ShowModal() == wxID_YES)
	//	{
	//		ApplicationConfiguration::GetInstance()->SetSceneFile(url);
	//		close_application = true;
	//	}
	//}
}
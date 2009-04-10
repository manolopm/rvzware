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
#include "stdafx.h"

#include "controllers/PersistentController.h"
#include <cpw/ApplicationConfiguration.h>
#include <cpw/callback/Callable.h>
#include <cpw/entity/EntityFactory.h>
#include <cpw/persistent/file/PersistentFileManager.h>
#include <cpw/entity/EntityRegistry.h>


using namespace cpw::controllers;


PersistentController::PersistentController(void)
{
	file_description["Entity"]  = "Element(*.cel)|*.cel|Layer(*.cla)|*.cla|All files(*.*)|*.*";
	file_description["Element"] = "Element(*.cel)|*.cel|All files(*.*)|*.*";
	file_description["Layer"]   = "Layer(*.cla)|*.cla|All files(*.*)|*.*";


	file_description["Primitive"]         = "Element Primitive(*.cep)|*.cep|Layer Primitive(*.clp)|*.clp|All files(*.*)|*.*";
	file_description["Element Primitive"] = "Element Primitive(*.cep)|*.cep|All files(*.*)|*.*";
	file_description["Layer Primitive"]   = "Layer Primitive(*.clp)|*.clp|All files(*.*)|*.*";
}

PersistentController::~PersistentController(void)
{
}






//load a primitive from disk without error management
cpw::PersistentError PersistentController::Load(cpw::Persistent **persistent, const std::string &url)
{
	cpw::Node *root;
	*persistent = NULL;

	cpw::PersistentFileManager persistent_manager;
	cpw::PersistentError error = persistent_manager.Load(url, &root);

	if (error == cpw::PERSISTENTOK)
	{
		if(root != NULL)
		{

			*persistent = cpw::EntityFactory::GetInstance()->CreateEntity(root->GetName());
			
			if(*persistent != NULL)
			{
				(*persistent)->SetPersistence(root);
				(*persistent)->SetUrl(url);
				(*persistent)->Saved();
			}
			delete root;
		}
	}
	else if(root != NULL) delete root;

	return error;

}


//load an object with error management
cpw::Persistent *PersistentController::Load(wxWindow* parent, const std::string &url, const std::string &type)
{
	cpw::Persistent *persistent; 
	cpw::PersistentError ferror = Load(&persistent, url);

	if (ferror == cpw::PERSISTENTOK)
	{
		if(persistent == NULL)
		  {
		    std::string msg = "Error loading the " + type + ". \nThe file may be corrupted.";
		  wxMessageDialog message(parent,(const wxString&) msg, 
					  wxT("Error"),
					  wxICON_WARNING |wxOK);
			message.ShowModal();
		}
	}
	else
	  {
	    std::string msg = "Error loading the " + type + ". \nFile can not be opened.";
	    wxMessageDialog message(parent, (const wxString&)msg, 
				    wxT("Error"),wxICON_WARNING |wxOK);
		message.ShowModal();
	}

	return persistent;
}


//load an object using a File Dialog
cpw::Persistent *PersistentController::Load(wxWindow* parent, const std::string &type)
{
	cpw::Persistent *persistent; 
	
	std::string msg = "Open " + type;
	wxFileDialog dialog (parent,(const wxString &)msg,
			     wxEmptyString,wxEmptyString,
			     (const wxString &)file_description[type]);

	if(dialog.ShowModal() == wxID_OK)
	{						
	  wxString url = dialog.GetPath();
	  persistent = Load(parent, (std::string &)url);
	}

	return persistent;
}


cpw::PersistentError PersistentController::Save(wxWindow* parent, cpw::Persistent *persistent, bool overwrite)
{

	cpw::PersistentError ferror;

	bool stop = false;
	
	do
	{

	  std::string url = persistent->GetUrl();
		ferror = persistent->Save(overwrite);
		
		int modal;

		switch (ferror)
		{
			case cpw::PERSISTENTALREADYEXISTS:
			  {
			    
			    wxMessageDialog message1(NULL,wxT("File already exists.\n Do you want to overwrite it?"), wxT("Warning"),wxICON_EXCLAMATION |wxYES_NO|wxCANCEL);				
					modal = message1.ShowModal();
					if (modal == wxID_YES)
					{
						ferror = persistent->Save(true); 
					}
					if (modal == wxID_NO)
					{
						// mostramos un filedialog para buscar elegir un nuevo nombre ???
						wxFileDialog dlg_file (NULL,_T("Save entity"),wxEmptyString,wxEmptyString,
							   _T("Element(*.cel)|*.cel|Layer(*.cla)|*.cla|All files(*.*)|*.*") );
						dlg_file.SetPath((const wxString&)url);
						if(dlg_file.ShowModal() == wxID_OK)
						  {
						    wxString str = dlg_file.GetPath();
						    std::string stdstr = std::string(str.mb_str());
						    persistent->SetUrl((const std::string &)stdstr);
						  }
						else stop = true;						
					}
					if (modal == wxID_CANCEL) stop = true;
					break;
				}
			case cpw::PERSISTENTPERMISSIONDENIED:
			  {
			    
			    wxMessageDialog message2(NULL, wxT("Error writing file, permission denied.\n Do you want to select other path?"), wxT("Warning"),wxICON_EXCLAMATION |wxYES_NO);
					modal = message2.ShowModal();
					if (modal == wxID_YES)
					{						
						wxFileDialog dlg_file (NULL,_T("Save entity"),wxEmptyString,wxEmptyString,
							   _T("Element(*.cel)|*.cel|Layer(*.cla)|*.cla|All files(*.*)|*.*") );
						dlg_file.SetPath((const wxString &)url);
						if(dlg_file.ShowModal() == wxID_OK)
						{							
						  //ferror = Save(entity, url, persistent_manager, overwrite);
						  wxString str = dlg_file.GetPath();
						  std::string stdstr = std::string(str.mb_str());
						  persistent->SetUrl((const std::string &)stdstr);
						}
						else
							stop = true;
						
					}
					else
						stop = true;
					
					break;
				}
		}
	}while(ferror != cpw::PERSISTENTOK && !stop); 
	
	return ferror;
}


cpw::PersistentError PersistentController::Save(wxWindow* parent, cpw::Persistent *persistent, 
												const std::string &extension, bool overwrite, bool primitive_directory)
{
	std::string directory;
	
	if(primitive_directory)
		
		directory = cpw::ApplicationConfiguration::GetInstance()->GetPrimitiveDirectory();

	else 

		directory = cpw::ApplicationConfiguration::GetInstance()->GetEntityDirectory();

	std::string file_name = directory + ((cpw::Entity *)persistent)->GetName() + extension;

	persistent->SetUrl(file_name);

	return Save(parent, persistent, overwrite);
}

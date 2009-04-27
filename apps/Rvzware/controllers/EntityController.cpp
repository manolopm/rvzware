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

#include <sstream>

#include <stdafx.h>

#include <controllers/EntityController.h>
#include <controllers/RelativeDirectory.h>
#include <controllers/PersistentController.h>

#include <cpw/ApplicationConfiguration.h>
#include <cpw/graphic/ApplicationScene.h>

#include <cpw/common/types.h>
#include <cpw/entity/EntityRegistry.h>
#include <cpw/entity/EntityFactory.h>
#include <cpw/entity/ContainerLayer.h>
#include <cpw/persistent/file/PersistentFileManager.h>



using namespace cpw::controllers;


EntityController::EntityController(void)
{
}

EntityController::EntityController(cpw::IStatusController *istatus_controller) : isc(istatus_controller)
{
}

EntityController::~EntityController(void)
{

}


cpw::TypeId EntityController::GetIdFromFile(const std::string &url, cpw::Node **root)
{
	cpw::TypeId id = cpw::TypeId();

	cpw::PersistentFileManager persistent_manager;

	cpw::PersistentError error = persistent_manager.Load(url, root);

	//std::stringstream ss;
	//ss << id << "\n" << url << "\n" << error << "\n";

	if (error == cpw::PERSISTENTOK)
	{
		//ss << *root << "\n";
		if(*root != NULL)
		{
			std::istringstream str((*root)->GetChildValue("id"));
			
			str >> id;

			//delete *root;
		}
	}
	//ss << id;
	//wxMessageBox(ss.str() , _("Information"), wxICON_INFORMATION);

	return id;

}


cpw::Entity* EntityController::Load(const std::string &url, bool all, cpw::Entity *parent)
{
	traceable_id = isc->Attach();

	isc->SetDetermined(traceable_id, false);
	isc->SetModal(traceable_id, true);
	isc->SetRange(traceable_id, 100);
	isc->SetValue(traceable_id, 0);
	isc->SetStep(traceable_id, 5);
	isc->SetLabel(traceable_id, "Loading data..");
	isc->Pulse(traceable_id);


	cpw::Entity *result = Load2(url, all, parent);

	isc->Detach(traceable_id);
	
	return result;
}
cpw::Entity* EntityController::Load2(const std::string &url, bool all, cpw::Entity *parent)
{
	controllers::RelativeDirectory path(url);

	cpw::Entity *entity = NULL;

	if(path.IsValid())
	{

		std::string filename = path.GetFilename();

		cpw::Node *root=NULL;
		cpw::TypeId id = GetIdFromFile(filename, &root);

		
		entity = cpw::EntityRegistry::GetInstance()->GetEntity(id); 

		if(entity == NULL)
		{

			if(root != NULL)
			{
				entity = cpw::EntityFactory::GetInstance()->CreateEntity(root->GetName());

				if(entity != NULL)
				{
					entity->SetPersistence(root);
					entity->SetUrl(filename);

					if (parent != NULL)
					{
						((cpw::Loggable*)parent)->SetRegisterChanges(false);
						parent->Add(entity);
						((cpw::Loggable*)parent)->SetRegisterChanges(true);
					}

					Register(entity);
					
					entity->GraphicInsert();

					if(all && entity->isContainer())
					{
						std::vector<cpw::Node *> components = root->GetChildren();
						std::vector<cpw::Node *>::iterator i = components.begin();
						while ((i!=components.end()) && ((*i)->GetName() != std::string("components"))) i++;
						if (i!=components.end())
						{
							std::vector<cpw::Node *> comp = (*i)->GetChildren();
							std::vector<cpw::Node *>::iterator j = comp.begin();
							for(j;j!=comp.end();j++)
							{
								//isc->SetLabel(traceable_id, "Loading data..");								
								isc->Pulse(traceable_id);
								cpw::Entity* et = Load2((*j)->GetChildValue("url"), all, entity);
							}
						}
					}
					entity->Saved();
				}
			}

		}
		else
		{
			if (parent != NULL)
			{
				
				((cpw::Loggable*)parent)->SetRegisterChanges(false);		
				parent->Add(entity);
				((cpw::Loggable*)parent)->SetRegisterChanges(true);
			}
		}

		if(root != NULL) 
			delete root;
		root = NULL;
	}

	return entity;
}

cpw::Entity* EntityController::Load(wxWindow* parent, cpw::LayerTree &layer_tree)
{
	std::string &entity_path = ApplicationConfiguration::GetInstance()->GetEntityDirectory();
	wxFileDialog dialog ((wxWindow *)parent,(const wxString&)_("Open entity"),
			     (const wxString&)entity_path,
			     wxEmptyString,
			     _("Element(*.cel)|*.cel|Layer(*.cla)|*.cla|All files(*.*)|*.*") );

	cpw::Entity *entity = NULL;

	if(dialog.ShowModal() == wxID_OK)
	{						
	  std::string url =(std::string)(dialog.GetPath().mb_str());

	entity = Load(url);
	
	if(entity == NULL)
	  {
	    wxMessageDialog message(parent,wxT("Error loading the entity. \nThe file may be corrupted."),
				    wxT("Error"),
				    wxICON_WARNING |wxOK);
	    message.ShowModal();
	  }
	
	}

	return entity;
}


cpw::PersistentError EntityController::Save(cpw::Entity *entity, bool all, bool overwrite)
{

	cpw::PersistentError ferror = cpw::PERSISTENTOK;
	
	if(entity->isModified()) 
	{
		ferror = entity->Save(overwrite);
	}
	if(all && entity->isContainer())

		for(int i = 0; i < entity->GetNumChildren(); i++)

			Save(entity->GetChild(i), all, overwrite);

	return ferror;
}



bool EntityController::AddEntity(wxWindow* parent, cpw::LayerTree &layer_tree)
{

	cpw::Entity *entity = Load(parent, layer_tree); 


	if(entity != NULL)
	{
		if(!layer_tree.AddToActiveLayer(entity))
		{
			
		  wxMessageDialog message(parent,wxT("Unable to add the entity to the layer tree. Recursive inclusion."), wxT("Capaware"),
					  wxICON_WARNING |wxOK);
			message.ShowModal();

			return false;
		}
		else
		{
			if(!cpw::ApplicationScene::GetInstance()->GetScene()->ObjectExistsInScene(entity->GetId()))
				entity->GraphicInsert();

			return true;
		}
	}
	return false;
}




void EntityController::InitLayerTree(const std::string &scene_file, cpw::LayerTree &layer_tree)
{
	cpw::Entity *entity = Load(scene_file);

			

	if(entity == NULL) {
		
		cpw::ContainerLayer layer;
		entity = (cpw::Layer*) cpw::EntityFactory::GetInstance()->CreateEntity(layer.GetClassName());

		entity->SetName("TopLayer");
		entity->SetUrl(scene_file);
		
		boost::filesystem::path directory(cpw::ApplicationConfiguration::GetInstance()->GetDefaultDirectory());
		boost::filesystem::path previous_path = boost::filesystem::initial_path();
		boost::filesystem::path default_path   = (directory.has_root_name())? directory : previous_path / directory;

		default_path /= "folder.png";

		entity->SetIcon(default_path.string());

		Register(entity);
		
		entity->GraphicInsert();
	}

	layer_tree.AddToActiveLayer(entity);
}


void EntityController::Register(cpw::Entity *entity)
{

	if(entity != NULL)
	{

		cpw::EntityRegistry::GetInstance()->Add(entity);

		RegisterPrimitive(entity->GetPrimitiveUrl());


	}

}


void EntityController::RegisterPrimitive(const std::string &primitive_url)
{

	if(!primitive_url.empty())
	{

		cpw::Entity* primitive = cpw::EntityFactory::GetInstance()->GetPrototypeFromUrl(primitive_url);

		if(primitive == NULL)
		{

			PersistentController persistent;
			
			cpw::PersistentError error = persistent.Load((cpw::Persistent**)&primitive, primitive_url);

			if(error == cpw::PERSISTENTOK)
			{
				if(!cpw::EntityFactory::GetInstance()->Register(primitive))
					delete primitive;

			}
		}
	}
}

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

#include <controllers/ProjectController.h>
#include <controllers/EntityController.h>
#include <controllers/RelativeDirectory.h>
#include <controllers/CwsLoader.h>

#include <cpw/ApplicationConfiguration.h>
#include <cpw/entity/EntityRegistry.h>
#include <cpw/graphic/ApplicationScene.h>

#include <UIApplicationMainFrame.h>


using namespace cpw::controllers;


ProjectController::ProjectController(): close_application(false), project_name(), project_fullname(), srs()
{
}

ProjectController::~ProjectController(void)
{

}

bool ProjectController::GetCloseApplication() 
{
  return close_application;
}


bool ProjectController::OpenProject(const std::string &url,
				    const std::string &default_path,
				    cpw::IStatusController *status_controller, 
				    cpw::LayerTree &layer_tree,
				    cpw::IGraphicFactory *graphic_factory)
{
  cpw::controllers::RelativeDirectory path(url);

  if(path.IsValid())
    {
      project_fullname = url;
      std::string filename = path.GetFilename();
      //TODO: BUG POR AQUI

      cpw::controllers::CwsLoader cws_loader(filename);

      project_name = filename;

      cpw::ISceneLoader *scene_loader = graphic_factory->CreateSceneLoader();
      void *scene_data;
      scene_data = scene_loader->LoadScene(filename);

      if (scene_loader)
	{
	  delete scene_loader;
	  scene_loader = NULL;
	}
      
      cpw::ApplicationScene *appScene  = cpw::ApplicationScene::GetInstance();
      appScene->GetScene()->InitScene();
      
      if (scene_data != NULL)	
	{
	  if(appScene->GetScene()->SetSceneData((cpw::IScene *)scene_data))
	    {
	      
	      cpw::controllers::EntityController ec(status_controller);
	      
	      controllers::RelativeDirectory path(cws_loader.GetTopLayer());
	      std::string filename2 = path.GetActualPath()+"/";
	      
	      srs = cws_loader.GetSRS();
	      
	      ec.InitLayerTree(cws_loader.GetTopLayer(), layer_tree);	
	      appScene->GetScene()->InitializeTextures();
	      
	      cpw::ApplicationConfiguration* appconfig = cpw::ApplicationConfiguration::GetInstance();
	      
	      appconfig->SetSceneFile(filename);
	      appconfig->SetEntityDirectory(filename2);
	      
	      return true;
	    }
	  else
	    return false;
	}
      else
	{	
	  return false;
	}
    }

  return false;

}


bool ProjectController::OpenProject(wxWindow* parent,
				    const std::string &default_path,
				    cpw::IStatusController *status_controller, 
				    cpw::LayerTree &layer_tree,
				    cpw::IGraphicFactory *graphic_factory)
{
  std::string &project_path = ApplicationConfiguration::GetInstance()->GetDataDirectory();
  std::string str = "Project(*.cws)|*.cws|All files(*.*)|*.*";
  wxString wxstr (str.c_str(), wxConvUTF8);
  std::string str2 = "Open Project";
  wxString wxstr2 (str2.c_str(), wxConvUTF8);
  wxString wxstr3 (project_path.c_str(), wxConvUTF8);
  wxFileDialog dialog ((wxWindow *)parent,
		       wxstr2,
		       wxstr3,
		       wxEmptyString,
		       (const wxString &)wxstr );
  dialog.Center();

  bool stop;

  do
    {
      std::string url;

      if(dialog.ShowModal() == wxID_OK)
	{						
	  ((cpw::gui::UIApplicationMainFrame *)parent)->Refresh();
	  ((cpw::gui::UIApplicationMainFrame *)parent)->Update();
	  ((cpw::gui::UIApplicationMainFrame *)parent)->RePaint();
	  ((cpw::gui::UIApplicationMainFrame *)parent)->UpdateAUIManager();

	  url = dialog.GetPath().mb_str();
			
	  bool result = OpenProject(url, default_path, status_controller, layer_tree, graphic_factory);

	  if(!result) 
	    {
	      wxMessageDialog dlg_error(parent,wxT("Error opening the scene file. Select a new scene from file."), wxT("Warning"),wxICON_EXCLAMATION|wxOK);
	      dlg_error.Center();
	      dlg_error.ShowModal();
	      stop = false;
	    }
	  else 
	    {
	      return true;
	    }
	}
      else 
	return false;

    }while(!stop);

  return false;

}

void ProjectController::CloseProject(cpw::LayerTree &layer_tree)
{
  cpw::ApplicationConfiguration* appconfig = cpw::ApplicationConfiguration::GetInstance();

  appconfig->SetSceneFile("");

  layer_tree.Clear();

  cpw::EntityRegistry::GetInstance()->Clear();

  cpw::ApplicationScene::GetInstance()->GetScene()->Clear();
}



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


#include <wx/wx.h>
#include <sstream>
#include <fstream>
#include <map>
#include <string>
#include <boost/filesystem.hpp>
#include "stdafx.h"
#include "Application.h"
 #include <windows.h>
#include <iosg/OsgFactory.h>

///// builder ///////
#include <vpb/Commandline>
#include <vpb/DataSet>
#include <vpb/DatabaseBuilder>
#include <vpb/System>
#include <vpb/Version>
#include <vpb/FileUtils>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <iostream>
#include <iosg/OsgNavigator.h>
#include <iosg/OsgScene.h>
//////////////////////////

#include <cpw/graphic/IGraphicFactory.h>
#include <cpw/persistent/file/PersistentFileCallBack.h>
#include <cpw/graphic/ISceneLoader.h>
#include <cpw/graphic/ApplicationScene.h>
#include <cpw/graphic/IHandler.h>
#include <cpw/graphic/TraslationGizmo.h>
#include <cpw/graphic/RotationGizmo.h>
#include <cpw/graphic/ScaleGizmo.h>
#include <cpw/entity/EntityRegistry.h>
#include <cpw/entity/EntityFactory.h>
#include <cpw/entity/Entity.h>
#include <cpw/entity/Element.h>
#include <cpw/entity/ContainerLayer.h>
#include <cpw/entity/RasterLayer.h>
#include <cpw/entity/VectorialLayer.h>
#include <cpw/common/ApplicationLog.h>
#include <cpw/common/IdGenerator.h>
#include <ogc/WmsDiskManager.h>
#include <wms/wms.h>
#include <ogcwcs/WcsDiskManager.h>
#include <wcs/wcs.h>
#include <cpw/ApplicationConfiguration.h>
#include <controllers/GraphicObjectController.h>
#include <controllers/ElementPrimitiveController.h>
#include <controllers/LayerPrimitiveController.h>
#include <controllers/EntityController.h>
#include <controllers/RemoteController.h>
#include <controllers/PersistentController.h>
#include <controllers/ConfigurationController.h>
#include <controllers/LayerController.h>
#include <controllers/DataBaseController.h>
#include <controllers/CwsLoader.h>
#include <controllers/RelativeDirectory.h>
#include <cpwutils/Fire.h>
#include <cpwutils/GraphicFireCallBack.h>



Application::Application(void): main_frame(), ec(), ecm(), fc(), evc(),evcm(), fcm(),  navigator_manager(), graphic_factory(), 
layer_tree(), plugin_map(), dc(NULL), builder_camera_controller(NULL), bb_controller(NULL), gen_wait(NULL), build_thread(NULL), wmsl(NULL), 
close(false),load_wcs(false), wcsl(NULL), wcsdir(), wcsurl()

{
	//for being able to load all type of images
	wxInitAllImageHandlers();

	//This is to activate memory-leaks detection
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	wmsInitialize();
	application_time = ApplicationTime::GetInstance();
	application_time->Off();
	cpw::IdGenerator::SetNamespace(GetMACaddress());
	SetThreadAffinityMask(GetCurrentThread(), 0);
}

Application::~Application(void)
{
	
	std::map<int, DynamicLibrary*>::iterator mi = plugin_map.begin();
	for( ; mi!=plugin_map.end() ; mi++)
		delete (*mi).second;
	delete status_controller;
	delete sun_controller;
	delete project_controller;
	delete remote_entity_controller;

}

void Application::EnableSunConfiguration(const bool &value)
{
	if (navigator_manager != NULL)
		navigator_manager->SetLightningToAllNavigators(value);	

	cpw::ApplicationScene::GetInstance()->GetScene()->SetLightning(value);
}

cpw::IWindow * Application::AddNavigator(wxWindow *parent, int id)
{
	return navigator_manager->AddNavigator(parent, id);
}

void Application::SetActiveNavigator(int id)
{
	navigator_manager->SetActive(id);
}

void Application::SetSunPosition(const float &pos)
{
	sun_controller->SetSunPosition(pos);
}


bool Application::OnInit()
{
	status_controller = new cpw::controllers::StatusController();
	sun_controller = new cpw::controllers::SunController();
	remote_entity_controller = new cpw::controllers::RemoteEntityController();
	project_controller = new cpw::controllers::ProjectController();
	graphic_factory = new cpw::iosg::OsgFactory(this);
	
	//Load the configuration in the Singleton class ApplicationConfiguration
	cpw::ApplicationConfiguration *appConfig = cpw::ApplicationConfiguration::GetInstance();
	cpw::controllers::ConfigurationController config;
	config.Load(appConfig);

	main_frame = new cpw::gui::UIApplicationMainFrame(this, wxString(_T("CapaBuilder")), 0,0 , 1024, 768, (wxBORDER_SUNKEN | wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN));
	main_frame->Show(false);
	main_frame->Maximize(true);
	main_frame->ShowSplash();

	bool file_error = InitCpwEntities();

	//Remote initialization stuff
	int listen_port = cpw::ApplicationConfiguration::GetInstance()->GetListenPort();
	remote_controller = new cpw::controllers::RemoteController(main_frame, &layer_tree, listen_port);
	
	main_frame->InitGUIContents(layer_tree, remote_controller->GetConnectionManager(), navigator_manager);
	main_frame->Show(true);	
	main_frame->EnableGUI(file_error);
	main_frame->HideSplash();

	//TestDependencies("WmsLayer");
	navigator_manager->GetFocusedOrFirstNavigator()->ShowHUD(false);

	cpw::ApplicationScene::GetInstance()->GetScene()->PreBuild();
	//BuildScene();

	//navigator_manager->SetAllNavigatorsToInitialPosition();
	navigator_manager->SetAllNavigatorsToLookDown();

	
	return true;
}

void Application::InitHandlers()
{
	std::string path = GetDefaultPath();
	cpw::IScene *scene  = cpw::ApplicationScene::GetInstance()->GetScene();

}


void Application::Save()
{
	
}
void Application::SaveAll()
{
	cpw::controllers::EntityController controller(status_controller);
	cpw::PersistentError ferror;
	ferror = controller.Save(layer_tree.GetTopLayer(), true, true);
	if (ferror != cpw::PERSISTENTOK)
	{
		wxMessageDialog message(NULL,wxString("Some entities from the layer tree could not be saved."), wxString("Warning"),wxICON_EXCLAMATION |wxOK);
		message.ShowModal();
	}
}

bool Application::Close()
{
	bool flag = true;
	bool cancel = false;
	
	handler_controller.ActivateAllHandlers(false);

	
	
	if (!cancel)
	{
		if (bb_controller != NULL)
			delete bb_controller;
		close = true;
		if(build_thread != NULL)
		{
			build_thread->join();
			delete build_thread;
			build_thread = NULL;
		}

		if(gen_wait != NULL)
		{
			delete gen_wait;
			gen_wait = NULL;
		}


		delete remote_controller;
		delete navigator_manager;
		delete graphic_factory;
		delete wms_request_manager;
		delete wcs_request_manager;
	
		delete builder_camera_controller;
		
		if (ec != NULL)
			delete ec;
		if (ecm != NULL)
			delete ecm;
		if (fc != NULL)
			delete fc;
		if (fcm != NULL)
			delete fcm;
		if (evc != NULL)
			delete evc;
		if (evcm != NULL)
			delete evcm;
		if (dc != NULL)
			delete dc;

		cpw::ApplicationScene::ReleaseInstance();
		cpw::EntityRegistry::ReleaseInstance();
		cpw::EntityFactory::ReleaseInstance();
		cpw::ApplicationLog::ReleaseInstance();
		cpw::ApplicationConfiguration::ReleaseInstance();

		cpw::controllers::KeyboardControl::ReleaseInstance();
		
		cpw::ApplicationConfiguration::ReleaseInstance();
		
		ApplicationTime::ReleaseInstance();

		wmsFinalize();

		delete main_frame;

	}

	return flag; 
}

void Application::PressKey(int key)   
{ 
	cpw::controllers::KeyboardControl::GetInstance()->KeyDown(key); 

}

void Application::ReleaseKey(int key) 
{ 
	cpw::controllers::KeyboardControl::GetInstance()->KeyUp(key);   
}


bool Application::InitCpwEntities() 
{
	
	builder_camera_controller = new cpw::BuilderCameraController;

	cpw::ApplicationScene *appScene  = cpw::ApplicationScene::GetInstance();
	appScene->SetScene(graphic_factory->CreateScene());

	navigator_manager = graphic_factory->CreateNavigatorManager();
	
	navigator_manager->SetCameraController(builder_camera_controller);

	appScene->SetNavigatorManager(navigator_manager);

	wms_request_manager = new cpw::ogc::WmsLayerManager(appScene->GetScene(), 30000, status_controller);
	wms_request_manager->AddThread(new cpw::ogc::WmsDiskManager(wms_request_manager, 90000));
	wms_request_manager->start();
	appScene->GetScene()->SetRequestManager(wms_request_manager);

	wcs_request_manager = new cpw::ogcwcs::WcsLayerManager(appScene->GetScene(), 30000, status_controller);
	wcs_request_manager->AddThread(new cpw::ogcwcs::WcsDiskManager(wcs_request_manager, 90000));
	wcs_request_manager->start();
	appScene->GetScene()->SetWCSRequestManager(wcs_request_manager);
	

	cpw::ogc::WmsLayer *wms_layer = new cpw::ogc::WmsLayer;
	wms_layer->SetPersistentCallBack(new cpw::PersistentFileCallBack);
	wms_layer->SetName("WmsLayer");

	cpw::EntityFactory::GetInstance()->RegisterBasic(wms_layer);

	cpw::ogcwcs::WcsLayer *wcs_layer = new cpw::ogcwcs::WcsLayer;
	wcs_layer->SetPersistentCallBack(new cpw::PersistentFileCallBack);
	wcs_layer->SetName("WcsLayer");

	cpw::EntityFactory::GetInstance()->RegisterBasic(wcs_layer);



	bool result = project_controller->OpenProject(std::string(""), 
								    GetDefaultPath(), status_controller, layer_tree, graphic_factory);

	



	navigator_manager->SetAllNavigatorsToInitialPosition();

	layer_tree.Attach(&video_controller);
	video_controller.SetLayerTree(&layer_tree);
	application_time->Attach(&video_controller);


	return false;
	
}


void Application::DeleteNavigator(unsigned int nav_id)
{
	navigator_manager->DeleteNavigator(nav_id);
}

void Application::Draw()
{

	

	if (navigator_manager!=NULL)
	{		
		navigator_manager->Draw();
	}

	
}

void Application::DrawModeWireframe(wxWindow *parent)
{
	navigator_manager->SetDrawingModeWireframe(0);
}

void Application::DrawModeSolid(wxWindow *parent)
{
	navigator_manager->SetDrawingModeSolid(0);
}


void Application::NewElementPrimitive(wxWindow *parent)
{
	//main_frame->
	cpw::controllers::ElementPrimitiveController epc;
	epc.CreateElementPrimitive(parent);
}

void Application::NewElement()
{
	//if it did exist, delete first
	if (ec != NULL)
		delete ec;
	
	ec = new cpw::controllers::ElementController(main_frame, layer_tree, graphic_factory, navigator_manager, &handler_controller);

	handler_controller.ShowActiveHandler(false);


	ec->CreateElement();
}

void Application::NewLayer(wxWindow *parent)
{
	cpw::controllers::LayerController lc;
	lc.CreateLayer(parent, layer_tree);
}

void Application::NewTable(wxWindow *parent)
{
	cpw::controllers::DataBaseController controller;
	controller.CreateTable(parent, layer_tree);
}

void Application::NewFire()
{
	//if it did exist, delete first
	if (fc != NULL)
	{
		fc->Cancel();
		delete fc;
	}

	cpw::IGraphicObjectLoader *loader = graphic_factory->CreateObjectLoader();	
	fc = new cpw::controllers::FireController(main_frame, layer_tree, graphic_factory, navigator_manager, loader);
	fc->CreateFire();
}

void Application::NewFirewall(wxWindow *parent)
{
	//if it did exist, delete first
	if (evc != NULL)
	{
		evc->Cancel();
		delete evc;
	}

	cpw::IGraphicObjectLoader *loader = graphic_factory->CreateObjectLoader();	
	evc = new cpw::controllers::ElementVectorialController(parent, layer_tree, graphic_factory, navigator_manager, loader);
	evc->SetVectorialName("Firewall");
	evc->SetVectorialIcon("firewall.png", "firewall_point.png");
	evc->CreateElementVectorial();
}

void Application::NewWmsLayer()//wxWindow *parent)
{
	cpw::controllers::ServerConnectionController scc(main_frame, layer_tree);
	scc.ConnectServer(main_frame);
	TestDependencies("WmsLayer");
}

void Application::NewRemoteEntity(wxWindow *parent)
{
	remote_controller->Connect();
}


void Application::AddEntity(wxWindow *parent)
{

	cpw::controllers::EntityController controller(status_controller);
	cpw::IGraphicObjectLoader *loader = graphic_factory->CreateObjectLoader();	
	cpw::controllers::GraphicObjectController graphic_object_controller(loader, navigator_manager);
	
	controller.AddEntity(parent, layer_tree);
	TestDependencies(layer_tree.GetLastEntity());

	delete loader;
}

void Application::AddRemoteEntity(wxWindow *parent)
{
	
}

void Application::AddDataBase(wxWindow *parent)
{
	cpw::controllers::DataBaseController controller;
	controller.AddDataBase(parent, layer_tree);
}


void Application::NewContainerLayerPrimitive(wxWindow *parent)
{
	cpw::controllers::LayerPrimitiveController lpc;
	lpc.CreateContainerLayerPrimitive(parent);
}

void Application::NewRasterLayerPrimitive(wxWindow *parent)
{
	cpw::controllers::LayerPrimitiveController lpc;
	lpc.CreateRasterLayerPrimitive(parent);
}

void Application::NewVectorialLayerPrimitive(wxWindow *parent)
{
	cpw::controllers::LayerPrimitiveController lpc;
	lpc.CreateVectorialLayerPrimitive(parent);
}




void Application::MouseLeftDown()
{
	int paso=0;

}
void Application::MouseLeftUp()
{
}

void Application::MouseRightDown()
{
}
void Application::MouseRightUp()
{
}

void Application::MouseMiddleDown()
{
}
void Application::MouseMiddleUp()
{
}
void Application::SwitchFullScreen()
{
	if (main_frame->IsFullScreen()) FullScreenOff();
	else FullScreenOn();
}
void Application::FullScreenOn()
{
	if (!main_frame->IsFullScreen())
	{
			main_frame->ShowFullScreen(true, wxFULLSCREEN_NOSTATUSBAR | wxFULLSCREEN_NOBORDER );// | wxFULLSCREEN_NOCAPTION);
		
	}
}
void Application::FullScreenOff()
{
	
	if (main_frame->IsFullScreen())
	{
		main_frame->ShowFullScreen(false);
		
	}
}

void Application::EntitiesToRegisterFromPlugins()
{
	
	for(std::map<int, DynamicLibrary*>::iterator id = plugin_map.begin(); id != plugin_map.end() ; id++)
	{
		std::vector<cpw::Entity*> vec;
		id->second->EntitiesToRegister(vec);
		std::vector<cpw::Entity*>::iterator i = vec.begin();
		for(;i!=vec.end();i++)
		{
			cpw::EntityFactory::GetInstance()->RegisterBasic((*i));
		}	
	}
}


void Application::ExecutePlugin(const int &id)
{
	std::vector<cpw::Entity*> vec;
	cpw::ApplicationScene *apps = cpw::ApplicationScene::GetInstance();
	std::string app_path  = cpw::ApplicationConfiguration::GetInstance()->GetEntityDirectory();
	std::string icon_path = cpw::ApplicationConfiguration::GetInstance()->GetIconDirectory();
	std::string data_path = cpw::ApplicationConfiguration::GetInstance()->GetDataDirectory();
	std::string language  = cpw::ApplicationConfiguration::GetInstance()->GetLanguage();

	plugin_map[id]->ExecutePlugin(vec,apps,app_path,icon_path, data_path, language,(wxWindow*)main_frame,navigator_manager,status_controller);

	std::vector<cpw::Entity*>::iterator i = vec.begin();
	cpw::IdGenerator id_gen;
	
	for(;i!=vec.end();i++)
	{
		std::stringstream old_name;
		old_name << app_path<<"/"<<((*i)->GetId());
		(*i)->SetID(id_gen.GetNewId());
		if((*i)->GetClassName() == "Fire")
			((cpw::utils::Fire *)(*i))->SetGraphicFactory(graphic_factory);
		else
		{
			std::stringstream new_name;
			new_name << app_path<<"/"<<((*i)->GetId());
		
			boost::filesystem::rename(old_name.str(), new_name.str());
		}

		cpw::EntityRegistry::GetInstance()->Add((*i));
		layer_tree.AddToActiveLayer((*i));

		cpw::controllers::PersistentController persistent;
		cpw::PersistentError ferror = persistent.Save(NULL, (*i), ".cel", false);

		(*i)->GraphicInsert();
	}
	video_controller.GetAnimatedEntityListFromLayerTree(&layer_tree);


}



void Application::RegisterAllPlugins()
{
	/*std::ifstream plugins_file(cpw::ApplicationConfiguration::GetInstance()->GetPluginsFile().c_str());	
	if (plugins_file.is_open())
	{
		while (!plugins_file.eof())
		{
			std::string plugin;
			plugins_file >> plugin;
	
			DynamicLibrary *library = new DynamicLibrary();
			library->Load(plugin);
			if (library->GetH() != NULL)
			{
				std::string str1;
				library->RegisterPluginMenu(str1);
				int id = main_frame->AddPluginMenu(str1);
				plugin_map[id] = library;
			}
			else
				delete library;
			
		}
		plugins_file.close();
	}*/
}


void Application::Update()
{
	
	Draw();
}

void Application::AnimateEntity()
{
	int debug;
	debug = 0;
	
}


void Application::NotAvailable()
{

	cpw::ApplicationScene::GetInstance()->GetScene()->TestPetitions();
}


void Application::TestDependencies(const std::string &class_name)
{	
	if(class_name == "WmsLayer")
		wms_request_manager->Initialize();
	if(class_name == "WcsLayer")
		wcs_request_manager->Initialize();
	if(class_name == "AnimableList") {}
		
	
}

void Application::TestDependencies(const cpw::Entity *entity)
{	

	if(entity->isContainer())
	{
		for(int i = 0; i < entity->GetNumChildren(); i++)
			TestDependencies(entity->GetChild(i));
	}
	else
		TestDependencies(entity->GetClassName());

}



void Application::SendFront(const cpw::TypeId &id)
{
	cpw::Entity* ent = cpw::EntityRegistry::GetInstance()->GetEntity(id);
	if (ent!=NULL)
	{
		ent->SetFirst();
		TestDependencies(ent);
	}
}


void Application::SendBack(const cpw::TypeId &id)
{
	cpw::Entity* ent = cpw::EntityRegistry::GetInstance()->GetEntity(id);
	if (ent!=NULL)
	{
		ent->SetLast();
		TestDependencies(ent);

	}
}


void Application::ModifyProperties(const cpw::TypeId &id, wxWindow* parent)
{
	cpw::Element3D el3d;
	cpw::ElementVectorial vectorial;
	cpw::ContainerLayer container;
	cpw::ogc::WmsLayer wmslayer;
	cpw::utils::Fire fire;

	cpw::Entity* ent = cpw::EntityRegistry::GetInstance()->GetEntity(id);
	if (ent!=NULL)
	{
		if(ent->GetClassName() == vectorial.GetClassName())
		{
			if (evcm != NULL)
				delete evcm;
			cpw::IGraphicObjectLoader *loader = graphic_factory->CreateObjectLoader();
			evcm = new cpw::controllers::ElementVectorialController(parent, layer_tree, graphic_factory, navigator_manager, loader);
			evcm->ModifyProperties(ent);
		}

		if(ent->GetClassName() == el3d.GetClassName())
		{
			if (ecm != NULL)
			{
				delete ecm;
				ecm = NULL;
			}

			ecm = new cpw::controllers::ElementController(parent, layer_tree, graphic_factory, navigator_manager, &handler_controller);
			ecm->ModifyProperties(ent);
		}

		if(ent->GetClassName() == container.GetClassName())
		{
			cpw::controllers::LayerController clc;
			clc.ModifyProperties(parent, ent);
		}

		if(ent->GetClassName() == wmslayer.GetClassName())
		{
			cpw::controllers::ServerConnectionController wmscm(parent, layer_tree);
			wmscm.ModifyProperties(ent);

			

		}

		if(ent->GetClassName() == fire.GetClassName())
		{
			if(fcm != NULL)
				delete fcm;
			cpw::IGraphicObjectLoader *loader = graphic_factory->CreateObjectLoader();
			fcm = new cpw::controllers::FireController(parent, layer_tree, graphic_factory, navigator_manager, loader);
			application_time->Off();
			fcm->ModifyProperties(ent);
		}

		TestDependencies(ent->GetClassName());
	}
}






	
void Application::GoTo(const cpw::TypeId &id)
{
	cpw::Entity* ent = cpw::EntityRegistry::GetInstance()->GetEntity(id);
	if (ent!=NULL)
	{
		cpw::Point3d<float> ent_pos;
		if(!ent->isContainer())
		{
			// Get ElementVectorial coords
			cpw::ElementVectorial vectotial;
			if(ent->GetClassName() == vectotial.GetClassName())
			{
				if(((cpw::ElementVectorial*)ent)->GetNumPoints()==0) return;

				ent_pos.x = ((cpw::ElementVectorial*)ent)->GetPoint(0)->x;
				ent_pos.y = ((cpw::ElementVectorial*)ent)->GetPoint(0)->y;
				ent_pos.z = 200.0f; //((cpw::ElementVectorial*)ent)->GetPoint(0)->z;
			}

			// Get Element3D coords
			cpw::Element3D e3d;
			if(ent->GetClassName() == e3d.GetClassName())
			{
				ent_pos.x = ((cpw::Element*)ent)->GetUtm(0);
				ent_pos.y = ((cpw::Element*)ent)->GetUtm(1);
				ent_pos.z = ((cpw::Element*)ent)->GetUtm(2);
			}

			// Get Ignition Point coords of the fire 
			cpw::utils::Fire f;
			if(ent->GetClassName() == f.GetClassName())
			{
				if (((cpw::utils::Fire*)ent)->GetNumIgnitionPoints() > 0)
				{
					ent_pos = *((cpw::utils::Fire*)ent)->GetIgnitionPoint(0);						
				}
				else
				{
					// If the fire have not ignition point get the first flame coords
					ent_pos = ((cpw::utils::Fire*)ent)->GetFlame(0)->fire.GetCoords();
				}
			}

			// Get UTMLATLON from wms-layer
			cpw::ogc::WmsLayer w;
			if(ent->GetClassName() == w.GetClassName())
			{
				return;  

				double lat1, lat2, lon1, lon2;
				((cpw::ogc::WmsLayer*)ent)->GetUTMLatLonBoundingBox(lat1,lat2,lon1,lon2);
				ent_pos.x = (lon2+lon1)/2.0;
				ent_pos.y = (lat2+lat1)/2.0;
				ent_pos.z = 200;
			}

			navigator_manager->GetFocusedOrFirstNavigator()->GetCameraController()->UpdateTimer();
			navigator_manager->GetFocusedOrFirstNavigator()->GetCameraController()->GoTo(ent_pos,3);
		}
	}
}

void Application::Copy(const cpw::TypeId &id)
{

}

void Application::Duplicate(const cpw::TypeId &id)
{

}

void Application::Cut(const cpw::TypeId &id)
{

}

void Application::Paste(const cpw::TypeId &id_orig, const cpw::TypeId &id_dest, const cpw::TypeId &parent_id_orig, const cpw::TypeId &parent_id_dest, const bool &cut, const bool &copy, const bool &duplicate)
{
	cpw::Entity *ent_orig, *ent_dest;
	cpw::Entity *parent_ent_orig, *parent_ent_dest;
	ent_orig = cpw::EntityRegistry::GetInstance()->GetEntity(id_orig);
	ent_dest = cpw::EntityRegistry::GetInstance()->GetEntity(id_dest);
	parent_ent_orig = cpw::EntityRegistry::GetInstance()->GetEntity(parent_id_orig);
	parent_ent_dest = cpw::EntityRegistry::GetInstance()->GetEntity(parent_id_dest);

	if((ent_orig == NULL) || (ent_dest == NULL)) return;
	if(ent_orig == ent_dest) return;
	if(parent_ent_orig == NULL) return;
	if((parent_ent_dest == NULL) && (ent_dest != layer_tree.GetTopLayer())) return;

	if(cut && !copy && !duplicate)
	{
		((cpw::ContainerLayer*)parent_ent_orig)->DeleteChild(ent_orig);

		if(ent_dest->isContainer()) parent_ent_dest = ent_dest;

		((cpw::ContainerLayer*)parent_ent_dest)->Add(ent_orig);

		std::stringstream ent_orig_str;
		ent_orig_str << ent_orig->GetId();
		std::stringstream parent_ent_orig_str;
		parent_ent_orig_str << parent_ent_orig->GetId();
		std::stringstream parent_ent_dest_str;
		parent_ent_dest_str << parent_ent_dest->GetId();

	
	}

	if(copy && !cut && !duplicate)
	{
		if(ent_dest->isContainer()) parent_ent_dest = ent_dest;

		((cpw::ContainerLayer*)parent_ent_dest)->Add(ent_orig);
				
	}
}


void Application::Delete(const cpw::TypeId &ent_id, const cpw::TypeId &parent_id)
{
	cpw::Entity *ent, *parent;
	ent = cpw::EntityRegistry::GetInstance()->GetEntity(ent_id);
	parent = cpw::EntityRegistry::GetInstance()->GetEntity(parent_id);

	handler_controller.ShowActiveHandler(false);
	

	if((ent!=NULL) && (parent != NULL))
	{
		
		if(ent->isContainer())
		{
			int num_children = ent->GetNumChildren();
			for(int i = 0; i < num_children; i++)
		
				Delete(ent->GetChild(0)->GetId(), ent_id);
		}

		std::string class_name = ent->GetClassName();

		if(ent != layer_tree.GetTopLayer())
		{
			((cpw::ContainerLayer*)parent)->DeleteChild(ent);

			if(ent->GetNumParents() <= 0)	
			{
				ent->GraphicDelete();
				cpw::EntityRegistry::GetInstance()->DeleteEntity(ent->GetId());
			}
		}

		layer_tree.MakeParentActive(((cpw::ContainerLayer*)parent)->GetId());		
		handler_controller.ClearActiveEntity();
		TestDependencies(class_name);
	}
}


void Application::Visualize(const cpw::TypeId &id, const bool &value)
{
	cpw::Entity* ent;
	ent = cpw::EntityRegistry::GetInstance()->GetEntity(id);

	if (ent != NULL) 
	{
		ent->SetVisible(value);
		TestDependencies(ent);
	}
}


void Application::AddLine()
{
	
}

unsigned char Application::GetWmsBlendingFactor()
{
	return (unsigned char) main_frame->GetWMSALphaValue();
}

unsigned char Application::GetFireViewMode()
{
	return (unsigned char) main_frame->GetFireViewMode();
}


void Application::Set2DNavigationMode()
{
	
}

void Application::Set3DNavigationMode()
{
	
}

void Application::CalculateDistance(wxWindow *parent)
{
	if (dc != NULL)
	{
		dc->Cancel();
		delete dc;
	}

	cpw::IGraphicObjectLoader *loader = graphic_factory->CreateObjectLoader();	
	dc = new cpw::controllers::DistanceController(parent, layer_tree, graphic_factory, navigator_manager, loader);
	dc->SetDistanceIcon("distance.png", "bullet_orange.png");
	dc->CreateDistance();
}



void Application::SetTranslateHandler()
{
	handler_controller.SetActiveHandler(traslation_x_gizmo_id);
}
void Application::SetRotateHandler()
{
	handler_controller.SetActiveHandler(rotate_x_gizmo_id);	
}
void Application::SetScaleHandler()
{
	handler_controller.SetActiveHandler(scale_x_gizmo_id);
}

std::string Application::GetDefaultPath()
{
	boost::filesystem::path directory(cpw::ApplicationConfiguration::GetInstance()->GetDefaultDirectory());
	boost::filesystem::path previous_path = boost::filesystem::initial_path();
	boost::filesystem::path actual_path   = (directory.has_root_name())? directory : previous_path / directory;
	
	return actual_path.string();
}

void Application::OpenProject()
{
	CloseProject();

	Draw();
	main_frame->Refresh();
	main_frame->Update();

	bool result = project_controller->OpenProject(main_frame, GetDefaultPath(), status_controller, layer_tree, graphic_factory);
	
	std::string title = project_controller->GetProjectFullName();
	if (title!="")
		main_frame->SetTitle(_T(title+" - Capaware"));

	navigator_manager->SetAllNavigatorsToInitialPosition();

	navigator_manager->GetFocusedOrFirstNavigator()->ShowHUD(result);

	main_frame->EnableGUI(result);

	TestDependencies("WmsLayer");
	
}

void Application::CloseProject()
{
	main_frame->SetTitle("Capaware");
	project_controller->CloseProject(layer_tree);

	navigator_manager->GetFocusedOrFirstNavigator()->ShowHUD(false);

	TestDependencies("WmsLayer");

	layer_tree.Detach(&video_controller);
	main_frame->EnableGUI(false);
	video_controller.Update();

	navigator_manager->Draw();
}

void Application::UpdateTimeSettings()
{
	video_controller.Update();
}

void Application::UpdateAnimableUI()
{
	main_frame->UpdateAnimableUI();
}


void Application::BuildScene()
{	
	layer_tree.Clear();
	cpw::EntityRegistry::GetInstance()->Clear(); 

	if(bb_controller != NULL)
	{
		delete bb_controller;
		bb_controller = NULL;
	}
	if(build_thread != NULL)
	{
		build_thread->join();
		delete build_thread;
		build_thread = NULL;
	}
	if(gen_wait != NULL)
	{
		delete gen_wait;
		gen_wait = NULL;
	}

	cpw::controllers::ServerConnectionController scc(main_frame, layer_tree, true);
	scc.ConnectServer(main_frame);
	TestDependencies("WmsLayer");

	double lat1, lat2, lon1, lon2;
	lat1 = lat2 = lon1 = lon2 = 0.0;


	wmsl = ((cpw::ogc::WmsLayer*)layer_tree.GetTopLayer());

	if(wmsl != NULL)
	{
		wmsl->GetUTMLatLonBoundingBox(lat1, lat2,lon1, lon2);
		
		
		cpw::ApplicationScene::GetInstance()->GetScene()->BuildScene(lat1,lon1,lat2,lon2,1024,1024);
		double dist = std::max((lon2-lon1), (lat2-lat1))*2;
		navigator_manager->SetAllNavigatorsToLookPos(cpw::Point3d<double>( (lon1+lon2)/2,(lat1+lat2)/2, dist ));

		bb_controller = new WMSBoundingBoxController(main_frame, navigator_manager, cpw::ApplicationScene::GetInstance(), wmsl, this);
		bb_controller->Init(lat1,lon1,lat2,lon2);
	
		//el hilo que queda esperando para generar el terreno
		gen_wait = new UIWait(this);	
		build_thread = new boost::thread(boost::ref(*gen_wait));
	}
	
	Draw();
	main_frame->Refresh();
	main_frame->Update();


}

void Application::LoadBuildScene()
{
	layer_tree.Clear();
	cpw::EntityRegistry::GetInstance()->Clear(); 

	if(bb_controller != NULL)
	{
		delete bb_controller;
		bb_controller = NULL;
	}
	if(build_thread != NULL)
	{
		build_thread->join();
		delete build_thread;
		build_thread = NULL;
	}

	if(gen_wait != NULL)
	{
		delete gen_wait;
		gen_wait = NULL;
	}



	wxFileDialog dialog (main_frame,_T("Open Project"),_T("."),wxEmptyString,
							   _T(" Capabuilder Project(*.cbp)|*.cbp|All files(*.*)|*.*") );
	dialog.Center();

	if(dialog.ShowModal() == wxID_OK)
	{		
		std::string filename = dialog.GetPath();
		std::ifstream pfile(filename.c_str());
		
		std::string dir, url;
		if (pfile.is_open())
		{
			pfile >> dir;
			pfile >> url;
			pfile >> wcsdir;
			pfile >> wcsurl;

			pfile.close();
		}

		wmsl = new cpw::ogc::WmsLayer(cpw::TypeId(dir));
		if(wmsl == NULL)
		{
			wxMessageDialog message1(NULL,wxString("Cannot load the project. Check directory name."), wxString("Capabuilder"),wxICON_WARNING |wxOK);				
			message1.ShowModal();

			return;
		}

		std::string server, name, wmsversion, wmssrs, format;
		double xmin, ymin, xmax, ymax;
		if(ProcessWMSURL(url, server, name, wmsversion, wmssrs, format, xmin, ymin, xmax, ymax))
		{
			wmsl->SetWmsName(name);
			wmsl->SetWmsTitle(name);
			wmsl->SetUTMLatLonBoundingBox(ymin, ymax, xmin, xmax); //comprobar orden
			wmsl->SetWmsVersion(wmsversion);
			wmsl->SetWmsUrl(server);
			wmsl->SetWmsFormat(format);
			wmsl->SetSRS(wmssrs);

			cpw::EntityRegistry::GetInstance()->Add((cpw::Entity *)wmsl);

			layer_tree.AddToActiveLayer((cpw::Entity *)wmsl);		
			//new_wms_layer->GraphicInsert();	


			//el hilo que queda esperando para generar el terreno
			gen_wait = new UIWait(this);	
			build_thread = new boost::thread(boost::ref(*gen_wait));


			bb_controller = new WMSBoundingBoxController(main_frame, navigator_manager, cpw::ApplicationScene::GetInstance(), wmsl, this);
			bb_controller->OK(url);
		}
		else
		{
			wxMessageDialog message1(NULL,wxString("Cannot load the project."), wxString("Capabuilder"),wxICON_WARNING |wxOK);				
			message1.ShowModal();
		}
	}


}

cpw::Entity* Application::GetWCS()
{
	if(!load_wcs)
	{
		cpw::controllers::WCSServerConnectionController scc2(main_frame, layer_tree, true);
		scc2.ConnectServer(main_frame);
		TestDependencies("WcsLayer");
	}
	else
	{
		wcsl = new cpw::ogcwcs::WcsLayer(cpw::TypeId(wcsdir));
		if(wcsl == NULL)
		{
			wxMessageDialog message1(NULL,wxString("Cannot load the project. Check directory name."), wxString("Capabuilder"),wxICON_WARNING |wxOK);				
			message1.ShowModal();

			return NULL;
		}

		std::string server, name, wcsversion, wcssrs, format;
		double xmin, ymin, xmax, ymax;
		int resx, resy;
		if(ProcessWCSURL(wcsurl, server, name, wcsversion, wcssrs, format, xmin, ymin, xmax, ymax, resx, resy))
		{
			wcsl->SetWcsName(name);
			wcsl->SetWcsTitle(name);
			wcsl->SetUTMLatLonBoundingBox(ymin, ymax, xmin, xmax); 
			wcsl->SetWcsVersion(wcsversion);
			wcsl->SetWcsUrl(server);
			wcsl->SetWcsFormat(format);
			wcsl->SetSRS(wcssrs);

			cpw::EntityRegistry::GetInstance()->Add((cpw::Entity *)wcsl);

			layer_tree.AddToActiveLayer((cpw::Entity *)wcsl);		
		}		
	}

	return layer_tree.GetLastEntity();
}

bool Application::ProcessWMSURL(std::string &url, std::string &server, std::string &name, std::string &wmsversion, std::string &wmssrs, std::string &format, double &xmin, double &ymin, double &xmax, double &ymax)
{
	int pos = url.find('?');
	if(pos == -1)
		return false;
	server = url.substr(0, pos);

	int pos2 = url.find("&LAYERS=");
	if(pos2 == -1)
		return false;
	int pos3 = url.find('&',pos2+8);
	name = url.substr(pos2+8,pos3-(pos2+8));

	int pos4 = url.find("&VERSION=");
	if(pos4 == -1)
		return false;
	int pos5 = url.find('&',pos4+9);
	wmsversion = url.substr(pos4+9,pos5-(pos4+9));

	int pos6 = url.find("&FORMAT=");
	if(pos6 == -1)
		return false;
	int pos7 = url.find('&',pos6+8);
	format = url.substr(pos6+8,pos7-(pos6+8));

	int pos8 = url.find("&SRS=");
	if(pos8 == -1)
		return false;
	int pos9 = url.find('&',pos8+5);
	wmssrs = url.substr(pos8+5,pos9-(pos8+5));

	//Calculate the lat lon bb
	int pos_bb = url.find("&BBOX=");
	int pos_coma1 = url.find_first_of(',',pos_bb);
	int pos_coma2 = url.find_first_of(',',pos_coma1+1);
	int pos_coma3 = url.find_first_of(',',pos_coma2+1);
	int pos_endbb = url.find_first_of('&',pos_coma3);

	std::stringstream sub_xmin, sub_xmax, sub_ymin, sub_ymax;

	sub_xmin << url.substr(pos_bb+6, pos_coma1-pos_bb-6);
	sub_xmin >> xmin;
	
	sub_ymin << url.substr(pos_coma1+1, pos_coma2-pos_coma1-1);
	sub_ymin >> ymin;
	
	sub_xmax << url.substr(pos_coma2+1, pos_coma3-pos_coma2-1);
	sub_xmax >> xmax;

	sub_ymax << url.substr(pos_coma3+1, pos_endbb-pos_coma3-1);
	sub_ymax >> ymax;

	load_wcs = true;
	return true;
}

bool Application::ProcessWCSURL(std::string &url, std::string &server, std::string &name, std::string &wcsversion, std::string &wcssrs, std::string &format, double &xmin, double &ymin, double &xmax, double &ymax, int &resx, int &resy)
{
	int pos = url.find('?');
	if(pos == -1)
		return false;
	server = url.substr(0, pos);

	int pos2 = url.find("&COVERAGE=");
	if(pos2 == -1)
		return false;
	int pos3 = url.find('&',pos2+10);
	name = url.substr(pos2+10,pos3-(pos2+10));

	int pos4 = url.find("&VERSION=");
	if(pos4 == -1)
		return false;
	int pos5 = url.find('&',pos4+9);
	wcsversion = url.substr(pos4+9,pos5-(pos4+9));

	int pos6 = url.find("&FORMAT=");
	if(pos6 == -1)
		return false;
	int pos7 = url.find('&',pos6+8);
	format = url.substr(pos6+8,pos7-(pos6+8));

	int pos8 = url.find("&CRS=");
	if(pos8 == -1)
		return false;
	int pos9 = url.find('&',pos8+5);
	wcssrs = url.substr(pos8+5,pos9-(pos8+5));

	//Calculate the lat lon bb
	int pos_bb = url.find("&BBOX=");
	int pos_coma1 = url.find_first_of(',',pos_bb);
	int pos_coma2 = url.find_first_of(',',pos_coma1+1);
	int pos_coma3 = url.find_first_of(',',pos_coma2+1);
	int pos_endbb = url.find_first_of('&',pos_coma3);

	std::stringstream sub_xmin, sub_xmax, sub_ymin, sub_ymax;

	sub_xmin << url.substr(pos_bb+6, pos_coma1-pos_bb-6);
	sub_xmin >> xmin;
	
	sub_ymin << url.substr(pos_coma1+1, pos_coma2-pos_coma1-1);
	sub_ymin >> ymin;
	
	sub_xmax << url.substr(pos_coma2+1, pos_coma3-pos_coma2-1);
	sub_xmax >> xmax;

	sub_ymax << url.substr(pos_coma3+1, pos_endbb-pos_coma3-1);
	sub_ymax >> ymax;


	int posrx = url.find("&RESX=");
	if(posrx == -1)
		return false;
	int posrx2 = url.find('&',posrx+6);
	std::stringstream ss_rx, ss_ry;
	ss_rx << url.substr(posrx+6,posrx2-(posrx+6));
	ss_rx >> resx;

	int posry = url.find("&RESY=");
	if(posry == -1)
		return false;
	int posry2 = url.find('&',posry+6);
	ss_ry << url.substr(posry+6,posry2-(posry+6));
	ss_ry >> resy;

	load_wcs = false;
	return true;
}

int Application::Osgdem(std::vector<std::string> &options)
{

	if(options.empty()) 
	{
		cpw::ApplicationLog::GetInstance()->GetLogger()->NewLogMessage("Terrain generation aborted. Without arguments.");
		return -1;
	}

	char** argv;

	argv = (char**)malloc(sizeof(char*)*options.size()); 

	std::string dp = cpw::ApplicationConfiguration::GetInstance()->GetDataDirectory();
	std::string s = options.back();
	s = dp+s;
	options[options.size()-1] = s;

	int i=0;
	std::vector<std::string>::iterator it= options.begin();
	for( ; it < options.end() ; it++)
	{		
		argv[i] = const_cast<char*>(it->c_str());
		i++;
	}

	int argc=options.size();

    osg::Timer_t startTick = osg::Timer::instance()->tick();

    osg::ArgumentParser arguments(&argc,argv);

    
	vpb::Commandline commandline;

  
    vpb::System::instance()->readArguments(arguments);
 

    osg::ref_ptr<osgTerrain::TerrainTile> terrain = 0;


    //std::cout<<"PID="<<getpid()<<std::endl;
  
    if (!terrain) terrain = new osgTerrain::TerrainTile;


    bool report = false;
    //PJ//while (arguments.read("--report")) { report = true; }

    int result = commandline.read(std::cout, arguments, terrain.get());
    if (result) 
	{
		free(argv);
		cpw::ApplicationLog::GetInstance()->GetLogger()->NewLogMessage("Terrain generation aborted. Arguments error.");
		return result;
	}


    // any option left unread are converted into errors to write out later.
    arguments.reportRemainingOptionsAsUnrecognized();

    // report any errors if they have occured when parsing the program aguments.
    if (arguments.errors())
    {
        //PJ//arguments.writeErrorMessages(std::cout);
		cpw::ApplicationLog::GetInstance()->GetLogger()->NewLogMessage("Terrain generation aborted. Arguments error.");
		
		osg::ArgumentParser::ErrorMessageMap error_map = arguments.getErrorMessageMap();
		for(osg::ArgumentParser::ErrorMessageMap::iterator it = error_map.begin() ; it != error_map.end() ; it++)
			cpw::ApplicationLog::GetInstance()->GetLogger()->NewLogMessage((*it).first);


		free(argv);
        return 1;
    }
    

    double duration = 0.0;

    // generate the database
    if (terrain.valid())
    {
        try 
        {

            vpb::DatabaseBuilder* db = dynamic_cast<vpb::DatabaseBuilder*>(terrain->getTerrainTechnique());
            vpb::BuildOptions* bo = db ? db->getBuildOptions() : 0;

            if (bo)
            {
                osg::setNotifyLevel(osg::NotifySeverity(bo->getNotifyLevel()));

            }
            osg::ref_ptr<vpb::DataSet> dataset = new vpb::DataSet;

            if (bo && !(bo->getLogFileName().empty()))
            {
                dataset->setBuildLog(new vpb::BuildLog(bo->getLogFileName()));
            }

			

            dataset->addTerrain(terrain.get());

            // make sure the OS writes changes to disk
            vpb::sync();

			int result = dataset->run();

			
            if (dataset->getBuildLog() && report)
            {
                dataset->getBuildLog()->report(std::cout);				
            }
            
            duration = osg::Timer::instance()->delta_s(startTick, osg::Timer::instance()->tick());
            
            //dataset->log(osg::NOTICE," %f",duration);
 	
			std::stringstream ss;
			ss.precision(15);
			ss<<"Elapsed time = "<<duration<<std::endl;
			cpw::ApplicationLog::GetInstance()->GetLogger()->NewLogMessage(ss.str());
           
        }
        catch(std::string str)
        {
            cpw::ApplicationLog::GetInstance()->GetLogger()->NewLogMessage("Caught exception : "+str);
            
		}
        catch(...)
        {
            cpw::ApplicationLog::GetInstance()->GetLogger()->NewLogMessage("Caught exception.");
        }

    }

    if (duration==0) duration = osg::Timer::instance()->delta_s(startTick, osg::Timer::instance()->tick());

   
    
    // make sure the OS writes changes to disk
    vpb::sync();



	std::string	filename,dir;
	int pos = options.back().find('\\');
	filename = options.back().substr(pos,options.back().size());
	dir = options.back().substr(0,pos);

	cpw::Point2d<double> pmin = cpw::ApplicationScene::GetInstance()->GetScene()->GetCoordsMin();
	std::stringstream sx, sy;
	sx.precision(15);
	sy.precision(15);
	sx<<pmin.x;
	sy<<pmin.y;
	std::string srs;
	srs = "";
	if(wmsl != NULL)
		srs = wmsl->GetSRS();
	std::ofstream fout(std::string(dir+"//terrain.cws").c_str());
	if(fout.is_open()) 
	{
		boost::filesystem::path directory(options.back());
		boost::filesystem::path previous_path = boost::filesystem::initial_path();
		boost::filesystem::path actual_path   = previous_path / directory;

		boost::filesystem::path out_path;

		boost::filesystem::path::iterator pit = actual_path.begin();
		
		while(pit != actual_path.end())
		{
			if(*pit == "..")
			{
				//test if the leaf is a slash or the actual directory
				if(out_path.leaf() == ".")
					out_path.remove_leaf();

				out_path.remove_leaf();
			}
			else if(*pit != ".")
				out_path /= *pit; 

			pit++;
		}

		std::stringstream finalpath;
		std::string auxff;
		auxff = out_path.string();

		std::string::iterator  it = auxff.begin();
		while(it != auxff.end())
		{
			if(*it == '/')
			{
				finalpath << "\\\\";
			}
			else
			{
				finalpath << *it;
			}
			it++;

		}



		fout << "[OSG]\n";
		fout << finalpath.str() << "\n";
		fout << sx.str() << "\n";
		fout << sy.str() << "\n";
		fout << "0" << "\n";
		fout << "\n";
		fout << "[TOPLAYER]\n";
		fout << "toplayer.cla\n\n";
		fout << "[SRS]\n";
		fout << srs << "\n";

		fout.close();
	}


	cpw::ApplicationLog::GetInstance()->GetLogger()->NewLogMessage("Generation Finished.");

	//std::string data_path = cpw::ApplicationConfiguration::GetInstance()->GetDataDirectory();
	cpw::ApplicationLog::GetInstance()->GetLogger()->NewLogMessage("Generated terrain is in "+options.back());

	free(argv);

	

    return 0;
}





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

#ifdef WIN32 
#include <windows.h>
#endif
#include <iosg/OsgFactory.h>

#include <cpw/graphic/IGraphicFactory.h>
#include <cpw/persistent/file/PersistentFileCallBack.h>
#include <cpw/graphic/ISceneLoader.h>
#include <cpw/graphic/ApplicationScene.h>
#include <cpw/graphic/IHandler.h>
#include <cpw/graphic/TraslationGizmo.h>
#include <cpw/graphic/RotationGizmo.h>
#include <cpw/graphic/ScaleGizmo.h>
#include <cpw/graphic/TwoAxisTraslationGizmo.h>

#include <cpw/entity/EntityRegistry.h>
#include <cpw/entity/EntityFactory.h>
#include <cpw/entity/Entity.h>
#include <cpw/entity/Element.h>
#include <cpw/entity/ContainerLayer.h>
#include <cpw/entity/RasterLayer.h>
#include <cpw/entity/VectorialLayer.h>
#include <cpw/common/ApplicationLog.h>
#include <cpw/common/IdGenerator.h>


#include <ogc/wms/WmsDiskManager.h>
#include <wms/wms.h>

#include <cpw/ApplicationConfiguration.h>

#include <controllers/GraphicObjectController.h>
#include <controllers/ElementPrimitiveController.h>
#include <controllers/LayerPrimitiveController.h>
#include <controllers/EntityController.h>
//#include <controllers/ViewSelectionController.h>
#include <controllers/RemoteController.h>
#include <controllers/PersistentController.h>
#include <controllers/ConfigurationController.h>
#include <controllers/LayerController.h>
#include <controllers/DataBaseController.h>
#include <controllers/CwsLoader.h>
#include <controllers/RelativeDirectory.h>

#include <cpwutils/Fire.h>
#include <cpwutils/GraphicFireCallBack.h>



Application::Application(void): main_frame(),
				ec(),
				ecm(),
				fc(),
				evc(),
				evcm(),
				fcm(),
				navigator_manager(),
				graphic_factory(), 
				layer_tree(),
				plugin_map(),
				three_dimension_camera_controller(NULL),
				two_dimension_camera_controller(NULL),
				dc(NULL),
				plugin_running(false),
				srs()
{


  //for being able to load all type of images
  wxInitAllImageHandlers();

#ifdef WIN32
  //This is to activate memory-leaks detection
  _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

  wmsInitialize();
  application_time = ApplicationTime::GetInstance();
  application_time->Off();
  cpw::IdGenerator::SetNamespace(GetMACaddress());

#ifdef WIN32
  SetThreadAffinityMask(GetCurrentThread(), 0);
#endif

}

Application::~Application(void)
{
	
  std::map<int, DynamicLibrary*>::iterator mi = plugin_map.begin();
  for( ; mi!=plugin_map.end() ; mi++)
    if ((*mi).second)           delete (*mi).second;
  if (status_controller)        delete status_controller;
  if (sun_controller)           delete sun_controller;
  if (project_controller)       delete project_controller;
  if (graphic_factory)          delete graphic_factory;
  if (remote_controller)        delete remote_controller;
  if (remote_entity_controller) delete remote_entity_controller;
	
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
  //project_controller = new cpw::controllers::ProjectController();
  remote_entity_controller = new cpw::controllers::RemoteEntityController();
  project_controller = new cpw::controllers::ProjectController();

  graphic_factory = new cpw::iosg::OsgFactory(this);
	
  //Load the configuration in the Singleton class ApplicationConfiguration
  cpw::ApplicationConfiguration *appConfig = cpw::ApplicationConfiguration::GetInstance();
  cpw::controllers::ConfigurationController config;
  config.Load(appConfig);

  main_frame = new cpw::gui::UIApplicationMainFrame(this,
						    wxString(_T("Rvzware")),
						    0,0 , 1024, 768,
						    (wxBORDER_SUNKEN | wxMINIMIZE_BOX | wxMAXIMIZE_BOX
						     | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCAPTION |
						     wxCLOSE_BOX | wxCLIP_CHILDREN));
  main_frame->Show(false);
  main_frame->Maximize(true);
  main_frame->ShowSplash();

  RegisterAllPlugins();
  EntitiesToRegisterFromPlugins();
	

  bool file_error = InitCpwEntities();

  //Remote initialization stuff
  int listen_port = cpw::ApplicationConfiguration::GetInstance()->GetListenPort();

  remote_controller = new cpw::controllers::RemoteController(main_frame, &layer_tree, listen_port);

  main_frame->InitGUIContents(layer_tree, remote_controller->GetRemoteProtocol(), navigator_manager);


  InitHandlers();

  main_frame->Show(true);	
  main_frame->EnableGUI(file_error);
  main_frame->HideSplash();

  //TestDependencies("WmsLayer");
  navigator_manager->GetFocusedOrFirstNavigator()->ShowHUD(file_error);

  return true;
}

void Application::InitHandlers()
{
  std::string path = GetDefaultPath();
  cpw::IScene *scene  = cpw::ApplicationScene::GetInstance()->GetScene();
	
  traslation_x_gizmo_id = std::string("tx_giz");
  traslation_y_gizmo_id = std::string("ty_giz");
  traslation_z_gizmo_id = std::string("tz_giz");
  rotate_x_gizmo_id     = std::string("rx_giz");
  rotate_y_gizmo_id     = std::string("ry_giz");
  rotate_z_gizmo_id     = std::string("rz_giz");
  scale_x_gizmo_id      = std::string("sx_giz");
  two_axis_gizmo_id     = std::string("ta_giz");
	
  //create the gizmos
  cpw::IHandler *traslation_x_handler = new cpw::TraslationGizmo();
  cpw::IHandler *traslation_y_handler = new cpw::TraslationGizmo();
  cpw::IHandler *traslation_z_handler = new cpw::TraslationGizmo();
  cpw::IHandler *rotation_x_handler = new cpw::RotationGizmo();
  cpw::IHandler *rotation_y_handler = new cpw::RotationGizmo();
  cpw::IHandler *rotation_z_handler = new cpw::RotationGizmo();
  cpw::IHandler *scale_x_handler = new cpw::ScaleGizmo();
  cpw::IHandler *two_axis_handler = new cpw::TwoAxisTraslationGizmo();

  //set the navigator_manager to all of them
  traslation_x_handler->SetNavigatorManager(navigator_manager);
  traslation_y_handler->SetNavigatorManager(navigator_manager);
  traslation_z_handler->SetNavigatorManager(navigator_manager);
  rotation_x_handler->SetNavigatorManager(navigator_manager);
  rotation_y_handler->SetNavigatorManager(navigator_manager);
  rotation_z_handler->SetNavigatorManager(navigator_manager);
  scale_x_handler->SetNavigatorManager(navigator_manager);
  two_axis_handler->SetNavigatorManager(navigator_manager);

	
  //set the axis
  ((cpw::Gizmo *)traslation_x_handler)->SetTransformationAxis( cpw::Gizmo::AXIS_X );
  ((cpw::Gizmo *)traslation_y_handler)->SetTransformationAxis( cpw::Gizmo::AXIS_Y );
  ((cpw::Gizmo *)traslation_z_handler)->SetTransformationAxis( cpw::Gizmo::AXIS_Z );
  ((cpw::Gizmo *)rotation_x_handler)->SetTransformationAxis( cpw::Gizmo::AXIS_X );
  ((cpw::Gizmo *)rotation_y_handler)->SetTransformationAxis( cpw::Gizmo::AXIS_Y );
  ((cpw::Gizmo *)rotation_z_handler)->SetTransformationAxis( cpw::Gizmo::AXIS_Z );
  ((cpw::Gizmo *)scale_x_handler)->SetTransformationAxis( cpw::Gizmo::AXIS_X );
	
  //link the gizmos
  traslation_x_handler->AddBrother(traslation_y_handler);
  traslation_x_handler->AddBrother(traslation_z_handler);
  traslation_x_handler->AddBrother(two_axis_handler);
  traslation_y_handler->AddBrother(traslation_x_handler);
  traslation_y_handler->AddBrother(traslation_z_handler);
  traslation_y_handler->AddBrother(two_axis_handler);
  traslation_z_handler->AddBrother(traslation_x_handler);
  traslation_z_handler->AddBrother(traslation_y_handler);
  traslation_z_handler->AddBrother(two_axis_handler);
  two_axis_handler->AddBrother(traslation_x_handler);
  two_axis_handler->AddBrother(traslation_y_handler);
  two_axis_handler->AddBrother(traslation_z_handler);

  rotation_x_handler->AddBrother(rotation_y_handler);
  rotation_x_handler->AddBrother(rotation_z_handler);
  rotation_y_handler->AddBrother(rotation_x_handler);
  rotation_y_handler->AddBrother(rotation_z_handler);
  rotation_z_handler->AddBrother(rotation_x_handler);
  rotation_z_handler->AddBrother(rotation_y_handler);

  //set the id´s
  traslation_x_handler->SetId( traslation_x_gizmo_id );
  traslation_y_handler->SetId( traslation_y_gizmo_id );
  traslation_z_handler->SetId( traslation_z_gizmo_id );
  rotation_x_handler->SetId( rotate_x_gizmo_id );
  rotation_y_handler->SetId( rotate_y_gizmo_id );
  rotation_z_handler->SetId( rotate_z_gizmo_id );
  scale_x_handler->SetId( scale_x_gizmo_id );
  two_axis_handler->SetId( two_axis_gizmo_id );
	
  //add them to the scene
  scene->AddHandler(traslation_x_gizmo_id, path + std::string("gizmos/gizmo_traslate_x.osg"));
  scene->AddHandler(traslation_y_gizmo_id, path + std::string("gizmos/gizmo_traslate_y.osg"));
  scene->AddHandler(traslation_z_gizmo_id, path + std::string("gizmos/gizmo_traslate_z.osg"));
  scene->AddHandler(two_axis_gizmo_id, path + std::string("gizmos/gizmo_ball.osg"));
  scene->AddHandler(rotate_x_gizmo_id, path + std::string("gizmos/gizmo_rotate_x.osg"));
  scene->AddHandler(rotate_y_gizmo_id, path + std::string("gizmos/gizmo_rotate_y.osg"));
  scene->AddHandler(rotate_z_gizmo_id, path + std::string("gizmos/gizmo_rotate_z.osg"));
  scene->AddHandler(scale_x_gizmo_id, path + std::string("gizmos/gizmo_scale_all.osg"));
	
  //add them to the handler controller
  handler_controller.InsertHandler(traslation_x_handler);
  handler_controller.InsertHandler(traslation_y_handler);
  handler_controller.InsertHandler(traslation_z_handler);
	
  handler_controller.InsertHandler(rotation_x_handler);
  handler_controller.InsertHandler(rotation_y_handler);
  handler_controller.InsertHandler(rotation_z_handler);

  handler_controller.InsertHandler(scale_x_handler);

  handler_controller.InsertHandler(two_axis_handler);

  //default handler
  handler_controller.SetDefaultHandler(traslation_x_handler);
		
  //link the handler controller to the navigators so it becomes an event receiver
  cpw::INavigatorManager *nm = cpw::ApplicationScene::GetInstance()->GetNavigatorManager();
  nm->GetFocusedOrFirstNavigator()->SetEventReceiver(1, &handler_controller);

  handler_controller.SetNavigatorManager( nm );
  handler_controller.SetScene( scene = cpw::ApplicationScene::GetInstance()->GetScene() );
  handler_controller.SetActiveHandler(traslation_x_gizmo_id);
}


void Application::Save()
{
	
}

void Application::SaveAll()
{
  if(IsControllerOpen()) return;

  cpw::controllers::EntityController controller(status_controller);
  cpw::PersistentError ferror;
  ferror = controller.Save(layer_tree.GetTopLayer(), true, true);
  if (ferror != cpw::PERSISTENTOK)
    {
      wxMessageDialog message(NULL,wxT("Some entities from the layer tree could not be saved."),
			      wxT("Warning"),wxICON_EXCLAMATION |wxOK);
      message.ShowModal();
    }
}

bool Application::Close()
{
  if(IsControllerOpen()) return false;
  
  bool flag = true;
  bool cancel = false;
  cpw::PersistentError ferror;
  cpw::ApplicationConfiguration* appconfig = cpw::ApplicationConfiguration::GetInstance();
	
  handler_controller.ActivateAllHandlers(false);
  
  appconfig->SetUIConfiguration(main_frame->GetUIConfiguration());
  
  if(appconfig->isModified()) 
    {
      cpw::controllers::ConfigurationController config;
      config.Save(appconfig);
    }
  
  if(layer_tree.isModified()) 
    {
      wxMessageDialog message(NULL,wxT("Save changes from the layer tree before quit?"),
			      wxT("Warning"),wxICON_EXCLAMATION |wxYES_NO |wxCANCEL);
      int modal = message.ShowModal();
      if(modal == wxID_YES)
	{	
	  cpw::controllers::EntityController controller(status_controller);
	  ferror = controller.Save(layer_tree.GetTopLayer(), true, true);
	  if (ferror != cpw::PERSISTENTOK)
	    {
	      wxMessageDialog message(NULL,wxT("Some entities from the layer tree could not be saved."),
				      wxT("Warning"),wxICON_EXCLAMATION |wxOK);
	      message.ShowModal();
	    }
	  
	  flag = true;
	}
      if(modal == wxID_NO)
	{
	  flag = true;
	}
      if(modal == wxID_CANCEL)
	{
	  cancel = true;
	  flag = false;
	}
    }
	
  if (!cancel)
    {
      if (remote_controller)
	delete remote_controller;
      
      DeleteControllers();
      
      if (navigator_manager)
	delete navigator_manager;
      navigator_manager = NULL;
      if (graphic_factory)
	delete graphic_factory;
      graphic_factory = NULL;
      if (wms_request_manager)
	delete wms_request_manager;
      wms_request_manager = NULL;
      if (three_dimension_camera_controller)
	delete three_dimension_camera_controller;
      three_dimension_camera_controller = NULL;
      if (two_dimension_camera_controller)
	delete two_dimension_camera_controller;
      two_dimension_camera_controller = NULL;
      
      video_controller.SetAllowUpdate(false);
      
      if (main_frame)
	delete main_frame;
      application_time->Off();
      
      cpw::EntityRegistry::ReleaseInstance();
      cpw::ApplicationScene::ReleaseInstance();
      cpw::EntityFactory::ReleaseInstance();
      cpw::ApplicationLog::ReleaseInstance();
      cpw::ApplicationConfiguration::ReleaseInstance();
      
      cpw::controllers::KeyboardControl::ReleaseInstance();
      
      cpw::ApplicationConfiguration::ReleaseInstance();
      
      ApplicationTime::ReleaseInstance();
      
      wmsFinalize();
      
    }
  
  return flag; 
}

void Application::DeleteControllers()
{
  if (ec != NULL)
    {
      delete ec;
      ec = NULL;
    }
  if (ecm != NULL)
    {
      delete ecm;
      ecm = NULL;
    }
  if (fc != NULL)
    {
      delete fc;
      fc = NULL;
    }
  if (fcm != NULL)
    {
      delete fcm;
      fcm = NULL;
    }
  if (evc != NULL)
    {
      delete evc;
      evc = NULL;
    }
  if (evcm != NULL)
    {
      delete evcm;
      evcm = NULL;
    }
  if (dc != NULL)
    {
      delete dc;
      dc = NULL;
    }
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
  three_dimension_camera_controller = new cpw::AerialCameraController;
  two_dimension_camera_controller = new cpw::TwoDimensionCameraController;

  cpw::ApplicationScene *appScene  = cpw::ApplicationScene::GetInstance();
  appScene->SetScene(graphic_factory->CreateScene());

  std::string default_icon = cpw::ApplicationConfiguration::GetInstance()->GetIconDirectory()+"default.png";
  cpw::ApplicationScene::GetInstance()->GetScene()->SetDefaultIcon( default_icon );

  navigator_manager = graphic_factory->CreateNavigatorManager();
  navigator_manager->SetCameraController(three_dimension_camera_controller);

  appScene->SetNavigatorManager(navigator_manager);

  wms_request_manager = new cpw::ogc::WmsLayerManager(appScene->GetScene(), 30000, status_controller);
  wms_request_manager->AddThread(new cpw::ogc::WmsDiskManager(wms_request_manager, 90000));
  wms_request_manager->start();
  appScene->GetScene()->SetRequestManager(wms_request_manager);

  cpw::utils::Fire *new_fire = new cpw::utils::Fire();
  new_fire->AddCallBack((cpw::ParticleSystemCallBack *) new cpw::utils::GraphicFireCallBack);
  new_fire->SetPersistentCallBack(new cpw::PersistentFileCallBack);
  new_fire->SetAnimateCallBack((cpw::ParticleSystemCallBack *) new cpw::utils::GraphicFireCallBack);
  new_fire->SetName("Fire");
	
  new_fire->SetGraphicFactory(graphic_factory);
  cpw::EntityFactory::GetInstance()->RegisterBasic(new_fire);

  cpw::ogc::WmsLayer *wms_layer = new cpw::ogc::WmsLayer;
  wms_layer->SetPersistentCallBack(new cpw::PersistentFileCallBack);
  wms_layer->SetName("WmsLayer");

  cpw::EntityFactory::GetInstance()->RegisterBasic(wms_layer);

  bool result = project_controller->OpenProject(cpw::ApplicationConfiguration::GetInstance()->GetSceneFile(), 
						GetDefaultPath(),
						status_controller,
						layer_tree,
						graphic_factory);


  std::string title = project_controller->GetProjectFullName();
  if (title!="")
    {
      main_frame->SetTitle(wxString((title+" - Rvzware").c_str(),
				    wxConvUTF8));
    }

  navigator_manager->SetAllNavigatorsToInitialPosition();

  layer_tree.Attach(&video_controller);
  video_controller.SetLayerTree(&layer_tree);
  application_time->Attach(&video_controller);

  wms_request_manager->SetSRS(project_controller->GetSRS());

  return result;
}


void Application::DeleteNavigator(unsigned int nav_id)
{
  navigator_manager->DeleteNavigator(nav_id);
}

void Application::Draw()
{

  cpw::IScene *scene = cpw::ApplicationScene::GetInstance()->GetScene();

  if (scene == NULL)
    return;

  scene->UpdatePerformance();

  std::vector<cpw::Entity *> layers;
  layers = cpw::EntityRegistry::GetInstance()->GetEntities("WmsLayer");

  int temp = layers.size();

  for(int i = 0; i < (int) layers.size(); i++)
    if(!layers[i]->isVisible()) 
      temp--;

  if (temp > 0)
    scene->ShowWmsLayers(true);
  else
    scene->ShowWmsLayers(false);




  if (navigator_manager!=NULL)
    {
      //MPM: TOBE FIXED
      //OSG_NAVIGATION_MANAGER?
      navigator_manager->Draw();
    }

  handler_controller.Update();
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
  if(IsControllerOpen()) return;

  cpw::controllers::ElementPrimitiveController epc;
  epc.CreateElementPrimitive(parent);
}

void Application::NewElement()
{
  if(IsControllerOpen()) return;
  //if it did exist, delete first
  if (ec != NULL)
    delete ec;
	
  ec = new cpw::controllers::ElementController(main_frame, layer_tree, graphic_factory, navigator_manager, &handler_controller);

  handler_controller.ShowActiveHandler(false);
	

  ec->CreateElement();
}

void Application::NewLayer(wxWindow *parent)
{
  if(IsControllerOpen()) return;

  cpw::controllers::LayerController lc;
  lc.CreateLayer(parent, layer_tree);
}

void Application::NewTable(wxWindow *parent)
{
  if(IsControllerOpen()) return;

  cpw::controllers::DataBaseController controller;
  controller.CreateTable(parent, layer_tree);
}

void Application::NewFire()
{
  if(IsControllerOpen()) return;

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
  if(IsControllerOpen()) return;

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
  if(IsControllerOpen()) return;

  cpw::controllers::ServerConnectionController scc(main_frame, layer_tree);
  scc.ConnectServer(main_frame);
  TestDependencies("WmsLayer");
}

void Application::NewRemoteEntity(wxWindow *parent)
{
  if(IsControllerOpen()) return;

  remote_controller->Connect();
}


void Application::AddEntity(wxWindow *parent)
{
  if(IsControllerOpen()) return;

  cpw::controllers::EntityController controller(status_controller);
	
	
  if(controller.AddEntity(parent, layer_tree))
    {
      boost::filesystem::path directory(layer_tree.GetLastEntity()->GetUrl());
	
      std::string filename = cpw::ApplicationConfiguration::GetInstance()->GetEntityDirectory() + directory.leaf();////
      layer_tree.GetLastEntity()->SetUrl(filename);

      cpw::controllers::PersistentController persistent;
      cpw::PersistentError ferror = persistent.Save(NULL, layer_tree.GetLastEntity(), ".cel", false);	
    }

  TestDependencies(layer_tree.GetLastEntity());

  //delete loader;
}

void Application::AddRemoteEntity(wxWindow *parent)
{
  if(IsControllerOpen()) return;

  remote_entity_controller->CreateRemoteEntity(parent);
}

void Application::AddDataBase(wxWindow *parent)
{
  if(IsControllerOpen()) return;

  cpw::controllers::DataBaseController controller;
  controller.AddDataBase(parent, layer_tree);
}


void Application::NewContainerLayerPrimitive(wxWindow *parent)
{
  if(IsControllerOpen()) return;

  cpw::controllers::LayerPrimitiveController lpc;
  lpc.CreateContainerLayerPrimitive(parent);
}

void Application::NewRasterLayerPrimitive(wxWindow *parent)
{
  if(IsControllerOpen()) return;

  cpw::controllers::LayerPrimitiveController lpc;
  lpc.CreateRasterLayerPrimitive(parent);
}

void Application::NewVectorialLayerPrimitive(wxWindow *parent)
{
  if(IsControllerOpen()) return;

  cpw::controllers::LayerPrimitiveController lpc;
  lpc.CreateVectorialLayerPrimitive(parent);
}



void Application::MouseLeftDown()
{
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
  //main_frame->Show(true);
  if (main_frame->IsFullScreen())
    {
      main_frame->ShowFullScreen(false);// | wxFULLSCREEN_NOCAPTION);
	
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
  if(IsControllerOpen()) return;

  std::vector<cpw::Entity*> vec;
  cpw::ApplicationScene *apps = cpw::ApplicationScene::GetInstance();
  std::string app_path  = cpw::ApplicationConfiguration::GetInstance()->GetEntityDirectory();
  std::string icon_path = cpw::ApplicationConfiguration::GetInstance()->GetIconDirectory();
  std::string data_path = cpw::ApplicationConfiguration::GetInstance()->GetDataDirectory();
  std::string language  = cpw::ApplicationConfiguration::GetInstance()->GetLanguage();
  plugin_running = true;
  plugin_map[id]->ExecutePlugin(vec,apps,app_path,icon_path, data_path, language,(wxWindow*)main_frame,navigator_manager,status_controller);

  std::vector<cpw::Entity*>::iterator i = vec.begin();
  cpw::IdGenerator id_gen;
  //main_frame->SetBusyStatus(true, "Loading Simulation");
  for(;i!=vec.end();i++)
    {
      // si el tiempo inicial es cero, asumimos que es el tiempo actual
      if ((*i)->GetStartTime().seconds == 0) {
	cpw::cpwTime current_time = application_time->GetTransformedCPWTime();
	(*i)->SetStartTime(current_time);
	current_time.seconds +=  (*i)->GetEndTime().seconds;
	(*i)->SetEndTime(current_time);
      }
			
      std::stringstream old_name;
      old_name << app_path<<"/"<<((*i)->GetId());
      (*i)->SetID(id_gen.GetNewId());
      if((*i)->GetClassName() == "Fire")
	{
	  ((cpw::utils::Fire *)(*i))->SetGraphicFactory(graphic_factory);
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
	

  plugin_running = false;
}



void Application::RegisterAllPlugins()
{
  std::ifstream plugins_file(cpw::ApplicationConfiguration::GetInstance()->GetPluginsFile().c_str());	
  if (plugins_file.is_open())
    {

      while (!plugins_file.eof())
	{
	  std::string plugin;
	  plugins_file >> plugin;
	  DynamicLibrary *library = new DynamicLibrary();
	  if (plugin=="")
	    continue;
	  library->Load(plugin);
	  if (library->GetH() != NULL)
	    {
	      std::string str1;
	      library->RegisterPluginMenu(str1);
	      int id = main_frame->AddPluginMenu(str1);
	      plugin_map[id] = library;
	    }
	  else
	    if (library)
	      delete library;
			
	}
      plugins_file.close();
    }
}


void Application::Update()
{
  application_time->Update();
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
  if(IsControllerOpen()) return;

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

	  ecm = new cpw::controllers::ElementController(parent, layer_tree, graphic_factory, navigator_manager, &handler_controller, true);
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
			
	  wms_request_manager->Modify(ent);

	  TestDependencies(wmslayer.GetClassName());

	}

      if(ent->GetClassName() == fire.GetClassName())
	{
	  if(fcm != NULL)
	    delete fcm;
	  cpw::IGraphicObjectLoader *loader = graphic_factory->CreateObjectLoader();
	  fcm = new cpw::controllers::FireController(parent, layer_tree, graphic_factory, navigator_manager, loader);
	  StopTimeAndControls();
	  fcm->ModifyProperties(ent);
	}

      TestDependencies(ent->GetClassName());
    }
}
	
void Application::GoTo(const cpw::TypeId &id)
{
  cpw::Entity* ent = cpw::EntityRegistry::GetInstance()->GetEntity(id);
  bool go_to = false;
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
	      go_to = true;
	    }

	  // Get Element3D coords
	  cpw::Element3D e3d;
	  if(ent->GetClassName() == e3d.GetClassName())
	    {
	      ent_pos.x = ((cpw::Element*)ent)->GetUtm(0);
	      ent_pos.y = ((cpw::Element*)ent)->GetUtm(1);
	      ent_pos.z = ((cpw::Element*)ent)->GetUtm(2);
	      go_to = true;
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
		  ent_pos = ((cpw::utils::Fire*)ent)->GetTimeSlice(0)->flames[0].fire.GetCoords();
		}
	      go_to = true;
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
	      go_to = true;
	    }
			
	  if(go_to)
	    {
	      navigator_manager->GetFocusedOrFirstNavigator()->GetCameraController()->UpdateTimer();
	      navigator_manager->GetFocusedOrFirstNavigator()->GetCameraController()->GoTo(ent_pos,3);
	    }
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
  if(IsControllerOpen()) return;

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
  if(IsControllerOpen()) return;

  cpw::Entity *ent, *parent;
  ent = cpw::EntityRegistry::GetInstance()->GetEntity(ent_id);
  parent = cpw::EntityRegistry::GetInstance()->GetEntity(parent_id);

  handler_controller.ShowActiveHandler(false);
	

  if((ent!=NULL) && (parent != NULL))
    {
      //Before deleting the entity, issue a disconnection so the entities are not
      //erased on the remote host
      remote_controller->DisconnectAllEntities(ent);

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
	      //update handler controller if neccesary
	      if (ent->GetId() == cpw::TypeId(handler_controller.GetActiveEntity()))
		{
		  handler_controller.ShowActiveHandler(false);
		  handler_controller.Deactivate();
		}

	      ent->GraphicDelete();
	      cpw::EntityRegistry::GetInstance()->DeleteEntity(ent->GetId());			
				
	    }
	}

      layer_tree.MakeParentActive(((cpw::ContainerLayer*)parent)->GetId());
      if(layer_tree.GetLastEntity() == ent)
	layer_tree.ChangeLastAddedEntityToTop();
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

      if (id == cpw::TypeId(handler_controller.GetActiveEntity()))
	{
	  handler_controller.ShowActiveHandler(value);
			
	  if (value == false)
	    handler_controller.Deactivate();
	}


    }
}


void Application::AddLine()
{
  //cpw::IScene *scene = cpw::ApplicationScene::GetInstance()->GetScene();
  //scene->AddLine();
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
  //navigator_manager->SetAllNavigatorsToInitialPosition();
  navigator_manager->SetCameraController(two_dimension_camera_controller);
  navigator_manager->SetAllNavigatorsToLookDown();
}

void Application::Set3DNavigationMode()
{
  navigator_manager->SetCameraController(three_dimension_camera_controller);
}

void Application::CalculateDistance(wxWindow *parent)
{
  if(IsControllerOpen()) return;

  if (dc != NULL)
    {
      dc->Cancel();
      delete dc;
      dc = NULL;
    }

  cpw::IGraphicObjectLoader *loader = NULL; //graphic_factory->CreateObjectLoader();	
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
  if(IsControllerOpen()) return;

  if(CloseProject())
    {
      Draw();
      main_frame->Refresh();
      main_frame->Update();

      bool result = project_controller->OpenProject(main_frame, GetDefaultPath(), status_controller, layer_tree, graphic_factory);

      wms_request_manager->SetSRS(project_controller->GetSRS());
		
      std::string title = project_controller->GetProjectFullName();
      if (title!="")
	{
	  main_frame->SetTitle(wxString((title + " - Rvzware").c_str(),wxConvUTF8));
	}

      navigator_manager->SetAllNavigatorsToInitialPosition();

      navigator_manager->GetFocusedOrFirstNavigator()->ShowHUD(result);

      main_frame->EnableGUI(result);

      TestDependencies("WmsLayer");

      remote_controller->ConnectAllEntities();
    }	
}

bool Application::CloseProject()
{
  if(IsControllerOpen()) return false;

  bool flag = true;
  bool cancel = false;
  cpw::PersistentError ferror;

  if(layer_tree.isModified()) 
    {
      wxMessageDialog message(NULL,wxT("Save changes from the layer tree before quit?"),
			      wxT("Warning"),wxICON_EXCLAMATION |wxYES_NO |wxCANCEL);
      int modal = message.ShowModal();
      if(modal == wxID_YES)
	{	
	  cpw::controllers::EntityController controller(status_controller);
	  ferror = controller.Save(layer_tree.GetTopLayer(), true, true);
	  if (ferror != cpw::PERSISTENTOK)
	    {
	      wxMessageDialog message(NULL,wxT("Some entities from the layer tree could not be saved."),
				      wxT("Warning"),wxICON_EXCLAMATION |wxOK);
	      message.ShowModal();
	    }

	  flag = true;
	}
      if(modal == wxID_NO)
	{
	  flag = true;
	}
      if(modal == wxID_CANCEL)
	{
	  cancel = true;
	  flag = false;
	}
    }
  if (!cancel)
    {
      remote_controller->DisconnectAllEntities();

      DeleteControllers();

      main_frame->SetTitle(wxT("Rvzware"));
      project_controller->CloseProject(layer_tree);

      navigator_manager->GetFocusedOrFirstNavigator()->ShowHUD(false);
      navigator_manager->GetFocusedOrFirstNavigator()->ClearMouseEventCallBack();

      TestDependencies("WmsLayer");

      layer_tree.Detach(&video_controller);
      main_frame->EnableGUI(false);
      video_controller.Update();



      navigator_manager->Draw();
    }

  return flag;
}

void Application::UpdateTimeSettings()
{
  video_controller.Update();
}

void Application::UpdateAnimableUI()
{
  main_frame->UpdateAnimableUI();
}


bool Application::IsControllerOpen()
{
  if (ec != NULL)
    {
      if(ec->IsOpen())
	return true;
    }
  if (ecm != NULL)
    {
      if(ecm->IsOpen())
	return true;
    }
  if (fc != NULL)
    {
      if(fc->IsOpen())
	return true;
    }
  if (fcm != NULL)
    {
      if(fcm->IsOpen())
	return true;
    }
  if (evc != NULL)
    {
      if(evc->IsOpen())
	return true;
    }
  if (evcm != NULL)
    {
      if(evcm->IsOpen())
	return true;
    }
  if (dc != NULL)
    {
      if(dc->IsOpen())
	return true;
    }
  if (plugin_running)
    return true;

  return false;
}

void Application::StopTimeAndControls()
{
  main_frame->StopTimeAndControls();
}

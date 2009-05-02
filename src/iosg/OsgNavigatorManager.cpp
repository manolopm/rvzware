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
#include <osgUtil/Optimizer>

#include <iosg/stdafx.h>

#include <iosg/OsgNavigatorManager.h>

#include <cpw/graphic/ApplicationScene.h>
#include <cpw/graphic/ICameraController.h>
#include <cpw/graphic/AerialCameraController.h>
#include <cpw/entity/EntityRegistry.h>
#include <cpw/common/ApplicationLog.h>
#include <ogc/wms/WmsLayer.h>

#include <iosg/OsgObjectRegistrySingleton.h>

using namespace cpw::iosg;

#define UPDATE_COLLISION_SCENE_EVERY_FRAMES 25

//int opt_cont = 0;

OsgNavigatorManager::OsgNavigatorManager(bool lite_version_) : navigator_id(0), active_navigator(0), dbp(NULL), check_entities_changes(true), hud_controller_inserted(false), application(NULL), next_new_navigator_params_valid(false), camera_controller(NULL), lite_version(lite_version_), collision_framerate(UPDATE_COLLISION_SCENE_EVERY_FRAMES), navigators_lightning(true)
{
  cpw::ogc::WmsLayer wms_layer;
  osg_composite_viewer = new osgViewer::CompositeViewer();
  wms_layer_classname = wms_layer.GetClassName();

  //osg_hud_controller = new OsgIHUDController;

  //osg_hud_controller.AddCallBack(std::string("prueba"), &iosg::OsgIHUDController::OpenFireDialog);
}

OsgNavigatorManager::~OsgNavigatorManager(void)
{
  //delete osg_hud_controller;
  //first: stop all the threads
  for (unsigned int i=0; i < osg_composite_viewer->getNumViews(); i++)
    {
      ((osgViewer::Viewer *) osg_composite_viewer->
       getView(i))->stopThreading();
    }

  while(osg_composite_viewer->getNumViews() > 0)
    {
      osg_composite_viewer->removeView
	(osg_composite_viewer->getView(0));
    }
		
  //deleting the composite viewer all the associated viewers are deleted
  if (osg_composite_viewer)
    {
      delete osg_composite_viewer;
      osg_composite_viewer = NULL;
    }
  

  //free all the navigators
  int it=0;
  while (!navigator_vector.empty())
    {
      
      OsgNavigator * tmp = navigator_vector.back();
      if (tmp)
	{
	  delete tmp;
	  tmp = NULL;
	}
      
      navigator_vector.pop_back();
    }

  if (dbp != NULL)
    {
      while(dbp->referenceCount() >=0)
	dbp->unref();
    }

  OsgObjectRegistrySingleton::ReleaseInstance();
}

void OsgNavigatorManager::DisableAllHUDCamerasButMine(const int &id)
{
  std::vector<OsgNavigator *>::iterator iter = navigator_vector.begin();

  for ( ; iter != navigator_vector.end(); iter++)
    {
      if ((*iter)->GetId() == id)
	(*iter)->EnableHUDCamera();
      else
	(*iter)->DisableHUDCamera();

		
    }
}

void OsgNavigatorManager::ClearMouseEventCallBackOnNavigators()
{
  std::vector<OsgNavigator *>::iterator i;
	
  for(i=navigator_vector.begin(); i!=navigator_vector.end(); i++)
    (*i)->ClearMouseEventCallBack();			

}


cpw::IWindow *OsgNavigatorManager::AddNavigator(cpw::IWindow *parent_frame, int id_)
{
  OsgNavigator* new_navigator = new OsgNavigator((wxWindow *) parent_frame, application,lite_version);
  new_navigator->SetID(id_);
  new_navigator->SetDefaultPath(default_path);
  navigator_id = id_;
  new_navigator->GetHUDController()->SetApplication(application);
  new_navigator->InitGUI(); 

  cpw::ICamera *camera = (cpw::ICamera *)new_navigator->GetCamera();

  if (camera_controller != NULL)
    {
      cpw::ICameraController *cc = camera_controller->Clone();
      cc->SetCamera(camera);
      cc->SetNavigatorManager(this);
      new_navigator->SetCameraController(cc);
    }

  if (next_new_navigator_params_valid)
    {
      new_navigator->SetFocus();
      camera->GoToJustInterpolate(camera_pos, camera_view, camera_up, 10.0);
      /*camera->SetPosition(camera_pos);
	camera->SetView(camera_view);
	camera->SetUp(camera_up);*/
      next_new_navigator_params_valid = false;
    }	
	

  //((osgViewer::Viewer *) new_navigator->GetViewer())->getUpdateVisitor()->setTraversalMode(osg::NodeVisitor::TRAVERSE_ACTIVE_CHILDREN);
  //osg_composite_viewer->getUpdateVisitor()->setTraversalMode(osg::NodeVisitor::TRAVERSE_ACTIVE_CHILDREN);
  osg_composite_viewer->addView
    ((osgViewer::Viewer *) new_navigator->GetViewer());

  new_navigator->SetNavigatorManager(this);

  navigator_vector.push_back(new_navigator);

  //get the databasepager
  /*if (dbp == NULL)
    {
    dbp = ((osgViewer::Viewer *) new_navigator->GetViewer())->getDatabasePager()->clone( );
    dbp->ref();
    }

    ((osgViewer::Viewer *) new_navigator->GetViewer())->setDatabasePager(dbp.get());*/

	

  //dbp->clear();

  osgDB::DatabasePager *dbp =
    osg_composite_viewer->getView(0)->getDatabasePager();

  //if (dbp != NULL)
  //	dbp->setUnrefImageDataAfterApplyPolicy(true, false);
	
  return (cpw::IWindow *) new_navigator->GetFrame();
}

void OsgNavigatorManager::DeleteNavigator(unsigned int navigator_id)
{
  //deletes a navigator from the navigator vector
  std::vector<OsgNavigator *>::iterator i;
  OsgNavigator *n;
  osgDB::DatabasePager *new_dbp = NULL;

  //make a copy of the dbp for not loosing it when we remove the viewer
  //this shouldn´t happen, maybe an OSG bug or something we´re not doing ok...
  //if (osg_composite_viewer->getNumViews() > 1)
  /*if (dbp != NULL)
    new_dbp = dbp->clone();*/

  for(i=navigator_vector.begin(); i!=navigator_vector.end(); i++)
    {
      n = *i;
      if (n->GetId() == navigator_id)
	{
				
	  if (n)
	    {
	      osg_composite_viewer->removeView
		((osgViewer::Viewer *)n->GetViewer());
	      //	osg_composite_viewer->en
	      //osg_composite_viewer->removeView(n->GetHudView());
	      if (n)
		{
		  delete n;
		  n = NULL;
		}
	    }
	  navigator_vector.erase(i);

	  //assing our copy of the dbp to the rest of the viewers
	  //for (unsigned int j=0; j < osg_composite_viewer->getNumViews(); j++)
	  //	osg_composite_viewer->getView(j)->setDatabasePager(new_dbp);

							
	  //dbp = new_dbp;
	  new_dbp = NULL;

	  return;
	}
    }

  if (new_dbp!=NULL)
    {
      while (new_dbp->referenceCount() >= 0)
	new_dbp->unref();
    }
}

void OsgNavigatorManager::Draw()
{
  bool reset_textures = false;

  if (navigator_vector.empty())
	  
    return;
	

  std::vector<OsgNavigator *>::iterator i=navigator_vector.begin();
  for (;i!=navigator_vector.end(); i++)
    {
      (*i)->Update();
    }

  osg::Camera *camera =  osg_composite_viewer->getCameraWithFocus();
	
		
  if (camera == NULL)
    {
      osgViewer::ViewerBase::Cameras vcam;
      osg_composite_viewer->getCameras(vcam, false);
      if(!vcam.empty())
	camera = vcam.front();
    }
	
  if (camera != NULL)
    {
      osg::Vec3 eye, center, up;
      camera->getViewMatrixAsLookAt(eye, center, up);
	
    }
			
  //osg_composite_viewer->frame();

  /*OsgScene *scene = (OsgScene *) cpw::ApplicationScene::GetInstance()->GetScene();
    osgUtil::Optimizer opt;
    if (opt_cont > 1000)
    {
    opt.optimize( scene->GetRootNode() );
    opt_cont = 0;
    }
    else
    opt_cont++;*/


  osg_composite_viewer->advance();
  osg_composite_viewer->eventTraversal();
  osg_composite_viewer->updateTraversal();

  //the visitors should be called here
  if (camera != NULL)
    {
      osg::Vec3 _eye, _center, _up;
      camera->getViewMatrixAsLookAt(_eye, _center, _up);
		
      cpw::Point3d<float> eye(_eye.x(), _eye.y(), _eye.z());
      cpw::Point3d<float> center(_center.x(), _center.y(), _center.z());
      cpw::Point3d<float> up(_up.x(), _up.y(), _up.z());

		

      // ESTO ES TEMPORAL PARA DEVOLVER EL PUNTO DEL TERRENO EN EL CENTRO DE LA VISTA
      int cx, cy;
      float x, y, z;
      cpw::Point3d<float> punto_mira;
      bool found_active_nav = false;
      collision_framerate++;
      for (std::vector<OsgNavigator *>::iterator iter = navigator_vector.begin();
	   iter != navigator_vector.end();
	   iter++)
	{
	  if ((*iter)->GetId() == active_navigator)
	    {

	      (*iter)->GetCenterScreenCoords (cx, cy);
	      found_active_nav = true;
	      if (collision_framerate >= UPDATE_COLLISION_SCENE_EVERY_FRAMES)
		{
		  (*iter)->IntersectMouseWithScene (cx, cy*0.666, x, y, z);
		  punto_mira.x = x;
		  punto_mira.y = y;
		  punto_mira.z = z;
		  collision_framerate = 0;
		}
	    }
	}
		
      if (!found_active_nav)
	{
	  navigator_vector[0]->GetCenterScreenCoords (cx, cy);
	  navigator_vector[0]->IntersectMouseWithScene (cx, cy*0.666, x, y, z);
	  punto_mira.x = x;
	  punto_mira.y = y;
	  punto_mira.z = z;
	}

		
      cpw::ApplicationScene::GetInstance()->GetScene()->UpdateTextures (cx, cy, eye, center, up, punto_mira, application->GetWmsBlendingFactor(), application->GetFireViewMode());
		

    }

	
  //TODO: TO BE FIXED: THIS IS THE PROBLEM: BUG : MPM
  return;
  osg_composite_viewer->renderingTraversals();


  //std::stringstream ss;
  //ss << "active navigator = " << active_navigator;
  //cpw::ApplicationLog::GetInstance()->GetLogger()->NewLogMessage(ss.str());
}


void OsgNavigatorManager::SetDrawingModeWireframe(int navigator_id)
{

  ////////////////////////////TEMP////////
  osg::PolygonMode *pm = new osg::PolygonMode;
  pm->setMode(osg::PolygonMode::FRONT, osg::PolygonMode::LINE);
  osg::StateSet *st =((osgViewer::Viewer *)(GetFocusedOrFirstNavigator()->GetViewer()))->getCamera()->getOrCreateStateSet();
  st->setAttribute(pm);
  return;
  ////////////////////////////////////////7
  //get the navigator from the navigator vector and changes it´s drawing mode
  std::vector<OsgNavigator *>::iterator i;
  OsgNavigator *n;
	
  //search for the navigator
  for(i=navigator_vector.begin(); i!=navigator_vector.end(); i++)
    {
      n = *i;
      if (n->GetId() == navigator_id)
	{
	  //change the stateset so the drawing mode changes
	  osg::PolygonMode *pm = new osg::PolygonMode;
	  pm->setMode(osg::PolygonMode::FRONT, osg::PolygonMode::LINE);
	
	  osg::StateSet *st = ((osgViewer::Viewer *)n->GetViewer())->getCamera()->getOrCreateStateSet();
	  st->setAttribute(pm);
	  return;
	}
    }
}

void OsgNavigatorManager::SetDrawingModeSolid(int navigator_id)
{
  ////////////////////TEM///////////////
  osg::PolygonMode *pm = new osg::PolygonMode;
  pm->setMode(osg::PolygonMode::FRONT, osg::PolygonMode::FILL);
  osg::StateSet *st =((osgViewer::Viewer *)(GetFocusedOrFirstNavigator()->GetViewer()))->getCamera()->getOrCreateStateSet();
  st->setAttribute(pm);
  return;
  //////////////////////////
  //get the navigator from the navigator vector and changes it´s drawing mode
  std::vector<OsgNavigator *>::iterator i;
  OsgNavigator *n;
	
  //search for the navigator
  for(i=navigator_vector.begin(); i!=navigator_vector.end(); i++)
    {
      n = *i;
      if (n->GetId() == navigator_id)
	{
	  //change the stateset so the drawing mode changes
	  osg::PolygonMode *pm = new osg::PolygonMode;
	  pm->setMode(osg::PolygonMode::FRONT, osg::PolygonMode::FILL);
	
	  osg::StateSet *st = ((osgViewer::Viewer *)n->GetViewer())->getCamera()->getOrCreateStateSet();
	  st->setAttribute(pm);
	  return;
	}
    }
}

cpw::INavigator * OsgNavigatorManager::GetFocusedOrFirstNavigator()
{
  std::vector<OsgNavigator *>::iterator i = navigator_vector.begin();

  for (i; i != navigator_vector.end(); i++)
    {
      if ((*i)->GetId() == active_navigator)
	{
	  return (cpw::INavigator *) *i;
	}
    }

  //if (!navigator_vector.empty())
  //	return (cpw::INavigator *) navigator_vector.front();

  return NULL;
}

void OsgNavigatorManager::SetActive(int id)
{ 
  active_navigator = id;
}

void OsgNavigatorManager::SetMouseEventCallBackOnNavigators(cpw::IEventCallBack *event_callback)
{
  std::vector<OsgNavigator *>::iterator i;
	
  for(i=navigator_vector.begin(); i!=navigator_vector.end(); i++)
    {
      (*i)->SetMouseEventCallBack(event_callback);
    }
	
}


void OsgNavigatorManager::Update()
{

}

void OsgNavigatorManager::SetApplication(cpw::IApplication *_application) 
{ 
  application = _application;
  //osg_hud_controller->SetApplication(_application); 
}

void OsgNavigatorManager::CameraParametersToNextNewNavigator(const cpw::Point3d<double> &position,
							     const cpw::Point3d<double> &view,
							     const cpw::Point3d<double> &up)
{
  next_new_navigator_params_valid = true;
  camera_pos = position;
  camera_view = view;
  camera_up = up;

}


void OsgNavigatorManager::Pause(bool pause)
{ 
  for (unsigned int i=0; i < osg_composite_viewer->getNumViews(); i++)
    {
      /*osg_composite_viewer->getView(i)->getDatabasePager()->updateSceneGraph(0.0);
	osg_composite_viewer->getView(i)->getDatabasePager()->setDatabasePagerThreadPause(pause);
	osg_composite_viewer->getView(i)->getDatabasePager()->setAcceptNewDatabaseRequests(!pause);
	osg_composite_viewer->getView(i)->getDatabasePager()->clear();
	osg_composite_viewer->getView(i)->getDatabasePager()->setDoPreCompile(false);*/
		
    }
	
}

OsgNavigator * OsgNavigatorManager::NavigatorExists(const int &id)
{
  std::vector<OsgNavigator *>::iterator i;
	
  for(i=navigator_vector.begin(); i!=navigator_vector.end(); i++)
    {
      if ( (*i)->GetId() == id)
	return *i;
    }
	
  return NULL;
}


bool OsgNavigatorManager::ChangeNavigatorWindow(cpw::IWindow *parent_frame, int id)
{
	
  OsgNavigator *nav;

  nav = NavigatorExists(id);

  if (nav == NULL)
    return false;

  nav->NewWindow((wxWindow *)parent_frame);	

  return false;
}

void OsgNavigatorManager::EveryNavigatorsToStartPosition()
{
  std::vector<OsgNavigator *>::iterator i = navigator_vector.begin();

  for ( ; i != navigator_vector.end(); i++)
    {
      (*i)->GetCamera()->GoToJustInterpolate(cpw::Point3d<double>(421429, 189000, 1083880), cpw::Point3d<double>(0.0, 0.0, -1.0), cpw::Point3d<double>(0.0, 1.0, 0.0), 10.0);
      //(*i)->GetCamera()->SetView(cpw::Point3d<double>(0.0, 0.0, -1.0));
      //(*i)->GetCamera()->SetUp(cpw::Point3d<double>(0.0, 1.0, 0.0));
    }

  for (unsigned int j=0; j<800; j++) //at 60fps this lasts the 3 seconds that the goto needs
    {
      Draw();
    }
}

void OsgNavigatorManager::SetCameraController(cpw::ICameraController *_camera_controller)
{ 
  camera_controller = _camera_controller; 

  for (std::vector<OsgNavigator *>::iterator iter = navigator_vector.begin();
       iter != navigator_vector.end();
       iter++)
    {
      //new camera controller
      cpw::ICameraController *cc = camera_controller->Clone();
      cc->SetCamera( (*iter)->GetCamera() );
      cc->SetNavigatorManager(this);

      //replace it	
      (*iter)->SetCameraController(cc);
    }
}

void OsgNavigatorManager::SetAllNavigatorsToInitialPosition()
{
  for (std::vector<OsgNavigator *>::iterator iter = navigator_vector.begin();
       iter != navigator_vector.end();
       iter++)
    {
      (*iter)->GoBackToInitialPosition();
    }

}


void OsgNavigatorManager::SetAllNavigatorsToLookDown()
{
  for (std::vector<OsgNavigator *>::iterator iter = navigator_vector.begin();
       iter != navigator_vector.end();
       iter++)
    {
      if ( (*iter)->GetCamera() != NULL )
	{
	  //(*iter)->GetCamera()->SetPosition(cpw::Point3d<double>(205172, 3150667,  50000.0));
	  (*iter)->GetCamera()->SetView(cpw::Point3d<double>(0.0, 0.0, -1.0));
	  (*iter)->GetCamera()->SetUp(cpw::Point3d<double>(0.0, 1.0, 0.0));
	}
    }
}

void OsgNavigatorManager::SetAllNavigatorsToLookPos(const cpw::Point3d<double> &position)
{
  for (std::vector<OsgNavigator *>::iterator iter = navigator_vector.begin();
       iter != navigator_vector.end();
       iter++)
    {
      if ( (*iter)->GetCamera() != NULL )
	{
	  (*iter)->GetCamera()->SetRoof(9000000);
	  (*iter)->GetCamera()->SetPosition(position);
	  (*iter)->GetCamera()->SetView(cpw::Point3d<double>(0.0, 0.0, -1.0));
	  (*iter)->GetCamera()->SetUp(cpw::Point3d<double>(0.0, 1.0, 0.0));
	}
    }
}


void OsgNavigatorManager::SetLightningToAllNavigators(const bool &lightning)
{
  if (navigators_lightning != lightning)
    {
      for (std::vector<OsgNavigator *>::iterator iter = navigator_vector.begin();
	   iter != navigator_vector.end();
	   iter++)
	{
	  (*iter)->SetIllumination(lightning);
	}
		
      navigators_lightning = lightning;
    }
}

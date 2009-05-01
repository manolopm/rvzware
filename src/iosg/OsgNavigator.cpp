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

#include <osgUtil/IntersectVisitor>
#include <osgDB/ReadFile>

#include <iosg/stdafx.h>

#include <cpw/entity/EntityRegistry.h>
#include <cpw/graphic/ApplicationScene.h>
#include <cpw/common/ApplicationLog.h>
#include <cpw/ApplicationConfiguration.h>

#include <iosg/gui/OsgCompass.h>
#include <iosg/gui/OsgPanel.h>
#include <iosg/OsgNavigatorPreDrawCallback.h>
#include <iosg/OsgNavigatorPostDrawCallback.h>
#include <iosg/OsgNavigator.h>

using namespace cpw::iosg;


OsgNavigator::OsgNavigator(wxWindow *parent_frame, cpw::IApplication *_application, bool lite_version_) : debug_text(NULL), camera_controller(NULL), application(_application),	init_position(421429.0, 189000.0, 1083880.0),init_view(0.0, 0.0, -1.0),	init_up(0.0, 1.0, 0.0),	lite_version(lite_version_)
{
	
	
	camera_predraw_callback = new OsgNavigatorPreDrawCallback;
	camera_postdraw_callback = new OsgNavigatorPostDrawCallback;

	osg_hud_controller = new OsgIHUDController;
	osg_hud_controller->SetNavigator(this);

	((OsgNavigatorPreDrawCallback *) camera_predraw_callback.get())->SetNavigator(this);
	((OsgNavigatorPreDrawCallback *) camera_postdraw_callback.get())->SetNavigator(this);

	osg_hud = new OsgIHUD;
	osg_hud_controller->SetHUD(osg_hud);
	_parent = parent_frame;
	osg_hud->SetParentWindow(parent_frame);
	osg_hud->SetApplication(application);
	gl_canvas = new OsgNavigatorWxGLCanvas(parent_frame, wxID_ANY, wxDefaultPosition, wxSize(640, 480));

	osg_viewer = new osgViewer::Viewer;
	osg_viewer->getCamera()->setGraphicsContext(gl_canvas);
	osg_viewer->getCamera()->setViewport(0,0,640,480);
	gl_canvas->SetViewer(osg_viewer);
		
	cpw::IScene *app_scene = cpw::ApplicationScene::GetInstance()->GetScene();
	osg::ref_ptr<osg::Node> scene = (osg::Node *) app_scene->GetSceneData();
	
	float px, py, pz, vx, vy, vz, ux, uy, uz;
	app_scene->GetDefaultCameraCoords(px, py, pz, vx, vy, vz, ux, uy, uz);

	//PEDRO PRUEBAS
	//px = 0; py = 0; pz= -1; vx = 0; vy=0; vz=100; ux=0; uy=1; uz=0;

	osg_viewer->getCamera()->setViewMatrixAsLookAt(osg::Vec3(px, py, pz), osg::Vec3(vx, vy, vz), osg::Vec3(ux, uy, uz));
	osg_viewer->getCamera()->setPreDrawCallback(camera_predraw_callback.get());
	
	//link the scene to the viewer
	osg_viewer->setSceneData(scene.get());

	//turn off the ilumination
	osg::StateSet *st = osg_viewer->getCamera()->getOrCreateStateSet();
	//st->setMode(GL_LIGHTING, false);
	st->setMode(GL_LIGHTING, true);
	st->setMode(GL_CULL_FACE,osg::StateAttribute::ON);
	osg_viewer->getCamera()->setStateSet(st);

	//gl_canvas->SetCallBackOnMouseEvent( &iosg::OsgNavigator::MouseHandler );
	gl_canvas->SetNavigator(this);

	camera = new OsgCamera;

	init_position.x = px; init_position.y = py; init_position.z = pz;
	init_view.x = vx;     init_view.y = vy;     init_view.z = vz;
	init_up.z = uz;       init_up.y = uy;       init_up.z = uz;

	init_view = init_view - init_position;
	cpw::Math::NormalizeVec(init_view);

	camera->SetPosition(init_position);
	camera->SetView(init_view);
	camera->SetUp(init_up);
	camera->SetNavigator(this);
	camera->SetOsgScene((OsgScene *)cpw::ApplicationScene::GetInstance()->GetScene());
	
	map_size = 100.0;

	// --'
	osgViewer::Viewer::Windows windows;
	osg_viewer->getWindows(windows); //sospechoso maximo, está devolviendo mal las ventanas?
	
	//AddEventReceiverToCanvas(osg_hud_controller);
	gl_canvas->AddFirstEventReceiver(osg_hud_controller);
	
	window = windows.back();
	//osgViewer::GraphicsWindow * window = windows.front();
	CreateHudCamera(window); //sospechoso maximo, está devolviendo mal las ventanas?

}

OsgNavigator::~OsgNavigator(void)
{
  if (camera)
    delete camera;

  if (camera_controller != NULL)
    delete camera_controller;

  //delete osg_compass;
  if (cpw::ApplicationScene::GetInstance())
    {
      OsgScene *AppScene = (OsgScene *)cpw::ApplicationScene::GetInstance()->GetScene();
      if (AppScene)
	{
	  osg::Group *scene_root_node = (osg::Group *) AppScene->GetRootNode();
	  
	  if (scene_root_node)
	    {
	      for (unsigned int i=0; i<scene_root_node->getNumChildren(); i++)
		{
		  if (scene_root_node->getChild(i) == hud_camera_active_switch.get())
		    {
		      scene_root_node->removeChild(i);
		      i = scene_root_node->getNumChildren();
		    }
		}
	    }
	  //hud_camera->setPostDrawCallback(camera_postdraw_callback.get());
	}
    }
  if (osg_hud_controller)
    delete osg_hud_controller;
  if (osg_hud)
    delete osg_hud;
}

void OsgNavigator::NewWindow(wxWindow *parent_frame)
{
	osg_hud->SetParentWindow(parent_frame);

	if (gl_canvas)
	  delete gl_canvas;
	
	gl_canvas = new OsgNavigatorWxGLCanvas(parent_frame, wxID_ANY, wxDefaultPosition, wxSize(640, 480));

	//create the osgviewer and link it to the canvas
	//osg_viewer = new osgV     iewer::Viewer;
	//osg_viewer->getCamera()->relre
	osg_viewer->getCamera()->setGraphicsContext(gl_canvas);
	osg_viewer->getCamera()->setViewport(0,0,640,480);
	gl_canvas->SetViewer(osg_viewer);

}


void OsgNavigator::InitGUI()
{
	cpw::Point3d<float> logo_position (0.0f, 0.0f, -200.0f);
	cpw::Point3d<float> logo_size(200.0f, 200.0f, 1.0f); //200, 75.195267
	cpw::Point3d<float> logo_rotation (0.0f, 0.0f, 0.0f);
	TAnchor logo_anchor;
	logo_anchor.x = -(100.0f + 5.0f);
	logo_anchor.y = 38.0f + 5.0f;
	logo_anchor.w = logo_anchor.h = 1.0f;
	logo_anchor.h = 0.0f;
	osg_logo = new OsgIImage(default_path, logo_position, logo_size, logo_rotation, logo_anchor, cpw::ApplicationConfiguration::GetInstance()->GetApplicationLogo());	
	osg_hud->AddWidget(osg_logo);
	osg_logo->SetVisible(true);


	cpw::Point3d<float> bcompass_position (0.0f, 0.0f, -200.0f);
	cpw::Point3d<float> bcompass_size(90.0f, 90.0f, 1.0f);
	cpw::Point3d<float> bcompass_rotation (0.0f, 0.0f, 0.0f);
	TAnchor bcompass_anchor;
	bcompass_anchor.x = bcompass_anchor.y = bcompass_anchor.w = bcompass_anchor.h = 0;
	bcompass_anchor.w = bcompass_anchor.h = 1.0f;
	bcompass_anchor.x = -60.0f;
	bcompass_anchor.y = -60.0f;
	osg_compass = new OsgICompass(default_path, bcompass_position, bcompass_size, bcompass_rotation, bcompass_anchor);	
	osg_compass->SetCamera(camera);
	osg_hud->AddWidget(osg_compass/*, bar_position, bar_size, bar_rotation*/);
	osg_compass->SetVisible(true);



	//minimap left corner, proportion 0.3759765
	cpw::Point3d<float> minimap_position (0.0f, 0.0f, -100.0f);
	cpw::Point3d<float> minimap_size(300.0f/1.5f, 112.7929f/1.5f, 1.0f/1.5f); //200, 75.195267
	cpw::Point3d<float> minimap_rotation (0.0f, 0.0f, 0.0f);
	TAnchor minimap_anchor;
	minimap_anchor.x = 150.0f/1.5f + 5.0f;
	minimap_anchor.y = -(56.3954f/1.5f + 5.0f);
	minimap_anchor.w = minimap_anchor.h = 0.0f;
	minimap_anchor.h = 1.0f;
	osg_minimap = new OsgIMinimap(default_path, minimap_position, minimap_size, minimap_rotation, minimap_anchor);	
	osg_hud->AddWidget(osg_minimap/*, bar_position, bar_size, bar_rotation*/);
	osg_minimap->SetCamera(camera);
	osg_minimap->SetVisible(false);
	//osg_minimap->setStatus(1);

	osg_hud_controller->SetMinimap(osg_minimap);
	osg_hud_controller->SetCompass(osg_compass);
	//osg_minimap->SetNavigator(this);

	//BAR proportion 4.31596; 4.146 x 25.098 -> 6.053545586
	cpw::Point3d<float> bar_position(0.0f, 0.0f, -172.0f);
	//cpw::Point3d<float> bar_size(80.0f, 690.4f, 5.0f); 
	cpw::Point3d<float> bar_size(80.0f*0.7f, 460.0f*0.7f, 5.0f*0.7f); 
	cpw::Point3d<float> bar_rotation(0.0f, 0.0f, 0.0f);
	TAnchor bar_anchor;
	bar_anchor.w = 1.0f;
	bar_anchor.h = 0.78f;
	bar_anchor.x = -37*0.7f;
	bar_anchor.y = 0;
	//osg_bar = new OsgIPanel( bar_position, bar_size, bar_rotation, bar_anchor );
	osg_bar = new OsgINavigationPanel(default_path, bar_position, bar_size, bar_rotation, bar_anchor,lite_version);
	osg_bar->SetCamera(camera);
	osg_bar->RegisterButtons(osg_hud_controller);
	//osg_hud.SetNavigator(this);
	osg_hud->AddWidget(osg_bar/*, bar_position, bar_size, bar_rotation*/);
	//osg_bar->SetCamera(camera);
	osg_hud_controller->SetNavigationPanel(osg_bar);
	osg_bar->StartToggleAnimation();


	cpw::Point3d<float> UTMLabel_position (0.0f, 0.0f, -200.0f);
	cpw::Point3d<float> UTMLabel_size(14.0f, 14.0f, 1.0f); //200, 75.195267
	cpw::Point3d<float> UTMLabel_rotation (0.0f, 0.0f, 0.0f);
	TAnchor UTMLabel_anchor;
	UTMLabel_anchor.x = 5.0f;
	UTMLabel_anchor.y = 5.0f;
	UTMLabel_anchor.w = UTMLabel_anchor.h = 0.0f;
	osg_UTMLabel = new OsgIUTMLabel(default_path, UTMLabel_position, UTMLabel_size, UTMLabel_rotation, UTMLabel_anchor);	
	osg_UTMLabel->SetCamera(camera);
	osg_hud->AddWidget(osg_UTMLabel);
	osg_UTMLabel->SetVisible(true);


}

void OsgNavigator::Show_HUD(bool s) 
{
	osg_UTMLabel->SetVisible(s);
	osg_compass->SetVisible(s);
	osg_bar->SetVisible(s);
	osg_minimap->SetVisible(false);
}

void OsgNavigator::SetDefaultPath(const std::string &url)
{
	default_path = url;
	
	if(camera != NULL)
		camera->SetDefaultPath(url);

}

void OsgNavigator::ClearMouseEventCallBack()
{
	gl_canvas->ClearMouseEventCallBack();
}
void OsgNavigator::SetMouseEventCallBack(cpw::IEventCallBack *event_callback)
{ 
	gl_canvas->SetCallBackOnMouseEvent(event_callback); 
}

bool OsgNavigator::SetEventReceiver(const unsigned int &position, cpw::IEventReceiver *event_receiver)
{

	return gl_canvas->AddEventReceiverInPosition(position, event_receiver);

}
void OsgNavigator::SetID(int id)
{
	nav_id = id; 
}


void OsgNavigator::DeleteMe()
{
	navigator_manager->DeleteNavigator(nav_id);
}


void OsgNavigator::DrawMe()
{
	navigator_manager->Draw();
}

void OsgNavigator::SetNavigatorManager(cpw::INavigatorManager *nm)
{
	navigator_manager = (OsgNavigatorManager *) nm; 
	osg_hud_controller->SetNavigatorManager(nm);
}

bool OsgNavigator::IntersectMouseWithScene(int mousex, int mousey, cpw::Point3d<float> &i_point, cpw::Point3d<float> &i_normal)
{
	osgUtil::LineSegmentIntersector::Intersections intersections;

	//test collisions against the scene
	if (osg_viewer->computeIntersections(mousex,mousey,intersections))
	{
		//if there is a collision
		osg::Vec3 intersection_point;
		const osgUtil::LineSegmentIntersector::Intersection& inter = *intersections.begin();
		intersection_point = inter.getWorldIntersectPoint();
		osg::Vec3 normal = inter.getLocalIntersectNormal();

		normal.normalize();
		
		i_normal.x = normal.x();
		i_normal.y = normal.y();
		i_normal.z = normal.z();

		i_point.x = intersection_point.x();
		i_point.y = intersection_point.y();
		i_point.z = intersection_point.z();

		cpw::Point3d<float> converted_ip =	OsgScene::SceneCoordsToUTM(i_point);

		i_point = converted_ip;

		return true;
	}
	else 
		return false;
}
bool OsgNavigator::IntersectMouseWithScene(int mousex, int mousey, float &x, float &y, float &z)
{
	osgUtil::LineSegmentIntersector::Intersections intersections;

	cpw::Point3d<float> i_point(x, y, z);
	//test collisions against the scene

	if (osg_viewer->computeIntersections(mousex,mousey,intersections, OSG_SCENE_TERRAIN_NODE_MASK))
	{
		//if there is a collision
		osg::Vec3 intersection_point;
		const osgUtil::LineSegmentIntersector::Intersection& inter = *intersections.begin();
		intersection_point = inter.getWorldIntersectPoint();
		osg::Vec3 normal = inter.getLocalIntersectNormal();
		
		x = intersection_point.x();
		y = intersection_point.y();
		z = intersection_point.z();

		cpw::Point3d<float> converted_ip = OsgScene::SceneCoordsToUTM(cpw::Point3d<float>(x, y, z));

		x = converted_ip.x;
		y = converted_ip.y;
		z = converted_ip.z;

		//std::vector< osg::Node *>::iterator iter = inter.nodePath.begin();

		for ( unsigned int iter = 0; iter < inter.nodePath.size(); iter++)
		{
			if (inter.nodePath[iter]->getNodeMask() == OSG_SCENE_TERRAIN_NODE_MASK)
			{
				return true;
			}
		}

		return false;
	}
	else 
		return false;

	//if no collisions are detected return the camera view vector
	//colision with the x-y plane

	osg::Vec3 pos, up, look_at, v;
	float t;
	osg_viewer->getCamera()->getViewMatrixAsLookAt(pos, look_at, up);

	v[0] = pos.x() - look_at.x();
	v[1] = pos.y() - look_at.y();
	v[2] = pos.z() - look_at.z();
	v.normalize();

	//calculate where in T the ray hits de xy plane
	if (v.z() != 0.0f)
	{
		t = (-1.0f * pos.z()) / v.z();
		
		if (t < 0.0f)
		{
			x = pos.x();
			y = pos.y();
			z = 0.0f;
			return true;
		}

		x = pos.x() + v.x() * t;
		y = pos.y() + v.y() * t;
		z = 0.0f;
		return true;
	}

	x = pos.x();
	y = pos.y();
	z = 0.0f;
	return true;
}

bool OsgNavigator::IntersectMouseWithEntities(int mousex, int mousey, std::string &entity_id)
{
	osgUtil::LineSegmentIntersector::Intersections intersections;

	//test collisions against the scene
	if (osg_viewer->computeIntersections(mousex,mousey,intersections))
	{
		std::vector<cpw::Entity *> entities = cpw::EntityRegistry::GetInstance()->GetAllEntities();
		std::vector<cpw::Entity *>::iterator entities_iterator;

		//if there is a collision
		osg::Vec3 intersection_point;
		
		
		std::multiset< osgUtil::LineSegmentIntersector::Intersection >::iterator intersect_iter = intersections.begin();
		for ( ; intersect_iter != intersections.end(); intersect_iter++)
		{
			const osgUtil::LineSegmentIntersector::Intersection& inter = *intersect_iter;
		
				
			for ( unsigned int iter = 0; iter < inter.nodePath.size(); iter++)
			{
				std::string node_name = inter.nodePath[iter]->getName();
				entities_iterator = entities.begin();

				if (!node_name.empty())
				{
					for ( ; entities_iterator != entities.end(); entities_iterator++)
					{
						if ((*entities_iterator)->GetId().GetValue() == node_name)
						{
							if ((*entities_iterator)->GetClassName() == std::string("Element3D"))
							{
								entity_id = node_name;
								return true;
							}
						}
					}
				}

				//check if the name is the same as one of the entities

			}
		}

		return false;
	}
	else 
		return false;
}

bool OsgNavigator::IntersectMouseWithGizmos(int mousex, int mousey, std::string &gizmo_id)
{
	osgUtil::LineSegmentIntersector::Intersections intersections;

	//test collisions against the scene
	if (osg_viewer->computeIntersections(mousex,mousey,intersections))
	{
		std::vector<cpw::Entity *> entities = cpw::EntityRegistry::GetInstance()->GetAllEntities();
		std::vector<cpw::Entity *>::iterator entities_iterator;

		//if there is a collision
		osg::Vec3 intersection_point;
		const osgUtil::LineSegmentIntersector::Intersection& inter = *intersections.begin();
				
		osgUtil::LineSegmentIntersector::Intersections::iterator intersections_iterator = intersections.begin();

		for ( ; intersections_iterator != intersections.end(); intersections_iterator++)
		{
			for (unsigned int i=0; i < intersections_iterator->nodePath.size(); i++)
			{
				if (intersections_iterator->nodePath[i]->getNodeMask() == OSG_SCENE_GIZMOS_NODE_MASK)
				{
					gizmo_id = intersections_iterator->nodePath[i]->getName();
					return true;
				}
			}
		}

		/*for ( unsigned int iter = 0; iter < inter.nodePath.size(); iter++)
		{
			if (inter.nodePath[iter]->getNodeMask() == OSG_SCENE_GIZMOS_NODE_MASK)
			{
					gizmo_id = inter.nodePath[iter]->getName();
					return true;
			}
		}*/
	}
	
	return false;
}

void OsgNavigator::GetCenterScreenCoords(int &coordx, int &coordy)
{
	//frame->GetSize(&coordx, &coordy);
	gl_canvas->GetSize(&coordx, &coordy);
	coordx /= 2;
	coordy /= 2;
}

void OsgNavigator::MakeMeActive()
{ 
	navigator_manager->SetActive(nav_id); 
}



void OsgNavigator::Update()
{
	camera->Update();
	cpw::Point3d<double> position = camera->GetPosition();
	cpw::Point3d<double> up = camera->GetUp();
	cpw::Point3d<double> view = camera->GetView();
	cpw::Point3d<float> cupf((float)up.x, (float)up.y, (float)up.z);
	cpw::Point3d<float> cviewf((float)view.x, (float)view.y, (float)view.z);

	view.x *= 100.0;
	view.y *= 100.0;
	view.z *= 100.0;

	view.x += position.x;
	view.y += position.y;
	view.z += position.z;

	osg_viewer->getCamera()->setViewMatrixAsLookAt(osg::Vec3d(position.x, position.y, position.z),
															osg::Vec3d(view.x, view.y, view.z),
															osg::Vec3d(up.x, up.y, up.z));

	

	//osg_compass->RotateZ(0.0);
	//rororor += 0.001;
	osg_hud->Update();

	//osg_viewer->getCamera()->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
	//osg_viewer->getCamera()->setComputeNearFarMode(osg::CullSettings::COMPUTE_NEAR_FAR_USING_BOUNDING_VOLUMES);
	//osg_viewer->getCamera()->setComputeNearFarMode(osg::CullSettings::COMPUTE_NEAR_FAR_USING_PRIMITIVES);
	//osg_viewer->getCamera()->setNearFarRatio(1e-9);


	// FORMA ORIGINAL
	osg_viewer->getCamera()->setComputeNearFarMode(osg::CullSettings::COMPUTE_NEAR_FAR_USING_PRIMITIVES);
	osg_viewer->getCamera()->setNearFarRatio(0.0000001);

	
}

void OsgNavigator::CreateHudCamera(osgViewer::GraphicsWindow * window)
{
	osg::ref_ptr<osg::MatrixTransform> matrix_transform = new osg::MatrixTransform();

	hud_camera = new osg::Camera;
    hud_camera->setProjectionMatrix(osg::Matrix::ortho2D(0,1280,0,1024));
    hud_camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    hud_camera->setViewMatrix(osg::Matrix::identity());
    hud_camera->setClearMask(GL_DEPTH_BUFFER_BIT);
	hud_camera->setRenderOrder(osg::Camera::POST_RENDER);
	//hud_camera->setRenderOrder(osg::Camera::PRE_RENDER);
	
   
	hud_camera->addChild(matrix_transform.get());
	osg_hud->SetRootNode(matrix_transform.get());

	hud_camera->setGraphicsContext(window); // --' pq no gl_canvas?
    hud_camera->setViewport(0,0,window->getTraits()->width, window->getTraits()->height);
  
	OsgScene *osg_scene = ((OsgScene *)cpw::ApplicationScene::GetInstance()->GetScene());

	hud_camera_active_switch = new osg::Switch;

	hud_camera_active_switch->addChild(hud_camera.get()); //o por aqui?
	if (((osg::Group *)osg_scene->GetRootNode()) != NULL)
		((osg::Group *)osg_scene->GetRootNode())->addChild(hud_camera_active_switch.get());

	hud_camera_active_switch->setAllChildrenOff();
	hud_camera_active_switch->setName("HUD-Camera");

	hud_camera->setPostDrawCallback(camera_postdraw_callback.get());

	
}

void OsgNavigator::EnableHUDCameraAndDisableOthers() { navigator_manager->DisableAllHUDCamerasButMine(nav_id); }

void OsgNavigator::SetDebugText(const std::string &_debug_text)
{
	//en release el setText peta si se llama muchas
	//veces seguidas y con parametros distintos
	//de momento se queda funcionando solamente en debug
	//hasta que salga el nuevo OSG (2.4) que en teoria le queda
	//poco y recompilaremos a ver que pasa
	#ifdef _DEBUG 
		debug_text.get()->setText(_debug_text);
	#endif
}
void OsgNavigator::UpdateProjectionMatrix(const int &x, const int &y)
{
	if (hud_camera == NULL)
		return;

	cpw::Point3d<float> new_compass_position;

	hud_camera->setProjectionMatrix(osg::Matrix::ortho2D(0,x,0,y));

	osg_hud->UpdateSize(x, y);
	
	/*new_compass_position.x = x - compass.GetSize() * 0.8;
	new_compass_position.y = y - compass.GetSize() * 0.8;
	new_compass_position.z = 0.0;

	compass.SetPosition(new_compass_position);

	map_position[0] = map_size;
	map_position[1] = map_size;
	map_position[2] = 0.0;*/
	//debug_text->setPosition(osg::Vec3(10.0f,10.0f,0.0f));//osg::Vec3(150.0f,800.0f,0.0f));

	
	
}

void OsgNavigator::SetCameraController(cpw::ICameraController *_camera_controller)
{
	if (camera_controller != NULL)
	{
		gl_canvas->RemoveEventReceiver((cpw::IEventReceiver *) camera_controller);
		delete camera_controller;
	}

	gl_canvas->AddLastEventReceiver((cpw::IEventReceiver *) _camera_controller);

	camera_controller = _camera_controller; 
}



bool OsgNavigator::IntersectMouseWithHUD(int mousex, int mousey, cpw::Point3d<float> &i_point, std::string &obj_id)
{
	osg::ref_ptr<osgUtil::LineSegmentIntersector> lsi;
	
	lsi = new osgUtil::LineSegmentIntersector(osg::Vec3(mousex, mousey, -10000), osg::Vec3(mousex, mousey, 10000));

	osg::ref_ptr<osgUtil::IntersectionVisitor> iv = new osgUtil::IntersectionVisitor(lsi.get());

	hud_camera->accept(*(iv.get()));

	if (lsi->containsIntersections())
	{
		osgUtil::LineSegmentIntersector::Intersection first_intersection = *(lsi->getIntersections().begin());
		osg::Vec3 intersection_point = first_intersection.getWorldIntersectPoint();
		i_point.x = intersection_point.x();
		i_point.y = intersection_point.y();
		i_point.z = intersection_point.z();

		obj_id = first_intersection.drawable->getName();
		return true;
	}

	return false;
}

bool OsgNavigator::IntersectMouseWithHUD(int mousex, int mousey, cpw::Point3d<float> &i_point, std::string &obj_id, cpw::Point3d<float> &local_i_point)
{
	osg::ref_ptr<osgUtil::LineSegmentIntersector> lsi;
	
	lsi = new osgUtil::LineSegmentIntersector(osg::Vec3(mousex, mousey, -10000), osg::Vec3(mousex, mousey, 10000));

	osg::ref_ptr<osgUtil::IntersectionVisitor> iv = new osgUtil::IntersectionVisitor(lsi.get());

	hud_camera->accept(*(iv.get()));

	if (lsi->containsIntersections())
	{
		osgUtil::LineSegmentIntersector::Intersection first_intersection = *(lsi->getIntersections().begin());
		
		osg::Vec3 intersection_point = first_intersection.getWorldIntersectPoint();
		i_point.x = intersection_point.x();
		i_point.y = intersection_point.y();
		i_point.z = intersection_point.z();

		intersection_point = first_intersection.getLocalIntersectPoint();
		local_i_point.x = intersection_point.x();
		local_i_point.y = intersection_point.y();
		local_i_point.z = intersection_point.z();



		obj_id = first_intersection.drawable->getName();
		return true;
	}

	return false;
}

void OsgNavigator::AddEventReceiverToCanvas(cpw::IEventReceiver *event_receiver)
{
	gl_canvas->AddLastEventReceiver(event_receiver);
}

bool OsgNavigator::IntersectMouseWithXYPlane(int mousex, int mousey, cpw::Point3d<float> &i_point)
{
	float ray_x, ray_y;
	
	
	osg_viewer->getCameraContainingPosition(mousex, mousey, ray_x, ray_y);
	osgUtil::LineSegmentIntersector::CoordinateFrame cf = osgUtil::Intersector::WINDOW;
	osgUtil::LineSegmentIntersector* picker = new osgUtil::LineSegmentIntersector(cf, ray_x, ray_y);

	osg::Vec3 start = picker->getStart();
	//osg::Vec3 end = picker->getEnd();

	
	return false;
}

bool OsgNavigator::IntersectMouseWithNodePath(int mousex, int mousey, cpw::Point3d<float> &i_point, osg::NodePath &nodepath)
{

	osg::Node *scene_data = osg_viewer->getSceneData();

	osg_viewer->setSceneData(*nodepath.begin());

	osgUtil::LineSegmentIntersector::Intersections intersections;

	//test collisions against the nodepath
	if (osg_viewer->computeIntersections(mousex,mousey, intersections))
	{
		//if there is a collision
		osg::Vec3 intersection_point;
		const osgUtil::LineSegmentIntersector::Intersection& inter = *intersections.begin();
		intersection_point = inter.getWorldIntersectPoint();
		osg::Vec3 normal = inter.getLocalIntersectNormal();

		normal.normalize();
		
		i_point.x = intersection_point.x();
		i_point.y = intersection_point.y();
		i_point.z = intersection_point.z();

		cpw::Point3d<float> converted_ip = OsgScene::SceneCoordsToUTM(i_point);

		i_point = converted_ip;

		osg_viewer->setSceneData(scene_data);
		return true;
	}
	else 
	{
		osg_viewer->setSceneData(scene_data);
		return false;
	}

}

void OsgNavigator::SetApplication(cpw::IApplication *_application)
{ 
	application = _application;
	osg_hud->SetApplication(_application); 
}

void OsgNavigator::SetFocus() { gl_canvas->SetNavigatorFocus(); }

void OsgNavigator::GoBackToInitialPosition()
{
	float px, py, pz, vx, vy, vz, ux, uy, uz;
	cpw::IScene *app_scene = cpw::ApplicationScene::GetInstance()->GetScene();

	if (app_scene == NULL)
		return;
	
	app_scene->GetDefaultCameraCoords(px, py, pz, vx, vy, vz, ux, uy, uz);
	osg_viewer->getCamera()->setViewMatrixAsLookAt(osg::Vec3(px, py, pz), osg::Vec3(vx, vy, vz), osg::Vec3(ux, uy, uz));
	
	init_position.x = px; init_position.y = py; init_position.z = pz;
	init_view.x = vx;     init_view.y = vy;     init_view.z = vz;
	init_up.z = uz;       init_up.y = uy;       init_up.z = uz;

	init_view = init_view - init_position;
	cpw::Math::NormalizeVec(init_view);

	camera->SetPosition(init_position);
	camera->SetView(init_view);
	camera->SetUp(init_up);
}

unsigned int OsgNavigator::GetEventReceiversCount()
{ 
	if (gl_canvas != NULL)
		return gl_canvas->GetEventReceiversCount();
	else 
		return 0;
}
bool OsgNavigator::RemoveEventReceiver(cpw::IEventReceiver *event_receiver)
{ 
	if (gl_canvas != NULL)
		return gl_canvas->RemoveEventReceiver(event_receiver);
	else 
		return false;
}


bool OsgNavigator::GetPerspective(double &fovy, double &aspect_ratio, double &_near, double &_far)
{
	if (osg_viewer == NULL)
		return false;

	osg_viewer->getCamera()->getProjectionMatrixAsPerspective(fovy, aspect_ratio, _near, _far);

	return true;
}

void OsgNavigator::GetScreenSize(int &x, int &y)
{
	if (gl_canvas != NULL)
		gl_canvas->GetClientSize(&x, &y);
	else
		x = y = 0;
}



void OsgNavigator::LookDown()
{
	if (camera != NULL)
	{
		camera->SetUp(cpw::Point3d<double>(0.0, 1.0, 0.0));
		camera->SetView(cpw::Point3d<double>(0.0, 0.0, -1.0));
	}
}

void OsgNavigator::SetIllumination(const bool &illumination)
{
	if (osg_viewer != NULL)
	{
		if (osg_viewer->getCamera() != NULL)
		{
			osg::StateSet *st = osg_viewer->getCamera()->getOrCreateStateSet();
			st->setMode(GL_LIGHTING, illumination);
			osg_viewer->getCamera()->setStateSet(st);
		}
	}
}

void OsgNavigator::Clear() {gl_canvas->Clear();}


bool OsgNavigator::IntersectMouseWithScene2(int mousex, int mousey, cpw::Point3d<float> &i_point)
{
	cpw::IScene *app_scene = cpw::ApplicationScene::GetInstance()->GetScene();

	if (app_scene == NULL)
		return false;

	cpw::Point3d<float> v0((float) camera->GetPosition().x,
								   (float) camera->GetPosition().y, 
								   (float) camera->GetPosition().z);

	cpw::Point3d<float> unp_pixel = UnProjectPixel(mousex, mousey);

	cpw::Point3d<float> v1(v0.x + unp_pixel.x * 50000000.0, 
						   v0.y + unp_pixel.y * 50000000.0, 
						   v0.z + unp_pixel.z * 50000000.0); 

	cpw::Point3d<float> ip;

	if (app_scene->IntersectRayWithTerrain(v0, v1, ip))
	{
		i_point = ip;
		return true;
	}

	return false;
}


bool OsgNavigator::IntersectMouseWithHorizontalPlane(int mousex, int mousey, cpw::Point3d<float> &i_point, const double &plane_height)
{
	cpw::Point3d<float> camera_pos((float) camera->GetPosition().x,
								   (float) camera->GetPosition().y, 
								   (float) camera->GetPosition().z);

	cpw::Point3d<float> camera_view = UnProjectPixel(mousex, mousey);

	float t(0.0f);

	//calculate where in T the ray hits de xy plane at the given height
	if (camera_view.z != 0.0f)
	{
		t = (plane_height - camera_pos.z) / camera_view.z;
		
		if (t > 0.0f)
		{
			i_point.x = camera_pos.x + camera_view.x * t;
			i_point.y = camera_pos.y + camera_view.y * t;
			i_point.z = camera_pos.z + camera_view.z * t;

			i_point = cpw::iosg::OsgScene::SceneCoordsToUTM(i_point);

			return true;
		}
	}

	return false;
}

cpw::Point3d<float> OsgNavigator::UnProjectPixel(const int &mousex, const int &mousey)
{
	osg::Matrixd view_proj_m_inv = osg_viewer->getCamera()->getViewMatrix() * osg_viewer->getCamera()->getProjectionMatrix();
	view_proj_m_inv = osg::Matrix::inverse(view_proj_m_inv);
	
	double fovy, aspect_ratio, znear, zfar;
	osg_viewer->getCamera()->getProjectionMatrixAsPerspective(fovy, aspect_ratio, znear, zfar);

	int size_x(0);
	int size_y(0);
	
	gl_canvas->GetWindowSize(size_x, size_y);

	//normalize between -1 and 1
	double norm_mouse_x = ((mousex * 2.0) / ((double)size_x)) - 1.0;
	double norm_mouse_y = ((mousey * 2.0) / ((double)size_y)) - 1.0;

	//get the ray
	osg::Vec3d p1(norm_mouse_x * znear, norm_mouse_y * znear, znear);
	osg::Vec3d p2(norm_mouse_x * zfar, norm_mouse_y * zfar, zfar);
	
	cpw::Point3d<float> p1_(norm_mouse_x * znear, norm_mouse_y * znear, znear);
	cpw::Point3d<float> p2_(norm_mouse_x * zfar, norm_mouse_y * zfar, zfar);

	//convert the ray to world coordinates
	p1 = p1 * view_proj_m_inv;
	p2 = p2 * view_proj_m_inv;

	p1 = p2 - p1;
	p1.normalize();
	
	return cpw::Point3d<float>(p1.x(), p1.y(), p1.z());
}

void OsgNavigator::SetKeepMouseInsideCanvas(const bool &keep_mouse_inside_canvas) 
{ 
	if (gl_canvas != NULL)
		gl_canvas->SetKeepMouseInsideCanvas(keep_mouse_inside_canvas); 
}

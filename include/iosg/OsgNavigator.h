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
#ifndef _OSGNAVIGATOR_
#define _OSGNAVIGATOR_

#include <osgViewer/ViewerEventHandlers>

#include <wx/wx.h>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osg/ref_ptr>
#include <osgText/FadeText>
#include <osg/Camera>

#include <iosg/OsgNavigatorWxFrame.h>
#include <iosg/OsgNavigatorWxGLCanvas.h>

#include <cpw/graphic/INavigator.h>
#include <cpw/graphic/ICameraController.h>
#include <cpw/callback/IEventCallBack.h>
#include <cpw/common/types.h>
#include <cpw/IApplication.h>

#include <iosg/OsgNavigatorManager.h>
#include <iosg/OsgCamera.h>
#include <iosg/gui/OsgCompass.h>
#include <iosg/gui/OsgLabel.h>
#include <iosg/gui/OsgUTMLabel.h>
#include <iosg/gui/OsgPanel.h>
#include <iosg/gui/OsgNavigationPanel.h>
#include <iosg/gui/OsgMinimap.h>
#include <iosg/gui/OsgVerticalButtonPanel.h>
#include <iosg/gui/OsgHUD.h>
#include <iosg/gui/OsgHUDController.h>

#include <iosg/Export.h>


namespace cpw 
{ 
	namespace iosg 
	{
		class OsgNavigatorManager;
		class OsgNavigatorWxFrame;
		class OsgNavigatorWxGLCanvas;
		class OsgINavigationPanel;
		class OsgCamera;
		class OsgIHUD;
		class OsgICompass;
		class OsgIUTMLabel;
		class OsgIMinimap;

		/** 
			\brief Por comentar ....
			\ingroup iosg
		*/
		class IOSGEXPORT OsgNavigator : public INavigator
		{
			public:
				OsgNavigator(wxWindow *parent_frame, IApplication *_application, bool lite_version_ = false);
				~OsgNavigator(void);
				void SetID(int id);
				virtual void SetDefaultPath(const std::string &url);
				void SetNavigatorManager(INavigatorManager *nm);
				int  GetId() { return nav_id; }
				void *GetViewer() { return (void *) osg_viewer; }
				void DeleteMe();
				void DrawMe();
				bool IntersectMouseWithScene(int mousex, int mousey, float &x, float &y, float &z);
				virtual bool IntersectMouseWithScene2(int mousex, int mousey, cpw::Point3d<float> &i_point);
				virtual bool IntersectMouseWithHorizontalPlane(int mousex, int mousey, cpw::Point3d<float> &i_point, const double &plane_height);
				bool IntersectMouseWithScene(int mousex, int mousey, Point3d<float> &i_point, Point3d<float> &i_normal);
				bool IntersectMouseWithNodePath(int mousex, int mousey, Point3d<float> &i_point, osg::NodePath &nodepath);
				virtual bool IntersectMouseWithEntities(int mousex, int mousey, std::string &entity_id);
				virtual bool IntersectMouseWithGizmos(int mousex, int mousey, std::string &gizmo_id);
				virtual bool IntersectMouseWithXYPlane(int mousex, int mousey, Point3d<float> &i_point);
   
				bool IntersectMouseWithHUD(int mousex, int mousey, Point3d<float> &i_point, std::string &obj_id);
				bool IntersectMouseWithHUD(int mousex, int mousey, Point3d<float> &i_point, std::string &obj_id, Point3d<float> &local_i_point);
				
				void GetCenterScreenCoords(int &coordx, int &coordy);
				void *GetFrame() { return ((void *)gl_canvas);} 
				OsgNavigatorWxGLCanvas * GetGlCanvas() { return gl_canvas; }
				void SetViewerSize(int x, int y) { osg_viewer->getCamera()->setViewport(0,0,x,y);};
				virtual void SetMouseEventCallBack(IEventCallBack *event_callback);
				virtual bool SetEventReceiver(const unsigned int &position, IEventReceiver *event_receiver);
				virtual void ClearMouseEventCallBack();
				void MakeMeActive();
				OsgCamera * GetCamera() { return camera; }
				void CreateHudCamera(osgViewer::GraphicsWindow * window);
				void Update();
				void SetDebugText(const std::string &_debug_text);
				//osgViewer::View * GetHudView() { return hudView.get(); }
				void UpdateProjectionMatrix(const int &x, const int &y);
				virtual void SetCameraController(ICameraController *_camera_controller);
				virtual ICameraController * GetCameraController(){ return camera_controller; }

				void AddEventReceiverToCanvas(IEventReceiver *event_receiver);
				virtual unsigned int GetEventReceiversCount();
				virtual bool RemoveEventReceiver(IEventReceiver *event_receiver);

				OsgIHUD * GetHUD() { return osg_hud;}

				void SetApplication(IApplication *_application);
				IApplication * GetApplication() { return application; }
				const IApplication * GetApplication() const { return application; }

				void SetHUDController(OsgIHUDController *_osg_hud_controller) { osg_hud_controller = _osg_hud_controller; }
				OsgIHUDController * GetHUDController() { return osg_hud_controller; }
				const OsgIHUDController * GetHUDController() const { return osg_hud_controller; }

				void InitGUI();

				void EnableHUDCamera() { hud_camera_active_switch->setAllChildrenOn(); }
				void DisableHUDCamera() { hud_camera_active_switch->setAllChildrenOff(); }
				void EnableHUDCameraAndDisableOthers();

				virtual void NewWindow(wxWindow *parent_frame);

				virtual void SetFocus();

				osgViewer::GraphicsWindow * GetGraphicWindow() { return window; }

				virtual void GoBackToInitialPosition();

				virtual bool GetPerspective(double &fovy, double &aspect_ratio, double &_near, double &_far);		

				virtual void GetScreenSize(int &x, int &y);

				virtual void LookDown();

				virtual void SetIllumination(const bool &illumination);
				virtual void ShowHUD(bool s) {Show_HUD(s);}

				virtual void Clear();

				virtual void SetKeepMouseInsideCanvas(const bool &keep_mouse_inside_canvas);

			private:
				
				cpw::Point3d<float> UnProjectPixel(const int &mousex, const int &mousey);

				bool lite_version;
				std::string default_path; 
				osgViewer::Viewer   *osg_viewer;
				OsgNavigatorWxFrame *frame;
				OsgNavigatorManager *navigator_manager;
				OsgNavigatorWxGLCanvas *gl_canvas;
				OsgCamera *camera;
				osg::ref_ptr<osg::Camera> hud_camera;
				osg::ref_ptr<osg::Switch> hud_camera_active_switch;
				osg::ref_ptr<osg::Node> map;
				
				double map_rotation, map_scale, map_radius, map_size;
				osg::Vec3 map_position;
				osg::ref_ptr<osg::MatrixTransform> map_mt;
				wxWindow *_parent;
				osg::ref_ptr<osgText::FadeText> debug_text;
				ICameraController *camera_controller;
				osg::ref_ptr<osg::Stats> stats;
				std::string stats_name;

				OsgIHUD *osg_hud;
				OsgIHUDController *osg_hud_controller;

				OsgICompass *osg_compass;
				OsgIMinimap *osg_minimap;
				OsgIImage *osg_logo;
				
				OsgINavigationPanel *osg_bar;
				OsgINavigationPanel *osg_barH;
				OsgIUTMLabel *osg_UTMLabel;
				IApplication *application;

				osg::ref_ptr<osg::Camera::DrawCallback> camera_predraw_callback;
				osg::ref_ptr<osg::Camera::DrawCallback> camera_postdraw_callback;

				int nav_id;

				osgViewer::GraphicsWindow *window;

				Point3d<double> init_position, init_up, init_view;

				void Show_HUD(bool s);
			
		};
	}
}


#endif

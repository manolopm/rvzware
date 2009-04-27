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

#ifndef _OSGNAVIGATORMANAGER_
#define _OSGNAVIGATORMANAGER_

#include <vector>
#include <osgViewer/CompositeViewer>
#include <osgViewer/View>
#include <osgViewer/Viewer>
#include <osgDB/DatabasePager>

#include <cpw/graphic/INavigatorManager.h>
#include <cpw/graphic/INavigator.h>
#include <cpw/entity/Entity.h>
#include <cpw/graphic/ICameraController.h>
#include <cpw/IApplication.h>

#include <iosg/OsgNavigator.h>
#include <iosg/gui/OsgHUDController.h>

//#include <../geviemer/Application.h>
//#include <../cpw::controllers/include/Application.h>

#include <iosg/Export.h>

namespace cpw 
{ 
	namespace iosg 
	{
		class OsgNavigator;
		class OsgIHUDController;
		/** 
			\brief Por comentar ....
			\ingroup iosg
			\todo Intentar quitar Application
		*/
		class IOSGEXPORT OsgNavigatorManager :	public INavigatorManager
		{
			public:
				OsgNavigatorManager(bool lite_version_ = false);
				~OsgNavigatorManager(void);
				
				IWindow *AddNavigator(IWindow *parent_frame, int id_ = -1);
				void DeleteNavigator(unsigned int navigator_id);
				void Draw();
				void SetDrawingModeWireframe(int navigator_id=0);
				void SetDrawingModeSolid(int navigator_id=0);
				virtual void SetDefaultPath(const std::string &url){
					default_path = url;
				}
				INavigator * GetFocusedOrFirstNavigator();
				//virtual void SetMouseEventCallBackOnActiveNavigator(void(*_callback_function)(int, int, int));
				virtual void ClearMouseEventCallBackOnNavigators();
				virtual void SetMouseEventCallBackOnNavigators(IEventCallBack *event_callback);

				virtual void SetActive(int id);
				void Update();

				virtual void SetApplication(IApplication *_application);
				virtual IApplication * GetApplication() { return application; }
				virtual const IApplication * GetApplication() const { return application; }

				virtual void DisableAllHUDCamerasButMine(const int &id);

				virtual void CameraParametersToNextNewNavigator(const Point3d<double> &position,
																const Point3d<double> &view,
																const Point3d<double> &up);

				virtual void Pause(bool pause);// { if (dbp != NULL) dbp->setDatabasePagerThreadPause(pause); }

				virtual iosg::OsgNavigator * NavigatorExists(const int &id);
				bool ChangeNavigatorWindow(IWindow *parent_frame, int id);
				virtual void EveryNavigatorsToStartPosition();

				virtual void SetCameraController(ICameraController *_camera_controller);
				virtual ICameraController * GetCameraController() {return camera_controller;}

				virtual void SetAllNavigatorsToInitialPosition();
				virtual void SetAllNavigatorsToLookDown();
				virtual void SetAllNavigatorsToLookPos(const Point3d<double> &position);

			virtual void SetLightningToAllNavigators(const bool &lightning);
			virtual const bool & GetNavigatorsLightning() { return navigators_lightning; }

			
		private:
			
			bool lite_version, navigators_lightning;

				void (*callback_function)(int, int, int);


				unsigned int navigator_id;	
				std::string default_path;
				int active_navigator;
				std::vector<OsgNavigator *> navigator_vector;
				osgViewer::CompositeViewer *osg_composite_viewer;
				osg::ref_ptr<osgDB::DatabasePager> dbp;

				std::vector<cpw::Entity *> animated_entities;
				bool check_entities_changes;
				std::string wms_layer_classname;

				OsgIHUDController *osg_hud_controller;
				bool hud_controller_inserted;

				int num_visible_layers;

				cpw::IApplication *application;

				cpw::ICameraController *camera_controller;

				//parameters to next new navigator
				cpw::Point3d<double> camera_pos, camera_view, camera_up;
				bool next_new_navigator_params_valid;
				int collision_framerate;
				float collision_x, collision_y, collision_z;
				cpw::Point3d<float> punto_mira;
				
		};
	}
}

#endif

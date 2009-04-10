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

#ifndef _APPLICATION_
#define _APPLICATION_


/** \mainpage Capaware Documentation 
	\section capawarelib Capaware library
	This is the documentation generated for the Capaware library.

	\section capaware Capaware
	An editting application for managing the entities of a 3D scene.
		
*/


#include "UIApplicationMainFrame.h"

#include <cpw/common/types.h>
#include <cpw/graphic/INavigatorManager.h>
#include <cpw/graphic/IGraphicFactory.h>
#include <cpw/graphic/ICameraController.h>
#include <cpw/graphic/AerialCameraController.h>
#include <cpw/graphic/TwoDimensionCameraController.h>
#include <cpw/entity/LayerTree.h>
#include <cpw/IApplication.h>

#include <ogc/WmsLayerManager.h>

#include <controllers/EntityController.h>
#include <controllers/RemoteEntityController.h>
#include <controllers/KeyboardControl.h>
#include <controllers/ElementController.h>
#include <controllers/LayerController.h>
#include <controllers/FireController.h>
#include <controllers/ElementVectorialController.h>
#include <controllers/ServerConnectionController.h>
#include <controllers/VideoController.h>
#include <controllers/RemoteController.h>
#include <controllers/StatusController.h>
#include <controllers/SunController.h>
#include <controllers/DistanceController.h>
#include <controllers/HandlerController.h>
#include <controllers/ProjectController.h>
#include <gui/UISplitterCanvas.h>



#include "ApplicationTime.h"
#include "DynamicLibrary.h"



namespace cpw{ namespace gui { class UIApplicationMainFrame; }}

/** 
	\brief Geviemer application
*/
class Application :	public wxApp, public cpw::IApplication
{
	public:

		Application(void);

		~Application(void);

		virtual bool OnInit();

		/** \todo Guardar configuración de las ventanas al salir */
		bool Close();
		void OpenProject();
		bool CloseProject();

		//redefinitions of IApplication
		virtual std::string GetDefaultPath();
		virtual void NewElement();
		virtual void NewFire();
		virtual void NewWmsLayer();
		virtual unsigned char GetWmsBlendingFactor();
		virtual unsigned char GetFireViewMode();


		cpw::IWindow *AddNavigator(wxWindow *parent, int id);
		void DeleteNavigator(unsigned int nav_id);
		void Update();
		void Draw();
		void DrawModeWireframe(wxWindow *parent);
		void DrawModeSolid(wxWindow *parent);
		void ViewSelection(cpw::gui::UIApplicationMainFrame *parent);
		void NewElementPrimitive(wxWindow *parent);
		//void NewFirePrimitive(wxWindow *parent);
		
		void NewLayer(wxWindow *parent);
		void NewTable(wxWindow *parent);
		void NewFirewall(wxWindow *parent);
		void NewRemoteEntity(wxWindow *parent);
		void AddEntity(wxWindow *parent);
		void AddRemoteEntity(wxWindow *parent);
		void AddDataBase(wxWindow *parent);
		void NewContainerLayerPrimitive(wxWindow *parent);
		void NewRasterLayerPrimitive(wxWindow *parent);
		void NewVectorialLayerPrimitive(wxWindow *parent);
		void SetActiveNavigator(int id);
		void PressKey(int key);
		void ReleaseKey(int key);
		void MouseLeftDown();
		void MouseLeftUp();
		void MouseRightDown();
		void MouseRightUp();
		void MouseMiddleDown();
		void MouseMiddleUp();
		void Set2DNavigationMode();
		void Set3DNavigationMode();
		void SetTranslateHandler();
		void SetRotateHandler();
		void SetScaleHandler();

		void SwitchFullScreen();
		void FullScreenOn();
		void FullScreenOff();

		void ExecutePlugin(const int &id);

		void NotAvailable();
		void AnimateEntity();

		void TestDependencies(const std::string &class_name);
		void TestDependencies(const cpw::Entity *entity);


		void ModifyProperties(const cpw::TypeId &id, wxWindow* parent);
		void SendFront(const cpw::TypeId &id);
		void SendBack(const cpw::TypeId &id);
		void Visualize(const cpw::TypeId &id, const bool &value);
		void SetSunPosition(const float &pos);
		void EnableSunConfiguration(const bool &value);

		void UpdateTimeSettings();
		void UpdateAnimableUI();

		void GoTo(const cpw::TypeId &id);

		void Copy(const cpw::TypeId &id);
		void Duplicate(const cpw::TypeId &id);
		void Cut(const cpw::TypeId &id);
		void Paste(const cpw::TypeId &id_orig, const cpw::TypeId &id_dest, const cpw::TypeId &parent_id_orig, const cpw::TypeId &parent_id_dest, const bool &cut, const bool &copy, const bool &duplicate);
		void Application::SaveAll();
		void Application::Save();


		/** \todo Al borrar entidad preguntar si realmente quiere borrarla */
		void Delete(const cpw::TypeId &ent_id, const cpw::TypeId &parent_id);

		cpw::controllers::StatusController *GetStatusController() {return status_controller;}

		wxWindow * GetWXMainFrame() { return (wxWindow *) main_frame; }

		

		void AddLine();
		void CalculateDistance(wxWindow *parent);

		void StopTimeAndControls();

		bool IsControllerOpen();

	
	private:

		bool plugin_running;
		
		void EntitiesToRegisterFromPlugins();
		bool InitCpwEntities();
		void RegisterAllPlugins();
		void InitHandlers();
		void DeleteControllers();
		cpw::gui::UIApplicationMainFrame *main_frame;
		cpw::controllers::ElementController *ec; 	
		cpw::controllers::ElementController *ecm;
        cpw::controllers::FireController *fc; 	
        cpw::controllers::FireController *fcm; 	
        cpw::controllers::ElementVectorialController *evc; 
		cpw::controllers::ElementVectorialController *evcm; 
		cpw::controllers::StatusController *status_controller;
		cpw::controllers::HandlerController handler_controller;
		cpw::controllers::ProjectController *project_controller;
		cpw::controllers::RemoteEntityController *remote_entity_controller;

		std::string traslation_x_gizmo_id, traslation_y_gizmo_id, traslation_z_gizmo_id,
					rotate_x_gizmo_id, rotate_y_gizmo_id, rotate_z_gizmo_id,
					scale_x_gizmo_id, scale_y_gizmo_id, scale_z_gizmo_id, two_axis_gizmo_id;

	
					
		cpw::controllers::DistanceController *dc;
		cpw::controllers::RemoteController *remote_controller;

		cpw::INavigatorManager  *navigator_manager;
		cpw::IGraphicFactory    *graphic_factory;

		cpw::LayerTree             layer_tree;
		ApplicationTime            *application_time;
		cpw::controllers::VideoController video_controller;
		cpw::controllers::SunController *sun_controller;

		std::map<int, DynamicLibrary*> plugin_map;

		cpw::ogc::WmsLayerManager *wms_request_manager;

		cpw::ICameraController *three_dimension_camera_controller;
		cpw::ICameraController *two_dimension_camera_controller;

		std::string srs;

		

};

#endif

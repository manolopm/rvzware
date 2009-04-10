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
#ifndef _OSGIHUDCONTROLLER_
#define _OSGIHUDCONTROLLER_

#include <map>

#include <cpw/callback/IEventReceiver.h>
#include <cpw/graphic/INavigatorManager.h>
#include <cpw/IApplication.h>

#include <iosg/gui/OsgWidget.h>
#include <iosg/gui/OsgHUD.h>
#include <iosg/gui/OsgMinimap.h>
#include <iosg/gui/OsgCompass.h>
#include <iosg/gui/OsgNavigationPanel.h>


#include <iosg/Export.h>


namespace cpw 
{ 
	namespace iosg 
	{

		/** 
			\todo Intentar quitar el Application de aqui 
		*/
		class TFunctor
		{
			public:
				virtual void Call()=0;
		};


		/** 
			\brief 
			\ingroup iosg
		*/
		template <class TClass> 
		class TSpecificFunctor : public TFunctor
		{
			public:
			
				// takes pointer to an object and pointer to a member and stores them in two private variables
				TSpecificFunctor(TClass* _pt2Object, void(TClass::*_fpt)()) { pt2Object = _pt2Object;  fpt=_fpt; }

				virtual void Call() { (*pt2Object.*fpt)(); }  // execute member function

			private:
			
				void (TClass::*fpt)();   // pointer to member function
				TClass* pt2Object;       // pointer to object
		};
		
		class OsgIHUD;
		class OsgIMinimap;
		class OsgICompass;
		class OsgINavigationPanel;

		/** 
			\brief 
			\ingroup iosg
		*/
		class IOSGEXPORT OsgIHUDController : public cpw::IEventReceiver
		{
			public:
				
				OsgIHUDController(void);
				~OsgIHUDController(void);

				virtual bool MouseLeftDoubleClick(const int &x, const int &y);
				virtual bool MouseRightDoubleClick(const int &x,const int &y);
				virtual bool MouseMiddleDoubleClick(const int &x,const int &y);
				
				virtual bool MouseLeftButtonDown(const int &x,const int &y);
				virtual bool MouseRightButtonDown(const int &x,const int &y);
				virtual bool MouseMiddleButtonDown(const int &x,const int &y);

				virtual bool MouseLeftButtonUp(const int &x,const int &y);
				virtual bool MouseRightButtonUp(const int &x,const int &y);
				virtual bool MouseMiddleButtonUp(const int &x,const int &y);

				virtual bool MouseDrag(const int &x0, const int &y0, const int &x1, const int &y1);
				virtual bool MouseWheel(const int &wheel_rotation, const int &wheel_delta);

				virtual bool MouseLeaveWindow(const int &x0, const int &y0);
				virtual bool MouseEnterWindow(const int &x0, const int &y0);
						
				virtual bool KeyDown(const int &key);
				virtual bool KeyUp(const int &key);

				virtual bool HasFocus() { return false; }

				void SetNavigatorManager(cpw::INavigatorManager *_navigator_manager) { navigator_manager = _navigator_manager; }
				cpw::INavigatorManager * GetNavigatorManager() { return navigator_manager; }
				void SetNavigator(OsgNavigator *_osg_navigator) { osg_navigator = _osg_navigator; }

				void SetHUD(OsgIHUD *_osg_hud) { osg_hud = _osg_hud; }
				OsgIHUD * GetHUD() { return osg_hud; }

				void OpenFireDialog();
				void OpenWMSDialog();
				void OpenElement3dDialog();
				void ToggleMinimap();
				void ToggleNavigationPanel();
				void ToggleCompass();
				

				void AddCallBack(const std::string &callback_id, void(OsgIHUDController::*fpt)());

				virtual void SetApplication(cpw::IApplication *_application) { application = _application; }
				virtual cpw::IApplication * GetApplication() { return application; }
				virtual const cpw::IApplication * GetApplication() const { return application; }

				void SetMinimap(OsgIMinimap *_mini_map); 
				void SetCompass(OsgICompass *_compass); 
				void SetNavigationPanel(OsgINavigationPanel *_navigation_bar); 

			private:

				cpw::INavigatorManager *navigator_manager;
				OsgNavigator *osg_navigator; 
				OsgIWidget *active_widget;
				OsgIHUD *osg_hud;
				std::map<std::string, TFunctor *> callbacks;

				std::string fire_button_id; 

				cpw::IApplication *application;

				bool minimap_active;

				OsgIMinimap *mini_map;
				OsgICompass *compass;
				OsgINavigationPanel *navigation_bar;

		};
	}
}

#endif
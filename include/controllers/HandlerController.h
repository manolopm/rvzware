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
#ifndef _HANDLERCONTROLLER_
#define _HANDLERCONTROLLER_

#include <string>
#include <map>

#include <cpw/callback/IEventReceiver.h>
#include <cpw/graphic/IHandler.h>
#include <cpw/graphic/IScene.h>
#include <cpw/graphic/INavigatorManager.h>


namespace cpw
{

	namespace controllers 
	{  

		/** 
			\brief This controller manages the different handlers of the entities
			\ingroup cwcontent
		*/
		class HandlerController : public cpw::IEventReceiver
		{
			public:

				enum HandlerType { TRASLATION, ROTATION, SCALE };

				HandlerController(void);
				~HandlerController(void);

				virtual bool MouseLeftButtonDown(const int &x,const int &y);
				virtual bool MouseDrag(const int &x0, const int &y0, const int &x1, const int &y1, const cpw::MouseButtonsState &mbs);
				virtual bool MouseLeftButtonUp(const int &x,const int &y);
				virtual bool MouseLeftDoubleClick(const int &x, const int &y);
				virtual bool MouseRightButtonDown(const int &x, const int &y);

				bool SetActiveHandler(const std::string &handler_id);
				void SetActiveEntity(const std::string &_active_entity);
				void Deactivate() { handler_active = entity_active = false; active_entity = std::string(""); /*active_handler = std::string("");*/}

				std::string & GetActiveEntity() { return active_entity; }
				
				void ShowDefaultGizmoOnActiveEntity();
				bool InsertHandler(cpw::IHandler *new_handler);
				void SetDefaultHandler(cpw::IHandler *_default_handler) { default_handler = _default_handler; }
				void SetScene(cpw::IScene *_scene) { scene = _scene; }
				void SetNavigatorManager(cpw::INavigatorManager *_navigator_manager) { navigator_manager = _navigator_manager; }

				void SetAllowUnderTerrainToAllHandles(const bool &value, const bool &update_entity=false);
				void SetAdaptPositionToTerrainHeightToAllHandles(const bool &value, const bool &update_entity=false);
				void ClearActiveEntity();
				void Update();
				void UpdateCamera();

				void ShowActiveHandler(bool value);
				void ActivateAllHandlers(const bool active);

			private:

				bool MakeHandlerAndBrothersVisible(const std::string &handler_id, const bool &visible);
				bool UpdateEntityAndBrothersCameraPosition();
				bool UpdateActiveHandlerAndBrothersTarget();
				bool AdaptActiveHandlerAndBrothersToActiveEntity();
				void HideAllHandlers();
				void MakeNextHandlerGroupActive();

				std::string active_entity, active_handler;
				bool entity_active, handler_active;

				cpw::IScene *scene;
				cpw::INavigatorManager *navigator_manager ;

				std::map<std::string, cpw::IHandler *> handlers;
				cpw::IHandler *default_handler;
		};
	}

}

#endif



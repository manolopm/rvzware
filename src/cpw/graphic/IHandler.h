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
#ifndef _HANDLER_
#define _HANDLER_

#include <string>
#include <vector>
#include <cpw/Export.h>
#include <cpw/callback/IEventReceiver.h>
#include <cpw/common/types.h>
#include <cpw/common/Observer.h>
#include <cpw/graphic/INavigatorManager.h>
#include <cpw/entity/Entity.h>

namespace cpw
{
	/** 
		\brief Por comentar ....
		\ingroup cpw
	*/
	class CPWEXPORT IHandler : public IEventReceiver, public Observer
	{
		public:

			IHandler();
			virtual ~IHandler(void);

			void SetId(const std::string &_id) { id = _id; }
			const std::string & GetId() { return id; }

			std::vector< IHandler *> & GetBrothers() { return brothers; }
			void AddBrother(IHandler *new_brother) { brothers.push_back(new_brother); }

			void SetCameraPosition(const cpw::Point3d<double> _camera_pos) { camera_pos = _camera_pos; }
			void SetCameraView(const cpw::Point3d<double> _camera_view) { camera_view = _camera_view; }

			const cpw::Point3d<double> & GetCameraPos() { return camera_pos; }
			const cpw::Point3d<double> & GetCameraView() { return camera_view; }

			void SetEntityTarget(cpw::Entity *_entity_target);
			cpw::Entity * GetEntityTarget() { return entity_target; }

			void ClearEntityTarget();

			void AdaptHandlerAndBrothersToModel(const bool &adapt_rotation = false);
			void AdaptHandlerAndBrothersToModel(const bool &adapt_rotation, const cpw::Point3d<float> &rotation);
			void ShowBrothers(const bool &visible = true);

			virtual void Update(bool subject_deleted = false){};
			virtual void Reset(){}

			void SetAllowGoingUnderTerrain(const bool &_allow_going_under_terrain) { allow_going_under_terrain = _allow_going_under_terrain; } 
			const bool & GetAllowGoingUnderTerrain() { return allow_going_under_terrain; }

			void SetAdaptPositionToTerrainHeight(const bool &_adapt_position_to_terrain_height) { adapt_position_to_terrain_height = _adapt_position_to_terrain_height; }
			const bool & GetAdaptPositionToTerrainHeight() { return adapt_position_to_terrain_height; }

			void SetNavigatorManager(cpw::INavigatorManager *_navigator_manager) { navigator_manager = _navigator_manager; }
			cpw::INavigatorManager * GetNavigatorManager() { return navigator_manager; }

			void SetActive(const bool &_active) { active = _active; }
			const bool & GetActive() { return active; }

		private:
			
			bool allow_going_under_terrain, adapt_position_to_terrain_height;
			std::vector< IHandler *> brothers;
			std::string id;
			cpw::Entity *entity_target;
			cpw::INavigatorManager *navigator_manager;
			cpw::Point3d<double> camera_pos, camera_view;
			bool active;

	};
}

#endif

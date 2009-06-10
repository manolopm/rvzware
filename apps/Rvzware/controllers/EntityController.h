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

#ifndef _ENTITYCONTROLLER_
#define _ENTITYCONTROLLER_

#include <wx/wx.h>
#include <string>

#include <cpw/common/types.h>
#include <cpw/entity/LayerTree.h>
#include <cpw/common/IStatusController.h>


namespace cpw
{

	namespace controllers 
	{  

		/** 
			\brief This controller is responsible for the management of general Entity (layers and elements)
			\ingroup cpw
		*/
		class EntityController
		{
		public:
			EntityController(void);
			EntityController(cpw::IStatusController *istatus_controller);
			~EntityController(void);

			cpw::TypeId GetIdFromFile(const std::string &url, cpw::Node **root);

			cpw::Entity* Load(const std::string &url, bool all=true, cpw::Entity *parent = NULL);
			cpw::Entity* Load2(const std::string &url, bool all, cpw::Entity *parent);

			void InitLayerTree(const std::string &scene_file, cpw::LayerTree &layer_tree);


			/** 
				\brief Loads an entity from an url
			*/
			cpw::Entity* Load(wxWindow* parent, cpw::LayerTree &layer_tree);

			/**
				\brief Saves recursively the entities without error handling
			*/
			cpw::PersistentError Save(cpw::Entity *entity, bool all = false, bool overwrite = false); //Put all = true to save all the tree


			bool AddEntity(wxWindow* parent, cpw::LayerTree &layer_tree);

		protected:


			void Register(cpw::Entity *entity);

		
		private:

			/** 
				\brief Registers a prototype from an entity
			*/
			void RegisterPrimitive(const std::string &primitive_url);

			cpw::IStatusController *isc;
			int traceable_id;

		
		};
	}

}
#endif






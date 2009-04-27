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

#ifndef _ENTITYREGISTRY_
#define _ENTITYREGISTRY_

#include <map>
#include <vector>
#include <string>

#include <cpw/entity/Entity.h>
#include <cpw/Export.h>

namespace cpw {


	/** 
		\brief Por comentar ....
		\ingroup cpw
	*/
	class CPWEXPORT EntityRegistry
	{

		typedef std::map<TypeId, Entity* > container;
	
	public:

		static EntityRegistry *GetInstance();

		static void ReleaseInstance();	

		void Add(Entity *entity);

		void Remove(const cpw::TypeId id);

		void DeleteEntity(const TypeId id);

		void Clear();
		
		Entity* GetEntity(const TypeId id);

		std::vector<Entity*> GetEntities(const std::string &class_name);

		Entity* GetEntityFromUrl(const std::string &entity_url);

		std::vector<Entity*> GetAnimatedEntities();

		std::vector<Entity*> GetPublishedEntities();

		std::vector<Entity*> GetAllEntities();

	private:

		EntityRegistry();
		
		~EntityRegistry();

		static EntityRegistry *instance;

		std::map<TypeId, Entity* > entities;

	};
}

#endif 

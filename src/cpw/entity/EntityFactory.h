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

#ifndef _ENTITYFACTORY_
#define _ENTITYFACTORY_

#include <string>
#include <map>

#include <cpw/entity/Entity.h>
#include <cpw/Export.h>

namespace cpw
{

	/** 
		\brief Por comentar ....
		\ingroup cpw
	*/
	class CPWEXPORT EntityFactory
	{
		public:

			static EntityFactory *GetInstance();

			static void ReleaseInstance();	

			cpw::Entity *CreateEntity(const std::string &type);

			cpw::Entity *CreateEntity(const cpw::TypeId &type);

			void RegisterBasic(Entity* entity);

			bool Register(Entity* entity);

			std::map<cpw::TypeId, cpw::Entity *> &GetPrototypes();

			std::map<cpw::TypeId, cpw::Entity *> GetPrototypes(const std::string &cn);

			cpw::Entity * GetBasicPrototype(const std::string &cn);

			cpw::Entity *GetPrototype(const cpw::TypeId &id);

			cpw::Entity *GetPrototypeFromUrl(const std::string &url);

			unsigned long int &GetEntityNumber();


		protected:

			EntityFactory();

			~EntityFactory();


		private:

			//capaware prototypes
			std::map<std::string, cpw::Entity *>  entity_prototypes_cn;

			//application prototypes
			std::map<cpw::TypeId, cpw::Entity *>  entity_prototypes_id;

			static EntityFactory *instance;

			unsigned long int entity_number;

	};
}



#endif

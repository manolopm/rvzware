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
#ifndef _LAYERTREE_
#define _LAYERTREE_

#include <cpw/entity/Entity.h>
#include <cpw/common/Subject.h>
#include <cpw/Export.h>

#include <string>

namespace cpw
{

	/** 
		\brief Por comentar ....
		\ingroup cpw
	*/
	class CPWEXPORT LayerTree : public Subject
	{
		public:
			LayerTree(): active(), active_parent(), top_layer(), last_added_entity(){}
			
			LayerTree(Entity* toplayer);
			
			virtual ~LayerTree();

			bool ActiveLayerIsContainer() { return active->isContainer(); }
			
			bool AddToActiveLayer(Entity *ent, const bool &add_parent = true);
			
			Entity *GetTopLayer() { return top_layer; }

			cpw::Entity *GetActiveParent() { return active_parent; }

			cpw::Entity *GetLastContainer() { if(active->isContainer()) return active; else return active_parent; }
			
			const Entity *GetTopLayer() const { return top_layer; }
			

			//cpw::Entity * MakeActive(const std::string &url);
			//void MakeParentActive(const std::string &url);

			cpw::Entity * MakeActive(const cpw::TypeId &id);
			void MakeParentActive(const cpw::TypeId &id);

			void ChangeLastAddedEntityToTop() {last_added_entity = top_layer; }

			//cpw::Entity * GetEntity(std::string url);

			cpw::Entity * GetEntity(const cpw::TypeId &id);

			Entity *GetLastEntity() { return last_added_entity; }

			bool isModified();

			void GetAnimatedEntitiesFromTopLayer(std::vector<cpw::Entity *> &entities); //from top layer

			void Clear();
		

		private:
	
			void CopyLayerToTop(Entity *layer);
			
			void GetAnimatedEntities(std::vector<cpw::Entity *> &entities, cpw::Entity *entity);
			
			//void SearchAndMakeActive(Entity *entity, const std::string &url, bool &entity_found, Entity **selected, bool parent);

			void SearchAndMakeActive(Entity *entity, const cpw::TypeId &id, bool &entity_found, Entity **selected, bool parent);

			bool TestModified(Entity *entity);

			Entity *active;
			Entity *active_parent;
			Entity *top_layer;
			Entity *last_added_entity;


	};
}

#endif 

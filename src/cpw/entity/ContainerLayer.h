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
#ifndef _CONTAINERLAYER_
#define _CONTAINERLAYER_

#include <cpw/entity/Layer.h>
#include <cpw/common/Attribute.h>
#include <cpw/Export.h>

#include <vector>

namespace cpw
{
	/** 
		\brief 
		\ingroup cpw
	*/
	class CPWEXPORT ContainerLayer : public Layer
	{
		public:

			ContainerLayer(const cpw::TypeId id = cpw::TypeId(), 
					       const std::string &class_name = std::string("ContainerLayer"));
			
			ContainerLayer(const cpw::TypeId id, const std::string &nam, 
						   const std::string &prim, const std::string &htm,	
						   const bool dyn, const bool vis, const bool anim, const std::string &desc,
						   const std::vector<Attribute> &attr, 
						   const std::string &class_name = std::string("ContainerLayer"));

			~ContainerLayer(void);
			
			ContainerLayer(const ContainerLayer &clayer);
			
			ContainerLayer &operator = (const ContainerLayer &clayer);

			virtual int Add(Entity *entity);

			virtual int DeleteChild(Entity *entity);

			virtual int DeleteAllChild();

			void DeleteReferenceToMe();

			Entity *GetChild(unsigned int i);

			int GetNumChildren();

			const Entity *GetChild(unsigned int i) const ;

			const int GetNumChildren() const ;

			virtual void SetValue(const std::string &f, const std::string &v);

			virtual bool isContainer();
			virtual const bool isContainer() const ;


			virtual Entity* Clone();

			virtual void SetVisible(const bool vis);
			virtual void SetPublished(const bool pub);
			virtual void SetAnimate(const bool anim);

		protected:

			virtual int CreatePersistence();
			virtual int AdaptPersistence(Node *root);

		private:
			std::vector<Entity *> entity_vector;
	};
}
#endif

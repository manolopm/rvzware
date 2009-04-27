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

#ifndef _LAYER_
#define _LAYER_

#include <string>
#include <vector>

#include <cpw/common/Attribute.h>
#include <cpw/entity/Entity.h>
#include <cpw/Export.h>


namespace cpw {


	/** 
		\brief Por comentar ....
		\ingroup cpw
	*/
	class CPWEXPORT Layer: public Entity
	{

	public:
		Layer(const cpw::TypeId id = cpw::TypeId(), 
			  const std::string &class_name = std::string("Layer"));

		Layer(const cpw::TypeId id, const std::string &nam, 
			  const std::string &prim, const std::string &htm,	
			  const bool dyn, const bool vis, const bool anim, const std::string &desc, 
			  const std::vector<Attribute> &attr, 
			  const std::string &class_name = std::string("Layer"));


		Layer(const Layer &layer);

		virtual ~Layer();

		Layer &operator = (const Layer &layer);

		virtual int Add(Entity *entity) { return 0; }
		virtual Entity *GetChild(unsigned int i) { return NULL; }
		virtual int GetNumChildren() { return 0; }
		
		virtual Entity* Clone() {return new Layer(*this); };

		virtual void SetValue(const std::string &f, const std::string &v);

		virtual bool isLayer() { return true;}
		virtual const bool isLayer() const { return false;}

		virtual void SetVisible(const bool vis) { Entity::SetVisible(vis);}
		virtual void SetPublished(const bool pub) { Entity::SetPublished(pub);}
		virtual void SetAnimate(const bool anim) { Entity::SetAnimate(anim);}

	protected:

		virtual int CreatePersistence();
		virtual int AdaptPersistence(Node *root);

	private:

		

	};

}


#endif 

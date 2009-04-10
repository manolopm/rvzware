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
#ifndef _ENTITY_
#define _ENTITY_

#include <string>
#include <vector>
#include <set>

#include <cpw/callback/Callable.h>
#include <cpw/common/Attribute.h>
#include <cpw/common/Remote.h>
#include <cpw/Export.h>

   
namespace cpw {

	/** 
		\brief Basic class to manage the entities that may be represented int he LayerTree
		\ingroup cpw
    */
	class CPWEXPORT Entity: public Remote, public Callable 
	{

	public:
		Entity(const cpw::TypeId id = cpw::TypeId(), const std::string &class_name = std::string("Entity"),
			const std::string &name = "", const std::string &desc = "");

		Entity(const cpw::TypeId id, const std::string &nam, const std::string &prim, 
			   const std::string &htm, const bool dyn, const bool vis, const bool anim, const std::string &desc, 
			   const std::vector<Attribute> &attr, const std::string &icon_str,
			   const std::string &class_nam = std::string("Entity"));

		Entity(const Entity &entity);
		virtual ~Entity();


		std::string &GetName();
		std::string &GetDescription();
		std::string GetIcon();
		std::string GetPrimitiveUrl() {return ComposeDirectory(primitive_url);}
		std::string GetHtml();
		std::string &GetFont() { return font; }
		Entity* GetLastParent() const;
		std::vector<Attribute> &GetAttributes() { return attributes; }

		bool IsDynamic() { return dynamic; }

		const std::string &GetName()  const;
		const std::string &GetDescription() const;
		const std::string GetIcon() const {return ComposeDirectory(icon);}
		const std::string GetPrimitiveUrl() const {return ComposeDirectory(primitive_url);}
		const std::string GetHtml() const ;
		const std::string &GetFont() const { return font; }
		const bool IsDynamic() const { return dynamic; }
		const std::vector<Attribute> &GetAttributes() const { return attributes; }

		/** Sets the name of the entity */
		void SetName(const std::string  &str);

		void SetDescription(const std::string &str);
		void SetIcon(const std::string &icon_str);
		void SetPrimitiveUrl(const std::string &str);
		void SetHtml(const std::string &str);
		void SetFont(const std::string &str);
		void SetDynamic(const bool dyn);
		virtual void SetVisible(const bool vis);
		virtual void SetAnimate(const bool anim);
		virtual void SetPublished(const bool publish);
		void SetAttributes(const std::vector<Attribute> &attr);	
		void SetEndTime(const cpw::cpwTime &et) ;
		void SetStartTime(const cpw::cpwTime &st) ;
	
		virtual void SetValue(const std::string &f, const std::string &v);

		void AddAttribute(const Attribute &attr) ;

		void AddParent(Entity* parent) {	
			if(parent != NULL)
				parents.push_back(parent); 
		}

		void DeleteParent(Entity* parent);
		void ClearAllParents() { parents.clear(); }
		int GetNumParents() { return (int) parents.size(); }
		Entity* GetParent(int i = 0);

		Entity &operator = (const Entity &entity);

		//member functions for the composite pattern
		virtual int Add(Entity *entity) { return 0; }
		virtual Entity *GetChild(unsigned int i) { return NULL; }
		virtual int GetNumChildren() { return 0; }
		virtual int DeleteChild(Entity *entity) {
			return 0;
		}

		virtual const Entity *GetChild(unsigned int i) const { return NULL; }
		virtual const int GetNumChildren() const { return 0; }
		const Entity* GetParent(int i = 0) const;
		const int GetNumParents() const { return (int) parents.size(); }

		virtual bool isContainer() {return false;}
		virtual bool isLayer() { return false;}
		virtual bool isVisible() { return visible;}
		virtual bool isAnimate() { return animate;}
		virtual bool isPublished() { return published;}

		virtual const bool isContainer() const {return false;}
		virtual const bool isLayer() const { return false;}
		virtual const bool isVisible() const { return visible;}
		virtual const bool isAnimate() const { return animate;}
		virtual const bool isPublished() const { return published;}


		virtual Entity* Clone() {return NULL;}

		cpw::cpwTime &GetEndTime() { return end_time; }
		cpw::cpwTime &GetStartTime() { return start_time; }

		virtual void SetIconVisibility( const bool &vis) {icon_vis = vis;}
		virtual bool GetIconVisibility() {return icon_vis;}
		virtual const bool GetIconVisibility() const {return icon_vis;}
		virtual const bool IsBeingEdited() { return being_edited; }
		virtual const bool IsBeingEdited() const { return being_edited; }
		virtual void SetBeingEdited(const bool &_being_edited) { being_edited = _being_edited; }


	protected:

		virtual int CreatePersistence();
		virtual int CreateProperties();
		virtual int AdaptPersistence(Node *root);


	private:

		std::string primitive_url;
		std::string name;
		std::string description; 
		std::string html;
		std::string font;
		std::string icon;
		
		bool dynamic;
		bool visible;
		bool animate;
		bool published;
		bool icon_vis;
		bool being_edited;

		std::vector<Entity*> parents;

		std::vector<Attribute> attributes;

		cpw::cpwTime end_time, start_time;

	};

}


#endif 

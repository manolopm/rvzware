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
#include <sstream>

//#include <stdafx.h>


#include <cpw/common/Remote.h>
#include <cpw/entity/Entity.h>


using namespace cpw;


Entity::Entity(const cpw::TypeId id, const std::string &class_nam, const std::string &nam, const std::string &des) : 
									Remote(id, class_nam), Callable(), name(nam), description(des), 
									primitive_url(), html(), font(), dynamic(), visible(true), attributes(), icon(),
									animate(true), published(), icon_vis(true), being_edited(false)

{
}


Entity::Entity(const cpw::TypeId id, const std::string &nam, const std::string &prim, 
			   const std::string &htm, const bool dyn, const bool vis, const bool anim, const std::string &desc, 
			   const std::vector<Attribute> &attr, const std::string &ico, const std::string &class_nam ): 
					Remote(id, class_nam), Callable(), name(nam), description(desc),
					primitive_url(prim), html(htm), font(), dynamic(dyn), visible(vis), icon(ico),
					attributes(attr), animate(anim), published(), icon_vis(true), being_edited(false)
{

	Modified();
	
}



Entity::Entity(const Entity &entity): Remote(entity), Callable(entity), 
	name(entity.name), description(entity.description), primitive_url(ComposeDirectory(entity.primitive_url)), 
	html(ComposeDirectory(entity.html)), font(entity.font), dynamic(entity.dynamic), visible(entity.visible), parents(entity.parents),
	attributes(entity.attributes), icon(ComposeDirectory(entity.icon)), animate(entity.animate), published(entity.published), icon_vis(true), being_edited(false)
{
	
	Modified();
}



Entity::~Entity()
{
}

void Entity::SetName(const std::string  &str)  
{ 
	name  = str; 
	Modified("name", name); 
}

void Entity::SetDescription(const std::string &str) 
{ 
	description = str; 
	Modified("description", description); 
}

void Entity::SetIcon(const std::string &icon_str) 
{
	icon = icon_str; 
	Modified("icon", icon);
}

void Entity::SetPrimitiveUrl(const std::string &str) 
{ 
	primitive_url = str; 
	Modified("primitive_url", primitive_url);
}

void Entity::SetHtml(const std::string &str) 
{ 
	html = str; 
	Modified("html", html);
}

void Entity::SetFont(const std::string &str) 
{ 
	font = str; 
	Modified("font", font);
}

void Entity::SetDynamic(const bool dyn) 
{ 
	dynamic = dyn; 
	
	Modified("dynamic", dyn);
}

void Entity::SetVisible(const bool vis) 
{ 
	visible = vis; 
	Visualize(vis);
	Modified("visible", visible);
}

void Entity::SetAnimate(const bool anim) 
{
	animate = anim; 

	Modified("animate", animate);
}

void Entity::SetPublished(const bool publish) 
{
	published = publish; 
	
	Modified("published", published);
}


void Entity::SetAttributes(const std::vector<Attribute> &attr) 
{ 
	attributes = attr; 
	Modified("attributes", "");
}

void Entity::SetEndTime(const cpw::cpwTime &et) 
{ 
	end_time = et;

	Modified("end_time", end_time.seconds, end_time.miliseconds);
}

void Entity::SetStartTime(const cpw::cpwTime &st) 
{ 
	start_time = st;

	Modified("start_time", start_time.seconds, start_time.miliseconds);
}


void Entity::SetValue(const std::string &f, const std::string &v)
{
	if(f == "primitive_url") 
		SetPrimitiveUrl(v); 
	else if (f == "name")
		SetName(v); 
	else if(f == "description")
		SetDescription(v); 
	else if(f == "html")
		SetHtml(v); 
	else if(f == "font")
		SetFont(v); 
	else if(f == "icon")
		SetIcon(v);
	else if(f == "dynamic")
	{
		bool r;
		std::istringstream str(v);
		str >> r;
		SetDynamic(r);
	}
	else if(f == "visible")
	{
		bool r;
		std::istringstream str(v);
		str >> r;
		SetVisible(r);
	}
	else if(f == "animate")
	{
		bool r;
		std::istringstream str(v);
		str >> r;
		SetAnimate(r);
	}
	else if(f == "published")
	{
		bool r;
		std::istringstream str(v);
		str >> r;
		SetPublished(r);
	}
	else if(f == "start_time")
	{
		cpwTime r;
		std::istringstream str(v);
		str >> r.seconds >> r.miliseconds;
		SetStartTime(r);
	}
	else if(f == "end_time")
	{
		cpwTime r;
		std::istringstream str(v);
		str >> r.seconds >> r.miliseconds;
		SetEndTime(r);
	}
	else if(f == "AddAttribute")
	{
		Attribute r;
		std::istringstream str(v);
		str >> r.type >> r.name >> r.value;
		AddAttribute(r);
	}
	else
		Remote::SetValue(f, v);

}



void Entity::AddAttribute(const Attribute &attr) 
{ 
	attributes.push_back(attr); 
	std::ostringstream str;
	str << attr.type << " " << attr.name << " " << attr.value;
	Modified("AddAttribute", str.str());
}



std::string &Entity::GetName()  
{ 
	return name;  
}

std::string &Entity::GetDescription() 
{ 
	return description; 
}

std::string Entity::GetHtml()
{ 
	if(html!="") 
		return ComposeDirectory(html); 
	else 
		return html;
}


std::string Entity::GetIcon()
{
	return ComposeDirectory(icon);
}

Entity* Entity::GetParent(int i) { 
	
	if(i < (int)parents.size())

		return parents[i];

	else 
	
		return NULL;
}

const std::string &Entity::GetName()  const 
{ 
	return name;  
}

const std::string &Entity::GetDescription() const 
{ 
	return description; 
}

const std::string Entity::GetHtml() const 
{ 
	if(html!="") 
		return ComposeDirectory(html); 
	else 
		return html;
}


const Entity* Entity::GetParent(int i) const { 

	if(i < (int)parents.size())

		return parents[i];

	else 
	
		return NULL;
}


Entity &Entity::operator = (const Entity &entity) {

	Remote::operator     = (entity);
	
	name           = entity.name;
	description    = entity.description;
	primitive_url  = entity.GetPrimitiveUrl();
	html           = entity.GetHtml();
	font		   = entity.font;
	dynamic        = entity.dynamic;
	visible        = entity.visible;
	animate		   = entity.animate;
	published      = entity.published;
	attributes     = entity.attributes;
	icon		   = entity.GetIcon();
	icon_vis	   = entity.icon_vis;

	return *this;
}

void Entity::DeleteParent(Entity* parent)
{
	std::vector<Entity *>::iterator i = parents.begin();

	while(i != parents.end() && parent->GetId() != (*i)->GetId()) 
		i++;

	if(i != parents.end()) 
		parents.erase(i);

	//Modified("DeleteParent", parent->GetId());	
}

int Entity::CreatePersistence() {

	Remote::CreatePersistence();

	//Aggregate the class attributes to the persistence
	AddPersistence((std::string)"name", name);
    AddPersistence((std::string)"description", description);
	AddPersistence((std::string)"primitive_url", RelativeDirectory(primitive_url));
	AddPersistence((std::string)"html", RelativeDirectory(html));
	AddPersistence((std::string)"font", font);
	AddPersistence((std::string)"icon", RelativeDirectory(icon));
	
	std::ostringstream str;
	str << dynamic;
	AddPersistence((std::string)"dynamic", str.str());

	std::ostringstream str1;
	str1 << visible;
	AddPersistence((std::string)"visible", str1.str());

	std::ostringstream str2;
	str2 << animate;
	AddPersistence((std::string)"animate", str2.str());

	std::ostringstream str3;
	str3 << published;
	AddPersistence((std::string)"published", str3.str());

	AddPersistenceLevel((std::string)"attributes");

	for(std::vector<Attribute>::iterator i = attributes.begin(); i != attributes.end(); i++) {

		AddPersistenceLevel((std::string)"attribute");
		AddPersistence((std::string)"name", i->name);
		AddPersistence((std::string)"type", i->type);
		AddPersistence((std::string)"value", i->value);
		RemovePersistenceLevel();
	}


	RemovePersistenceLevel();

	return 0;
}


int Entity::CreateProperties() {

	Remote::CreateProperties();

	AddPersistence((std::string)"Name", name);
    AddPersistence((std::string)"Description", description);
	AddPersistence((std::string)"Primitive", RelativeDirectory(primitive_url));
	AddPersistence((std::string)"Html", RelativeDirectory(html));
	AddPersistence((std::string)"Font", font);
	AddPersistence((std::string)"Icon", RelativeDirectory(icon));

	RemovePersistenceLevel();

	if(!attributes.empty())
	{
		AddPersistenceLevel((std::string)"Attributes");
	
		for(std::vector<Attribute>::iterator i = attributes.begin(); i != attributes.end(); i++) {

			AddPersistence((std::string)i->type+ " " + i->name, i->value);
		}

		RemovePersistenceLevel();
	}

	return 0;
}




int Entity::AdaptPersistence(Node *root)
{
	Remote::AdaptPersistence(root);

	name = (root->GetChildValue("name"));

	description = (root->GetChildValue("description"));

	SetPrimitiveUrl(root->GetChildValue("primitive_url"));
	
	SetIcon(root->GetChildValue("icon"));

	SetHtml(root->GetChildValue("html"));

	font = (root->GetChildValue("font"));

	std::istringstream strd (root->GetChildValue("dynamic"));
	strd >> dynamic;

	std::istringstream strv (root->GetChildValue("visible"));
	strv >> visible;

	std::istringstream stranim (root->GetChildValue("animate"));
	stranim >> animate;

	std::istringstream strpublish (root->GetChildValue("published"));
	strpublish >> published;

	std::vector<cpw::Node *> &components = root->GetChildren();
	std::vector<cpw::Node *>::iterator i = components.begin();
	while ((i!=components.end()) && ((*i)->GetName() != std::string("attributes"))) i++;
	if (i!=components.end())
	{
		std::vector<cpw::Node *> &attr = (*i)->GetChildren();
		std::vector<cpw::Node *>::iterator j = attr.begin();
		for(j;j!=attr.end();j++)
		{
			cpw::Attribute at;
			at.name  = (*j)->GetChildValue("name");
			at.type  = (*j)->GetChildValue("type");
			at.value = (*j)->GetChildValue("value");
			attributes.push_back(at);
		}
	}
	return 0;
}


Entity* Entity::GetLastParent() const
{
	if(!parents.empty())
	{
		return parents.back();
	}

	return NULL;
}


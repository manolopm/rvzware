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
//#include <stdafx.h>

#include <cpw/entity/EntityRegistry.h>


using namespace cpw;


EntityRegistry* EntityRegistry::instance = NULL;




EntityRegistry * EntityRegistry::GetInstance()
{
	if(instance == NULL)
		instance = new EntityRegistry;

	return instance;
}


void EntityRegistry::ReleaseInstance()
{
	if(instance != NULL)
	{
		delete instance;
		instance = NULL;
	}
}


EntityRegistry::EntityRegistry(){
}


EntityRegistry::~EntityRegistry()
{
	
	//container::iterator i = entities.begin();

	while (!entities.empty())
	{
		container::iterator i = entities.begin();
		cpw::Entity *entity = i->second;
		entities.erase(i);
		delete entity;
	}

	//for(;i !=  entities.end(); i++) 
	//{
	//	delete i->second;	
	//	i->second = NULL;
	//}

	entities.clear();
	
}


void EntityRegistry::Add(cpw::Entity *entity) {

	if(entity != NULL)
	{
		container::iterator i = entities.find(entity->GetId());

		if(i == entities.end())
		{
			entities[entity->GetId()] = entity;
			//entity->SetLast();
		}
	}
}

void EntityRegistry::Remove(const cpw::TypeId id) 
{
	container::iterator i = entities.find(id);
	
	if (i!=entities.end())
	{	
		entities.erase(i);
	}	
}


void EntityRegistry::DeleteEntity(const cpw::TypeId id) 
{
	container::iterator i = entities.find(id);
	
	if (i!=entities.end())
	{	
		delete i->second;
		entities.erase(i);
	}	
}

void EntityRegistry::Clear()
{
	container::iterator i = entities.begin();

	for(;i !=  entities.end(); i++) 
		delete i->second;

	entities.clear();

}

cpw::Entity* EntityRegistry::GetEntity(const cpw::TypeId id)
{
	container::iterator i = entities.find(id);
	
	if (i!=entities.end())
	
		return i->second;

	return NULL;

}

std::vector<cpw::Entity*>  EntityRegistry::GetEntities(const std::string &class_name)
{
	
	std::vector<cpw::Entity*> out;

	container::iterator i = entities.begin();

	for(;i !=  entities.end(); i++) 

		if(i->second->GetClassName() == class_name)

			out.push_back(i->second);

	return out;

}



 cpw::Entity* EntityRegistry::GetEntityFromUrl(const std::string &entity_url)
{
	std::map<cpw::TypeId, Entity *>::iterator i;
	for(i = entities.begin(); i != entities.end() ; i++ )
	{
		if(i->second->GetUrl() == entity_url)	
			return i->second;
	}
	return NULL;
}


std::vector<cpw::Entity*> EntityRegistry::GetAnimatedEntities()
{
	
	std::vector<cpw::Entity*> out;

	container::iterator i = entities.begin();

	for(;i !=  entities.end(); i++) 

		if(i->second->IsAnimated())

			out.push_back(i->second);

	return out;


}

std::vector<cpw::Entity*> EntityRegistry::GetPublishedEntities()
{
	std::vector<cpw::Entity*> out;

	container::iterator i = entities.begin();

	for(;i !=  entities.end(); i++) 

		if(i->second->isPublished())

			out.push_back(i->second);

	return out;



}


std::vector<cpw::Entity*> EntityRegistry::GetAllEntities()
{
	std::vector<cpw::Entity*> out;

	for(std::map<cpw::TypeId, Entity* >::iterator i = entities.begin();
		i !=  entities.end();
		i++) 
	{
		out.push_back(i->second);
	}

	return out;

}
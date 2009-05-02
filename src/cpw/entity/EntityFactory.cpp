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

#include <sstream>

//#include <stdafx.h>

#include <cpw/entity/EntityFactory.h>
#include <cpw/entity/Element3D.h>
#include <cpw/entity/ElementVectorial.h>
#include <cpw/entity/ElementParticleSystem.h>
#include <cpw/entity/ContainerLayer.h>
#include <cpw/entity/RasterLayer.h>
#include <cpw/entity/VectorialLayer.h>
#include <cpw/common/IdGenerator.h>
#include <cpw/persistent/database/DataBase.h>
#include <cpw/persistent/database/Table.h>
#include <cpw/persistent/file/PersistentFileCallBack.h>
#include <cpw/persistent/database/PersistentDBCallBack.h>
#include <cpw/callback/Model3DCallBack.h>
#include <cpw/callback/VectorialCallBack.h>
#include <cpw/callback/ParticleSystemCallBack.h>
#include <cpw/callback/ContainerLayerCallBack.h>


using namespace cpw;



EntityFactory *EntityFactory::instance = NULL;




EntityFactory *EntityFactory::GetInstance()
{
  if(instance == NULL)
    instance = new EntityFactory;

  return instance;
}


void EntityFactory::ReleaseInstance()
{
  if(instance != NULL)
    {
      delete instance;
      instance = NULL;
    }
}



EntityFactory::EntityFactory():entity_number()
{
  ElementVectorial *elvectorial = new ElementVectorial;
  elvectorial->AddCallBack(new VectorialCallBack);
  elvectorial->SetPersistentCallBack(new PersistentFileCallBack);
  elvectorial->SetName("ElementVectorial");
  RegisterBasic(elvectorial);

  Element3D *el3d = new Element3D;
  el3d->AddCallBack(new Model3DCallBack);
  el3d->SetPersistentCallBack(new PersistentFileCallBack);
  el3d->SetName("Element3d");
  RegisterBasic(el3d);
	
  ElementParticleSystem *eps = new ElementParticleSystem;
  eps->AddCallBack(new ParticleSystemCallBack);
  eps->SetPersistentCallBack(new PersistentFileCallBack);
  eps->SetName("ParticleSystem");
  RegisterBasic(eps);
	
  ContainerLayer *cl = new ContainerLayer;
  cl->AddCallBack(new ContainerLayerCallBack);
  cl->SetPersistentCallBack(new PersistentFileCallBack);
  cl->SetName("ContainerLayer");
  RegisterBasic(cl);

  RasterLayer *rl = new RasterLayer;
  rl->SetPersistentCallBack(new PersistentFileCallBack);
  rl->SetName("RasterLayer");
  RegisterBasic(rl);

  VectorialLayer *vl = new VectorialLayer;
  vl->SetPersistentCallBack(new PersistentFileCallBack);
  vl->SetName("VectorialLayer");
  RegisterBasic(vl);

  DataBase *db = new DataBase;
  db->SetPersistentCallBack(new PersistentFileCallBack);
  db->SetName("DataBase");
  RegisterBasic(db);
	
  Table *tb = new Table;
  tb->SetPersistentCallBack(new PersistentDBCallBack);
  tb->SetName("Table");
  RegisterBasic(tb);	
}

EntityFactory::~EntityFactory()
{
  std::map<std::string, cpw::Entity *>::iterator i;
  //Release all the registered entities and primitives
  for( i = entity_prototypes_cn.begin(); i != entity_prototypes_cn.end(); i++)
    {
      if (i->second)
	{
	  delete i->second;
	  i->second = NULL;
	}
    }

  std::map<cpw::TypeId , cpw::Entity *>::iterator j;
  //Release all the registered entities and primitives
  for( j = entity_prototypes_id.begin(); j != entity_prototypes_id.end(); j++)
    {
      if (j->second)
	{
	  delete j->second;
	  j->second = NULL;
	}
    }
}

cpw::Entity *EntityFactory::CreateEntity(const std::string &type)
{
  cpw::IdGenerator id_gen;
  cpw::Entity *entity = NULL;

  //search for the entity class
  std::map<std::string, cpw::Entity *>::iterator i = entity_prototypes_cn.find(type);
	
  if( i != entity_prototypes_cn.end() )
    {
      entity = i->second->Clone();
      entity->SetID(id_gen.GetNewId());
      std::stringstream str;
      str << entity->GetName() << GetEntityNumber();
      entity->SetName(str.str());
      entity_number++;
    }

  return entity;
}


unsigned long int &EntityFactory::GetEntityNumber()
{
  return entity_number;
}

cpw::Entity *EntityFactory::CreateEntity(const cpw::TypeId &type)
{
  cpw::IdGenerator id_gen;
  cpw::Entity *entity = NULL;

  //if (type != NULL)
  //{
  //search for the entity class
  std::map<cpw::TypeId, cpw::Entity *>::iterator i = entity_prototypes_id.find(type);
		
  if( i != entity_prototypes_id.end() )
    {
      entity = i->second->Clone();
      entity->SetID(id_gen.GetNewId());
      std::stringstream str;
      str << entity->GetName() << GetEntityNumber();
      entity->SetName(str.str());
      entity_number++;
			
    }
  //}
	
  return entity;
}




void EntityFactory::RegisterBasic(Entity* entity)
{

  std::map<std::string, cpw::Entity *>::iterator i = entity_prototypes_cn.find(entity->GetClassName());

  if(i == entity_prototypes_cn.end()) 
    {
      entity->SetName(entity->GetClassName());			
      entity_prototypes_cn[entity->GetClassName()] = entity;
    }
}


bool EntityFactory::Register(Entity* entity)
{

  std::map<cpw::TypeId, cpw::Entity *>::iterator i = entity_prototypes_id.find(entity->GetId());

  if(i == entity_prototypes_id.end()) 
    {
      entity_prototypes_id[entity->GetId()] = entity;
      return true;
    }
  return false;	
}

std::map<cpw::TypeId, cpw::Entity *> &EntityFactory::GetPrototypes()
{
  return entity_prototypes_id;
}


std::map<cpw::TypeId, cpw::Entity *> EntityFactory::GetPrototypes(const std::string &cn)
{

  std::map<cpw::TypeId, cpw::Entity *>::iterator i = entity_prototypes_id.begin();
	
  std::map<cpw::TypeId, cpw::Entity *> out;

  for(;i != entity_prototypes_id.end();i++)

    if(i->second->GetClassName() == cn) 
	
      out.insert(*i);
	
  return out;
}

cpw::Entity * EntityFactory::GetBasicPrototype(const std::string &cn)
{

  std::map<std::string, cpw::Entity *>::iterator i = entity_prototypes_cn.begin();
	
  for(;i != entity_prototypes_cn.end();i++)
    {
      if(i->first == cn) 
	return i->second;
    }
	
  return NULL;
}



cpw::Entity *EntityFactory::GetPrototype(const cpw::TypeId &id)
{
  std::map<cpw::TypeId, cpw::Entity *>::iterator i = entity_prototypes_id.find(id);

  if(i != entity_prototypes_id.end()) 

    return entity_prototypes_id[id];
	
  else

    return NULL;
}


cpw::Entity *EntityFactory::GetPrototypeFromUrl(const std::string &url)
{

  std::map<cpw::TypeId, cpw::Entity *>::iterator i = entity_prototypes_id.begin();

  while(i != entity_prototypes_id.end() && i->second->GetUrl() != url) i++;

  if(i != entity_prototypes_id.end())

    return i->second;

  else

    return NULL;
}

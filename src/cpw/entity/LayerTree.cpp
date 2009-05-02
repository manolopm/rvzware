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

//#include <stdafx.h>

#include <cpw/entity/LayerTree.h>
#include <cpw/entity/ContainerLayer.h>
#include <cpw/entity/EntityRegistry.h>


using namespace cpw;



LayerTree::LayerTree(Entity* toplayer)
{

  if(top_layer != NULL)
    {
      delete top_layer;
      top_layer = NULL;
    }

  top_layer = toplayer;
  active    = top_layer;
  last_added_entity = top_layer;
  active_parent = top_layer;
}



LayerTree::~LayerTree()
{
}

bool LayerTree::TestModified(Entity *entity)
{
  if(entity->isModified()) return true;
  else {
    bool modified = false;
    if(entity->isContainer())
      {
	int i = 0;
	while(i < entity->GetNumChildren() && !(modified = TestModified(entity->GetChild(i)))) 
	  i++;
      }
    return modified;
  }
}

bool LayerTree::isModified()
{
  if(top_layer != NULL)
	
    return TestModified(top_layer);	
	
  return false;
}


void LayerTree::CopyLayerToTop(Entity *layer)
{	
  if(top_layer != NULL)
    {
      delete top_layer;
      top_layer = NULL;
    }

  top_layer = layer;
  active    = top_layer;
  active_parent = top_layer;
  last_added_entity = top_layer;
  Subject::Notify();
}


bool LayerTree::AddToActiveLayer(Entity *ent, const bool &add_parent)
{
  if(top_layer == NULL) 
    {
      CopyLayerToTop(ent);
    }
  else
    {
      //test for recursive inclusions
      cpw::TypeId id = ent->GetId();
      cpw::Entity *layer = active_parent; 
		
      while(layer != NULL && layer->GetId() != id) 
	layer = layer->GetParent();

      if(layer != NULL)
	return false;
      else
	{
	  if (active_parent == NULL)
	    active_parent = top_layer;

	  active_parent->Add(ent);
	  last_added_entity = ent;
	  active = last_added_entity;
	  Subject::Notify();
	}
    }
  return true;

}


cpw::Entity *LayerTree::MakeActive(const cpw::TypeId &id)
{
  //search for the node with the url
  bool ok = false;
  Entity *selected = NULL;
  SearchAndMakeActive(top_layer, id, ok, &selected, false);

  return selected;
}

void LayerTree::MakeParentActive(const cpw::TypeId &id)
{
  //search for the node with the url
  bool ok = false;
  Entity *selected = NULL;
  SearchAndMakeActive(top_layer, id, ok, &selected, true);
}


void LayerTree::SearchAndMakeActive(Entity *entity, const cpw::TypeId &id, bool &entity_found, Entity **selected, bool parent)
{
  //if already found don´t continue searching
  if (entity_found)
    return;

  //if this is the entity make it active and return
  if (entity->GetId() == id)
    {
      entity_found = true;

      *selected = entity;

      if (parent)
	active_parent = entity;
      else
	active = entity;
		
      return;
    }
	
  //keep searching
  if (entity->isContainer())
    {
      for (int i = 0; i < ((cpw::ContainerLayer *)entity)->GetNumChildren(); i++)
	SearchAndMakeActive(((cpw::ContainerLayer *)entity)->GetChild(i), id, entity_found, selected, parent);
    }
}


void LayerTree::GetAnimatedEntitiesFromTopLayer(std::vector<cpw::Entity *> &entities)
{
  GetAnimatedEntities(entities, top_layer);
}

void LayerTree::GetAnimatedEntities(std::vector<cpw::Entity *> &entities, cpw::Entity *entity)
{
  if (entity == NULL)
    return;

  for (unsigned int i=0; i<(unsigned int)entity->GetNumChildren(); i++)
    GetAnimatedEntities(entities, entity->GetChild(i));

  if (entity->IsAnimated())
    entities.push_back(entity);
	
}


cpw::Entity * LayerTree::GetEntity(const cpw::TypeId &id)
{
  return cpw::EntityRegistry::GetInstance()->GetEntity(id);	
}


void LayerTree::Clear()
{
  top_layer = NULL;  
  active    = top_layer;
  last_added_entity = top_layer;
  active_parent = top_layer;
  Subject::Notify();

}

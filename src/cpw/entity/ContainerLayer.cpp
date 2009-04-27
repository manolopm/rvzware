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

#include <cpw/entity/ContainerLayer.h>


using namespace cpw;

ContainerLayer::ContainerLayer(const cpw::TypeId id, 
					           const std::string &class_name): Layer(id, class_name), entity_vector()
{
}

ContainerLayer::ContainerLayer(const cpw::TypeId id, 
							   const std::string &nam, const std::string &prim, 
							   const std::string &htm, const bool dyn, const bool vis, const bool anim,
							   const std::string &desc, const std::vector<cpw::Attribute> &attr,
							   const std::string &class_name) : 
	Layer(id, nam, prim, htm, dyn, vis, anim, desc, attr, class_name), entity_vector()
{
}

ContainerLayer::~ContainerLayer(void)
{
}

int ContainerLayer::Add(Entity *entity)
{ 
	entity_vector.push_back(entity); 
	entity->AddParent(this);
	
	std::ostringstream str;
	str << entity->GetId();
	Modified("AddChild", str.str());
	
	return 0;
}

int ContainerLayer::DeleteChild(Entity *entity)
{
	std::vector<Entity *>::iterator i = entity_vector.begin();

	while(i != entity_vector.end() && entity->GetId() != (*i)->GetId()) 
		i++;

	if(i != entity_vector.end()) 
	{
		entity->DeleteParent(this);
		entity_vector.erase(i);
		Modified("DeleteChild", entity->GetId());
		return 1;
	}

	return 0;
}

int ContainerLayer::DeleteAllChild()
{
	entity_vector.clear();
	Modified("DeleteAllChild", this->GetId());

	return 1;
}

void ContainerLayer::DeleteReferenceToMe()
{
	std::vector<Entity *>::iterator i = entity_vector.begin();
	for(;i!=entity_vector.end();i++)
	{
		(*i)->DeleteParent(this);
	}
}

ContainerLayer::ContainerLayer(const ContainerLayer &clayer):Layer(clayer), entity_vector(clayer.entity_vector)
{
}

ContainerLayer &ContainerLayer::operator = (const ContainerLayer &clayer)
{
	cpw::Layer::operator = (clayer);

	entity_vector = clayer.entity_vector;

	return *this;
}


Entity *ContainerLayer::GetChild(unsigned int i) 
{ 
	if (i >= entity_vector.size()) 
		return NULL;
	else 
		return entity_vector[i];
}

int ContainerLayer::GetNumChildren() { return ((int) entity_vector.size()); }

const Entity *ContainerLayer::GetChild(unsigned int i) const 
{ 
	if (i >= entity_vector.size()) return NULL;
	   return entity_vector[i];
}

const int ContainerLayer::GetNumChildren() const 
{ 
	return ((int) entity_vector.size()); 
}


bool ContainerLayer::isContainer() {return true;}

const bool ContainerLayer::isContainer() const 
{
	return true;
}


Entity* ContainerLayer::Clone() 
{
	return new ContainerLayer(*this);
}


void ContainerLayer::SetValue(const std::string &f, const std::string &v)
{
	Layer::SetValue(f, v);
}



int ContainerLayer::CreatePersistence() 
{
	Layer::CreatePersistence();

	AddPersistenceLevel((std::string)"components");
	for(std::vector<Entity*>::iterator i = entity_vector.begin(); i != entity_vector.end(); i++) {

		AddPersistenceLevel((std::string)"component");
		AddPersistence((std::string)"url", RelativeDirectory((*i)->GetUrl()));
		RemovePersistenceLevel();
	}

	RemovePersistenceLevel();

	return 0;
}


int ContainerLayer::AdaptPersistence(cpw::Node *root)
{

	Layer::AdaptPersistence(root);

	return 0;
}


void ContainerLayer::SetVisible(const bool vis)
{
	Layer::SetVisible(vis);

	std::vector<Entity *>::iterator iter;
	for(iter = entity_vector.begin(); iter != entity_vector.end(); iter++)
	{
		(*iter)->SetVisible(vis);
	}
}


void ContainerLayer::SetPublished(const bool pub)
{
	Layer::SetPublished(pub);

	std::vector<Entity *>::iterator iter;
	for(iter = entity_vector.begin(); iter != entity_vector.end(); iter++)
	{
		(*iter)->SetPublished(pub);
	}
}


void ContainerLayer::SetAnimate(const bool anim)
{
	Layer::SetAnimate(anim);

	std::vector<Entity *>::iterator iter;
	for(iter = entity_vector.begin(); iter != entity_vector.end(); iter++)
	{
		(*iter)->SetAnimate(anim);
	}
}

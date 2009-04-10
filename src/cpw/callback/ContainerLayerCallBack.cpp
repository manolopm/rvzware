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

#include <cpw/callback/ContainerLayerCallBack.h>
#include <cpw/graphic/ApplicationScene.h>
#include <cpw/graphic/IScene.h>
#include <cpw/entity/Entity.h>


using namespace cpw;

ContainerLayerCallBack::ContainerLayerCallBack(void)
{
}

ContainerLayerCallBack::ContainerLayerCallBack(const ContainerLayerCallBack &igcb)
{
}		

ContainerLayerCallBack::~ContainerLayerCallBack(void)
{
}

ContainerLayerCallBack &ContainerLayerCallBack::operator = (const ContainerLayerCallBack &igcb)
{
	return *this;
}

ContainerLayerCallBack* ContainerLayerCallBack::Clone() 
{
	return new ContainerLayerCallBack(*this);
}


void ContainerLayerCallBack::Insert(const cpw::Callable* callable)
{
	cpw::Entity *entity = (Entity *) callable;
	//const cpw::TypeId id = entity->GetPrimitive()->GetId();
	
	if (entity->isContainer())
	{
			
		//std::ostringstream stro_id, stro_id2;
		//stro_id << entity->GetId();
		//stro_id2 << entity->GetParent()->GetId();
		////app_scene->SetGroupVisible(stro_id.str(), entity->isVisible());
		//app_scene->SetVisible(stro_id2.str(), stro_id.str(), entity->isVisible());

		for (int i=0; i<entity->GetNumChildren(); i++)
		{
			//Insert(entity->GetChild(i)->Insert);
			entity->GetChild(i)->GraphicInsert();
		}
	}

	return;
}


void ContainerLayerCallBack::Update(cpw::Callable* callable) {};
void ContainerLayerCallBack::Delete(cpw::Callable* callable) 
{
	//return;
	cpw::Entity *entity = (Entity *) callable;
	if (entity->isContainer())
	{
		//cpw::IScene *app_scene = cpw::ApplicationScene::GetInstance()->GetScene();
		for(int i=0; i<entity->GetNumChildren(); i++)
		{
			if (entity->GetChild(i)->GetNumParents()==1)
				entity->GetChild(i)->GraphicDelete();
		}
//return;

	}
};

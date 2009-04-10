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

#include <cpw/callback/Model3DCallBack.h>
#include <cpw/graphic/ApplicationScene.h>
#include <cpw/graphic/IScene.h>
#include <cpw/entity/Entity.h>
#include <cpw/entity/Element3D.h>

using namespace cpw;



Model3DCallBack::Model3DCallBack()
{
}

Model3DCallBack::~Model3DCallBack()
{
}

void Model3DCallBack::Insert(const cpw::Callable* callable)
{
	cpw::Element3D *element = (cpw::Element3D *) callable;

	std::string model_url = element->GetModelUrl();

	/** \todo Quitar la comprobación element->GetLastParent() !=NULL al insertar un Elemento3D */

	if (model_url != "" /*&& element->GetLastParent() !=NULL*/)
	{
		//cpw::Entity *ent = entity->GetLastParent();
		std::ostringstream /*stro_pid, */stro_id;
		
		//stro_pid << element->GetLastParent()->GetId();
		//std::string group_name = stro_pid.str();

		stro_id << element->GetId();
		std::string entity_id = stro_id.str();

		const std::string &icon_filename =  element->GetIcon();

		cpw::IScene *app_scene = cpw::ApplicationScene::GetInstance()->GetScene();

		std::string str = "";
		cpw::Point3d<float> p1(element->GetUtm(0), element->GetUtm(1), element->GetUtm(2));
		cpw::Point3d<float> p2(element->GetOrientation(0), element->GetOrientation(1), element->GetOrientation(2));
		cpw::Point3d<float> p3(element->GetScale(0), element->GetScale(1), element->GetScale(2));
		app_scene->AddObjectToGroup(entity_id,(const std::string &)str,
					    model_url,
					    (cpw::Point3d<float> &)p1,
					    (cpw::Point3d<float> &)p2,
					    (cpw::Point3d<float> &)p3,
			element->isVisible(),
			element->GetName());
		
		cpw::Point3d<float> icon_despl(0.0f, 0.0f, 800.0f);
		app_scene->AddPointSprite(entity_id, icon_despl, 0.0f, 0.0f, icon_filename);

		if(!element->GetIconVisibility())
			app_scene->SetIconVisibility(entity_id, false);

		element->Notify();
	}
	
	return;
}


void Model3DCallBack::Update(cpw::Callable* callable)
{
	cpw::Element3D *entity = (cpw::Element3D *) callable;
	cpw::IScene *app_scene = cpw::ApplicationScene::GetInstance()->GetScene();
	std::ostringstream pid;
	pid << entity->GetId();
	std::string id = pid.str();

	app_scene->Update(id, 
		cpw::Point3d<float>(entity->GetUtm(0), entity->GetUtm(1), entity->GetUtm(2)),
		cpw::Point3d<float>(entity->GetOrientation(0), entity->GetOrientation(1), entity->GetOrientation(2)),
		cpw::Point3d<float>(entity->GetScale(0), entity->GetScale(1), entity->GetScale(2)));


}
void Model3DCallBack::Delete(cpw::Callable* callable)
{
	cpw::Entity *entity = (Entity *) callable;
	cpw::IScene *app_scene = cpw::ApplicationScene::GetInstance()->GetScene();

	std::ostringstream sid1, sid2;
	
	sid1 << entity->GetId();
	//sid2 << entity->GetParent()->GetId();
	
	std::string id = sid1.str();
	//std::string pid = sid2.str();

	//app_scene->Remove(pid, id);
	app_scene->Remove(id);
}


Model3DCallBack::Model3DCallBack(const Model3DCallBack &m3dcb)
{
}

Model3DCallBack &Model3DCallBack::operator = (const Model3DCallBack &m3dcb)
{
	return *this;
}

ICallBack* Model3DCallBack::Clone() 
{
	return new Model3DCallBack(*this);
}

void Model3DCallBack::Animate(cpw::Callable* callable, cpw::cpwTime &time)
{
}

void Model3DCallBack::Visualize(cpw::Callable* callable, const bool &value)
{
	cpw::Entity *ent = (Entity *) callable;
	if (ent!=NULL)
	{
		std::ostringstream uint_to_string1;
		uint_to_string1 << ent->GetId();  //child

		//cpw::Entity* parent = ent->GetParent(0);//layer_tree->GetEntity(GetItemId(tree_ctrl->GetItemParent(id)));
		//uint_to_string2 << "";
		//if (parent != NULL)
		//	uint_to_string2 << parent->GetId(); //parent
		//
		//cpw::ApplicationScene::GetInstance()->GetScene()->SetVisible(uint_to_string2.str(), uint_to_string1.str(), value);

		cpw::Entity* parent;
		for(int i=0; i<ent->GetNumParents(); i++)
		{
			std::ostringstream uint_to_string2;
			parent = ent->GetParent(i);
			if (parent != NULL)
			{
				uint_to_string2 << parent->GetId(); //parent		
				cpw::ApplicationScene::GetInstance()->GetScene()->SetVisible(uint_to_string2.str(), uint_to_string1.str(), value);
			}
		}
	}

}

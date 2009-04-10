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

#include <cpw/callback/VectorialCallBack.h>
#include <cpw/graphic/ApplicationScene.h>
#include <cpw/graphic/IScene.h>
#include <cpw/entity/Entity.h>
#include <cpw/entity/ElementVectorial.h>

#include <cpw/debugger.h>

using namespace cpw;



VectorialCallBack::VectorialCallBack()
{
}

VectorialCallBack::~VectorialCallBack()
{
}

void VectorialCallBack::Insert(const cpw::Callable* callable)
{
	cpw::ElementVectorial *element = (cpw::ElementVectorial *) callable;

	if (element->GetNumPoints()!=0)
	{
		std::ostringstream stro_id;
		
		stro_id << element->GetId();
		std::string entity_id = stro_id.str();

		const std::string &icon_filename =  element->GetIcon();

		cpw::IScene *app_scene = cpw::ApplicationScene::GetInstance()->GetScene();

		int r, g, b;
		element->GetRGBColour(r,g,b);
		if(element->GetLine())
			app_scene->AddLine(entity_id, element->GetPoints(), element->GetColor(),r,g,b, element->GetWidth(), element->isVisible());
		else
			app_scene->AddLine2(entity_id, element->GetPoints(), element->GetColor(),r,g,b, element->GetWidth(), element->isVisible());

		cpw::Point3d<float> icon_despl(0.0f, 0.0f, 1000.0f);
		app_scene->AddPointSprite(entity_id, icon_despl, 0.0f, 0.0f, icon_filename);

		if(!element->GetIconVisibility())
			app_scene->SetIconVisibility(entity_id, false);
	}
	
	return;
}


void VectorialCallBack::Update(cpw::Callable* callable)
{
	cpw::ElementVectorial *entity = (cpw::ElementVectorial *) callable;
	cpw::IScene *app_scene = cpw::ApplicationScene::GetInstance()->GetScene();
	std::ostringstream id;
	id << entity->GetId();
	std::string sid = id.str();

	int r, g, b;
	entity->GetRGBColour(r,g,b);
	if(entity->GetLine())
		app_scene->UpdateLine(sid,entity->GetPoints(),entity->GetColor(),r,g,b, entity->GetWidth(), true, entity->GetUpdatePoint());//debug assertion failed
	else
	{
		app_scene->AddLine2(sid, entity->GetPoints(), entity->GetColor(),r,g,b, entity->GetWidth(), entity->isVisible());
		cpw::Point3d<float> icon_despl(0.0f, 0.0f, 1000.0f);
		std::string icon_filename =  entity->GetIcon();
		app_scene->AddPointSprite(sid, icon_despl, 0.0f, 0.0f, icon_filename);

	}
	
	entity->UpdatePoint(-1);

	//app_scene->AddLine(sid,entity->GetPoints(),entity->GetColor(),r,g,b, entity->GetWidth(), true);
	//cpw::Point3d<float> icon_despl(0.0f, 0.0f, 1000.0f);
	//const std::string &icon_filename =  entity->GetIcon();
	//app_scene->AddPointSprite(sid, icon_despl, 0.0f, 0.0f, icon_filename);
}


void VectorialCallBack::Delete(cpw::Callable* callable)
{
	cpw::Entity *entity = (Entity *) callable;
	cpw::IScene *app_scene = cpw::ApplicationScene::GetInstance()->GetScene();

	std::ostringstream sid1;
	
	sid1 << entity->GetId();
	
	std::string id = sid1.str();

	app_scene->Remove(id);
}


VectorialCallBack::VectorialCallBack(const VectorialCallBack &vectorcb)
{
}

VectorialCallBack &VectorialCallBack::operator = (const VectorialCallBack &vectorcb)
{
	return *this;
}

ICallBack* VectorialCallBack::Clone() 
{
	return new VectorialCallBack(*this);
}

void VectorialCallBack::Animate(cpw::Callable* callable, cpw::cpwTime &time)
{
}

void VectorialCallBack::Visualize(cpw::Callable* callable, const bool &value)
{
	cpw::Entity *ent = (Entity *) callable;
	if (ent!=NULL)
	{
		std::ostringstream uint_to_string1;
		uint_to_string1 << ent->GetId();  

		cpw::ApplicationScene::GetInstance()->GetScene()->SetVisible("", uint_to_string1.str(), value);
	}
}

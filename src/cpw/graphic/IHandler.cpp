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

#include <cpw/graphic/IHandler.h>
#include <cpw/graphic/ApplicationScene.h>
#include <cpw/entity/Element.h>
#include <cpw/graphic/IScene.h>

using namespace cpw;

IHandler::IHandler() : entity_target(NULL), Observer(), allow_going_under_terrain(false), adapt_position_to_terrain_height(true), navigator_manager(NULL), active(true)
{}

IHandler::~IHandler(void)
{}

void IHandler::AdaptHandlerAndBrothersToModel(const bool &adapt_rotation)
{
	if (!active)
		return;

	IScene *scene = cpw::ApplicationScene::GetInstance()->GetScene();

	if ((scene == NULL) || (entity_target == NULL) || (navigator_manager == NULL))
		return;

	int width, height;
	double fov_y, aspect_ratio, znear, zfar;
	cpw::INavigator *focused_navigator = navigator_manager->GetFocusedOrFirstNavigator();
	
	if (focused_navigator == NULL)
		return;

	focused_navigator->GetPerspective(fov_y, aspect_ratio, znear, zfar);
	focused_navigator->GetScreenSize(width, height);
	cpw::Point3d<double> camera_pos = focused_navigator->GetCameraController()->GetCamera()->GetUTMPosition();

	cpw::Element *el = (cpw::Element *) entity_target;

	cpw::Point3d<float> orientation(el->GetOrientation(0), el->GetOrientation(1), el->GetOrientation(2));

	//scene->AdaptHandlerToModel(id, entity_target->GetId(), orientation, adapt_rotation);
	scene->AdaptHandlerToModelFixedScreenSize(id, entity_target->GetId(), orientation, adapt_rotation, (const int &)(height * 0.66f), camera_pos, znear);

	for (std::vector< IHandler *>::iterator iter = brothers.begin();
		 iter != brothers.end();
		 iter++)
	{
		//scene->AdaptHandlerToModel((*iter)->GetId(), entity_target->GetId(), orientation, adapt_rotation);
	  scene->AdaptHandlerToModelFixedScreenSize((*iter)->GetId(), entity_target->GetId(), orientation, adapt_rotation, (const int &)(height * 0.66f), camera_pos, znear);
	}
}


void IHandler::AdaptHandlerAndBrothersToModel(const bool &adapt_rotation, const cpw::Point3d<float> &rotation)
{
	if (!active)
		return;

	IScene *scene = cpw::ApplicationScene::GetInstance()->GetScene();

	if ((scene == NULL) || (entity_target == NULL) || (navigator_manager == NULL))
		return;

	int width, height;
	double fov_y, aspect_ratio, znear, zfar;
	navigator_manager->GetFocusedOrFirstNavigator()->GetPerspective(fov_y, aspect_ratio, znear, zfar);
	navigator_manager->GetFocusedOrFirstNavigator()->GetScreenSize(width, height);
	cpw::Point3d<double> camera_pos = navigator_manager->GetFocusedOrFirstNavigator()->GetCameraController()->GetCamera()->GetUTMPosition();

	cpw::Element *el = (cpw::Element *) entity_target;

	cpw::Point3d<float> orientation(el->GetOrientation(0), el->GetOrientation(1), el->GetOrientation(2));

	//scene->AdaptHandlerToModel(id, entity_target->GetId(), rotation, adapt_rotation);
	scene->AdaptHandlerToModelFixedScreenSize(id, entity_target->GetId(), rotation, adapt_rotation, (const int &)(height * 0.66f), camera_pos, znear);

	for (std::vector< IHandler *>::iterator iter = brothers.begin();
		 iter != brothers.end();
		 iter++)
	{
		//scene->AdaptHandlerToModel((*iter)->GetId(), entity_target->GetId(), rotation, adapt_rotation);
	  scene->AdaptHandlerToModelFixedScreenSize((*iter)->GetId(), entity_target->GetId(), rotation, adapt_rotation, (const int &)(height * 0.66f), camera_pos, znear);
	}
}

void IHandler::SetEntityTarget(cpw::Entity *_entity_target)
{ 
	if (entity_target != NULL)
		entity_target->Detach(this);

	entity_target = _entity_target; 
	
	if (entity_target != NULL)
		entity_target->Attach(this);
}

void IHandler::ShowBrothers(const bool &visible)
{
	IScene *scene = cpw::ApplicationScene::GetInstance()->GetScene();

	if (scene != NULL)
	{
		for (std::vector< IHandler *>::iterator iter = brothers.begin();
			 iter != brothers.end();
			 iter++)
		{
			scene->SetHandlerVisible( (*iter)->GetId(), visible );
		}
	}

}

void IHandler::ClearEntityTarget()
{
	entity_target = NULL;
}

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

#include <cpw/graphic/ScaleGizmo.h>

#include <cpw/graphic/ApplicationScene.h>
#include <cpw/entity/EntityRegistry.h>
#include <cpw/entity/Element.h>
#include <cpw/common/TypeId.h>
#include <cpw/graphic/IScene.h>

using namespace cpw;

ScaleGizmo::ScaleGizmo(void)  
{
}

ScaleGizmo::~ScaleGizmo(void)
{
}

bool ScaleGizmo::MouseDrag(const int &x0, const int &y0, const int &x1, const int &y1, const cpw::MouseButtonsState &mbs)
{
	IScene *scene = cpw::ApplicationScene::GetInstance()->GetScene();
	EntityRegistry *entity_reg =EntityRegistry::GetInstance();
	const float scale_mult = 0.005f;

	//Element *entity = (Element *) entity_reg->GetEntity(TypeId(GetEntityTarget()));

	if (GetEntityTarget() == NULL)
		return false;

	Element *entity = (Element *) GetEntityTarget();

	float new_x = entity->GetScale(0); 
	float new_y = entity->GetScale(1); 
	float new_z = entity->GetScale(2);

	cpw::Point3d<double> camera_view = GetCameraView();
	
	int scale_x = x1 - x0;
	int scale_y = y1 - y0;
	
	float scale_factor = 0.0f;

	GetNavigatorManager()->GetFocusedOrFirstNavigator()->SetKeepMouseInsideCanvas(true);

	/*if ((camera_view.x < 0.0f) && (camera_view.y < 0.0f))
	{
		scale_factor = (scale_x + scale_y) * scale_mult;		
	}
	else if ((camera_view.x < 0.0f) && (camera_view.y > 0.0f))
	{
		scale_factor = (scale_x + scale_y) * scale_mult;
	}
	else if ((camera_view.x > 0.0f) && (camera_view.y > 0.0f))
	{
		scale_factor = (scale_x + scale_y) * scale_mult;
	}
	else if ((camera_view.x > 0.0f) && (camera_view.y < 0.0f))
	{
		scale_factor = (scale_x + scale_y) * scale_mult;
	}*/

	scale_factor = 1.0f + (scale_y * scale_mult);

	new_x *= scale_factor;
	new_y *= scale_factor;
	new_z *= scale_factor;

	if (new_x <= 0.0f)
		new_x = 0.01f;

	if (new_y <= 0.0f)
		new_y = 0.01f;

	if (new_z <= 0.0f)
		new_z = 0.01f;

	entity->SetScale(new_x, new_y, new_z);
	entity->GraphicUpdate();

	IHandler::AdaptHandlerAndBrothersToModel();

	if (scene == false)
		return false;

	return true;
}

void ScaleGizmo::Update(bool subject_deleted)
{
	if (subject_deleted)
	{
		if (GetEntityTarget() != NULL)
		{
			//GetEntityTarget()->Detach(this);
			ClearEntityTarget();
		}
	}
	else
		IHandler::AdaptHandlerAndBrothersToModel();
}

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

#include <cpw/graphic/RotationGizmo.h>

#include <cpw/graphic/ApplicationScene.h>
#include <cpw/entity/EntityRegistry.h>
#include <cpw/entity/Element.h>
#include <cpw/common/TypeId.h>
#include <cpw/graphic/IScene.h>

using namespace cpw;

RotationGizmo::RotationGizmo(void)  
{
	accumulative_rotation.x = accumulative_rotation.y = accumulative_rotation.z = 0.0f;
}

RotationGizmo::~RotationGizmo(void)
{
}

bool RotationGizmo::MouseDrag(const int &x0, const int &y0, const int &x1, const int &y1, const cpw::MouseButtonsState &mbs)
{
	IScene *scene = cpw::ApplicationScene::GetInstance()->GetScene();
	EntityRegistry *entity_reg =EntityRegistry::GetInstance();
	float rotation_mult = 0.05f;
	int rotation = 0;

	if ((GetEntityTarget() == NULL) || (scene == false))
		return false;

	int rotation_x = x1 - x0;
	int rotation_y = y1 - y0;

	cpw::Point3d<double> camera_pos = GetCameraPos();
	cpw::Point3d<double> camera_view = GetCameraView();
	
	cpw::Point3d<float> new_rotation;

	GetNavigatorManager()->GetFocusedOrFirstNavigator()->SetKeepMouseInsideCanvas(true);
	
	if (GetTransformationAxis() == AXIS_X)
	{
		if ((camera_view.x < 0.0f) && (camera_view.y < 0.0f))
			rotation = - rotation_x + rotation_y;
		else if ((camera_view.x < 0.0f) && (camera_view.y > 0.0f))
			rotation = -rotation_x - rotation_y;
		else if ((camera_view.x >= 0.0f) && (camera_view.y >= 0.0f))
			rotation = rotation_x - rotation_y;
		else if ((camera_view.x > 0.0f) && (camera_view.y < 0.0f))
			rotation = rotation_x + rotation_y;
		
		
		rotation_mult *= rotation;

		accumulative_rotation.x += rotation_mult;
		scene->AcumulativeRotation(X_AXIS, rotation_mult, GetEntityTarget()->GetId(), new_rotation);
	}
	else if (GetTransformationAxis() == AXIS_Y)
	{
		if ((camera_view.x < 0.0f) && (camera_view.y < 0.0f))
			rotation = -rotation_x - rotation_y;
		else if ((camera_view.x < 0.0f) && (camera_view.y > 0.0f))
			rotation = rotation_x - rotation_y;
		else if ((camera_view.x >= 0.0f) && (camera_view.y >= 0.0f))
			rotation = rotation_x + rotation_y;
		else if ((camera_view.x > 0.0f) && (camera_view.y < 0.0f))
			rotation = -rotation_x + rotation_y;
		
		
		rotation_mult *= rotation;

		accumulative_rotation.y += rotation_mult;
		scene->AcumulativeRotation(Y_AXIS, rotation_mult, GetEntityTarget()->GetId(), new_rotation);
	}
	else
	{
		if ((camera_view.x < 0.0f) && (camera_view.y < 0.0f))
			rotation = rotation_x - rotation_y;
		else if ((camera_view.x < 0.0f) && (camera_view.y > 0.0f))
			rotation = rotation_x + rotation_y;
		else if ((camera_view.x >= 0.0f) && (camera_view.y >= 0.0f))
			rotation = -rotation_x + rotation_y;
		else if ((camera_view.x > 0.0f) && (camera_view.y < 0.0f))
			rotation = -rotation_x - rotation_y;
		
		
		rotation_mult *= rotation;
		accumulative_rotation.z += rotation_mult;
		scene->AcumulativeRotation(Z_AXIS, rotation_mult, GetEntityTarget()->GetId(), new_rotation);
	}
				
	

	((Element *)GetEntityTarget())->SetOrientation(new_rotation.x, new_rotation.y, new_rotation.z);
		
	IHandler::AdaptHandlerAndBrothersToModel(true, accumulative_rotation);

	return true;
}


void RotationGizmo::Update(bool subject_deleted)
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
		IHandler::AdaptHandlerAndBrothersToModel(true, accumulative_rotation);
}


void RotationGizmo::Reset()
{

	accumulative_rotation.x = accumulative_rotation.y = accumulative_rotation.z = 0.0f;
}

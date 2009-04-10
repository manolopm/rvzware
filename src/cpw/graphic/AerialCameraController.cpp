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
#include <stdlib.h>

//#include <stdafx.h>

#include <cpw/graphic/AerialCameraController.h>
#include <cpw/graphic/INavigator.h>
#include <cpw/graphic/ApplicationScene.h>
#include <cpw/common/ApplicationLog.h>




using namespace cpw;




AerialCameraController::AerialCameraController(void): camera(), left_button_pressed(false), middle_button_pressed(false), right_button_pressed(false), 
													  free_view_dragging(false), navigator_manager(NULL), ctrl_key_pressed(false), window_size_x(0), window_size_y(0),
													  left_shift_key_pressed(false)
{
	
}

AerialCameraController::AerialCameraController(const AerialCameraController &acc) : camera(acc.camera), navigator_manager(acc.navigator_manager),
																					window_size_x(acc.window_size_x), window_size_y(acc.window_size_y)	
{
	
	free_view_dragging = left_button_pressed = middle_button_pressed = right_button_pressed = ctrl_key_pressed = left_shift_key_pressed = false;
}

AerialCameraController::~AerialCameraController(void)
{
}

bool AerialCameraController::MouseLeftDoubleClick(const int &x, const int &y)
{
	INavigator *navigator = navigator_manager->GetFocusedOrFirstNavigator();

	if (navigator != NULL)
	{
		cpw::Point3d<float> i_pointf;
		cpw::Point3d<double> i_pointd;
		
		if (navigator->IntersectMouseWithScene(x, y, i_pointf.x, i_pointf.y, i_pointf.z))
		{
			i_pointd.x = (double) i_pointf.x;
			i_pointd.y = (double) i_pointf.y;
			i_pointd.z = (double) i_pointf.z;

			camera->GoToWithNoUpViewChanges(i_pointd, 3.0);
			return true;
		}
	}

	return false;
}

void AerialCameraController::GoTo(const cpw::Point3d<float> &position, const double &time)
{
	cpw::Point3d<double> pos(position.x, position.y, position.z);
	camera->GoToPanoramic(pos, time);	
}
bool AerialCameraController::MouseRightDoubleClick(const int &x,const int &y)
{
	return false;
}
bool AerialCameraController::MouseMiddleDoubleClick(const int &x,const int &y)
{
	return false;
}
			
bool AerialCameraController::MouseLeftButtonDown(const int &x,const int &y)
{
	bool collision = false;
	INavigator *navigator = navigator_manager->GetFocusedOrFirstNavigator();
	cpw::Point3d<float> i_pointf;
	cpw::Point3d<double> i_pointd;

	left_button_pressed = true;

	if (navigator != NULL)
	{
		if (navigator->IntersectMouseWithScene(x, y, i_pointf.x, i_pointf.y, i_pointf.z))
		{
			i_pointd.x = (double) i_pointf.x;
			i_pointd.y = (double) i_pointf.y;
			i_pointd.z = (double) i_pointf.z;

			collision = true;
		}
		else
			i_pointd = camera->GetUTMPosition();
	}
	
	if (right_button_pressed && collision)
	{
		//start rotation around a point
		camera->NewRotationPoint(i_pointd);		
		return true;
	}
	/*else if (collision)
	{
		camera->StartXYTraslation(x, y);
		return true;
	}*/
	

	return false;

}
bool AerialCameraController::MouseRightButtonDown(const int &x,const int &y)
{
	right_button_pressed = true;
	bool collision = false;

	camera->StopXYTraslation();

	if ((left_button_pressed) && (navigator_manager != NULL) && (camera != NULL))
	{
		//start rotation around a point
		INavigator *navigator = navigator_manager->GetFocusedOrFirstNavigator();

		if (navigator != NULL)
		{
			cpw::Point3d<float> i_pointf;
			cpw::Point3d<double> i_pointd;
			
			if (navigator->IntersectMouseWithScene(x, y, i_pointf.x, i_pointf.y, i_pointf.z))
			{
				i_pointd.x = (double) i_pointf.x;
				i_pointd.y = (double) i_pointf.y;
				i_pointd.z = (double) i_pointf.z;
				
				collision = true;
			}
			else
				i_pointd = camera->GetUTMPosition();
			
			if (collision)
				camera->NewRotationPoint(i_pointd);			

			return true;
		}
	}

	return false;
}
bool AerialCameraController::MouseMiddleButtonDown(const int &x,const int &y)
{
	return false;
}

bool AerialCameraController::MouseLeftButtonUp(const int &x,const int &y)
{
	left_button_pressed = false;

	camera->StopViewSceneCollisionPointVerticalRotation();
	camera->StopViewSceneCollisionPointHorizontalRotation();
	camera->StopXYTraslation();
	free_view_dragging = false;
	return true;
	
}
bool AerialCameraController::MouseRightButtonUp(const int &x,const int &y)
{
	right_button_pressed = false;

	camera->StopViewSceneCollisionPointVerticalRotation();
	camera->StopViewSceneCollisionPointHorizontalRotation();
	free_view_dragging = false;
	return true;
	
}
bool AerialCameraController::MouseMiddleButtonUp(const int &x,const int &y)
{
	return false;
}

void AerialCameraController::UpdateMouse(const MouseButtonsState &mbs)
{
	left_button_pressed = mbs.mouse_left_down;
	middle_button_pressed = mbs.mouse_middle_down;
	right_button_pressed = mbs.mouse_right_down;
}

bool AerialCameraController::MouseDrag(const int &x0, const int &y0, const int &x1, const int &y1, const MouseButtonsState &mbs)
{
	UpdateMouse(mbs);

	navigator_manager->GetFocusedOrFirstNavigator()->SetKeepMouseInsideCanvas(true);

	if (camera == NULL)
		return false;

	if (left_button_pressed && !right_button_pressed)
	{
		//camera->Strafe((double)(x1 - x0));
		//camera->PlaneXTranslation((double)x1-x0);
		//camera->PlaneYTranslation((double)y1-y0);

		if (camera->TraslatingXY())
			camera->UpdateXYPositionWithMouseCollision(x0, y0, x1, y1);
		else
			camera->StartXYTraslation(x1, y1);

		return true;
	}

	if (!left_button_pressed && right_button_pressed)
	{
		camera->GoForward((double)(y0 - y1)*0.6, false, true);
		return true;
	}

	if (left_button_pressed && right_button_pressed)
	{
		//double x_rot, y_rot;
		//NormalizeRotationToScreenSize(x0-x1, y0-y1, x_rot, y_rot);
		camera->StopXYTraslation();
		
		camera->StartViewSceneCollisionPointHorizontalRotation(((double)(x0-x1))*0.1, false);
		camera->StartViewSceneCollisionPointVerticalRotation(((double)(y0-y1))*0.1, false);

		//camera->StartViewSceneCollisionPointHorizontalRotation(x_rot, false);
		//camera->StartViewSceneCollisionPointVerticalRotation(y_rot, false);
		free_view_dragging = true;
		return true;

	}

	return false;
	
}
bool AerialCameraController::MouseWheel(const int &wheel_rotation, const int &wheel_delta, const int &x, const int &y)
{
	double w_rotation = ((double)wheel_rotation) / ((double)wheel_delta);
	
	INavigator *navigator = navigator_manager->GetFocusedOrFirstNavigator();

	if (navigator != NULL)
	{
		cpw::Point3d<float> i_pointf;
		cpw::Point3d<double> i_pointd;

		if (navigator->IntersectMouseWithScene(x, y, i_pointf.x, i_pointf.y, i_pointf.z))
		{
			//i_pointf = cpw::ApplicationScene::GetInstance()->GetScene()->UTMToSceneCoords(i_pointf);
		
			i_pointd.x = (double) i_pointf.x;
			i_pointd.y = (double) i_pointf.y;
			i_pointd.z = (double) i_pointf.z;

			if (w_rotation > 0.0)
				camera->ZoomToPoint(i_pointd, 0.35, 0.3, true);
			else
				camera->ZoomToPoint(i_pointd, 0.35, 0.3, false);

			return true;
			
		}
	}

	return true;
}
					
bool AerialCameraController::KeyDown(const int &key)
{
	bool key_computed = false;

	if (key == KEY_LCTRL)
		ctrl_key_pressed = true;

	if (key == KEY_LEFT_SHIFT)
		left_shift_key_pressed = true;


	if (key == KEY_CURSOR_DOWN)
	{
		if (left_shift_key_pressed)
			camera->RotateVertical(-10.0f, true);
		else if (ctrl_key_pressed)
			camera->StartViewSceneCollisionPointVerticalRotation(-2.0, true);		
		else
			camera->StartContinousPlaneYTranslation(-200, true);

		key_computed = true;
	}

	if (key == KEY_CURSOR_UP)
	{
		if (left_shift_key_pressed)
			camera->RotateVertical(10.0f, true);
		else if (ctrl_key_pressed)
			camera->StartViewSceneCollisionPointVerticalRotation(2.0, true);
		else
			camera->StartContinousPlaneYTranslation(200, true);

		key_computed = true;
	}

	if (key == KEY_CURSOR_LEFT)
	{
		if (left_shift_key_pressed)
			camera->RotateHorizontal(25.0f, true);
		else if (ctrl_key_pressed)
			camera->StartViewSceneCollisionPointHorizontalRotation(5.0, true);
		else
			camera->StartContinousPlaneXTranslation(200, true);

		key_computed = true;
	}

	if (key == KEY_CURSOR_RIGHT)
	{
		if (left_shift_key_pressed)
			camera->RotateHorizontal(-25.0f, true);
		else if (ctrl_key_pressed)
			camera->StartViewSceneCollisionPointHorizontalRotation(-5.0, true);
		else
			camera->StartContinousPlaneXTranslation(-200, true);

		key_computed = true;
	}

	if (key == KEY_A)
	{
		camera->StartGainHeightMovementWithInertia(1);
		key_computed = true;
	}

	if (key == KEY_Z)
	{
		camera->StartGainHeightMovementWithInertia(-1);
		key_computed = true;
	}

	return key_computed;

}
bool AerialCameraController::KeyUp(const int &key)
{
	bool key_computed = false;

	if (key == KEY_LCTRL)
		ctrl_key_pressed = false;

	if (key == KEY_LEFT_SHIFT)
		left_shift_key_pressed = false;

	if ((key == KEY_CURSOR_DOWN) || (key == KEY_CURSOR_UP))
	{
		//if (ctrl_key_pressed)
			camera->StopViewSceneCollisionPointVerticalRotation();
		//else
			camera->StopContinousPlaneYTranslation();
			key_computed = true;
	}

	if ((key == KEY_CURSOR_LEFT) || (key == KEY_CURSOR_RIGHT))
	{
		//if (ctrl_key_pressed)
			camera->StopViewSceneCollisionPointHorizontalRotation();
		//else
			camera->StopContinousPlaneXTranslation();
			key_computed = true;
	}

	if ((key == KEY_A) || (key == KEY_Z))
	{
		camera->StopGainHeightMovementWithInertia();
		key_computed = true;
	}

	return key_computed;
}

			


bool AerialCameraController::MouseLeaveWindow(const int &x0, const int &y0)
{
	camera->StopViewSceneCollisionPointVerticalRotation();
	camera->StopViewSceneCollisionPointHorizontalRotation();
	camera->StopXYTraslation();
	free_view_dragging = false;
	left_button_pressed = middle_button_pressed = right_button_pressed = false;
	return true;

}
bool AerialCameraController::MouseEnterWindow(const int &x0, const int &y0)
{
	return false;
}

bool AerialCameraController::WindowResize(const int &x, const int &y)
{
	window_size_x = x;
	window_size_y = y;
	return true;

}

void AerialCameraController::NormalizeRotationToScreenSize(const int &x_increment, const int &y_increment, double &x_normalized, double &y_normalized)
{
	double window_size_xd = (double) window_size_x;
	double window_size_yd = (double) window_size_y;
	double x_incrementd = (double) x_increment;
	double y_incrementd = (double) y_increment;
	
	x_normalized = (x_incrementd * 360.0) / window_size_xd;
	y_normalized = (y_incrementd * 360.0) / window_size_yd;
	
}
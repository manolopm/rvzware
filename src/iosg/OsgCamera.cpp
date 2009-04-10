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

#include <math.h>
#include <osgDB/ReadFile>
#include <osg/Node>
#include <osg/Depth>

#include "stdafx.h"

#include <cpw/graphic/ApplicationScene.h>
#include <cpw/common/ApplicationLog.h>
#include <iosg/OsgCamera.h>





using namespace cpw::iosg;

const std::string rotate_pivot_id("osg_rotate_pivot");
const std::string camera_group_id("osg_camera_group");
//const double disp = 150.0; //mimimun distance between terrain and camera
// el valor original es 150
//const double disp = 10.0;
const float max_traslation = 20000.0f;



OsgCamera::OsgCamera(void) : disp(150.0),
							 vertical_rotation_deacceleration(1.0),
                             horizontal_rotation_deacceleration(2.0),
							 view_collision_horizontal_rotation_deacceleration(10.0),
							 view_collision_vertical_rotation_deacceleration(10.0),
							 following_line_deacceleration(300000.0),
							 view_rotation_deacceleration(10.0),
							 vertical_rotation_speed(0.038),
							 //view_collision_horizontal_rotation_speed(0.038),
							 //view_collision_vertical_rotation_speed(0.038),
							 view_collision_horizontal_rotation_speed(1.0),
							 view_collision_vertical_rotation_speed(1.0),
							 view_collision_horizontal_rotation_current_speed(0.0),
							 view_collision_vertical_rotation_current_speed(0.0),
							 horizontal_rotation_speed(0.038),
							 view_rotation_speed(1.0),
							 vertical_rotation_current_speed(0.0),
							 horizontal_rotation_current_speed(0.0),
							 view_rotation_current_speed(0.0),
							 vertical_rotation_max_speed(10.0),
							 view_collision_horizontal_rotation_max_speed(10.0),
							 view_collision_vertical_rotation_max_speed(10.0),
							 horizontal_rotation_max_speed(10.0),
							 view_rotation_max_speed(10.0),
							 position(0.0, 0.0, 0.0),
							 up(0.0, 1.0, 0.0),
							 view(0.0, 0.0, 1.0),
							 forward_deacceleration(300000.0),
							 forward_max_speed(5000.0),
							 following_line_max_speed(5000.0),
							 forward_speed(250.0),
							 following_line_speed(250.0),
							 following_line_current_speed(0.0),
							 forward_current_speed(0.0),
							 planex_current_speed(0.0), 
							 planex_deacceleration(100000.0), 
							 planex_max_speed(5000.0), 
							 planex_speed(100.0),
							 planey_current_speed(0.0), 
							 planey_deacceleration(100000.0), 
							 planey_max_speed(5000.0), 
							 planey_speed(100.0),
							 movement_constant_multiplier(1.0),
							 max_movement_constant_multiplier(5.0),
							 up_speed(250.0),
							 up_current_speed(0.0),
							 up_deacceleration(1000000.0),
							 strafe_speed(250.0),
							 strafe_current_speed(0.0),
							 strafe_deacceleration(1000000.0),
							 height_speed(20000.0),
							 height_max_speed(5000.0),
							 height_deacceleration(10000.0),
							 current_height_multiplier(0.0),
							 current_height_speed(0.0),
							 min_height(1100.0),
							 max_height(355500.0),
							 min_height_multiplier(0.05),
							 max_height_multiplier(5.0),
							 going_to(false),
							 rotating(false),
							 correct_up(true),
							 forward_inertia(false),
							 rotate_vertical_inertia(false),
							 rotate_horizontal_inertia(false),
							 strafe_inertia(false),
							 up_inertia(false),
							 rotate_view_inertia(false),
							 horizontal_continuous_rotation(false),
							 vertical_continuous_rotation(false),
							 forward_continous_movement(false),
							 gain_height_continous_movement(false), 
							 view_collision_horizontal_continous_rotation(false),
							 view_collision_vertical_continous_rotation(false),
							 ip_valid(false), 
							 rotation_interrupted(false),
							 osg_init(false),
							 rotation_icon_size_factor(0.0),
							 sn(NULL),
							 mtn(NULL),
							 going_to_no_uav_changes(false),
							 panoramic_goto_final_height(100000),
							 going_to_panoramic(false),
							 going_to_just_interpolate(false),
							 plane_x_continous_movement(false),
							 plane_y_continous_movement(false),
							 forward_stop_on_collision(false),
							 following_line_inertia(true),
							 osg_navigator(NULL),
							 traslating_xy(false),
							 osg_traslate_init(false),
							 zooming_to_point(false),
							 snt(NULL),
							 osg_zooming_init(false),
							 roof(1150000.0),
							 //PJ//x_limits(-2500000, 2500000),
							 //PJ//y_limits(-2500000, 2500000)
 
							 x_limits(-9500000, 9500000),
							 y_limits(-9500000, 9500000)

{
	last_t = osg::Timer::instance()->tick();

	xy_plane.MakeFrom3Points(cpw::Point3d<double>(0.0, 0.0, 0.0),
							 cpw::Point3d<double>(1.0, 0.0, 0.0),
							 cpw::Point3d<double>(0.0, 1.0, 0.0));

	line_to_follow.x = 0.0;
	line_to_follow.y = 0.0;
	line_to_follow.z = 0.0;

}

OsgCamera::~OsgCamera(void) 
{
}

cpw::Point3d<double> OsgCamera::GetUTMPosition()
{
	return OsgScene::SceneCoordsToUTM(position);
}


void OsgCamera::UpdateSpeed(const double &new_speed, double &current_speed)
{
	if (current_speed == 0.0)
	{
		current_speed = new_speed;
		return;
	}

	if (current_speed > 0.0)
	{
		if (new_speed > current_speed)
			current_speed = new_speed;
		else 
			if (new_speed < 0.0)
				current_speed = new_speed;
	}
	else
	{
		if (new_speed < current_speed)
			current_speed = new_speed;
		else
			if (new_speed >= 0.0)
				current_speed = new_speed;
	}

}

void OsgCamera::StartViewSceneCollisionPointHorizontalRotation(const double &mult, const bool &inertia)
{
	view_collision_horizontal_inertia = inertia;
	view_collision_horizontal_rotation_continous_mult = mult;
	view_collision_horizontal_continous_rotation = true;
}

void OsgCamera::StartViewSceneCollisionPointVerticalRotation(const double &mult, const bool &inertia)
{
	view_collision_vertical_inertia = inertia;
	view_collision_vertical_rotation_continous_mult = mult;
	view_collision_vertical_continous_rotation = true;
}

void OsgCamera::StartHorizontalRotationWithInertia(const double &mult)
{
	horizontal_continuous_rotation = true;
	horizontal_rotation_continous_mult = mult;
	rotate_horizontal_inertia = true;
}

void OsgCamera::Zoom(const cpw::Point3d<double> &point, const double &mult)
{
	line_to_follow.x = point.x - position.x;
	line_to_follow.y = point.y - position.y;
	line_to_follow.z = point.z - position.z;

	cpw::Math::NormalizeVec(line_to_follow);

	UpdateSpeed(following_line_speed * mult, following_line_current_speed);

}
void OsgCamera::StartVerticalRotationWithInertia(const double &mult)
{
	vertical_continuous_rotation = true;
	vertical_rotation_continous_mult = mult;
	rotate_vertical_inertia = true;
}

void OsgCamera::StartForwardMovementWithInertia(const double &mult)
{
	forward_continous_movement = true;
	forward_continous_movement_mult = mult;
	forward_inertia = true;
}

void OsgCamera::StartGainHeightMovementWithInertia(const double &mult)
{
	gain_height_continous_movement = true;
	gain_height_continous_mult = mult;
	gain_height_inertia = true;
}
void OsgCamera::SetMovementConstantMultiplier(const double &new_multiplier)
{ 
	if (new_multiplier <= 0.0) 
		return; 

	if (new_multiplier > max_movement_constant_multiplier)
	{
		movement_constant_multiplier = max_movement_constant_multiplier;
		return;
	}
	
	movement_constant_multiplier = new_multiplier; 
}
void OsgCamera::RotateVertical(const double &multiplier, const bool &inertia)
{
	UpdateSpeed(vertical_rotation_speed * multiplier, vertical_rotation_current_speed);
	rotate_vertical_inertia = inertia;
}
void OsgCamera::RotateHorizontal(const double &multiplier, const bool &inertia)
{
	UpdateSpeed(horizontal_rotation_speed * multiplier, horizontal_rotation_current_speed);
	rotate_horizontal_inertia = inertia;
}
void OsgCamera::RotateView(const double &multiplier, const bool &inertia)
{
	UpdateSpeed(view_rotation_speed * multiplier, view_rotation_current_speed);
	rotate_view_inertia = inertia;
}

void OsgCamera::GoForward(const double &multiplier, const bool &inertia, const bool &stop_on_collision)
{
	UpdateSpeed(forward_speed * multiplier, forward_current_speed);
	forward_inertia = inertia;
	forward_stop_on_collision = stop_on_collision;
}

void OsgCamera::PlaneXTranslation(const double &multiplier, const bool &inertia)
{
	UpdateSpeed(planex_speed * multiplier, planex_current_speed);
	planex_inertia = inertia;
}

void OsgCamera::PlaneYTranslation(const double &multiplier, const bool &inertia)
{
	UpdateSpeed(planey_speed * multiplier, planey_current_speed);
	planey_inertia = inertia;
}

void OsgCamera::StartContinousPlaneXTranslation(const double &multiplier, const bool &inertia)
{
	plane_x_continous_movement = true;
	UpdateSpeed(planex_speed * multiplier, planex_current_speed);
	planex_inertia = inertia;
}
void OsgCamera::StartContinousPlaneYTranslation(const double &multiplier, const bool &inertia)
{
	plane_y_continous_movement = true;
	UpdateSpeed(planey_speed * multiplier, planey_current_speed);
	planey_inertia = inertia;
}
void OsgCamera::StopContinousPlaneXTranslation()
{
	plane_x_continous_movement = false;
	planex_inertia = false;
	planex_current_speed = 0.0;
	
}
void OsgCamera::StopContinousPlaneYTranslation()
{
	plane_y_continous_movement = false;
	planey_inertia = false;
	planey_current_speed = 0.0;
}


void OsgCamera::GoUp(const double &multiplier, const bool &inertia)
{
	UpdateSpeed(up_speed * multiplier, up_current_speed);
	up_inertia = inertia;
}

void OsgCamera::Strafe(const double &multiplier, const bool &inertia)
{
	UpdateSpeed(strafe_speed * multiplier, strafe_current_speed);
	strafe_inertia = inertia;
}

void OsgCamera::GainHeight(const double &multiplier)
{
	UpdateSpeed(height_speed * multiplier, current_height_speed);
}
void OsgCamera::LooseHeight(const double &multiplier)
{
	UpdateSpeed(height_speed * (multiplier * -1.0), current_height_speed);
}



void OsgCamera::UpdateTimer()
{
	actual_t = osg::Timer::instance()->tick();
	time_between_frames = (actual_t - last_t) * osg::Timer::instance()->getSecondsPerTick();
	last_t = actual_t;
}

void OsgCamera::Update()
{
	cpw::Point3d<double> new_position, intersection_point;
	double frame_displacement;
	actual_t = osg::Timer::instance()->tick();
	time_between_frames = (actual_t - last_t) * osg::Timer::instance()->getSecondsPerTick();
	last_t = actual_t;

	/*if (time_between_frames > 0.05)
	{
		time_between_frames = 0.1;
		cpw::ApplicationLog::GetInstance()->GetLogger()->NewLogMessage("frenando cámara");
	}*/

	//UpdatePivotSize(time_between_frames);

	if (going_to)
	{
		UpdateGoingTo(time_between_frames);
		return;
	}

	if (going_to_no_uav_changes)
	{
		UpdateGoingToNoUAVChanges(time_between_frames);
		return;
	}

	if (going_to_panoramic)
	{
		UpdateGoingToPanoramic(time_between_frames);
		return;
	}

	if (zooming_to_point)
	{
		UpdateZoomToPoint(time_between_frames);
		return;
	}

	if (going_to_just_interpolate)
	{
		UpdateGoingToJustInterpolate(time_between_frames);
		return;
	}

	if (horizontal_continuous_rotation)
		UpdateSpeed(horizontal_rotation_speed * horizontal_rotation_continous_mult ,horizontal_rotation_current_speed);

	if (vertical_continuous_rotation)
		UpdateSpeed(vertical_rotation_speed * vertical_rotation_continous_mult ,vertical_rotation_current_speed);

	if (forward_continous_movement)
		UpdateSpeed(forward_speed * forward_continous_movement_mult, forward_current_speed);

	if (gain_height_continous_movement)
		UpdateSpeed(height_speed * gain_height_continous_mult, current_height_speed);

	if (view_collision_horizontal_continous_rotation)
		UpdateSpeed(view_collision_horizontal_rotation_speed * view_collision_horizontal_rotation_continous_mult, view_collision_horizontal_rotation_current_speed);

	if (view_collision_vertical_continous_rotation)
		UpdateSpeed(view_collision_vertical_rotation_speed * view_collision_vertical_rotation_continous_mult, view_collision_vertical_rotation_current_speed);
	

	if (horizontal_rotation_current_speed != 0.0)
	{
		cpw::Math::NormalizeVec(up);
		cpw::Math::NormalizeVec(view);
		
		//if (rotate_horizontal_inertia)
		//cpw::Math::RotatePoint(view, up, horizontal_rotation_current_speed);
		cpw::Math::RotatePoint(view, cpw::Point3d<double>(0.0, 0.0, 1.0), horizontal_rotation_current_speed);
		cpw::Math::RotatePoint(up, cpw::Point3d<double>(0.0, 0.0, 1.0), horizontal_rotation_current_speed);
		//else

		cpw::Math::NormalizeVec(view);
		cpw::Math::NormalizeVec(up);
				
		if (rotate_horizontal_inertia)
		{
			SlowDown(horizontal_rotation_current_speed,
					horizontal_rotation_deacceleration,
					time_between_frames);
		}
		else
			horizontal_rotation_current_speed = 0.0;

	}

	if (vertical_rotation_current_speed != 0.0)
	{
		cpw::Point3d<double> new_up(up);
		cpw::Math::NormalizeVec(up);
		cpw::Math::NormalizeVec(view);
		cpw::Point3d<double> left = cpw::Math::CrossProduct(view, up);
		cpw::Math::NormalizeVec(left);
		
		cpw::Math::RotatePoint(new_up, left, vertical_rotation_current_speed);

		if (new_up.z > 0.0)
		{
			up = new_up;
			cpw::Math::NormalizeVec(up);
			cpw::Math::RotatePoint(view, left, vertical_rotation_current_speed);
			cpw::Math::NormalizeVec(view);
		}
		
		

		
		if (rotate_vertical_inertia)
		{
			SlowDown(vertical_rotation_current_speed,
					vertical_rotation_deacceleration,
					time_between_frames);
		}
		else
			vertical_rotation_current_speed = 0.0;
	}

	if (view_rotation_current_speed != 0.0)
	{
		cpw::Math::NormalizeVec(up);
		cpw::Math::NormalizeVec(view);
		cpw::Math::RotatePoint(up, view, view_rotation_current_speed);
		cpw::Math::NormalizeVec(up);
		
		if (rotate_view_inertia)
		{
			SlowDown(view_rotation_current_speed,
					view_rotation_deacceleration,
					time_between_frames);
		}
		else
			view_rotation_current_speed = 0.0;
	}

	if (view_collision_horizontal_rotation_current_speed != 0.0)
	{
		/*if (!ip_valid)
			GetIntersectionViewWithScene();*/

		if (ip_valid)
		{

			cpw::Point3d<double> position_relative_to_ip = position - view_intersection_point;
			
			//rotate around the intersection point
			cpw::Math::NormalizeVec(view);
			cpw::Math::NormalizeVec(up);
			
			cpw::Math::RotatePoint(position_relative_to_ip, cpw::Point3d<double>(0.0, 0.0, 1.0), view_collision_horizontal_rotation_current_speed);

			cpw::Point3d<double> new_position = position_relative_to_ip + view_intersection_point;
			cpw::Point3d<double> new_up = up;

			cpw::Math::RotatePoint(new_up, cpw::Point3d<double>(0.0, 0.0, 1.0), view_collision_horizontal_rotation_current_speed); 

			//if ((new_position.z < view_intersection_point.z) || ( new_up.z < 0.0))
			if ( new_up.z < 0.0 )
			{
			}
			else if (RotationPossible(position, new_position))
			{
				cpw::Math::RotatePoint(view, cpw::Point3d<double>(0.0, 0.0, 1.0), view_collision_horizontal_rotation_current_speed); 
				cpw::Math::RotatePoint(up, cpw::Point3d<double>(0.0, 0.0, 1.0), view_collision_horizontal_rotation_current_speed); 
				cpw::Math::NormalizeVec(view);
				cpw::Math::NormalizeVec(up);

				position = new_position;

				//UpdatePositionAccordingToCollisions(position, new_position);
			}
		}

		if (view_collision_horizontal_inertia)
		{
			SlowDown(view_collision_horizontal_rotation_current_speed,
					view_collision_horizontal_rotation_deacceleration,
					time_between_frames);
		}
		else
		{
			view_collision_horizontal_rotation_current_speed = 0.0;
			view_collision_horizontal_continous_rotation = false;
		}
	}

	if (view_collision_vertical_rotation_current_speed != 0.0)
	{
		/*if (!ip_valid)
			GetIntersectionViewWithScene();*/

		if (ip_valid)
		{
			cpw::Point3d<double> position_relative_to_ip = position - view_intersection_point;
			
			//rotate around the intersection point
			cpw::Math::NormalizeVec(view);
			cpw::Math::NormalizeVec(up);
			cpw::Point3d<double> left = cpw::Math::CrossProduct(view, up);
			cpw::Math::NormalizeVec(left);
		
			cpw::Math::RotatePoint(position_relative_to_ip, left, view_collision_vertical_rotation_current_speed);

			cpw::Point3d<double> new_position = position_relative_to_ip + view_intersection_point;
			cpw::Point3d<double> new_up=up;
			cpw::Math::RotatePoint(new_up, left, view_collision_vertical_rotation_current_speed); 

			//if ((new_position.z < view_intersection_point.z) || (new_up.z < 0.0))
			if (new_up.z < 0.0)
			{

			}
			else if (RotationPossible(position, new_position) || (new_position.z > position.z))
			{
				cpw::Math::RotatePoint(view, left, view_collision_vertical_rotation_current_speed); 
				cpw::Math::RotatePoint(up, left, view_collision_vertical_rotation_current_speed); 
				cpw::Math::NormalizeVec(view);
				cpw::Math::NormalizeVec(up);
				position = new_position;
				//UpdatePositionAccordingToCollisions(position, new_position);
			}
		}

		if (view_collision_vertical_inertia)
		{
			SlowDown(view_collision_vertical_rotation_current_speed,
					view_collision_vertical_rotation_deacceleration,
					time_between_frames);
				
		}
		else
		{
			view_collision_vertical_rotation_current_speed = 0.0;
			view_collision_vertical_continous_rotation = false;
		}
	}

	//update position
	UpdateHeightMultiplier();

	if (forward_current_speed != 0.0)
	{
		if (forward_inertia)
			frame_displacement = time_between_frames * forward_current_speed * current_height_multiplier * movement_constant_multiplier;
		else
			frame_displacement = forward_current_speed * current_height_multiplier * movement_constant_multiplier;

		
		cpw::Math::NormalizeVec(view);

		new_position.x = (view.x * frame_displacement) + position.x;
		new_position.y = (view.y * frame_displacement) + position.y;
		new_position.z = (view.z * frame_displacement) + position.z;

		if (forward_stop_on_collision)
		{
			if (!MovementCollides(position, new_position))
				position = new_position;
		}
		else
			UpdatePositionAccordingToCollisions(position, new_position);		

		if (forward_inertia)
			SlowDown(forward_current_speed, forward_deacceleration + (forward_deacceleration * current_height_multiplier), time_between_frames);
		else
			forward_current_speed = 0.0;
	}
	
	if (following_line_current_speed != 0.0)
	{
		if (following_line_inertia)
			frame_displacement = time_between_frames * following_line_current_speed * current_height_multiplier * movement_constant_multiplier;
		else
			frame_displacement = following_line_current_speed * current_height_multiplier * movement_constant_multiplier;

		new_position.x = (line_to_follow.x * frame_displacement) + position.x;
		new_position.y = (line_to_follow.y * frame_displacement) + position.y;
		new_position.z = (line_to_follow.z * frame_displacement) + position.z;

		if (!MovementCollides(position, new_position))
			position = new_position;
		
		if (following_line_inertia)
			SlowDown(following_line_current_speed, following_line_deacceleration + (following_line_deacceleration * current_height_multiplier), time_between_frames);
		else
			following_line_current_speed = 0.0;
	}

	if (up_current_speed != 0.0)
	{
		if (up_inertia)
			frame_displacement = time_between_frames * up_current_speed * current_height_multiplier * movement_constant_multiplier;
		else
			frame_displacement = up_current_speed * current_height_multiplier * movement_constant_multiplier;
				
		cpw::Math::NormalizeVec(up);
		cpw::Math::NormalizeVec(view);
		
		new_position.x = (up.x * frame_displacement) + position.x;
		new_position.y = (up.y * frame_displacement) + position.y;
		new_position.z = (up.z * frame_displacement) + position.z;

		UpdatePositionAccordingToCollisions(position, new_position);		

		if (up_inertia)
			SlowDown(up_current_speed, up_deacceleration + (up_deacceleration * current_height_multiplier), time_between_frames);
		else
			up_current_speed = 0.0;
	}

	if (strafe_current_speed != 0.0)
	{
		if (strafe_inertia)
			frame_displacement = time_between_frames * strafe_current_speed * current_height_multiplier * movement_constant_multiplier;
		else
			frame_displacement = strafe_current_speed * current_height_multiplier * movement_constant_multiplier;


		cpw::Math::NormalizeVec(view);
		cpw::Math::NormalizeVec(up);
		cpw::Point3d<double> left = cpw::Math::CrossProduct(view, up);
		cpw::Math::NormalizeVec(left);

		new_position.x = position.x - (left.x * frame_displacement);
		new_position.y = position.y - (left.y * frame_displacement);
		new_position.z = position.z - (left.z * frame_displacement);

		UpdatePositionAccordingToCollisions(position, new_position);		

		if (strafe_inertia)
			SlowDown(strafe_current_speed, strafe_deacceleration + (strafe_deacceleration * current_height_multiplier), time_between_frames);
		else
			strafe_current_speed = 0.0;
	}

	if (current_height_speed != 0.0)
	{
		frame_displacement = time_between_frames * current_height_speed * current_height_multiplier * movement_constant_multiplier;

		new_position = position;
		new_position.z += 1.0 * frame_displacement;

		UpdatePositionAccordingToCollisions(position, new_position);

		if (gain_height_inertia)
			SlowDown(current_height_speed, height_deacceleration + (strafe_deacceleration * current_height_multiplier), time_between_frames);
		else
			current_height_speed = 0.0;

	}

	if (planex_current_speed != 0.0)
	{
		if (this->planex_inertia)
			frame_displacement = time_between_frames * planex_current_speed * current_height_multiplier * movement_constant_multiplier;
		else
			frame_displacement = planex_current_speed * current_height_multiplier * movement_constant_multiplier;

		cpw::Math::NormalizeVec(view);
		cpw::Math::NormalizeVec(up);
		cpw::Point3d<double> left = cpw::Math::CrossProduct(view, up);
		cpw::Math::NormalizeVec(left);
		cpw::Point3d<double> left_projection;

		if (left.z != 0.0) //check if the vec isn´t already in the XY PLANE
		{
			left_projection = xy_plane.ProjectVectorOnPlane(left);
			cpw::Math::NormalizeVec(left_projection);
		}
		else
			left_projection = left;
		
		new_position.x = position.x - (left_projection.x * frame_displacement);
		new_position.y = position.y - (left_projection.y * frame_displacement);
		new_position.z = position.z - (left_projection.z * frame_displacement);

		UpdatePositionAccordingToCollisions(position, new_position);

		if (!plane_x_continous_movement)
		{
			if (planex_inertia)
				SlowDown(planex_current_speed, planex_deacceleration + (planex_deacceleration * current_height_multiplier), time_between_frames);
			else
				planex_current_speed = 0.0;
		}
		
	}

	if (planey_current_speed != 0.0)
	{
		if (this->planey_inertia)
			frame_displacement = time_between_frames * planey_current_speed * current_height_multiplier * movement_constant_multiplier;
		else
			frame_displacement = planey_current_speed * current_height_multiplier * movement_constant_multiplier;

		cpw::Math::NormalizeVec(view);
		cpw::Point3d<double> view_projection;

		if ((up.x != 0.0) && (up.y != 0.0)) //check if the vec isn´t perpendicular to XY PLANE
		{
			view_projection = xy_plane.ProjectVectorOnPlane(up);
			cpw::Math::NormalizeVec(view_projection);
		}
		else
			view_projection = cpw::Point3d<double>(0.0, 1.0, 0.0);
		
		new_position.x = position.x + (view_projection.x * frame_displacement);
		new_position.y = position.y + (view_projection.y * frame_displacement);
		new_position.z = position.z + (view_projection.z * frame_displacement);

		UpdatePositionAccordingToCollisions(position, new_position);

		if (!plane_y_continous_movement)
		{
			if (planey_inertia)
				SlowDown(planey_current_speed, planey_deacceleration + (planey_deacceleration * current_height_multiplier), time_between_frames);
			else
				planey_current_speed = 0.0;
		}
	}

	if (correct_up) //up correction
		CorrectUp();


	//movement limits
	if (position.z <=1.0)
		position.z = 1.01;
	
	if (position.z > roof)
		position.z = roof;

	if (position.x > x_limits.max)
		position.x = x_limits.max;
	
	if (position.x < x_limits.min)
		position.x = x_limits.min;

	if (position.y > y_limits.max)
		position.y = y_limits.max;

	if (position.y < y_limits.min)
		position.y = y_limits.min;
}

void OsgCamera::SlowDown(double &speed, double deacceleration, const double &time)
{
	if (speed == 0.0)
		return;

	if (speed > 0.0)
	{
		speed -= time * deacceleration;
			
		if (speed < 0.0)
			speed = 0.0;
	}
	else
	{
		speed += time * deacceleration;
			
		if (speed > 0.0)
			speed = 0.0;
	}
}

void OsgCamera::UpdateHeightMultiplier()
{
	double current_height = (double) position.z;

	if (current_height < min_height)
	{
		current_height_multiplier = min_height_multiplier;
		return;
	}

	if (current_height > max_height)
	{
		current_height_multiplier = max_height_multiplier;
		return;
	}

	double aux1 = current_height - min_height;
	double aux2 = max_height - min_height;

	current_height_multiplier= ((aux1 / aux2) * (max_height_multiplier - min_height_multiplier)) + min_height_multiplier;
}


void OsgCamera::GoTo(const cpw::Point3d<double> &place, const double &arrive_time)
{
	going_to = true;
	rotating = true;
	time_to_arrive = arrive_time;
	rotating_time = arrive_time;

	place_to_start = position;
	current_increment_mult = 0.0;
	acceleration_time = arrive_time * 0.07;
	deacceleration_time = arrive_time * 0.8;
	current_going_to_time = 0.0;

	//the final position
	cpw::Point3d<double> fp;
	fp.x = place.x - position.x;
	fp.y = place.y - position.y;
	fp.z = place.z - position.z;
	cpw::Math::NormalizeVec(fp);

	place_to_go.x = place.x - (2000.0 * fp.x);
	place_to_go.y = place.y - (2000.0 * fp.y);
	place_to_go.z = place.z - (2000.0 * fp.z);//) + 2000.0;

	place_to_look.x = place.x;
	place_to_look.y = place.y;
	place_to_look.z = place.z;

	go_to_view_vec.x = place_to_look.x - place_to_go.x;
	go_to_view_vec.y = place_to_look.y - place_to_go.y;
	go_to_view_vec.z = place_to_look.z - place_to_go.z;

	cpw::Math::NormalizeVec(go_to_view_vec);
}

void OsgCamera::GoToJustInterpolate(const cpw::Point3d<double> &_final_position, const cpw::Point3d<double> &_final_view, 
						            const cpw::Point3d<double> &_final_up, const double &arrive_time)
{
	final_up_ji = _final_up;
	final_view_ji = _final_view;
	final_position_ji = _final_position;

	initial_position_ji = position;
	initial_up_ji = up;
	initial_view_ji = view;
		
	going_to_just_interpolate = true;
	
	time_to_arrive = arrive_time;
	
	place_to_start = position;
	current_going_to_time = 0.0;

	position_increment = final_position_ji - initial_position_ji;
	view_increment = final_view_ji - initial_view_ji;
	up_increment = final_up_ji - initial_up_ji;
}

void OsgCamera::UpdateGoingToJustInterpolate(const double &time)
{
	current_going_to_time += time;

	if (current_going_to_time >= time_to_arrive)
	{
		going_to_just_interpolate = false;
		position = final_position_ji;
		view = final_view_ji;
		up = final_up_ji;

		double terrain_height = GetTerrainHeightUnderCamera();
	
		if ((position.z - terrain_height) < disp)
			position.z = terrain_height + disp;

		//current_going_to_time = time_to_arrive;
		SetAllCurrentSpeedsToCero();
		
		return;
	}

	

	double time_left = time_to_arrive - current_going_to_time;
	
	////////// inertial movement
	double proportional_time_left = time_left / time_to_arrive;
	double proportional_animation_time = cos(proportional_time_left * osg::PI);
	proportional_animation_time = (proportional_animation_time + 1) * 0.5;
	////////////////////////////

	up.x = initial_up_ji.x + up_increment.x * proportional_animation_time;
	up.y = initial_up_ji.y + up_increment.y * proportional_animation_time;
	up.z = initial_up_ji.z + up_increment.z * proportional_animation_time;

	view.x = initial_view_ji.x + view_increment.x * proportional_animation_time;
	view.y = initial_view_ji.y + view_increment.y * proportional_animation_time;
	view.z = initial_view_ji.z + view_increment.z * proportional_animation_time;

	position.x = initial_position_ji.x + position_increment.x * proportional_animation_time;
	position.y = initial_position_ji.y + position_increment.y * proportional_animation_time;
	position.z = initial_position_ji.z + position_increment.z * proportional_animation_time;

	double terrain_height = GetTerrainHeightUnderCamera();
	
	if ((position.z - terrain_height) < disp)
		position.z = terrain_height + disp;
		
}


void OsgCamera::UpdateGoingTo(double elapsed_time)
{
	if ((time_to_arrive < 0.0) || (time_to_arrive < elapsed_time))
	{
		going_to = false;
		SetAllCurrentSpeedsToCero();
		
		return;
	}

	//compute the increment mult
	if (current_going_to_time < acceleration_time) //accelerating
	{
		//current_increment_mult = (1.0 - (current_going_to_time / acceleration_time)) * 10;
		double temp = current_going_to_time / acceleration_time;
		if (temp < 0.05)
			temp = 0.05;
		elapsed_time *= temp;
	}
	else if ((current_going_to_time > acceleration_time) && (current_going_to_time < deacceleration_time)) //constant speed
	{
		
	}
	else  //deaccelerating
	{
		double ttime = current_going_to_time + time_to_arrive;
		double t = current_going_to_time - deacceleration_time;
		double t2 = ttime - deacceleration_time;
		t2 = 1.0 - (t / t2);

		elapsed_time *= t2;
		if (elapsed_time < 0.001)
			elapsed_time = 0.001;
		
	}

	cpw::Math::NormalizeVec(up);
	cpw::Math::NormalizeVec(view);

	if (rotating)
	{
		cpw::Point3d<double> p1, p2, p3, left;
		cpw::Plane plane1, plane2;

		//rotation around up
		p1.x = position.x;
		p1.y = position.y;
		p1.z = position.z;

		p2.x = position.x + (up.x * 100.0);
		p2.y = position.y + (up.y * 100.0);
		p2.z = position.z + (up.z * 100.0);

		p3.x = position.x + (view.x * 100.0);
		p3.y = position.y + (view.y * 100.0);
		p3.z = position.z + (view.z * 100.0);

		plane1.MakeFrom3Points(p1, p2, p3);
		
		p3.x = place_to_look.x;
		p3.y = place_to_look.y;
		p3.z = place_to_look.z;

		plane2.MakeFrom3Points(p1, p2, p3);

		double angle_b2p = plane1.Angle(plane2);
		
		double angle_to_rotate = (angle_b2p * elapsed_time) / (rotating_time);

		angle_to_rotate *= GetRotationOrientation(plane2, up, angle_to_rotate, view);

		cpw::Math::RotatePoint(view, up, angle_to_rotate);
		cpw::Math::NormalizeVec(view);

		//rotation around left
		left = cpw::Math::CrossProduct(view, up);
		cpw::Math::NormalizeVec(left);

		p2.x = position.x + (left.x * 100.0);
		p2.y = position.y + (left.y * 100.0);
		p2.z = position.z + (left.z * 100.0);

		p3.x = position.x + (view.x * 100.0);
		p3.y = position.y + (view.y * 100.0);
		p3.z = position.z + (view.z * 100.0);

		plane1.MakeFrom3Points(p1, p2, p3);
	
		p3.x = place_to_look.x;
		p3.y = place_to_look.y;
		p3.z = place_to_look.z;

		plane2.MakeFrom3Points(p1, p2, p3);
	
		angle_b2p = plane1.Angle(plane2);
		angle_to_rotate = (angle_b2p * elapsed_time) / (rotating_time);
		
		angle_to_rotate *= GetRotationOrientation(plane2, left, angle_to_rotate, view);
		
		cpw::Math::RotatePoint(view, left, angle_to_rotate);
		cpw::Math::NormalizeVec(view);
		cpw::Math::RotatePoint(up, left, angle_to_rotate);
		cpw::Math::NormalizeVec(up);

		rotating_time -= elapsed_time;
		if (rotating_time < 0.0)
			rotating = false;
	}	
	
	cpw::Point3d<double> difference;
	difference.x = abs(place_to_go.x - place_to_start.x);
	difference.y = abs(place_to_go.y - place_to_start.y);
	difference.z = abs(place_to_go.z - place_to_start.z);

	double travel_total_time = current_going_to_time + time_to_arrive;
	cpw::Point3d<double> new_positionb;
	
	new_positionb.x = (current_going_to_time * difference.x) / (travel_total_time * current_increment_mult);
	new_positionb.y = (current_going_to_time * difference.y) / (travel_total_time * current_increment_mult);
	new_positionb.z = (current_going_to_time * difference.z) / (travel_total_time * current_increment_mult);

	cpw::Point3d<double> new_position;

	double position_increment = abs(place_to_go.x - position.x);
	if (place_to_go.x < position.x)
		new_position.x = position.x - ((elapsed_time * position_increment) / time_to_arrive);
	else
		new_position.x = position.x + ((elapsed_time * position_increment) / time_to_arrive);

	position_increment = abs(place_to_go.y - position.y);
	if (place_to_go.y < position.y)
		new_position.y = position.y - ((elapsed_time * position_increment) / time_to_arrive);
	else
		new_position.y = position.y + ((elapsed_time * position_increment) / time_to_arrive);

	position_increment = abs(place_to_go.z - position.z);
	if (place_to_go.z < position.z)
		new_position.z = position.z - ((elapsed_time * position_increment) / time_to_arrive);
	else
		new_position.z = position.z + ((elapsed_time * position_increment) / time_to_arrive);
	
	UpdatePositionAccordingToCollisions(position, new_position);
	
	time_to_arrive -= elapsed_time;
	current_going_to_time += elapsed_time;
	if (correct_up) //up correction
		CorrectUp();
}

void OsgCamera::SetAllCurrentSpeedsToCero()
{
	forward_current_speed = 0.0;
	vertical_rotation_current_speed = 0.0;
	horizontal_rotation_current_speed = 0.0;
	view_rotation_current_speed = 0.0;
	strafe_current_speed = 0.0;
	up_current_speed = 0.0;
}

double OsgCamera::GetRotationOrientation(const cpw::Plane &p, const cpw::Point3d<double> &axis, const double &angle, const cpw::Point3d<double> &point_to_rotate)
{
	cpw::Point3d<double> aux1, aux2;
	
	aux1 = point_to_rotate;
	aux2 = point_to_rotate;
	
	cpw::Math::RotatePoint(aux1, axis, angle);
	cpw::Math::RotatePoint(aux2, axis, -angle);

	aux1.x = aux1.x + position.x;
	aux1.y = aux1.y + position.y;
	aux1.z = aux1.z + position.z;

	aux2.x = aux2.x + position.x;
	aux2.y = aux2.y + position.y;
	aux2.z = aux2.z + position.z;

	
	double d1 = p.Distance(aux1);
	double d2 = p.Distance(aux2);

	if (d1 > d2)
		return -1.0;
	else
		return 1.0;
}

bool OsgCamera::UpdatePositionAccordingToCollisions(const cpw::Point3d<double> &start_position, const cpw::Point3d<double> &end_position)
{
	bool ok = false;
	cpw::Point3d<double> intersection_point;
	cpw::Point3d<double> intersection_normal;
	cpw::Point3d<double> ep(end_position.x - start_position.x,
		                    end_position.y - start_position.y,
							end_position.z - start_position.z);

	cpw::Math::NormalizeVec(ep);

	ep.x = end_position.x + (ep.x * disp);
	ep.y = end_position.y + (ep.y * disp);
	ep.z = end_position.z + (ep.z * disp);

	if (osg_scene->IntersectRayWithScene(start_position, ep, intersection_point, intersection_normal))
	{
		intersection_point = osg_scene->UTMToSceneCoords(intersection_point);

		position.x = intersection_point.x;// + (intersection_normal.x * disp);
		position.y = intersection_point.y;// + (intersection_normal.y * disp);
		double aux = intersection_point.z + (intersection_normal.z * disp);
		
		if (aux < (intersection_point.z + disp))
		{
			position.z = intersection_point.z + disp;
			ok = true;
		}
		else
			position.z = aux;

		
	}
	else if (osg_scene->IntersectRayWithScene(end_position, cpw::Point3d<double>(end_position.x, end_position.y, end_position.z - disp),
		                                      intersection_point, intersection_normal))
	{
		intersection_point = osg_scene->UTMToSceneCoords(intersection_point);

		position.x = intersection_point.x;// + (intersection_normal.x * disp);
		position.y = intersection_point.y;// + (intersection_normal.y * disp);
		position.z = intersection_point.z + (intersection_normal.z * disp);
		ok = true;
		
	}
	else
	{
		position = end_position;
	}

	//don´t let the camera go under the XY plane
	if (position.z <= 0.0)
		position.z = 0.0;

	return ok;
}

double OsgCamera::GetAproachingFactor(const cpw::Point3d<double> &place_to_go)
{
	double min_approach_distance = 300.0;
	double max_approach_distance = 2000.0;
	double max_distance = 300.0;
	double min_distance = 3000.0;
	double distance = cpw::Math::DistanceBetweenTwoPoints(position, place_to_go);

	if (distance < min_distance)
		return min_approach_distance;

	if (distance > max_distance)
		return max_approach_distance;

	double a = ((distance - min_distance) * (max_approach_distance - min_approach_distance)) / (max_distance - min_distance);
	return a + min_approach_distance;
}

cpw::Plane OsgCamera::CreatePlaneFromCameraVectors(  )
{
	//rotation around up
	cpw::Point3d<double> p1, p2, p3, projected_up;
	cpw::Plane out;

	p1.x = position.x;
	p1.y = position.y;
	p1.z = position.z;

	p2.x = position.x + (up.x * 100.0);
	p2.y = position.y + (up.y * 100.0);
	p2.z = position.z + (up.z * 100.0);

	p3.x = position.x + (view.x * 100.0);
	p3.y = position.y + (view.y * 100.0);
	p3.z = position.z + (view.z * 100.0);

	out.MakeFrom3Points(p1, p2, p3);

	return out;
}

void OsgCamera::CorrectUp()
{
	//rotation around up
	cpw::Point3d<double> p1, p2, p3;
	cpw::Plane plane;

	//plane containing the view, position and corrected up
	p1.x = position.x;
	p1.y = position.y;
	p1.z = position.z;

	p2.x = position.x;
	p2.y = position.y;
	p2.z = position.z + 100.0;

	p3.x = position.x + (view.x * 100.0);
	p3.y = position.y + (view.y * 100.0);
	p3.z = position.z + (view.z * 100.0);

	plane.MakeFrom3Points(p1, p2, p3);
	
	//project the up vector against that plane
	up =  plane.ProjectVectorOnPlane(up);
	cpw::Math::NormalizeVec(up);
}

bool OsgCamera::GetIntersectionViewWithScene()
{
	cpw::Point3d<double> r2;

	r2.x = position.x + view.x * 1000000.0;
	r2.y = position.y + view.y * 1000000.0;
	r2.z = position.z + view.z * 1000000.0;

	//intersect with scene
	if (osg_scene->IntersectRayWithScene(position, r2, view_intersection_point))
	{
		view_intersection_point = osg_scene->UTMToSceneCoords(view_intersection_point);
		ip_valid = true;
		return true;
	}
	else
	{
		//intersect with XY PLANE
		cpw::Point3d<double> vec = r2 - position;
		cpw::Math::NormalizeVec(vec);

		if (vec.z != 0.0)
		{
			double alpha = ((position.z * -1.0) / vec.z);
			if (alpha > 0.0)
			{
				view_intersection_point.x = position.x + alpha * vec.x;
				view_intersection_point.y = position.y + alpha * vec.y;
				view_intersection_point.z = position.z + alpha * vec.z;
				ip_valid = true;
				return true;
			}
		}
	}

	ip_valid = false;
	return false;
}

void OsgCamera::StopViewSceneCollisionPointHorizontalRotation()
{
	view_collision_horizontal_continous_rotation = false;
		
	if (!view_collision_vertical_continous_rotation)
	{
		ip_valid = false;
		if (sn != NULL)
			sn->setAllChildrenOff();
	}
}
void OsgCamera::StopViewSceneCollisionPointVerticalRotation()
{
	view_collision_vertical_continous_rotation = false;
	
	if (!view_collision_horizontal_continous_rotation)
	{
		ip_valid = false;
		if (sn != NULL)
			sn->setAllChildrenOff();
	}

}

void OsgCamera::SetDefaultPath(const std::string &url)
{
	default_directory = url;
}

void OsgCamera::NewRotationPoint(const cpw::Point3d<double> &_new_cp)
{
	cpw::Point3d<float> pos, orientation, scale;
	cpw::Point3d<double> new_cp = OsgScene::UTMToSceneCoords(_new_cp);
	view_intersection_point = new_cp;
	ip_valid = true;

	if (!osg_init)
	{
		std::string model_name = default_directory + "/rotate_pivot.osg";

		osg::ref_ptr<osg::Node> rotate_pivot_node = osgDB::readNodeFile(model_name);

		if (rotate_pivot_node != NULL)
		{
			rotate_pivot_node->getOrCreateStateSet()->setAttributeAndModes(new osg::Depth(osg::Depth::ALWAYS), osg::StateAttribute::ON);
			mtn = new osg::MatrixTransform();
			sn = new osg::Switch();

			sn->addChild(mtn.get());
			mtn->addChild(rotate_pivot_node.get());

			sn->setAllChildrenOn();

			osg::Group *root_node = (osg::Group *) ((OsgScene *)cpw::ApplicationScene::GetInstance()->GetScene())->GetCameraGizmosGroup();
			if (root_node != NULL)
			{
				root_node->addChild(sn.get());
			}
		
		}

		osg_init = true;
	}
	
	orientation.x = 0.0f;
	orientation.y = 0.0f;
	orientation.z = 0.0f;

	scale.x = 1.0f;
	scale.y = 1.0f;
	scale.z = 1.0f;

	pos.x = (float) new_cp.x;
	pos.y = (float) new_cp.y;
	pos.z = (float) new_cp.z;
	
	osg::Matrix mtt, mts;

	mts.makeIdentity();
	mtt.makeIdentity();

	double distance = cpw::Math::DistanceBetweenTwoPoints(position, new_cp);
	distance *= 0.007;

	//mts.makeScale(400.0, 400.0, 400.0);
	mts.makeScale(distance, distance, distance);
	mtt.makeTranslate(pos.x, pos.y, pos.z);

	mtn->setMatrix(mts * mtt);

	sn->setAllChildrenOn();
}

bool OsgCamera::RotationPossible(const cpw::Point3d<double> &start_position, const cpw::Point3d<double> &end_position)
{
	bool ok = false;
	cpw::Point3d<double> intersection_point;
	cpw::Point3d<double> intersection_normal;
	cpw::Point3d<double> ep(end_position.x - start_position.x,
		                    end_position.y - start_position.y,
							end_position.z - start_position.z);

	if (cpw::Math::DistanceBetweenTwoPoints(start_position, end_position) == 0.0)
		return true;

	cpw::Math::NormalizeVec(ep);

	ep.x = end_position.x + (ep.x * disp);
	ep.y = end_position.y + (ep.y * disp);
	ep.z = end_position.z + (ep.z * disp);

	if (osg_scene->IntersectRayWithScene(start_position, end_position, intersection_point, intersection_normal))
	{
		return false;
	}
	else if (osg_scene->IntersectRayWithScene(end_position, ep, intersection_point, intersection_normal))
	{
		return false;		
	}
	
	//don´t let the camera go under the XY plane
	if (end_position.z <= 0.0)
		return false;

	/*if (ip_valid)
	{
		if (position.z < view_intersection_point.z)
		{
			return false;
		}
	}*/

	return true;
}

void OsgCamera::UpdatePivotSize(const double &time_between_frames)
{
	if (osg_init)
	{
		bool reset_scale = false;
		rotation_icon_size_factor += 1.0;
		
		if (rotation_icon_size_factor >= 30)
		{
			rotation_icon_size_factor = 0.0;
			reset_scale = true;
		}

		osg::Matrix mts, mtt, existing_mt;

		existing_mt = mtn->getMatrix();

		mts.makeIdentity();
		mtt.makeIdentity();

		osg::Vec3d trans = existing_mt.getTrans();
		osg::Vec3d scale = existing_mt.getScale();

		cpw::Point3d<double> pivot_position(trans.x(), trans.y(), trans.z());
		double distance = cpw::Math::DistanceBetweenTwoPoints(position, pivot_position);
		distance *= 0.007;

		if (reset_scale)
		{
			mts.makeScale(distance, distance, distance);
		}
		else
		{
			mts.makeScale(distance + rotation_icon_size_factor,
				distance + rotation_icon_size_factor,
				distance + rotation_icon_size_factor);
		}
			
		mtt.makeTranslate(trans);

		mtn->setMatrix(mts * mtt);
	}
}

void OsgCamera::ZoomToPoint(const cpw::Point3d<double> &_point_to_zoom, const double &zoom_time, const double &percentage_to_advance, const bool &forward)
{
	
	if (zooming_to_point)
		return;

	cpw::Point3d<double> point_to_zoom = OsgScene::UTMToSceneCoords(_point_to_zoom);


	zoom_forward = forward;
	distance_percentage = percentage_to_advance;
	zoom_final_position = point_to_zoom;

	cpw::Point3d<double> point2, point3;
	cpw::Point3d<double> left = cpw::Math::CrossProduct(view, up);

	point2.x = point_to_zoom.x + view.x * 100;
	point2.y = point_to_zoom.y + view.y * 100;
	point2.z = point_to_zoom.z + view.z * 100;

	point3.x = point_to_zoom.x + up.x * 100;
	point3.y = point_to_zoom.y + up.y * 100;
	point3.z = point_to_zoom.z + up.z * 100;
	
	planev.MakeFrom3Points(point_to_zoom, point2, point3);
	distance_to_v = planev.DistanceSigned(position);

	distance_to_v *= percentage_to_advance;

	cpw::Math::NormalizeVec(left);

	point3.x = point_to_zoom.x + left.x * 100;
	point3.y = point_to_zoom.y + left.y * 100;
	point3.z = point_to_zoom.z + left.z * 100;

	planeh.MakeFrom3Points(point_to_zoom, point2, point3);
	distance_to_h = planeh.DistanceSigned(position);

	distance_to_h *= percentage_to_advance;

	point2.x = point_to_zoom.x + up.x * 100;
	point2.y = point_to_zoom.y + up.y * 100;
	point2.z = point_to_zoom.z + up.z * 100;
	
	planew.MakeFrom3Points(point_to_zoom, point2, point3);

	distance_to_advance = planew.DistanceSigned(position);
	
	distance_to_advance *= percentage_to_advance;
	
	zooming_to_point = true;

	current_going_to_time = 0.0;
	time_to_arrive = zoom_time;

	initial_position = position;

	//the zoom model
	if (!osg_zooming_init)
	{
		std::string model_name = default_directory + "/scale_in_pivot.osg";
		std::string model_name2 = default_directory + "/scale_out_pivot.osg";

		osg::ref_ptr<osg::Node> zoom_node = osgDB::readNodeFile(model_name);
		osg::ref_ptr<osg::Node> zoom_node2 = osgDB::readNodeFile(model_name2);

		if ((zoom_node != NULL) && (zoom_node2 != NULL))
		{
			zoom_node->getOrCreateStateSet()->setAttributeAndModes(new osg::Depth(osg::Depth::ALWAYS), osg::StateAttribute::ON);
			zoom_node2->getOrCreateStateSet()->setAttributeAndModes(new osg::Depth(osg::Depth::ALWAYS), osg::StateAttribute::ON);
			mt_zm = new osg::MatrixTransform;
			snz = new osg::Switch;

			mt_zm->addChild(snz.get());
			snz->addChild(zoom_node.get());
			snz->addChild(zoom_node2.get());

			snz->setAllChildrenOff();

			osg::Group *root_node = (osg::Group *) ((OsgScene *)cpw::ApplicationScene::GetInstance()->GetScene())->GetCameraGizmosGroup();
			
			if (root_node != NULL)
			{
				root_node->addChild(mt_zm.get());
			}

		
			osg_zooming_init = true;
		}
	}

	UpdateZoomPivot(point_to_zoom, zoom_forward);
}

void OsgCamera::UpdateZoomPivot(const cpw::Point3d<double> &i_point, const bool &zoom_in)
{
	if (snz == NULL)
		return;

	osg::Matrix ms, mt;

	ms.makeIdentity();
	mt.makeIdentity();

	cpw::Point3d<double> traslate_pivot_positiond(i_point.x, i_point.y, i_point.z);
	
	double distance = cpw::Math::DistanceBetweenTwoPoints(position, i_point);
	distance *= 0.009;

	mt.makeTranslate(i_point.x, i_point.y, i_point.z);
	ms.makeScale(distance, distance, distance);
	
	mt_zm->setMatrix( ms * mt );
	
	if (zoom_in)
		snz->setSingleChildOn(1);
	else
		snz->setSingleChildOn(0);
}

void OsgCamera::GoToWithNoUpViewChanges(const cpw::Point3d<double> &_place, const double &arrive_time)
{
	//construct the two planes in which intersection we must be
	cpw::Point3d<double> point2, point3;
	cpw::Point3d<double> left = cpw::Math::CrossProduct(view, up);

	cpw::Point3d<double> place = OsgScene::UTMToSceneCoords(_place);

	point2.x = place.x + view.x * 100;
	point2.y = place.y + view.y * 100;
	point2.z = place.z + view.z * 100;

	point3.x = place.x + up.x * 100;
	point3.y = place.y + up.y * 100;
	point3.z = place.z + up.z * 100;

	
	planev.MakeFrom3Points(place, point2, point3);
	distance_to_v = planev.DistanceSigned(position);

	cpw::Math::NormalizeVec(left);

	point3.x = place.x + left.x * 100;
	point3.y = place.y + left.y * 100;
	point3.z = place.z + left.z * 100;

	planeh.MakeFrom3Points(place, point2, point3);
	distance_to_h = planeh.DistanceSigned(position);

	point2.x = place.x + up.x * 100;
	point2.y = place.y + up.y * 100;
	point2.z = place.z + up.z * 100;
	
	planew.MakeFrom3Points(place, point2, point3);

	distance_to_advance = planew.DistanceSigned(position);
	distance_to_advance *= 0.7;
	
	going_to_no_uav_changes = true;
	current_going_to_time = 0.0;
	time_to_arrive = arrive_time;

	initial_position = position;
}

void OsgCamera::UpdateZoomToPoint(const double &time)
{
	cpw::Point3d<double> left = cpw::Math::CrossProduct(view, up);
	cpw::Math::NormalizeVec(left);

	double time_left = time_to_arrive - current_going_to_time;

	////////// inertial movement
	double proportional_time_left;
	double proportional_animation_time;
	
	proportional_time_left = time_left / time_to_arrive;
	proportional_animation_time = cos(proportional_time_left * osg::PI);
	proportional_animation_time = (proportional_animation_time + 1) * 0.5;
	

	double left_increment = distance_to_v * proportional_animation_time;
	double forward_increment = distance_to_advance * proportional_animation_time;
	double up_increment = distance_to_h * proportional_animation_time;
	double x_increment, y_increment, z_increment;

	x_increment = y_increment = z_increment = 0.0;

	if (zoom_forward)
	{
		x_increment -= left.x * left_increment;
		y_increment -= left.y * left_increment;
		z_increment -= left.z * left_increment;

		x_increment += up.x * up_increment;
		y_increment += up.y * up_increment;
		z_increment += up.z * up_increment;

		x_increment -= view.x * forward_increment;
		y_increment -= view.y * forward_increment;
		z_increment -= view.z * forward_increment;
	}
	else
	{
		x_increment += left.x * left_increment;
		y_increment += left.y * left_increment;
		z_increment += left.z * left_increment;

		x_increment -= up.x * up_increment;
		y_increment -= up.y * up_increment;
		z_increment -= up.z * up_increment;

		x_increment += view.x * forward_increment;
		y_increment += view.y * forward_increment;
		z_increment += view.z * forward_increment;
	}

	cpw::Point3d<double> new_position(initial_position.x + x_increment,
									  initial_position.y + y_increment,
									  initial_position.z + z_increment);

	current_going_to_time += time;

	if ( MovementCollides(position, new_position) || 
		(current_going_to_time >= time_to_arrive) ||
		(new_position.z > roof))
	{
		zooming_to_point = false;
		snz->setAllChildrenOff();
	}
	else
		position = new_position;
		
	
	
}
void OsgCamera::UpdateGoingToNoUAVChanges(const double &time)
{
	cpw::Point3d<double> left = cpw::Math::CrossProduct(view, up);
	cpw::Math::NormalizeVec(left);

	double time_left = time_to_arrive - current_going_to_time;

	////////// inertial movement
	double proportional_time_left = time_left / time_to_arrive;
	double proportional_animation_time = cos(proportional_time_left * osg::PI);
	proportional_animation_time = (proportional_animation_time + 1) * 0.5;
	//////////////////////

	double left_increment = distance_to_v * proportional_animation_time;
	double forward_increment = distance_to_advance * proportional_animation_time;
	double up_increment = distance_to_h * proportional_animation_time;
	double x_increment, y_increment, z_increment;

	x_increment = y_increment = z_increment = 0.0;

	x_increment -= left.x * left_increment;
	y_increment -= left.y * left_increment;
	z_increment -= left.z * left_increment;

	x_increment += up.x * up_increment;
	y_increment += up.y * up_increment;
	z_increment += up.z * up_increment;

	x_increment -= view.x * forward_increment;
	y_increment -= view.y * forward_increment;
	z_increment -= view.z * forward_increment;

	cpw::Point3d<double> new_position(initial_position.x + x_increment,
									  initial_position.y + y_increment,
									  initial_position.z + z_increment);


	double terrain_height = GetTerrainHeightUnderCamera();
	
	position = new_position;
	
	if ((position.z - terrain_height) < disp)
	{
		position.z = terrain_height + disp;
		going_to_no_uav_changes = false;
	}

	//if (MovementCollides(position, new_position))
	//	going_to_no_uav_changes = false;
	//else
	//	position = new_position;
		
	current_going_to_time += time;
	if (current_going_to_time >= time_to_arrive)
		going_to_no_uav_changes = false;
}

void OsgCamera::GoToPanoramic(const cpw::Point3d<double> &_place, const double &arrive_time)
{
	cpw::Point3d<double> &place = OsgScene::UTMToSceneCoords(_place);

	initial_position = position;
	initial_up = up;
	initial_view = view;

	panoramic_final_position.x = place.x;
	panoramic_final_position.y = place.y;
	panoramic_final_position.z = panoramic_goto_final_height;

	if (position.z > panoramic_goto_final_height + 100)
		need_intermediate_position = false;
	else
		need_intermediate_position = true;
	
	
	panoramic_max_height = panoramic_goto_final_height * 2.5;

	time_to_arrive = arrive_time;
	going_up = true;
	
	middle_time = arrive_time * 0.5;
	current_going_to_time = 0.0;

	going_to_panoramic = true;

	panoramic_final_up.x = 0.0;
	panoramic_final_up.y = 1.0;
	panoramic_final_up.z = 0.0;
	panoramic_final_view.x = 0.0;
	panoramic_final_view.y = 0.0;
	panoramic_final_view.z = -1.0;

	position_increment = panoramic_final_position - initial_position;
	view_increment = panoramic_final_view - initial_view;
	up_increment = panoramic_final_up - initial_up;
}

void OsgCamera::UpdateGoingToPanoramic(const double &time)
{
	double time_left = time_to_arrive - current_going_to_time;
	
	////////// inertial movement
	double proportional_time_left = time_left / time_to_arrive;
	double proportional_animation_time = cos(proportional_time_left * osg::PI);
	proportional_animation_time = (proportional_animation_time + 1) * 0.5;
	////////////////////////////

	up.x = initial_up.x + up_increment.x * proportional_animation_time;
	up.y = initial_up.y + up_increment.y * proportional_animation_time;
	up.z = initial_up.z + up_increment.z * proportional_animation_time;

	view.x = initial_view.x + view_increment.x * proportional_animation_time;
	view.y = initial_view.y + view_increment.y * proportional_animation_time;
	view.z = initial_view.z + view_increment.z * proportional_animation_time;

	position.x = initial_position.x + position_increment.x * proportional_animation_time;
	position.y = initial_position.y + position_increment.y * proportional_animation_time;
	//position.z = initial_position.z + position_increment.z * proportional_animation_time;

	//the position
	//going up???
	if (need_intermediate_position)
	{
		if (going_up)
		{
			time_left = (time_to_arrive * 0.5) - current_going_to_time;
			proportional_time_left = time_left / (time_to_arrive * 0.5);
			proportional_animation_time = cos(proportional_time_left * osg::PI);
			proportional_animation_time = (proportional_animation_time + 1) * 0.5;
			
			position.z = initial_position.z + (panoramic_max_height - initial_position.z) * proportional_animation_time;
		}
		else
		{
			time_left = (time_to_arrive * 0.5) - (current_going_to_time - (time_to_arrive * 0.5));
			proportional_time_left = time_left / (time_to_arrive * 0.5);
			proportional_animation_time = cos(proportional_time_left * osg::PI);
			proportional_animation_time = (proportional_animation_time + 1) * 0.5;

		
			position.z = panoramic_max_height + (panoramic_final_position.z - panoramic_max_height) * proportional_animation_time;
		}
	}
	else
	{
		position.z = initial_position.z + position_increment.z * proportional_animation_time;
	}

	cpw::Math::NormalizeVec(up);
	cpw::Math::NormalizeVec(view);

	current_going_to_time += time;
	
	if (current_going_to_time >= time_to_arrive)
	{
		going_to_panoramic = false;
		view = panoramic_final_view;
		up = panoramic_final_up;
	}

	if (need_intermediate_position)
	{
		if (going_up && (current_going_to_time >= middle_time))
			going_up = false;
	}
}

bool OsgCamera::MovementCollides(const cpw::Point3d<double> &start_position, const cpw::Point3d<double> &end_position)
{
	return !RotationPossible(start_position, end_position);
}


void OsgCamera::StartXYTraslation(const int &x, const int &y)
{
	cpw::Point3d<float> i_point;

	if (!osg_traslate_init)
	{
		std::string model_name = default_directory + "traslate_pivot.osg";

		osg::ref_ptr<osg::Node> model = osgDB::readNodeFile(model_name);

		if (model != NULL)
		{
			model->getOrCreateStateSet()->setAttributeAndModes(new osg::Depth(osg::Depth::ALWAYS), osg::StateAttribute::ON);

			mt_tm = new osg::MatrixTransform();
			snt = new osg::Switch();

			snt->addChild(mt_tm.get());
			mt_tm->addChild(model.get());

			snt->setAllChildrenOn();

			osg::Group *root_node = (osg::Group *) ((OsgScene *)cpw::ApplicationScene::GetInstance()->GetScene())->GetCameraGizmosGroup();
			
			if (root_node != NULL)
			{
				root_node->addChild(snt.get());
			}
		}

		osg_traslate_init = true;
	}

	

	if (osg_navigator->IntersectMouseWithScene(x, y, i_point.x, i_point.y, i_point.z))
	{
		i_point = osg_scene->UTMToSceneCoords(i_point);

		UpdateTraslatePivot(i_point, true);
				
		traslation_last_collision_point.x = i_point.x;
		traslation_last_collision_point.y = i_point.y;
		traslation_last_collision_point.z = i_point.z;

	/*	osg::Matrix m = mt_tm->getMatrix();
		osg::Matrix m2;
		m2.makeScale(100.0, 100.0, 100.0);
		m.makeTranslate(i_point.x, i_point.y, i_point.z);
		mt_tm->setMatrix(m2 * m);
		snt->setAllChildrenOn();*/


		traslating_xy = true;
	}
}

void OsgCamera::UpdateTraslatePivot(const cpw::Point3d<float> &i_point, const bool &update_position)
{
	if (snt == NULL)
		return;

	if (update_position)
		traslate_pivot_position = i_point;

	
	osg::Matrix ms, mt;

	ms.makeIdentity();
	mt.makeIdentity();

	cpw::Point3d<double> traslate_pivot_positiond(traslate_pivot_position.x, traslate_pivot_position.y, traslate_pivot_position.z);
	double distance = cpw::Math::DistanceBetweenTwoPoints(position, traslate_pivot_positiond);
	distance *= 0.01;

	mt.makeTranslate(traslate_pivot_position.x, traslate_pivot_position.y, traslate_pivot_position.z);
	ms.makeScale(distance, distance, distance);
	
	mt_tm->setMatrix( ms * mt );
	
	snt->setAllChildrenOn();
}

void OsgCamera::UpdateXYPositionWithMouseCollision(const unsigned int &x0, const unsigned int &y0, const unsigned int &x1, const unsigned int &y1)
{
	if (osg_navigator == NULL)
		return;

	cpw::Point3d<float> i_point0, i_point1, traslation;

	if (osg_navigator->IntersectMouseWithScene(x0, y0, i_point0.x, i_point0.y, i_point0.z))
	{
		cpw::Point3d<double> vec0, i_pointd0;

		i_point0 = osg_scene->UTMToSceneCoords(i_point0);

		

		i_pointd0.x = i_point0.x;
		i_pointd0.y = i_point0.y;
		i_pointd0.z = i_point0.z;

		vec0 = i_pointd0 - position;
		cpw::Math::NormalizeVec(vec0);

		double t0 = (traslation_last_collision_point.z - position.z) / vec0.z;

		i_pointd0.x = position.x + (t0 * vec0.x);
		i_pointd0.y = position.y + (t0 * vec0.y);
		i_pointd0.z = position.z + (t0 * vec0.z);

		if (osg_navigator->IntersectMouseWithScene(x1, y1, i_point1.x, i_point1.y, i_point1.z))
		{
			bool invalid_movement = false;
			
			i_point1 = osg_scene->UTMToSceneCoords(i_point1);

			UpdateTraslatePivot(i_point1, false);

			cpw::Point3d<double> i_pointd(i_point1.x, i_point1.y, i_point1.z);

			/*if ((cpw::Math::DistanceBetweenTwoPoints(i_pointd, position) > 45000.0f) &&
				 position.z < 25000.0f)
			{
				return;
			}*/

			//get the vec
			cpw::Point3d<double> i_pointd1, vec;
		
			i_pointd1.x = i_point1.x;
			i_pointd1.y = i_point1.y;
			i_pointd1.z = i_point1.z;

			vec = i_pointd1 - position;
			cpw::Math::NormalizeVec(vec);

			double t = (traslation_last_collision_point.z - position.z) / vec.z;

			//the new collision point on the xy plane containing the first traslation_collision point
			i_pointd1.x = position.x + (t * vec.x);
			i_pointd1.y = position.y + (t * vec.y);
			i_pointd1.z = position.z + (t * vec.z);

			traslation.x = i_pointd1.x - i_pointd0.x;
			traslation.y = i_pointd1.y - i_pointd0.y;

			/*if (traslation.x > max_traslation)
				traslation.x = max_traslation;
			else if (traslation.x < -max_traslation)
				traslation.x = -max_traslation;

			if (traslation.y > max_traslation)
				traslation.y = max_traslation;
			else if (traslation.y < -max_traslation)
				traslation.y = -max_traslation;*/

			if (traslation.x > max_traslation)
				invalid_movement = true;
			else if (traslation.x < -max_traslation)
				invalid_movement = true;

			if (traslation.y > max_traslation)
				invalid_movement = true;
			else if (traslation.y < -max_traslation)
				invalid_movement = true;
		
			//if (!invalid_movement)
			{
				cpw::Point3d<double> new_position(position.x - traslation.x, position.y - traslation.y, position.z);

				UpdatePositionAccordingToCollisions(position, new_position);
				//if (!MovementCollides(position, new_position))
				//	position = new_position;
			}
		}
	}
}

void OsgCamera::StopXYTraslation()
{ 
	traslating_xy = false;
	if (snt != NULL)
		snt->setAllChildrenOff();
}


double OsgCamera::GetTerrainHeightUnderCamera()
{
	cpw::Point3d<float> rayf(position.x, position.y, position.z);
	cpw::Point3d<float> positionf(position.x, position.y, position.z);
	cpw::Point3d<float> intersection_point, intersection_normal;

	rayf.z -= 100000000;
             
	if (osg_scene->IntersectRayWithScene(positionf, rayf, intersection_point, intersection_normal))
	{
		intersection_point = osg_scene->UTMToSceneCoords(intersection_point);

		return (double) intersection_point.z;
	}
	else
		return 0.0;
}

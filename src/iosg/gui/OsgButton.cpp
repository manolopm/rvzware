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

#include "stdafx.h"

#include <sstream>
#include <iosg/gui/OsgButton.h>

#include <cpw/common/ApplicationLog.h>

using namespace cpw::iosg; 

#define DELAY_START 0.1f

OsgIButton::OsgIButton(const std::string &url, cpw::Point3d<float> _position, cpw::Point3d<float> _size, cpw::Point3d<float> _rotation, const TAnchor &_anchor, const std::string &filename) : OsgIWidget(url, _position, _size, _rotation, _anchor), being_animated(false)
{
	if (filename != "")
	{
		SetObject(filename);
	}
	pick_initial = true;
}

OsgIButton::~OsgIButton(void)
{
}

bool OsgIButton::MouseLeftButtonDown(const int &x,const int &y)
{
	std::ostringstream debug_message;
	debug_message << "Se picó en botón -" << this->GetName() << "-";
	cpw::ApplicationLog::GetInstance()->GetLogger()->NewLogMessage(debug_message.str());
	StartPickAnimation(0.35);
	GetApplication()->NewFire();
	return false;
}

void OsgIButton::StartPickAnimation(const double &_animation_time)
{
	animation_time = _animation_time;
	//initial_size = GetSize();
	//initial_rotation = GetRotation();
	last_t = osg::Timer::instance()->tick();
	being_animated = true;
	current_animation_time = 0.0;

	if (pick_initial)
	{
		//initial_size	 = GetSize();
		initial_scale	 = GetScale();
		initial_rotation = GetRotation();
		//initial_position = GetPosition();
		//initial_anchor   = GetAnchor();
		pick_initial = false;
	}

}
void OsgIButton::UpdateAnimation()
{
	actual_t = osg::Timer::instance()->tick();
	double time_between_frames = (actual_t - last_t) * osg::Timer::instance()->getSecondsPerTick();
	last_t = actual_t;

	current_animation_time += time_between_frames;

	if (current_animation_time >= animation_time)
	{
		//SetRotation(initial_rotation);
		//being_animated = false;
		SetScale(initial_scale);
		being_animated = false;
		
	}
	else
	{
		//float current_rotation = (current_animation_time * 2 * osg::PI) / animation_time;
		//SetRotation(cpw::Point3d<float>(0.0f, current_rotation, 0.0f ));
		float current_scale = (current_animation_time * 0.5) / animation_time;
		SetScale(cpw::Point3d<float>(current_scale, current_scale, initial_scale.z ));
	}


}

void OsgIButton::Update(osg::Matrix *mp)
{
	if (being_animated)
		UpdateAnimation();

	OsgIWidget::Update(mp);

}
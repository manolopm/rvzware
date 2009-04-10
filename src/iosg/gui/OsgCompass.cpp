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

#include "../stdafx.h"

#include <iosg/gui/OsgCompass.h>


using namespace cpw::iosg; 

OsgICompass::OsgICompass(const std::string &url, cpw::Point3d<float> _position, cpw::Point3d<float> _size, cpw::Point3d<float> _rotation, const TAnchor &_anchor) : OsgIContainer(url, _position, _size, _rotation, _anchor), camera(NULL)
{
	OsgIWidget::OsgIWidget(url, _position, _size, _rotation, _anchor);
	
	SetObject(url + "compass.osg");

	rotation.x = 0.0f;
	rotation.y = 0.0f;
	rotation.z = 0.0f;
	status = -1;
}

OsgICompass::~OsgICompass(void)
{
}

void OsgICompass::Update(osg::Matrix *mp)
{

	rotation.x = 0.0f;
	rotation.y = 0.0f;
	rotation.z = 0.0f;

	if (camera == NULL)
		return;

	cpw::Point3d<float> norm_up((float)camera->GetUp().x, (float)camera->GetUp().y, (float)camera->GetUp().z);
	cpw::Point3d<float> norm_view((float)camera->GetView().x, (float)camera->GetView().y, (float)camera->GetView().z);
	cpw::Point3d<float> north(0.0f, 1.0f, 0.0f);
	cpw::Plane xy_plane(0.0, 0.0, 1.0, 0.0);	
	
	cpw::Math::NormalizeVec(norm_up);
	cpw::Math::NormalizeVec(norm_view);

	norm_up = xy_plane.ProjectVectorOnPlane(norm_up);
	float angle = cpw::Math::AngleBetweenTwoVectorsInRadians(norm_up, north);
	
	if (norm_up.x < 0)
		angle *= -1.0f;

	cpw::Point3d<float> rotation;
	rotation.x = rotation.y = rotation.z = 0.0;
	rotation.x = osg::PI;
	rotation.z = angle;
	SetRotation(rotation);

	if (angle != 0.0)
	{
		int debug = 1;
	}

	OsgIContainer::Update(mp);
}



bool OsgICompass::MouseLeftButtonDown(const int &x,const int &y)
{ 
	return false;
}



void OsgICompass::SetVisible(const bool &_visible)
{
	OsgIWidget::SetVisible(_visible);
}

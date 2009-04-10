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

#include <iosg/gui/OsgPanel.h>

using namespace cpw::iosg; 

OsgIPanel::OsgIPanel(const std::string &url, cpw::Point3d<float> _position, cpw::Point3d<float> _size, cpw::Point3d<float> _rotation, const TAnchor &_anchor) : OsgIContainer(url, _position, _size, _rotation, _anchor)
{

	SetObject(url + "bar.osg");

	rotation.x = 0.0f;
	rotation.y = 0.0f;
	rotation.z = 0.0f;

}

OsgIPanel::~OsgIPanel(void)
{
}

void OsgIPanel::Update(osg::Matrix *mp)
{
	
	rotation.x -= 0.02f;
	rotation.y -= 0.02f;
	rotation.z -= 0.02f;

	
	//UpdateTransformationMatrix(mp);
	//osg::Matrix m = GetMatrixTransform()->getMatrix();
	OsgIContainer::Update(mp);
	
	
	
}

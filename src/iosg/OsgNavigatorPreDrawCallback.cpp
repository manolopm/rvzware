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

#include <iosg/OsgNavigatorPreDrawCallback.h>
#include <cpw/graphic/ApplicationScene.h>


using namespace cpw::iosg;

OsgNavigatorPreDrawCallback::OsgNavigatorPreDrawCallback(void)
{
}

OsgNavigatorPreDrawCallback::~OsgNavigatorPreDrawCallback(void)
{
}

void OsgNavigatorPreDrawCallback::operator () (const osg::Camera &camera) const
{
	//osg::Vec3 eye, center, up;
	//camera.getViewMatrixAsLookAt(eye, center, up);

	cpw::Point3d<float> position(navigator->GetCamera()->GetPosition().x,
								 navigator->GetCamera()->GetPosition().y,
								 navigator->GetCamera()->GetPosition().z);
	
	//cpw::ApplicationScene::GetInstance()->GetScene()->UpdateIconsSize(35.0f, 0.0f, 1600000.0f, 50000.0f, position, 8000.0f, 300.0f);
	cpw::ApplicationScene::GetInstance()->GetScene()->UpdateIconsSize(26.0f, 0.0f, 1600000.0f, 50000.0f, position, 8000.0f, 300.0f);

	navigator->EnableHUDCameraAndDisableOthers();
}
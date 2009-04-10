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
#include <osg/Node>
#include <osg/ref_ptr>
#include <osgDB/ReadFile>

#include "stdafx.h"

#include <iosg/OsgObjectLoader.h>

#include <cpw/graphic/ApplicationScene.h>
#include <cpw/graphic/IScene.h>


using namespace cpw::iosg;

OsgObjectLoader::OsgObjectLoader()
{
}



OsgObjectLoader::~OsgObjectLoader()
{
}

cpw::IGraphic * OsgObjectLoader::LoadObject(const std::string &filename)
{
	cpw::IGraphic *new_model = (cpw::IGraphic*) osgDB::readNodeFile(filename);
	cpw::IScene *scene = cpw::ApplicationScene::GetInstance()->GetScene();

	scene->InsertAsReferencedObject(new_model);

	return new_model;
}
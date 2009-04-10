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

#include <iosg/OsgFactory.h>
#include <iosg/OsgValidator.h>
#include <iosg/OsgScene.h>
#include <iosg/OsgSceneLoader.h>
#include <iosg/OsgNavigatorManager.h>
#include <iosg/OsgObjectLoader.h>
#include <iosg/OsgCamera.h>
#include <iosg/OsgOverlayImage.h>


using namespace cpw::iosg;

OsgFactory::OsgFactory(cpw::IApplication *_app, bool lite_version_) : application(_app), lite_version(lite_version_)
{
}

OsgFactory::~OsgFactory()
{
}

cpw::IValidator * OsgFactory::CreateValidator()
{
	return (cpw::IValidator *) new iosg::OsgValidator();
}

cpw::IScene * OsgFactory::CreateScene() 
{
	const std::string url = application->GetDefaultPath();
	return new iosg::OsgScene(url);
}



cpw::ISceneLoader * OsgFactory::CreateSceneLoader()
{
	return (cpw::ISceneLoader *) new iosg::OsgSceneLoader();
}

cpw::INavigatorManager * OsgFactory::CreateNavigatorManager()
{
	OsgNavigatorManager *nm = new iosg::OsgNavigatorManager(lite_version);
	nm->SetApplication(application);
	std::string dp = application->GetDefaultPath();
	nm->SetDefaultPath(application->GetDefaultPath());
	return (cpw::INavigatorManager *) nm;
}

cpw::IGraphicObjectLoader * OsgFactory::CreateObjectLoader()
{
	return (cpw::IGraphicObjectLoader *) new iosg::OsgObjectLoader();
}

cpw::ICamera * OsgFactory::CreateCamera()
{
	return (cpw::ICamera *) new iosg::OsgCamera();
}

cpw::IOverlayImage * OsgFactory::CreateOverlayImage(const cpw::Point2d<float> &utm0,
									  const cpw::Point2d<float> &utm1,
									  const int &x, const int &y, 
									  const cpw::IOverlayImage::CPW_PIXEL_FORMAT &pf)
{
	return (cpw::IOverlayImage *) new iosg::OsgOverlayImage(utm0, utm1, x, y, pf);
}

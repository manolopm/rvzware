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
#include <stdlib.h>

//#include <stdafx.h>

#include <cpw/graphic/ApplicationScene.h>


using namespace cpw;


cpw::ApplicationScene* ApplicationScene::instance = NULL;
//OpenThreads::Mutex ApplicationScene::constructor_lock;

ApplicationScene::ApplicationScene(): scene()
{	
}

ApplicationScene::~ApplicationScene()
{
	delete scene;
}


cpw::ApplicationScene * ApplicationScene::GetInstance()
{

	if(instance == NULL)
		instance = new ApplicationScene;

	return instance;
}


void ApplicationScene::ReleaseInstance()
{

	if(instance != NULL)
	{
		delete instance;
		instance = NULL;
	
	}
}


void ApplicationScene::SetScene(IScene *scen) 
{ 
	scene = scen; 
}

IScene *ApplicationScene::GetScene() 
{
	return scene; 
}

void ApplicationScene::SetNavigatorManager(INavigatorManager *_navigator_manager)
{
	navigator_manager = _navigator_manager; 
}
INavigatorManager * ApplicationScene::GetNavigatorManager()
{
	return navigator_manager;
}
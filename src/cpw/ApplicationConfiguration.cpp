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
#include <cstdlib>
#include <fstream>

#include "stdafx.h"

#include <cpw/ApplicationConfiguration.h>


using namespace cpw;

cpw::ApplicationConfiguration* ApplicationConfiguration::instance = NULL;
OpenThreads::Mutex ApplicationConfiguration::appmutex;


ApplicationConfiguration::ApplicationConfiguration(): root_path(), listen_port(3000), is_themed(false), icon_size(16), big_icon_size(16)
{
/*	char *root = getenv("CAPAVIEWER_ROOT");

	if(root != NULL)
		root_path = root;
	else 
*/		root_path = ".";
}

ApplicationConfiguration::~ApplicationConfiguration()
{
}

ApplicationConfiguration* ApplicationConfiguration::GetInstance()
{
	appmutex.lock();
						
	if (instance == NULL)
		instance = new ApplicationConfiguration(); 

	appmutex.unlock();
	return instance;
}

void ApplicationConfiguration::ReleaseInstance()
{
	appmutex.lock();
	
	if (instance != NULL)
	{
		delete instance;
		instance = NULL;
	}

	appmutex.unlock();
}


void ApplicationConfiguration::SetBackgroundColour(const std::string &rgb)
{
	backgroundColour = rgb;
	Modified();
}

void ApplicationConfiguration::SetForegroundColour(const std::string &rgb)
{	
	foregroundColour = rgb;
	Modified();
}

void ApplicationConfiguration::SetTitleBarGradient1Colour(const std::string &rgb)
{	
	titleBarGradient1Colour = rgb;
	Modified();
}

void ApplicationConfiguration::SetTitleBarGradient2Colour(const std::string &rgb)
{	
	titleBarGradient2Colour = rgb;
	Modified();
}

void ApplicationConfiguration::SetBackgroundGradient1Colour(const std::string &rgb)
{	
	backgroundGradient1Colour = rgb;
	Modified();
}

void ApplicationConfiguration::SetBackgroundGradient2Colour(const std::string &rgb)
{	
	backgroundGradient2Colour = rgb;
	Modified();
}

void ApplicationConfiguration::SetFontLightColour(const std::string &rgb)
{	
	fontLightColour = rgb;
	Modified();
}

void ApplicationConfiguration::SetFontDarkColour(const std::string &rgb)
{	
	fontDarkColour = rgb;
	Modified();
}

void ApplicationConfiguration::SetPageColour(const std::string &rgb)
{	
	pageColour = rgb;
	Modified();
}

void ApplicationConfiguration::SetPageFontColour(const std::string &rgb)
{
	pageFontColour = rgb;
	Modified();
}

void ApplicationConfiguration::SetIconSize(const std::string &is)
{	
	icon_size = atoi(is.c_str());
	Modified();
}

void ApplicationConfiguration::SetBigIconSize(const std::string &is)
{	
	big_icon_size = atoi(is.c_str());
	Modified();
}

void ApplicationConfiguration::SetUIConfiguration(const std::string &p)
{
	ui_config = p;
	Modified();
}



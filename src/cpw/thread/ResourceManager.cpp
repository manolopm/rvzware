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

#include <vector>
#include <string>
#include <sstream>

//#include <stdafx.h>

#include <cpw/thread/ResourceManager.h>

using namespace cpw;

ResourceManager::ResourceManager(cpw::IRequestReceiver *in_thread, int npet): RequestThread(in_thread, npet)
{
}
	
ResourceManager::~ResourceManager()
{

}


void ResourceManager::Process(cpw::Request &request) 
{

}

void ResourceManager::ProcessReturnRequest(cpw::Request &request) 
{

}


void ResourceManager::AddNewLayer(cpw::TypeId &layer)
{

}

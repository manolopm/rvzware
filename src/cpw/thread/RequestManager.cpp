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
#include <iostream>
#include <fstream>

//#include <stdafx.h>

#include <cpw/thread/RequestManager.h>
#include <cpw/entity/EntityRegistry.h>



using namespace cpw;


RequestManager::RequestManager(IRequestReceiver *visual, const std::string &cn, int npet, IStatusController *status): RequestThread(visual, npet, status), layer_class_name(cn), layers()
{
}

RequestManager::~RequestManager(void)
{
}


void RequestManager::Process(cpw::Request &request) 
{

	GetLayers();

	for(int i = 0; i < (int) layers.size(); i++)
	{
		request.layer = layers[i];
		SendRequest(request);
	}
}


std::vector<cpw::Entity *> &RequestManager::GetLayers() 
{
	layers = cpw::EntityRegistry::GetInstance()->GetEntities(layer_class_name);
	
	//remove not visible
	for(int i = 0; i < (int) layers.size(); i++)

		if(!layers[i]->isVisible()) 
		{
			layers.erase(layers.begin() + i);
			i--;
		}


	return layers;
}


/*
 * Rvzware based in CAPAWARE 3D
 *
 * Rvzware is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * Rvzware is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this application; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * The Rvzware development team
 */

#include <sstream>

#include <iosg/stdafx.h>

#include <iosg/OsgPetitionManager.h>


#define NOTAVAILABLE 0xffffffff

using namespace cpw::iosg;

OsgPetitionManager::OsgPetitionManager() : id_generator(0), request_manager()
{
}

OsgPetitionManager::~OsgPetitionManager(void)
{
}


void OsgPetitionManager::NewAttendedPetition(osg::Texture2D *new_img, const unsigned long int &petition_id)
{
	petitions_mutex.lock();

	std::map<unsigned long int, osg::ref_ptr<osg::Texture2D> >::iterator i = pending_petitions.find(petition_id);

	if (i == pending_petitions.end())
	{
		//AQUI NO DEBERIA ENTRAR NUNCA
		petitions_mutex.unlock();
		return;
	}

	if(new_img == NULL)
		i->second = (osg::Texture2D *) NOTAVAILABLE;  //si pasa por aquí es que hay un tile al que no se le asigna textura
	else
		i->second = new_img;

	petitions_mutex.unlock();
}


bool OsgPetitionManager::IsPetitionAttended(const unsigned long int &petition_id, osg::ref_ptr<osg::Texture2D> &out_text)
{
	//petitions_mutex.lock();
 
	bool result = true;

	std::map<unsigned long int, osg::ref_ptr<osg::Texture2D> >::iterator i = pending_petitions.find(petition_id);

	if (i == pending_petitions.end())
	{
		//petitions_mutex.unlock();
		out_text = NULL;

	}
	else
	{
		if(i->second == (osg::Texture2D *) NOTAVAILABLE)
			
			result = false;

		out_text = i->second;
		
		if (out_text != NULL)
			pending_petitions.erase(i);	
	}

	//petitions_mutex.unlock();
	return result;
}

unsigned long int OsgPetitionManager::NewPetition(const cpw::Point2d<float> &x0_y0, const cpw::Point2d<float> &x1_y1, unsigned int tile_size, const std::string &filename)
{
	unsigned long int id_to_return;
	petitions_mutex.lock();
	id_generator++;
		
	pending_petitions[id_generator] = NULL;
	id_to_return = id_generator;
	petitions_mutex.unlock();

	//fill the request
	cpw::Request request;
	request.id = id_to_return;
	request.xo = x0_y0.x;
	request.yo = x0_y0.y;
	request.xf = x1_y1.x;
	request.yf = x1_y1.y;
	request.tile_size = tile_size;
	request.tile_width = tile_size;
	request.tile_height = tile_size;

	std::stringstream str;
	if(!filename.empty())
	{
		str << filename << "_" << tile_size;
	}
	else
	{
		str << "tile_" << request.xo << "_" << request.xf << "_" << request.yo << "_" << request.yf << "_" << tile_size;
	}
	request.tile_id = str.str();


	request_manager->ProcessRequest(request);

	return id_to_return;
}

void OsgPetitionManager::Clear()
{
	pending_petitions.clear();
	request_manager->Clear();
}

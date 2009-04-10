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
#ifndef _REQUEST_
#define _REQUEST_

#include <string>
#include <map>

#include <cpw/entity/Entity.h>

#include <cpw/Export.h>

namespace cpw {

	/** 
		\brief 
		\ingroup cpw
	*/
	struct Request{
		long int id;
		Entity *layer;
		std::string tile_id;
		
		int nid;
		float xo, yo,  xf,  yf;

		std::string file;
		std::string filename;
		std::map<int, std::string> tiles;

		unsigned int tile_size;
		unsigned int tile_width;
		unsigned int tile_height;

		std::string composed_url;

		bool force;
		bool error;

		Request(): id(), layer(), tile_id(), nid(), xo(), yo(), xf(), yf(), tiles(), tile_size(256),tile_width(256),tile_height(256), error(false), composed_url(), force(false) {}
		Request(long int n, float xi=0.0, float yi=0.0, float xl=0.0, float yl=0.0): id(n), layer(), tile_id(), nid(), xo(xi), yo(yi), xf(xl), yf(yl), tiles(), tile_size(256),tile_width(256),tile_height(256), error(false), composed_url(), force(false)
		{}
	};

}

#endif
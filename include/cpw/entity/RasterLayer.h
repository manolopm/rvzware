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
#ifndef _RASTERLAYER_
#define _RASTERLAYER_

#include <string>
#include <vector>
#include "Layer.h"
#include <cpw/Export.h>


namespace cpw {


	/** 
		\brief Por comentar ....
		\ingroup cpw
	*/
	class CPWEXPORT RasterLayer: public Layer
	{

	public:

		RasterLayer(const cpw::TypeId id = cpw::TypeId(), const std::string &class_name = std::string("RasterLayer"));

		RasterLayer(std::string nam, std::string prim, std::string htm,	
			  bool dyn, std::string desc,
			  std::vector<Attribute> &attr);


		RasterLayer(const RasterLayer &layer);

		virtual ~RasterLayer();

		RasterLayer &operator = (const RasterLayer &layer);

		virtual void SetValue(const std::string &f, const std::string &v);

		virtual Entity* Clone() {return new RasterLayer(*this);}

	protected:

		virtual int CreatePersistence();
		virtual int AdaptPersistence(Node *root);

	private:


	};

}


#endif 

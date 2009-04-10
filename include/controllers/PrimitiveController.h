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
#ifndef _PRIMITIVECONTROLLER_
#define _PRIMITIVECONTROLLER_

#include <wx/wx.h>
#include <string>

#include <cpw/entity/EntityFactory.h>
#include <cpw/persistent/IPersistentManager.h>


namespace cpw
{

	namespace controllers 
	{  

		class PrimitiveController
		{
		public:
			PrimitiveController(void);

			~PrimitiveController(void);

			std::string GetDefaultContainerLayer(wxWindow* parent, cpw::EntityFactory &entity_primitive_factory, cpw::IPersistentManager *persistent_manager);

		protected:


		};
	}

}

#endif
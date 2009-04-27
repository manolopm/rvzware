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
#ifndef _IDGENERATOR_
#define _IDGENERATOR_

#include <string>

#include <cpw/common/TypeId.h>
#include <cpw/Export.h>


namespace cpw
{
	/** 
		\brief Allows to create unique Id's for the entities of the application
		\ingroup cpw
	*/
	class CPWEXPORT IdGenerator
	{
		public:

			/** Constructor */
			IdGenerator(void);

			/** Destructor */
			~IdGenerator(void);

			/** Introduce a new namespace for the Id generator
				\param namespc
			*/
			static void SetNamespace(std::string namespc);
			
			/** Returns a new unique ID */
			cpw::TypeId GetNewId();
			
		private:

			static time_t last_id_generated;  
			
			static TypeId namespc;	///< keeps the actual namespace
			
	};
}
#endif



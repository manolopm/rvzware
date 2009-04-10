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
#ifndef _IPERSISTENTCALLBACK_
#define _IPERSISTENTCALLBACK_

#include <cpw/common/types.h>

namespace cpw
{
	class CPWEXPORT Persistent;


	/** 
		\brief Interface for the pesistent callbacks
		\ingroup cpw
	*/
	class IPersistentCallBack
	{
		public:
			
			/** Destructor initialized empty */ 
			virtual ~IPersistentCallBack(){};

			/** Saves...
				\param persistent
				\param overwrite
			*/
			virtual PersistentError Save(cpw::Persistent* persistent, bool overwrite = false) = 0;

			/** Loads...
				\param persistent
			*/
			virtual PersistentError Load(cpw::Persistent* persistent) = 0;

			/** Updates...
				\param persistent
			*/
			virtual PersistentError Update(cpw::Persistent* persistent) = 0;

			/** Deletes...
				\param persistent
			*/
			virtual PersistentError Delete(cpw::Persistent* persistent) = 0;

			/** Virtual function to implement the prototype pattern */
			virtual IPersistentCallBack* Clone() = 0; 
	};
}

#endif


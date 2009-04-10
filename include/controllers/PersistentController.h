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
#ifndef _PERSISTENTCONTROLLER_
#define _PERSISTENTCONTROLLER_

#include <wx/wx.h>
#include <string>
#include <map>

#include <cpw/entity/Entity.h>
#include <cpw/common/types.h>
#include <cpw/persistent/Persistent.h>

namespace cpw
{

	namespace controllers 
	{  

		/** 
			\brief This controller is responsible for the management of persistence
			\ingroup cwcontent
		*/
		class PersistentController
		{
		public:
			PersistentController(void);
			~PersistentController(void);

			

			//load an object from disk without error handling
			cpw::PersistentError Load(cpw::Persistent **persistent, const std::string &url);

			//load an object with error handling
			cpw::Persistent *Load(wxWindow* parent, const std::string &url, const std::string &type);
			
			//load an object using a File Dialog using the primitive url
			cpw::Persistent *Load(wxWindow* parent, const std::string &type);

			
			cpw::PersistentError Save(wxWindow* parent, cpw::Persistent *persistent, bool overwrite);

			cpw::PersistentError Save(wxWindow* parent, cpw::Persistent *persistent, const std::string &extension, bool overwrite, bool primitive_directory = false);


		private:

			std::map<std::string, std::string> file_description;

		};
	}

}

#endif

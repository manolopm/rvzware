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
#ifndef _PROJECTCONTROLLER_
#define _PROJECTCONTROLLER_

#include <wx/wx.h>
#include <string>

#include <cpw/common/types.h>
#include <cpw/common/types.h>
#include <cpw/common/IStatusController.h>

#include <cpw/graphic/IGraphicFactory.h>
#include <cpw/entity/LayerTree.h>

namespace cpw
{


namespace controllers 
	{  

		/** 
			\brief This controller is responsible for the management of general Project(layers and elements)
			\ingroup cpw
		*/
		class ProjectController
		{
		public:
			ProjectController();
			~ProjectController();

			/** 
				\brief Loads a project from an url
			*/
			bool GetCloseApplication();
			void CloseProject(cpw::LayerTree &layer_tree);

			void Delete(const cpw::TypeId &ent_id, const cpw::TypeId &parent_id);

			bool OpenProject(const std::string &url, const std::string &default_path, 
							 cpw::IStatusController *status_controller,
							 cpw::LayerTree &layer_tree, cpw::IGraphicFactory *graphic_factory);

			bool OpenProject(wxWindow* parent, const std::string &default_path, 
					         cpw::IStatusController *status_controller,
							 cpw::LayerTree &layer_tree, cpw::IGraphicFactory *graphic_factory);

			std::string GetProjectName() {return project_name;}
			std::string GetProjectFullName() {return project_fullname;}
			std::string GetSRS() {return srs;}

		private:
			bool close_application;
			std::string project_name, project_fullname;
			std::string srs;

		};
	}

}

#endif
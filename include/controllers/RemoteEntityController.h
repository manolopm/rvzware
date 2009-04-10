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
#ifndef _REMOTEENTITYCONTROLLER_
#define _REMOTEENTITYCONTROLLER_

#include <wx/wx.h>
#include <string>

#include <cpw/common/types.h>
#include <gui/UIRemoteImportation.h>

namespace cpw
{

	namespace controllers 
	{  



		/** 
			\brief This controller is responsible for the management of remote entities (layers and elements)
			\ingroup cpw::controllers
		*/
		class RemoteEntityController
		{
		public:
			RemoteEntityController();
			RemoteEntityController(wxWindow* parent);
			~RemoteEntityController(void);

			/** 
				\brief Shows remote entity browse window
			*/
			void CreateRemoteEntity(wxWindow *parent);

		private:
			
			cpw::gui::UIRemoteImportation *ui_dialog;
			wxWindow *parent_;

		};
	}
}

#endif



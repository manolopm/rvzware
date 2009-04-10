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
#ifndef _WCSSERVERCONNECTIONCONTROLLER_
#define _WCSSERVERCONNECTIONCONTROLLER_

#include <wx/wx.h>
#include "EntityController.h"
#include "../gui/UIWCSServerConnection.h"
#include <ogcwcs/WcsLayerController.h>

 
namespace cpw
{
	namespace gui {class UIWCSServerConnection; }

	namespace controllers
	{

		/** 
			\brief This controller is responsible for the management of WcsLayer
			\ingroup cwcontent
		*/
		class WCSServerConnectionController: public EntityController
		{
			public:
				
				WCSServerConnectionController(wxWindow *_parent,cpw::LayerTree &tr_lay, bool _build=false);
				
				virtual ~WCSServerConnectionController();

				void ConnectServer(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("Server Connection"), const wxPoint& pos = wxDefaultPosition, 
								   const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE);

				int Connect(const std::string &server_url, std::string *abstr);

				std::vector<std::string> GetImageFormats(const std::string &folder);
				std::vector<std::string> GetSRS(const std::string &folder);

				void ModifyProperties(cpw::Entity* ent);

				void FillLayerTree();

			private:
				ogcwcs::WcsLayerController* controller;
				cpw::gui::UIWCSServerConnection *ui_ServerConnection;

				void ReadServersFile(std::vector<std::string> &servers);
				void WriteServersFile(std::vector<std::string> &servers);
				void FillWCSL(std::string &layer, ogcwcs::WcsLayer* wcsl);

				void ReadSRS(std::vector<std::string> &srs_list);

				wxWindow* parent;
				cpw::LayerTree *layer_tree;		
				bool build;
		};

	}

}

#endif
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
#ifndef _UIREMOTECONNECTION_
#define _UIREMOTECONNECTION_


#include <string>

#include <wx/wx.h>
#include <wx/dialog.h>

/*!
 *  \file UIRemoteConnection.h
 */

namespace cpw 
{ 
	namespace gui
	{
		/*!
		 *  \class UIRemoteConnection UIRemoteConnection.h "cwcontent/gui/UIRemoteConnection.h"
		 *  \ingroup cwcontent
		 *  \brief This is a dialog showing the hostname and port
		 *
		 *  This is a dialog showing the hostname and port.
		 */
		class UIRemoteConnection : public wxDialog
		{
		public:
			UIRemoteConnection(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("Connection"),
				const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
				long style = wxDEFAULT_DIALOG_STYLE);
			virtual ~UIRemoteConnection();

			void SetHostname(std::string hostname);
			std::string GetHostname();
			void SetPort(int port);
			int GetPort();

		private:
			wxWindow *paren;
			wxStaticText *WxStaticText2;
			wxTextCtrl *tc_host;
			wxStaticText *WxStaticText1;
			wxTextCtrl *tc_port;
			wxButton *connect_button;
			wxButton *cancel_button;
		
			void Close();
			void OnClose(wxCloseEvent& event);
			void CreateGUIControls();

			void OnButtonCancel(wxCommandEvent& WXUNUSED(event));
			void OnButtonConnect(wxCommandEvent& WXUNUSED(event));

			void OnPaint(wxPaintEvent& event);
			void render(wxDC& dc);
		
		DECLARE_EVENT_TABLE()
		};
	}

}

#endif

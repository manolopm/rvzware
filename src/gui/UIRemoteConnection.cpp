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
#include <string>

#include "stdafx.h"

#include <gui/UIRemoteConnection.h>
#include <cpw/ApplicationConfiguration.h>


enum
{
	ID_CONNECTBUTTON = 1003,
	ID_CANCELBUTTON = 1002,
	ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
};


using namespace cpw::gui;


BEGIN_EVENT_TABLE(UIRemoteConnection,wxDialog)	
	EVT_CLOSE(UIRemoteConnection::OnClose)
	EVT_BUTTON  (ID_CANCELBUTTON,    UIRemoteConnection::OnButtonCancel)
	EVT_BUTTON  (ID_CONNECTBUTTON,   UIRemoteConnection::OnButtonConnect)
	EVT_PAINT	    (				 UIRemoteConnection::OnPaint )
END_EVENT_TABLE()



/*!
 *  \param parent A pointer to the main panel
 *  \param id Id of the window
 *  \param title Title of window
 *  \param position Position of the window
 *  \param size Width of the widget
 *  \param style Style of panel
 */
UIRemoteConnection::UIRemoteConnection(wxWindow *parent, wxWindowID id, const wxString &title,
	const wxPoint &position, const wxSize& size, long style)
	: wxDialog(parent, id, title, position, size, style), paren(parent)
{
	if (!(cpw::ApplicationConfiguration::GetInstance()->IsThemed()))
	{
		SetFont(paren->GetFont());
		SetForegroundColour(paren->GetForegroundColour());
		SetBackgroundColour(paren->GetBackgroundColour());
	}		
	
	CreateGUIControls();
}


UIRemoteConnection::~UIRemoteConnection()
{
} 


void UIRemoteConnection::CreateGUIControls()
{
	SetTitle(wxT("Connection"));
	SetIcon(wxNullIcon);
	SetSize(8,8,390,141);
	Center(); 

	tc_host = new wxTextCtrl(this, wxID_ANY, wxT(""), wxPoint(85,15), wxSize(276,23), 0, wxDefaultValidator, wxT("host_address"));
	tc_port = new wxTextCtrl(this, wxID_ANY, wxT("3000"), wxPoint(85,40), wxSize(276,23), 0, wxDefaultValidator, wxT("port"));

	connect_button = new wxButton(this, ID_CONNECTBUTTON, wxT("Connect"), wxPoint(203,75), wxSize(75,23), 0, wxDefaultValidator, wxT("next_button"));
	connect_button->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	cancel_button = new wxButton(this, ID_CANCELBUTTON, wxT("Cancel"), wxPoint(287,75), wxSize(75,23), 0, wxDefaultValidator, wxT("cancel_button"));
	cancel_button->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

}


void UIRemoteConnection::OnClose(wxCloseEvent& /*event*/)
{
	Close();
	//Destroy();
}


void UIRemoteConnection::OnButtonCancel(wxCommandEvent& WXUNUSED(event))
{
	Close();
}

void UIRemoteConnection::Close()
{
	/*wxMessageDialog message(this,wxString("Save changes before quit?"), wxString("Warning"),wxICON_EXCLAMATION |wxYES_NO |wxCANCEL);
	int modal = message.ShowModal();
	if(modal == wxID_YES)
	{		
		this->EndModal(wxID_OK);		
	}
	if(modal == wxID_NO )
	{
		this->EndModal(wxID_CANCEL);		
	}*/
	this->EndModal(wxID_CANCEL);
}

void UIRemoteConnection::OnButtonConnect(wxCommandEvent& WXUNUSED(event))
{
	this->EndModal(wxID_OK);
}


void UIRemoteConnection::SetHostname(std::string hostname)
{
	tc_host->SetValue(wxString::From8BitData(hostname.c_str()));
}


std::string UIRemoteConnection::GetHostname()
{
	return std::string((const char*)tc_host->GetLineText(0).To8BitData());
}


void UIRemoteConnection::SetPort(int port)
{
	tc_port->SetValue(wxString::Format(_("%d"),port));
}


int UIRemoteConnection::GetPort()
{
	long port;
	tc_port->GetLineText(0).ToLong(&port);
	return port;
}

void UIRemoteConnection::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	render(dc);
}


void UIRemoteConnection::render(wxDC& dc)
{
	int client_h = 0;
	int client_w = 0;
	GetClientSize (&client_w, &client_h);

	if (!(cpw::ApplicationConfiguration::GetInstance()->IsThemed()))
	{
	  wxColour c_pen   = wxString(cpw::ApplicationConfiguration::GetInstance()->GetBackgroundGradient2Colour().c_str(),wxConvUTF8);
	wxColour c_backg = wxString(cpw::ApplicationConfiguration::GetInstance()->GetBackgroundGradient1Colour().c_str(),wxConvUTF8);	
wxColour c_brush = wxString(cpw::ApplicationConfiguration::GetInstance()->GetBackgroundColour().c_str(),wxConvUTF8);
dc.SetTextForeground(wxColour(wxString(cpw::ApplicationConfiguration::GetInstance()->GetFontLightColour().c_str(),wxConvUTF8)));
		dc.SetPen(wxPen(c_pen));
		dc.SetBrush(wxBrush(c_brush));
		dc.GradientFillLinear( wxRect(0,0,client_w,client_h), c_backg, c_pen, wxSOUTH);
	}

	dc.DrawRotatedText(_T("Server IP: "),20,15, 0);
	dc.DrawRotatedText(_T("Server Port:"),20,40, 0);
}



#ifdef _DONTBUILD_

#include <wx/wx.h>

#include <gui/UIRemoteConnection.h"
#include <controllers/RemoteController.h"


using namespace cpw::gui;


BEGIN_EVENT_TABLE(UIRemoteConnection, wxDialog)
	EVT_BUTTON(wxID_OK, UIRemoteConnection::OnButtonOK)
	EVT_BUTTON(wxID_CANCEL, UIRemoteConnection::OnButtonCancel)
END_EVENT_TABLE()

/*!
 *  \fn UIRemoteConnection::UIRemoteConnection(const RemoteController *remote_controller,
		const wxString& title, const wxPoint& pos, const wxSize& size)
 *  \brief 
 */
UIRemoteConnection::UIRemoteConnection(RemoteController *remote_controller,
	const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxDialog((wxFrame*)NULL, wxID_ANY, title, pos, size)
{
	this->remote_controller = remote_controller;

	SetTitle(wxT("Connect to..."));
	Center();

	label_host = new wxStaticText(this, wxID_ANY, wxT("Hostname: "));
	label_port = new wxStaticText(this, wxID_ANY, wxT("Port: "));

	tc_host = new wxTextCtrl(this, wxID_ANY, wxT(""));
	tc_port = new wxTextCtrl(this, wxID_ANY, wxT("3000"));

	button_ok = new wxButton(this, wxID_OK, wxT("Connect"));
	button_cancel = new wxButton(this, wxID_CANCEL, wxT("Cancel"));

	do_layout();
}


/*!
 *  \fn void UIRemoteConnection::do_layout()
 *  \brief 
 */
void UIRemoteConnection::do_layout()
{
	wxBoxSizer *host_sizer = new wxBoxSizer(wxHORIZONTAL);
	host_sizer->Add(label_host);
	host_sizer->Add(tc_host);


	wxBoxSizer *port_sizer = new wxBoxSizer(wxHORIZONTAL);
	port_sizer->Add(label_port);
	port_sizer->Add(tc_port);


	wxBoxSizer *button_sizer = new wxBoxSizer(wxHORIZONTAL);
	button_sizer->Add(button_ok);
	button_sizer->Add(button_cancel);

	wxBoxSizer *top_sizer = new wxBoxSizer(wxVERTICAL);
	top_sizer->Add(host_sizer,0,0,0);
	top_sizer->Add(port_sizer,0,0,0);
	top_sizer->Add(button_sizer,0,0,0);
	SetSizer(top_sizer);


}


/*!
 *  \fn void UIRemoteConnection::SetHostname(wxString hostname)
 *  \brief 
 */
void UIRemoteConnection::SetHostname(wxString hostname)
{
	tc_host->SetValue(hostname);
}


/*!
 *  \fn wxString UIRemoteConnection::GetHostname()
 *  \brief 
 */
wxString UIRemoteConnection::GetHostname()
{
	return tc_host->GetLineText(0);
}


/*!
 *  \fn void UIRemoteConnection::SetPort(int port)
 *  \brief 
 */
void UIRemoteConnection::SetPort(int port)
{
	tc_port->SetValue(wxString::Format(_("%d"),port));
}


/*!
 *  \fn int UIRemoteConnection::GetPort()
 *  \brief 
 */
int UIRemoteConnection::GetPort()
{
	long port;
	tc_port->GetLineText(0).ToLong(&port);
	return port;
}



//---------------------------------------------------------------------------
//--------------------------     EVENTS      --------------------------------
//---------------------------------------------------------------------------


/*!
 *  \fn void UIRemoteConnection::OnButtonOK(wxCommandEvent& WXUNUSED(event))
 *  \brief 
 */
void UIRemoteConnection::OnButtonOK(wxCommandEvent& WXUNUSED(event))
{
	EndDialog(wxID_OK);
}


/*!
 *  \fn void UIRemoteConnection::OnButtonOK(wxCommandEvent& WXUNUSED(event))
 *  \brief 
 */
void UIRemoteConnection::OnButtonCancel(wxCommandEvent& WXUNUSED(event))
{
	EndDialog(wxID_CANCEL);
}




#endif

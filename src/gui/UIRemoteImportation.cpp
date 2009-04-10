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

#include <gui/UIRemoteImportation.h>
#include <cpw/ApplicationConfiguration.h>


enum
{
	ID_OKBUTTON = 1003,
	ID_CANCELBUTTON = 1002,
	ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
};


using namespace cpw::gui;


BEGIN_EVENT_TABLE(UIRemoteImportation,wxDialog)	
	EVT_CLOSE(UIRemoteImportation::OnClose)
	EVT_BUTTON  (ID_CANCELBUTTON,    UIRemoteImportation::OnButtonCancel)
	EVT_BUTTON  (ID_OKBUTTON,   UIRemoteImportation::OnButtonOK)
	EVT_PAINT	(				      UIRemoteImportation::OnPaint )
END_EVENT_TABLE()



/*!
 *  \param parent A pointer to the main panel
 *  \param id Id of the window
 *  \param title Title of window
 *  \param position Position of the window
 *  \param size Width of the widget
 *  \param style Style of panel
 */
UIRemoteImportation::UIRemoteImportation(wxWindow *parent, wxWindowID id, const wxString &title,
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


UIRemoteImportation::~UIRemoteImportation()
{
} 


void UIRemoteImportation::CreateGUIControls()
{
	SetTitle(wxT("Remote Entity"));
	SetIcon(wxNullIcon);
	SetSize(8,8,390,111);
	Center(); 

	tc_host = new wxTextCtrl(this, wxID_ANY, wxT("http://"), wxPoint(85,15), wxSize(276,23), 0, wxDefaultValidator, wxT("host_address"));
	
	ok_button = new wxButton(this, ID_OKBUTTON, wxT("Connect"), wxPoint(203,46), wxSize(75,23), 0, wxDefaultValidator, wxT("next_button"));
	ok_button->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	cancel_button = new wxButton(this, ID_CANCELBUTTON, wxT("Cancel"), wxPoint(287,46), wxSize(75,23), 0, wxDefaultValidator, wxT("cancel_button"));
	cancel_button->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

}


void UIRemoteImportation::OnClose(wxCloseEvent& /*event*/)
{
	this->EndDialog(wxID_CANCEL);
	Destroy();
}


void UIRemoteImportation::OnButtonCancel(wxCommandEvent& WXUNUSED(event))
{
	this->EndDialog(wxID_CANCEL);		
}


void UIRemoteImportation::OnButtonOK(wxCommandEvent& WXUNUSED(event))
{
	this->EndDialog(wxID_OK);
}


void UIRemoteImportation::SetHostname(std::string hostname)
{
	tc_host->SetValue(wxString::From8BitData(hostname.c_str()));
}


std::string UIRemoteImportation::GetHostname()
{
	return std::string((const char*)tc_host->GetLineText(0).To8BitData());
}


void UIRemoteImportation::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	render(dc);
}


void UIRemoteImportation::render(wxDC& dc)
{
	int client_h = 0;
	int client_w = 0;
	GetClientSize (&client_w, &client_h);

	if (!(cpw::ApplicationConfiguration::GetInstance()->IsThemed()))
	{
		wxColour c_pen   = cpw::ApplicationConfiguration::GetInstance()->GetBackgroundGradient2Colour();
		wxColour c_backg = cpw::ApplicationConfiguration::GetInstance()->GetBackgroundGradient1Colour();	
		wxColour c_brush = cpw::ApplicationConfiguration::GetInstance()->GetBackgroundColour();
		dc.SetTextForeground(wxColour(cpw::ApplicationConfiguration::GetInstance()->GetFontLightColour()));
		dc.SetPen(wxPen(c_pen));
		dc.SetBrush(wxBrush(c_brush));
		dc.GradientFillLinear( wxRect(0,0,client_w,client_h), c_backg, c_pen, wxSOUTH);
	}

	dc.DrawRotatedText(_T("URL: "),25,20, 0);
}

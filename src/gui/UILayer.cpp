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
#include "stdafx.h"

#include <gui/UILayer.h>

#include "UIApplicationMainFrame.h"
#include <cpw/ApplicationConfiguration.h>


using namespace cpw::gui;

enum
{
	ID_BROWSEBUTTON = 1014,
	ID_PRIMITIVE_CHOICE = 1013,
	BUTTON_SELECT_HTML = 1011,
	ID_TC_DESCRIPTION = 1010,
	ID_WXSTATICTEXT3 = 1009,
	ID_BUTTON_CANCEL = 1008,
	ID_BUTTON_OK = 1007,
	BUTTON_ATTRIBUTES = 1005,
	ID_TC_HTML = 1004,
	ID_TC_NAME = 1003,
};

BEGIN_EVENT_TABLE(UILayer,wxDialog)
	EVT_BUTTON  (wxID_OK,				UILayer::OnButtonOK)
	EVT_BUTTON  (wxID_CANCEL,			UILayer::OnButtonCancel)
	EVT_BUTTON  (BUTTON_ATTRIBUTES,		UILayer::OnButtonAttributes)
	EVT_BUTTON  (BUTTON_SELECT_HTML,	UILayer::OnButtonHtml)
	EVT_BUTTON	(ID_BROWSEBUTTON,		UILayer::OnBrowsePrimitivesButton)
    EVT_MOVE    (UILayer::OnMove										)
	EVT_PAINT	(						UILayer::OnPaint )
	EVT_CHOICE		(ID_PRIMITIVE_CHOICE, UILayer::OnChoiceChanged)
END_EVENT_TABLE()

void UILayer::OnMove(wxMoveEvent& event)
{
}


UILayer::UILayer(cpw::controllers::LayerController *lc_, wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style), paren(parent), modify(false),primitive_url(), lc(lc_), open_primitive(NULL)
{
	CreateGUIControls();
}

UILayer::~UILayer()
{
	if (open_primitive!=NULL) delete open_primitive;
} 

void UILayer::CreateGUIControls()
{
	if (!(cpw::ApplicationConfiguration::GetInstance()->IsThemed()))
	{
		SetFont(paren->GetFont());
		SetForegroundColour(paren->GetForegroundColour());
		SetBackgroundColour(paren->GetBackgroundColour());
	}	

	SetTitle(wxT("Layer"));
	SetIcon(wxNullIcon);
	SetSize(8,8,390,335-44);

	Center();
	
	std::string &primitive_path = ApplicationConfiguration::GetInstance()->GetPrimitiveDirectory();
	open_primitive =  new wxFileDialog(this, wxT("Choose a file"), wxT(primitive_path), wxT(""), wxT("*.cla"), wxOPEN);

	int col_= 20;int row_ = 20;
	
	primitiveChoice = new wxChoice(this, ID_PRIMITIVE_CHOICE, wxPoint(col_+43,row_), wxSize(270,19), arrayStringFor_formatChoice, 0, wxDefaultValidator, wxT("WxChoice1"));
	primitiveChoice->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
	primitiveChoice->SetSelection(-1);

	primitiveBrowseButton = new wxButton(this, ID_BROWSEBUTTON, wxT("..."), wxPoint(col_+320,row_), wxSize(23,22), 0, wxDefaultValidator, wxT("BrowseButton"));
	primitiveBrowseButton->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
	
	row_ += 25;

	tc_name= new wxTextCtrl(this, ID_TC_NAME, wxT(""), wxPoint(col_+43,row_), wxSize(270,19), 0, wxDefaultValidator, wxT(""));
	tc_name->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
	tc_name->SetFocus();

	tc_html = new wxTextCtrl(this, ID_TC_HTML, wxT(""), wxPoint(col_+43,row_+25), wxSize(270,19), 0, wxDefaultValidator, wxT("tc_html"));
	tc_html->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	button_html = new wxButton(this, BUTTON_SELECT_HTML, wxT("..."), wxPoint(col_+320,row_+25), wxSize(23,22), 0, wxDefaultValidator, wxT("WxButton5"));
	button_html->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	tc_description = new wxTextCtrl(this, ID_TC_DESCRIPTION, wxT(""), wxPoint(20,row_+75), wxSize(342,80), wxTE_MULTILINE, wxDefaultValidator, wxT("tc_description"));
	tc_description->SetMaxLength(0);
	tc_description->AppendText(wxT(""));	
	tc_description->SetInsertionPointEnd();
	tc_description->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
	if (!(cpw::ApplicationConfiguration::GetInstance()->IsThemed()))
	{
		tc_description->SetBackgroundColour(wxColour(cpw::ApplicationConfiguration::GetInstance()->GetPageColour()));
		tc_description->SetForegroundColour(wxColour(cpw::ApplicationConfiguration::GetInstance()->GetPageFontColour()));
	}

	row_ += 85+80-24+5+10;

	button_attributes = new wxButton(this, BUTTON_ATTRIBUTES, wxT("Attributes..."), wxPoint(col_,row_+24), wxSize(78,23), 0, wxDefaultValidator, wxT("button_attributes"));
	button_attributes->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	button_ok = new wxButton(this, wxID_OK, wxT("Finish"), wxPoint(col_+183,row_+24), wxSize(75,23), 0, wxDefaultValidator, wxT("button_ok"));
	button_ok->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
	button_ok->Enable(false);

	button_cancel = new wxButton(this, wxID_CANCEL, wxT("Cancel"), wxPoint(col_+267,row_+24), wxSize(75,23), 0, wxDefaultValidator, wxT("button_cancel"));
	button_cancel->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
}

void UILayer::SetPrimitivesUrl(const std::map<std::string, cpw::TypeId> &vec_url)
{
	v_url = vec_url;
	std::map<std::string, cpw::TypeId>::const_iterator i = vec_url.begin();
	for(i; i!=vec_url.end();i++)
	{
		wxString* url = new wxString((wxString)(i->first));
		primitiveChoice->Insert(*url,0);
		delete url;
	}
}

void UILayer::OnChoiceChanged(wxCommandEvent& WXUNUSED(event))
{
    primitive_url = "";
	if (primitiveChoice->GetSelection() != wxNOT_FOUND)
	{		
		std::string pname = primitiveChoice->GetString(primitiveChoice->GetSelection());
		std::map<std::string, cpw::TypeId>::iterator i = v_url.find(pname);
		if (i!= v_url.end())
		{
			primitive_id = i->second;
			lc->InstacePrimitiveLayer(this);
			button_ok->Enable(true);
		}
	}

}

void UILayer::OnBrowsePrimitivesButton(wxCommandEvent& event)
{
    primitive_url = "";
	primitive_id = cpw::TypeId();
    if(open_primitive->ShowModal() == wxID_OK ) 
    {
        primitive_url = open_primitive->GetPath();
		std::string pname = primitiveChoice->GetString(primitiveChoice->GetSelection());
		lc->InstacePrimitiveLayer(this);
		primitiveChoice->Clear();
		lc->SetPrimitivesUrl(this);
		primitiveChoice->SetSelection(primitiveChoice->FindString(lc->GetPrimitiveName()));
		button_ok->Enable(true);
	}
}


void UILayer::OnButtonOK(wxCommandEvent& WXUNUSED(event))
{
	ButtonOK();
}

void UILayer::ButtonOK()
{
	std::string p = std::string(tc_name->GetValue());
	int cc = primitiveChoice->GetSelection();
	if((cc == wxNOT_FOUND && primitiveChoice->IsEnabled()) || (tc_name->IsEmpty()) || (p.at(0) == ' ' ) )
	{
		wxMessageDialog message(this,wxString("The layer needs a template and a name."), wxString("Warning"),wxICON_EXCLAMATION |wxOK);
		message.ShowModal();
	}
	else
	{	
		EndModal(wxID_OK);
		Show(true);
	}
}

void UILayer::OnButtonHtml(wxCommandEvent& WXUNUSED(event))
{
	std::string &root_path = ApplicationConfiguration::GetInstance()->GetRootDirectory();
	wxFileDialog dialog(this,_T("Open HTML file"),_T(root_path),wxEmptyString,
						   _T("File html(*.html)|*.html|All files(*.*)|*.*") );
	if(dialog.ShowModal() == wxID_OK)
	{		
		tc_html->SetValue(dialog.GetPath());
	}
}

void UILayer::OnButtonCancel(wxCommandEvent& WXUNUSED(event))
{
	if((!tc_name->IsEmpty())||(!tc_description->IsEmpty())||(!tc_html->IsEmpty()))
	{
		wxMessageDialog message(this,wxString("Save changes before quit?"), wxString("Warning"),wxICON_EXCLAMATION |wxYES_NO |wxCANCEL);
		int modal = message.ShowModal();
		if(modal == wxID_YES)
		{		
			ButtonOK();
		}
		if(modal == wxID_NO)
		{
			this->EndModal(wxID_CANCEL);
		}		
	}
	else
	{
		this->EndModal(wxID_CANCEL);
	}
}

void UILayer::OnButtonAttributes(wxCommandEvent& WXUNUSED(event))
{
	UIEAttributes new_attr_dlg(0,this,1, wxString("Layer attributes"),wxDefaultPosition);
	if(!attributes.empty())
		new_attr_dlg.SetAttributes(attributes);

	if(new_attr_dlg.ShowModal() == wxID_OK)
		attributes = new_attr_dlg.GetAttributes();
}



void UILayer::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	render(dc);
}


void UILayer::render(wxDC& dc)
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
		wxPoint box1_first(15-4,82-2-9);
		wxPoint box1_last(215+154+2,244+15);
		//dc.DrawRoundedRectangle(box1_first.x, box1_first.y, box1_last.x-box1_first.x, box1_last.y-box1_first.y,3.1f);
	}

	dc.DrawRotatedText(_T("Template "),19,35-25+10-3-4+10, 0);
	dc.DrawRotatedText(_T("Name "),19,35+10-3-4+10, 0);
	dc.DrawRotatedText(_T("HTML"),19,35+25+10-3+10, 0);
	dc.DrawRotatedText(_T("Description "),19,35+50+10+10, 0);
}

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
#include <sstream>

#include "stdafx.h"

#include <gui/UILayerPrimitive.h>
#include <gui/UIAttributes.h>

#include "UIApplicationMainFrame.h"
#include <cpw/ApplicationConfiguration.h>
#include <cpw/ApplicationConfiguration.h>



#define BUTTON_EXPLORER1 5000
#define BUTTON_EXPLORER2 5001
#define BUTTON_FONT  	 5002
#define BUTTON_ATTR      5003


using namespace cpw::gui;


BEGIN_EVENT_TABLE(UILayerPrimitive, wxDialog)
	EVT_BUTTON  (BUTTON_EXPLORER1,   UILayerPrimitive::OnButtonExplorer1)
	EVT_BUTTON  (BUTTON_ATTR,   UILayerPrimitive::OnButtonAttr)
	EVT_BUTTON  (wxID_CANCEL,   UILayerPrimitive::OnButtonCancel)
	EVT_BUTTON  (wxID_OK,   UILayerPrimitive::OnButtonOK)
    EVT_MOVE    (UILayerPrimitive::OnMove           )
	EVT_PAINT	(UILayerPrimitive::OnPaint			)
END_EVENT_TABLE()


void UILayerPrimitive::OnMove(wxMoveEvent& event)
{
}


UILayerPrimitive::UILayerPrimitive(wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
				:wxDialog(parent, id, title, pos, size, wxDEFAULT_DIALOG_STYLE), paren(parent)
{
	if (!(cpw::ApplicationConfiguration::GetInstance()->IsThemed()))
	{
		SetFont(paren->GetFont());
		SetForegroundColour(paren->GetForegroundColour());
		SetBackgroundColour(paren->GetBackgroundColour());
	}	

	SetTitle(title);
	Center();
	label_name = new wxStaticText(this, wxID_ANY, wxT("Name:"));
	label_name->Show(false);
    text_name = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    panel_name = new wxPanel(this, wxID_ANY);
	panel_name->Show(false);

	label_icon = new wxStaticText(this, wxID_ANY, wxT("Icon:"));
	label_icon->Show(false);
    text_icon = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    button_icon = new wxButton(this, BUTTON_EXPLORER1, wxT("..."));
 
	label_description = new wxStaticText(this, wxID_ANY, wxT("Description:"));
	label_description->Show(false);
    panel_desc1 = new wxPanel(this, wxID_ANY);
    panel_desc2 = new wxPanel(this, wxID_ANY);
	panel_desc1->Show(false);
	panel_desc2->Show(false);
    text_description = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);

    button_attributes = new wxButton(this, BUTTON_ATTR, wxT("Attributes..."));
    panel_attr = new wxPanel(this, wxID_ANY);
	panel_attr->Show(false);

	button_ok = new wxButton(this, wxID_OK, wxT("OK"));
    button_cancel = new wxButton(this, wxID_CANCEL, wxT("Cancel"));

    set_properties();
    do_layout();
}

UILayerPrimitive::~UILayerPrimitive()
{

}


void UILayerPrimitive::set_properties()
{      
    text_name->SetMinSize(wxSize(200, 20));
    text_icon->SetMinSize(wxSize(200, 20));    
	text_description->SetMinSize(wxSize(340-24, 100));
	if (!(cpw::ApplicationConfiguration::GetInstance()->IsThemed()))
	{
	  text_description->SetBackgroundColour(wxColour(wxString(cpw::ApplicationConfiguration::GetInstance()->GetPageColour().c_str(),wxConvUTF8)));
	  text_description->SetForegroundColour(wxColour(wxString(cpw::ApplicationConfiguration::GetInstance()->GetPageFontColour().c_str(),wxConvUTF8)));
	}
	button_icon->SetMinSize(wxSize(25, 20));
	panel_attr->SetMinSize(wxSize(53,20));

}


void UILayerPrimitive::do_layout()
{
    wxBoxSizer* sizer_1 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_2 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_3 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_5 = new wxBoxSizer(wxHORIZONTAL);

    wxFlexGridSizer* grid_sizer_1 = new wxFlexGridSizer(3, 3, 1, 1);
	sizer_1->AddSpacer(20);
	grid_sizer_1->Add(85,20,0);
    grid_sizer_1->Add(text_name, 0, 0, 0);
    grid_sizer_1->Add(panel_name, 1, wxEXPAND, 0);

	grid_sizer_1->Add(85,20,0);
    grid_sizer_1->Add(text_icon, 0, 0, 0);
    grid_sizer_1->Add(button_icon, 0, wxALIGN_LEFT, 0);

	grid_sizer_1->Add(85,20,0);
    grid_sizer_1->Add(panel_desc1,  1, wxEXPAND, 0);
    grid_sizer_1->Add(panel_desc2,  1, wxEXPAND, 0);
	//
	sizer_5->AddSpacer(20);
    sizer_5->Add(grid_sizer_1, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	sizer_5->AddSpacer(20);
	sizer_1->Add(sizer_5, 0, 0, 0);

 	sizer_2->Add(text_description, 1, 0, 0);
	sizer_1->Add(sizer_2, 1, wxALIGN_CENTER_HORIZONTAL, 0);

	sizer_1->AddSpacer(20);	

	sizer_3->AddSpacer(20);
	sizer_3->Add(button_attributes, 0, wxALIGN_LEFT, 0); 	
	sizer_3->Add(panel_attr,0,wxALIGN_RIGHT,0);	
	sizer_3->Add(78,0,0);
    sizer_3->Add(button_ok, 0, wxALIGN_RIGHT, 0);
	sizer_3->AddSpacer(5);
    sizer_3->Add(button_cancel, 0, wxALIGN_RIGHT, 0);
	sizer_3->AddSpacer(20);
	
    sizer_1->Add(sizer_3, 0, wxALIGN_RIGHT, 0);
	sizer_1->AddSpacer(10);

    SetSizer(sizer_1);
    sizer_1->Fit(this);
    Layout();
}


void UILayerPrimitive::OnButtonExplorer1(wxCommandEvent& WXUNUSED(event))
{
	std::string &icon_path = ApplicationConfiguration::GetInstance()->GetIconDirectory();
	wxFileDialog dialog(this,_T("Open Icon"),wxString(icon_path.c_str(),wxConvUTF8),wxEmptyString,
						   _T("PNG file(*.png)|*.png|BMP file(*.bmp)|*.bmp|JPG file(*.jpg)|*.jpg|All files(*.*)|*.*") );
	if(dialog.ShowModal() == wxID_OK)
	{		
		text_icon->SetValue(dialog.GetPath());
	}
	else
	{
	  text_icon->SetValue(wxT(""));		
	}
}

void UILayerPrimitive::OnButtonAttr(wxCommandEvent& WXUNUSED(event))
{
	UIAttributes new_attr_dlg(0,this,1, wxT("Primitive layer attributes"),wxDefaultPosition);
	if(!v_attr.empty())
		new_attr_dlg.SetAttributes(v_attr);

	if(new_attr_dlg.ShowModal() == wxID_OK)
		v_attr = new_attr_dlg.GetAttributes();

}

void UILayerPrimitive::OnButtonCancel(wxCommandEvent& WXUNUSED(event))
{
	if(!text_name->IsEmpty())
	{
		wxMessageDialog message(this,wxT("Save changes before quit?"), wxT("Warning"),wxICON_EXCLAMATION |wxYES_NO |wxCANCEL);
		int modal = message.ShowModal();
		if(modal == wxID_YES)
		{			
			this->EndModal(wxID_OK);
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

void UILayerPrimitive::OnButtonOK(wxCommandEvent& WXUNUSED(event))
{
  std::string p = std::string(text_name->GetValue().mb_str());
	if((text_name->IsEmpty()) || (p.at(0) == ' ' ) )
	{
		wxMessageDialog message(this,wxT("The primitive needs a name."), wxT("Warning"),wxICON_EXCLAMATION |wxOK);
		message.ShowModal();
	}
	else
	{
		this->EndModal(wxID_OK);
	}
}

std::string UILayerPrimitive::GetClassName()
{
	return std::string("ContainerLayer");
}

void UILayerPrimitive::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	render(dc);
}

void UILayerPrimitive::render(wxDC& dc)
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

	dc.DrawRotatedText(_T("Name: "),20,20, 0);
	dc.DrawRotatedText(_T("Description"),20,66, 0);
	dc.DrawRotatedText(_T("Icon:"),20,43, 0);
}

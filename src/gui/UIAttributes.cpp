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

#include <gui/UIAttributes.h>
#include <gui/UILayerPrimitive.h>
#include <gui/UIElementPrimitive.h>

#include <cpw/ApplicationConfiguration.h>

#define BUTTON_ADD		 5004
#define BUTTON_DELETE	 5005
#define GRID_ID			 5006

using namespace cpw::gui;


BEGIN_EVENT_TABLE(UIAttributes, wxDialog)
	EVT_BUTTON  (BUTTON_ADD,   UIAttributes::OnButtonAdd)
	EVT_BUTTON  (BUTTON_DELETE,   UIAttributes::OnButtonDelete)
	EVT_BUTTON  (wxID_CANCEL,   UIAttributes::OnButtonCancel)
	EVT_CHAR_HOOK (UIAttributes::OnCharHook)
	EVT_PAINT	    ( UIAttributes::OnPaint )
END_EVENT_TABLE()


void UIAttributes::OnCharHook(wxKeyEvent& event)
{    
	wxWindow *win = FindFocus ();
	if (win == NULL) 
	{
		event.Skip ();
		return;
	}
	if (event.GetKeyCode () == WXK_TAB) 
	{
		switch (win->GetId ()) 
		{
			case wxID_CANCEL:
				text_name->SetFocus();
				return;
		}
	}
	event.Skip ();

}

UIAttributes::UIAttributes(int flag, wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
				:wxDialog(parent, id, title, pos, size, wxDEFAULT_DIALOG_STYLE), paren(parent), parent_flag(flag), aux_value("")
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

	label_type = new wxStaticText(this, wxID_ANY, wxT("Type:"));
	label_type->Show(false);
    const wxString combo_box_type_choices[] = {
        wxT("numeric"),
        wxT("text"),
        wxT("yes/no")
    };
    combo_box_type = new wxComboBox(this, wxID_ANY, wxT("numeric"), wxDefaultPosition, wxDefaultSize, 3, combo_box_type_choices, wxCB_DROPDOWN);

	label_value = new wxStaticText(this, wxID_ANY, wxT("Value:"));
	label_value->Show(false);
    text_value = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
   
	button_add = new wxButton(this, BUTTON_ADD, wxT("Add"));
	button_delete = new wxButton(this, BUTTON_DELETE, wxT("Delete"));

	button_ok = new wxButton(this, wxID_OK, wxT("OK"));
    button_cancel = new wxButton(this, wxID_CANCEL, wxT("Cancel"));

	grid_1 = new wxGrid(this, GRID_ID,wxPoint(0,0),wxSize(250,100),wxALWAYS_SHOW_SB);
	grid_modify = false;
    set_properties();
    do_layout();
}

UIAttributes::~UIAttributes()
{

}


void UIAttributes::set_properties()
{    
    
    text_name->SetMinSize(wxSize(220, 20));
    text_value->SetMinSize(wxSize(220, 20));    
	
	grid_1->CreateGrid(0, 3);
    grid_1->SetRowLabelSize(0);
	grid_1->SetColLabelSize(20);
    grid_1->SetColLabelValue(0, wxT("Name"));
    grid_1->SetColLabelValue(1, wxT("Type"));
    grid_1->SetColLabelValue(2, wxT("Value"));
	grid_1->SetColumnWidth(0,80);
	grid_1->SetColumnWidth(1,80);
	grid_1->SetColumnWidth(2,80);
    grid_1->SetMinSize(wxSize(250, 100));
	grid_1->SetSelectionMode(wxGrid::wxGridSelectRows);
	grid_1->SetLabelFont(*wxNORMAL_FONT);

    grid_1->Enable(true);    
    grid_1->EnableEditing(false);
	grid_1->EnableGridLines(false);
	grid_1->DisableDragGridSize();
	grid_1->DisableCellEditControl();
	grid_1->HideCellEditControl();
}


void UIAttributes::do_layout()
{
    wxBoxSizer* sizer_1 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_2 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_3 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_4 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_5 = new wxBoxSizer(wxHORIZONTAL);

    wxFlexGridSizer* grid_sizer_1 = new wxFlexGridSizer(3, 2, 5, 1);

	sizer_1->AddSpacer(20);
	// Add name
	grid_sizer_1->Add(53,0,0);
    grid_sizer_1->Add(text_name, 0, 0, 0);    

	// Add type
	grid_sizer_1->Add(53,0,0);
    grid_sizer_1->Add(combo_box_type, 0, 0, 0);

	// Add value
	grid_sizer_1->Add(53,0,0);
    grid_sizer_1->Add(text_value, 0, 0, 0);

	sizer_2->AddSpacer(20);
    sizer_2->Add(grid_sizer_1, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	sizer_2->AddSpacer(20);
	sizer_1->Add(sizer_2, 0, 0, 0);
	sizer_1->AddSpacer(20);	
	
	sizer_3->Add(button_add, 0, wxALIGN_RIGHT, 0); 
	sizer_3->AddSpacer(5);
	sizer_3->Add(button_delete, 0, wxALIGN_RIGHT, 0); 
	sizer_3->AddSpacer(20);
	sizer_1->Add(sizer_3, 0, wxALIGN_RIGHT, 0);
	sizer_1->AddSpacer(10);

	sizer_4->Add(grid_1, 0, 0, 0);
	sizer_1->Add(sizer_4,1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL,0);
	sizer_1->AddSpacer(20);

    sizer_5->Add(button_ok, 0, wxALIGN_RIGHT, 0);
	sizer_5->AddSpacer(5);
    sizer_5->Add(button_cancel, 0, wxALIGN_RIGHT, 0);
	sizer_5->AddSpacer(20);

    sizer_1->Add(sizer_5, 0, wxALIGN_RIGHT, 0);
	sizer_1->AddSpacer(10);


    SetSizer(sizer_1);
    sizer_1->Fit(this);
    Layout();
}

void UIAttributes::OnButtonAdd(wxCommandEvent& WXUNUSED(event))
{
	if (!text_name->IsEmpty())
	{
		grid_1->InsertRows();
		grid_1->SetCellValue(0,0,(text_name->GetValue().Trim()).Trim(false));
		grid_1->SetCellValue(0,1,combo_box_type->GetValue());
		grid_1->SetCellValue(0,2,(text_value->GetValue().Trim()).Trim(false));
		
		text_name->Clear();
		text_value->Clear();
		combo_box_type->SetSelection(0);

		grid_modify = true;
	}
	else
	{
		text_name->Clear();
		wxMessageDialog message(this,wxT("You can't create an attibute without name."), wxT("Warning"),wxICON_EXCLAMATION |wxOK);
		message.ShowModal();
	}
}

void UIAttributes::OnButtonDelete(wxCommandEvent& WXUNUSED(event))
{
	int k= -1;
	for(int i=0; i<grid_1->GetNumberRows(); i++)
	{
		for( int j=0; j<grid_1->GetNumberCols(); j++)
		{
			if (grid_1->IsInSelection(i,j))
			{
				k = i;
				grid_modify = true;
			}
		}		
	}
	if (k!= -1) grid_1->DeleteRows(k);
}

void UIAttributes::OnButtonCancel(wxCommandEvent& WXUNUSED(event))
{
	if(grid_modify)
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


std::vector<cpw::Attribute> UIAttributes::GetAttributes()
{
	std::vector<cpw::Attribute> v_attr;
	cpw::Attribute attr;
	for(int i=0; i<grid_1->GetNumberRows(); i++)
	{
	  attr.name  = std::string(grid_1->GetCellValue(i,0).mb_str());
	  attr.type  = std::string(grid_1->GetCellValue(i,1).mb_str());	
	  attr.value = std::string(grid_1->GetCellValue(i,2).mb_str());
		v_attr.push_back(attr);
	}
	return v_attr;
}


void UIAttributes::SetAttributes(const std::vector<cpw::Attribute> &attr)
{
	for(std::vector<cpw::Attribute>::const_iterator i= attr.begin(); i!= attr.end(); i++)
	{
		grid_1->InsertRows();
		grid_1->SetCellValue(0,0,wxString(i->name.c_str(),wxConvUTF8));
		grid_1->SetCellValue(0,1,wxString(i->type.c_str(),wxConvUTF8));
		grid_1->SetCellValue(0,2,wxString(i->value.c_str(),wxConvUTF8));
	}
}

void UIAttributes::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	render(dc);
}


void UIAttributes::render(wxDC& dc)
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

	dc.DrawRotatedText(_T("Name:"),20,20, 0);
	dc.DrawRotatedText(_T("Type:"),20,45, 0);
	dc.DrawRotatedText(_T("Value:"),20,70, 0);
}

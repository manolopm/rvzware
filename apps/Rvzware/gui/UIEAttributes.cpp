/*
 * Rvzware based in CAPAWARE 3D
 *
 * Rvzware is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * Rvzware is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this application; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * The Rvzware development team
 */


#include <stdafx.h>

#include <gui/UIEAttributes.h>
#include <gui/UILayer.h>
#include <gui/UIElement.h>

#include <cpw/ApplicationConfiguration.h>

#define BUTTON_EAMODIFY	 1000
#define BUTTON_EADELETE	 1001
#define GRID_ATTR 1002


using namespace cpw::gui;

BEGIN_EVENT_TABLE(UIEAttributes, wxDialog)
	EVT_BUTTON  (BUTTON_EAMODIFY,   UIEAttributes::OnButtonEAModify)
	EVT_BUTTON  (wxID_CANCEL,   UIEAttributes::OnButtonCancel)
	EVT_BUTTON  (wxID_OK,   UIEAttributes::OnButtonOK)
	EVT_GRID_CMD_CELL_LEFT_DCLICK(GRID_ATTR, UIEAttributes::OnGridCellLeftClick)
	EVT_CHAR_HOOK (UIEAttributes::OnCharHook)
	EVT_PAINT	(           UIEAttributes::OnPaint )
END_EVENT_TABLE()


void UIEAttributes::OnCharHook(wxKeyEvent& event)
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
				text_value->SetFocus();
				return;
		}
	}
	event.Skip ();

}

UIEAttributes::UIEAttributes(int flag,wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
				:wxDialog(parent, id, title, pos, size, wxDEFAULT_DIALOG_STYLE), parent_flag(flag), paren(parent)
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
    text_type = new wxTextCtrl(this, wxID_ANY, wxEmptyString);

	label_value = new wxStaticText(this, wxID_ANY, wxT("Value:"));
	label_value->Show(false);
    text_value = new wxTextCtrl(this, wxID_ANY, wxEmptyString);

	button_modify = new wxButton(this, BUTTON_EAMODIFY, wxT("Modify"));

	button_ok = new wxButton(this, wxID_OK, wxT("OK"));
    button_cancel = new wxButton(this, wxID_CANCEL, wxT("Cancel"));

	grid_attr = new wxGrid(this, GRID_ATTR,wxDefaultPosition,wxDefaultSize,wxALWAYS_SHOW_SB);

	grid_modify = false;
	sel_row = -1;


    set_properties();
    do_layout();
}

UIEAttributes::~UIEAttributes()
{

}


void UIEAttributes::set_properties()
{   
    text_name->SetMinSize(wxSize(220, 20));
	text_name->Enable(false);
    text_type->SetMinSize(wxSize(220, 20));
	text_type->Enable(false);
    text_value->SetMinSize(wxSize(220, 20));    

	grid_attr->CreateGrid(0, 3);
    grid_attr->SetRowLabelSize(0);
	grid_attr->SetColLabelSize(20);
    grid_attr->SetColLabelValue(0, wxT("Name"));
    grid_attr->SetColLabelValue(1, wxT("Type"));
    grid_attr->SetColLabelValue(2, wxT("Value"));
	grid_attr->SetColumnWidth(0,80);
	grid_attr->SetColumnWidth(1,80);
	grid_attr->SetColumnWidth(2,80);
    grid_attr->SetMinSize(wxSize(250, 100));
	grid_attr->SetSelectionMode(wxGrid::wxGridSelectRows);
	grid_attr->SetLabelFont(*wxNORMAL_FONT);
	
    grid_attr->Enable(true);    
    grid_attr->EnableEditing(false);
	grid_attr->EnableGridLines(false);
	grid_attr->DisableDragGridSize();
	grid_attr->DisableCellEditControl();
	grid_attr->HideCellEditControl();
}


void UIEAttributes::do_layout()
{
    wxBoxSizer* sizer_1 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_2 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_3 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_4 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_5 = new wxBoxSizer(wxHORIZONTAL);

    wxFlexGridSizer* grid_sizer_1 = new wxFlexGridSizer(3, 2, 5, 1);

	sizer_1->AddSpacer(20);

	sizer_2->Add(grid_attr, 0, 0, 0);
	sizer_1->Add(sizer_2, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	sizer_1->AddSpacer(10);

	sizer_4->Add(button_modify, 0, wxALIGN_RIGHT, 0); 
	sizer_1->Add(sizer_4, 0, wxALIGN_CENTER, 0);
	sizer_1->AddSpacer(10);


	// Add name
	grid_sizer_1->Add(33,0,0);
    grid_sizer_1->Add(text_name, 0, 0, 0);    

	// Add type
	grid_sizer_1->Add(33,0,0);
    grid_sizer_1->Add(text_type, 0, 0, 0);

	// Add value
	grid_sizer_1->Add(33,0,0);
    grid_sizer_1->Add(text_value, 0, 0, 0);

	sizer_3->AddSpacer(20);
    sizer_3->Add(grid_sizer_1, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	sizer_3->AddSpacer(20);
	sizer_1->Add(sizer_3, 0, 0, 0);
	sizer_1->AddSpacer(20);	
	

	// OK - CANCEL BUTTON
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

void UIEAttributes::OnButtonEAModify(wxCommandEvent& WXUNUSED(event))
{
	//sel_row = this->GetSelRow();
	std::string p;
	p=std::string(text_name->GetValue().mb_str());
	if ( (!text_name->IsEmpty()) && (p[0]!=' '))
	{
		grid_attr->SetCellValue(sel_row,0,text_name->GetValue());
		grid_attr->SetCellValue(sel_row,1,text_type->GetValue());
		grid_attr->SetCellValue(sel_row,2,text_value->GetValue());

		text_name->Clear();
		text_type->Clear();
		text_value->Clear();
		grid_modify = true;
	}
	else
	{
		text_name->Clear();
		wxMessageDialog message(this,wxT("You can't create an attibute without name."), wxT("Warning"),wxICON_EXCLAMATION |wxOK);
		message.ShowModal();
	}
}


void UIEAttributes::OnButtonOK(wxCommandEvent& WXUNUSED(event))
{
	std::string p;
	p=std::string(text_name->GetValue().mb_str());
	if ( (!text_name->IsEmpty()) && (p[0]!=' '))
	{
		grid_attr->SetCellValue(sel_row,0,text_name->GetValue());
		grid_attr->SetCellValue(sel_row,1,text_type->GetValue());
		grid_attr->SetCellValue(sel_row,2,text_value->GetValue());

		text_name->Clear();
		text_type->Clear();
		text_value->Clear();
	}
	this->EndModal(wxID_OK);
}


void UIEAttributes::OnButtonCancel(wxCommandEvent& WXUNUSED(event))
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


std::vector<cpw::Attribute> UIEAttributes::GetAttributes()
{
	std::vector<cpw::Attribute> v_attr;
	cpw::Attribute attr;
	for(int i=0; i<grid_attr->GetNumberRows(); i++)
	{
	  attr.name  = std::string(grid_attr->GetCellValue(i,0).mb_str());
	  attr.type  = std::string(grid_attr->GetCellValue(i,1).mb_str());	
	  attr.value = std::string(grid_attr->GetCellValue(i,2).mb_str());
		v_attr.push_back(attr);
	}
	return v_attr;
}


void UIEAttributes::SetAttributes(const std::vector<cpw::Attribute> &attr)
{
	int numrows;
	for(std::vector<cpw::Attribute>::const_iterator i= attr.begin(); i!= attr.end(); i++)
	{
		grid_attr->AppendRows();
		numrows = grid_attr->GetRows()-1;
		grid_attr->SetCellValue(numrows,0,wxString((i->name).c_str(),wxConvUTF8));
		grid_attr->SetCellValue(numrows,1,wxString((i->type).c_str(),wxConvUTF8));
		grid_attr->SetCellValue(numrows,2,wxString((i->value).c_str(),wxConvUTF8));
	}

}


void UIEAttributes::OnGridCellLeftClick(wxGridEvent& WXUNUSED(event))
{
	bool h=false;
	for(int i=0; i<grid_attr->GetNumberRows(); i++)
	{
		for( int j=0; j<grid_attr->GetNumberCols(); j++)
		{
			if ((grid_attr->IsInSelection(i,j)) && (!h))
			{
				sel_row = i;
				text_name->SetValue(grid_attr->GetCellValue(i,0));
				text_type->SetValue(grid_attr->GetCellValue(i,1));
				text_value->SetValue(grid_attr->GetCellValue(i,2));
				grid_attr->SelectRow(i);
				h=true;
			}
		}		
	}
}

void UIEAttributes::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	render(dc);
}


void UIEAttributes::render(wxDC& dc)
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

	dc.DrawRotatedText(_T("Name: "),20,163, 0);
	dc.DrawRotatedText(_T("Type: "),20,188, 0);
	dc.DrawRotatedText(_T("Value:"),20,213, 0);
}

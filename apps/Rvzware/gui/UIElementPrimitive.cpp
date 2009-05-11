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

#include <sstream>

#include <stdafx.h>

#include <gui/UIElementPrimitive.h>

#include <UIApplicationMainFrame.h>
#include <cpw/ApplicationConfiguration.h>


#define BUTTON_EXPLORER1 5000
#define BUTTON_EXPLORER2 5001
#define BUTTON_FONT  	 5002
#define BUTTON_ATTR      5003


using namespace cpw::gui;

BEGIN_EVENT_TABLE(UIElementPrimitive, wxDialog)
	EVT_BUTTON  (BUTTON_EXPLORER1,   UIElementPrimitive::OnButtonExplorer1)
	EVT_BUTTON  (BUTTON_EXPLORER2,   UIElementPrimitive::OnButtonExplorer2)
	EVT_BUTTON  (BUTTON_FONT,   UIElementPrimitive::OnButtonFont)
	EVT_BUTTON  (BUTTON_ATTR,   UIElementPrimitive::OnButtonAttr)
	EVT_BUTTON  (wxID_CANCEL,   UIElementPrimitive::OnButtonCancel)
	EVT_BUTTON  (wxID_OK,   UIElementPrimitive::OnButtonOK)
	EVT_PAINT	(           UIElementPrimitive::OnPaint )
END_EVENT_TABLE()


UIElementPrimitive::UIElementPrimitive(wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
				:wxDialog(parent, id, title, pos, size, wxDEFAULT_DIALOG_STYLE),paren(parent)
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

	label_model = new wxStaticText(this, wxID_ANY, wxT("Model:"));
	label_model->Show(false);
    text_model = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    button_model = new wxButton(this, BUTTON_EXPLORER1, wxT("..."));

	label_icon = new wxStaticText(this, wxID_ANY, wxT("Icon:"));
	label_icon->Show(false);
    text_icon = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    button_icon = new wxButton(this, BUTTON_EXPLORER2, wxT("..."));

	label_font = new wxStaticText(this, wxID_ANY, wxT("Font"));
	label_font->Show(false);
    text_font = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    button_font = new wxButton(this, BUTTON_FONT, wxT("..."));

	label_description = new wxStaticText(this, wxID_ANY, wxT("Description:"));
	label_description->Show(false);
    panel_desc1 = new wxPanel(this, wxID_ANY);
	panel_desc1->Show(false);
    panel_desc2 = new wxPanel(this, wxID_ANY);
	panel_desc2->Show(false);
    text_description = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	if (!(cpw::ApplicationConfiguration::GetInstance()->IsThemed()))
	{
	  text_description->SetBackgroundColour(wxColour(wxString(cpw::ApplicationConfiguration::GetInstance()->GetPageColour().c_str(),wxConvUTF8)));
	  text_description->SetForegroundColour(wxColour(wxString(cpw::ApplicationConfiguration::GetInstance()->GetPageFontColour().c_str(),wxConvUTF8)));
	}

    button_attributes = new wxButton(this, BUTTON_ATTR, wxT("Attributes..."));
    panel_attr = new wxPanel(this, wxID_ANY);
	panel_attr->Show(false);

	button_ok = new wxButton(this, wxID_OK, wxT("OK"));
    button_cancel = new wxButton(this, wxID_CANCEL, wxT("Cancel"));

    set_properties();
    do_layout();
}

UIElementPrimitive::~UIElementPrimitive()
{

}


void UIElementPrimitive::set_properties()
{   
    text_name->SetMinSize(wxSize(200, 20));
    text_model->SetMinSize(wxSize(200, 20));    
    text_icon->SetMinSize(wxSize(200, 20));    
	text_font->SetMinSize(wxSize(200, 20));
	text_font->Enable(false);
	text_description->SetMinSize(wxSize(285, 100));
	button_model->SetMinSize(wxSize(25, 20));
	button_icon->SetMinSize(wxSize(25, 20));
	button_font->SetMinSize(wxSize(25, 20));
	panel_attr->SetMinSize(wxSize(53,20));    
}


void UIElementPrimitive::do_layout()
{
    wxBoxSizer* sizer_1 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_2 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_3 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_5 = new wxBoxSizer(wxHORIZONTAL);

    wxFlexGridSizer* grid_sizer_1 = new wxFlexGridSizer(5, 3, 1, 1);

	sizer_1->AddSpacer(20);
	// Add name
	grid_sizer_1->Add(59,0,0);
    grid_sizer_1->Add(text_name, 0, 0, 0);
    grid_sizer_1->Add(panel_name, 1, wxEXPAND, 0);
	// Add model
	grid_sizer_1->Add(59,0,0);
    grid_sizer_1->Add(text_model, 0, 0, 0);
    grid_sizer_1->Add(button_model, 0, wxALIGN_LEFT, 0);

	// Add icon
	grid_sizer_1->Add(59,0,0);
    grid_sizer_1->Add(text_icon, 0, 0, 0);
    grid_sizer_1->Add(button_icon, 0, wxALIGN_LEFT, 0);

	// Add font
	grid_sizer_1->Add(59,0,0);
    grid_sizer_1->Add(text_font, 0, 0, 0);
    grid_sizer_1->Add(button_font, 0, wxALIGN_LEFT, 0);

	// Add description
	grid_sizer_1->Add(59,20,0);
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
	sizer_3->Add(79,0,0);

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

void UIElementPrimitive::OnButtonExplorer1(wxCommandEvent& WXUNUSED(event))
{
	std::string &model_path = ApplicationConfiguration::GetInstance()->GetModelDirectory();
	wxFileDialog dialog(this,_T("Open Model"),wxString(model_path.c_str(),wxConvUTF8),wxEmptyString,
			    _T("OSG file(*.osg)|*.osg|3DS file(*.3ds)|*.3ds|All files (*.*)|*.*") );
	if(dialog.ShowModal() == wxID_OK)
	{		
		text_model->SetValue(dialog.GetPath());
	}
	else
	{
	  text_model->SetValue(wxT(""));		
	}
}

void UIElementPrimitive::OnButtonExplorer2(wxCommandEvent& WXUNUSED(event))
{
	std::string &icon_path = ApplicationConfiguration::GetInstance()->GetIconDirectory();
	wxFileDialog dialog(this,_T("Open Icon"),wxString(icon_path.c_str(),wxConvUTF8),wxEmptyString,
						_T("PNG file(*.png)|*.png|BMP file(*.bmp)|*.bmp|JPG file(*.jpg)|*.jpg|All files (*.*)|*.*") );
	if(dialog.ShowModal() == wxID_OK)
	{		
		text_icon->SetValue(dialog.GetPath());
	}
	else
	{
	  text_icon->SetValue(wxT(""));		
	}
}


void UIElementPrimitive::OnButtonFont(wxCommandEvent& WXUNUSED(event))
{
	wxFontData fontdata;
	wxFont font;
	wxColour colour;
	std::string strfont;
	//FIXME: MAYBE THE NEXT LINE IS A BUG
	wxFontDialog fontdialog((wxWindow *)this, wxFontData());
	if (fontdialog.ShowModal() == wxID_OK)
	{
		fontdata = fontdialog.GetFontData();
		font = fontdata.GetChosenFont();
		colour = fontdata.GetColour(); 
		std::string face, size, col, style;
		face = std::string(font.GetFaceName().mb_str());  
		size = font.GetPointSize(); 
		style = std::string(font.GetWeightString().mb_str()); 
		std::ostringstream str;
		str << font.GetPointSize();
		size = str.str();
		col = std::string(colour.GetAsString(wxC2S_NAME).mb_str()); 
		
		if (font.GetUnderlined())		
			strfont = "["+face+"]" + "["+size+"]" + "["+style+"]" + "["+col+"]" + "[UNDERLINED]";
		else
			strfont = "["+face+"]" + "["+size+"]" + "["+style+"]" + "["+col+"]";
		
		text_font->SetValue(wxString(strfont.c_str(),wxConvUTF8));
	}
	else
	{
	  text_font->SetValue(wxT(""));
	}
}

void UIElementPrimitive::OnButtonAttr(wxCommandEvent& WXUNUSED(event))
{
	UIAttributes new_attr_dlg(1,this,1, wxT("Primitive element attributes"),wxDefaultPosition);
	if(!v_attr.empty())
		new_attr_dlg.SetAttributes(v_attr);

	if(new_attr_dlg.ShowModal() == wxID_OK)
		v_attr = new_attr_dlg.GetAttributes();

}

void UIElementPrimitive::OnButtonCancel(wxCommandEvent& WXUNUSED(event))
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


void UIElementPrimitive::OnButtonOK(wxCommandEvent& WXUNUSED(event))
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

void UIElementPrimitive::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	render(dc);
}


void UIElementPrimitive::render(wxDC& dc)
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
	dc.DrawRotatedText(_T("Model: "),20,40, 0);
	dc.DrawRotatedText(_T("Icon:"),20,60, 0);
	dc.DrawRotatedText(_T("Font:"),20,80, 0);
	dc.DrawRotatedText(_T("Description"),20,100, 0);
}

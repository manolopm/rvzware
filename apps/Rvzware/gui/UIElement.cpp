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

#include <wx/statbox.h>
#include <sstream>
#include <string>

#include <stdafx.h>

#include <iostream>
#include <iomanip>

#include <gui/UIElement.h>

#include <UIApplicationMainFrame.h>
#include <cpw/ApplicationConfiguration.h>

#include <gui/UIEAttributes.h>
#include <controllers/PrimitiveController.h>

#define BUTTON_BACK 1001
#define TEXT_COORDS 1002
#define TEXT_COORDS2 1003
#define BUTTON_EATTR 1004
#define BUTTON_SELECT_HTML 1005
#define TEXT_NAME 1006



using namespace cpw::gui;

BEGIN_EVENT_TABLE(UIElement, wxDialog)
	EVT_BUTTON  (wxID_OK,       UIElement::OnButtonOK)
	EVT_BUTTON  (wxID_CANCEL,   UIElement::OnButtonCancel)
	EVT_BUTTON  (BUTTON_EATTR,  UIElement::OnButtonAttributes)
	EVT_BUTTON  (BUTTON_SELECT_HTML, UIElement::OnButtonHtml)
	EVT_BUTTON	(ID_BROWSEBUTTON,		UIElement::OnBrowsePrimitivesButton)
	EVT_TEXT    (TEXT_COORDS, UIElement::CheckCoords)
	EVT_TEXT    (TEXT_NAME, UIElement::CheckName)
	EVT_SPIN_UP		(ID_SPIN_OX,  UIElement::OnOXSpinUp)
	EVT_SPIN_DOWN   (ID_SPIN_OX,  UIElement::OnOXSpinDown)
	EVT_SPIN_UP		(ID_SPIN_OY,  UIElement::OnOYSpinUp)
	EVT_SPIN_DOWN   (ID_SPIN_OY,  UIElement::OnOYSpinDown)
	EVT_SPIN_UP		(ID_SPIN_OZ,  UIElement::OnOZSpinUp)
	EVT_SPIN_DOWN   (ID_SPIN_OZ,  UIElement::OnOZSpinDown)
	EVT_SPIN_UP		(ID_SPIN_SX,  UIElement::OnSXSpinUp)
	EVT_SPIN_DOWN   (ID_SPIN_SX,  UIElement::OnSXSpinDown)
	EVT_SPIN_UP		(ID_SPIN_SY,  UIElement::OnSYSpinUp)
	EVT_SPIN_DOWN   (ID_SPIN_SY,  UIElement::OnSYSpinDown)
	EVT_SPIN_UP		(ID_SPIN_SZ,  UIElement::OnSZSpinUp)
	EVT_SPIN_DOWN   (ID_SPIN_SZ,  UIElement::OnSZSpinDown)
	EVT_SPIN_UP		(ID_SPIN_UTMLON,  UIElement::OnUTMLonSpinUp)
	EVT_SPIN_DOWN   (ID_SPIN_UTMLON,  UIElement::OnUTMLonSpinDown)
	EVT_SPIN_UP		(ID_SPIN_UTMLAT,  UIElement::OnUTMLatSpinUp)
	EVT_SPIN_DOWN   (ID_SPIN_UTMLAT,  UIElement::OnUTMLatSpinDown)
	EVT_SPIN_UP		(ID_SPIN_UTMHEI,  UIElement::OnUTMHeiSpinUp)
	EVT_SPIN_DOWN   (ID_SPIN_UTMHEI,  UIElement::OnUTMHeiSpinDown)
	EVT_CHECKBOX    (ID_LINK_HEIGHT_CHK, UIElement::OnLinkHeightCheck)
	EVT_CHECKBOX    (ID_OVER_TERRAIN_CHK, UIElement::OnOverTerrainCheck)
	EVT_PAINT	    (				  UIElement::OnPaint )

	EVT_CHOICE		(ID_PRIMITIVE_CHOICE, UIElement::OnChoiceChanged)

END_EVENT_TABLE()


UIElement::~UIElement()
{
	delete open_primitive;
}

UIElement::UIElement(cpw::controllers::ElementController *ec, wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
				:wxDialog(parent, id, title, pos, size, wxDEFAULT_DIALOG_STYLE), paren(parent), initialized(false), element_controller(ec),modify(false), height_linked(false), check_coords(true)
{
	
	if (!(cpw::ApplicationConfiguration::GetInstance()->IsThemed()))
	{
		SetFont(paren->GetFont());
		SetForegroundColour(paren->GetForegroundColour());
		SetBackgroundColour(paren->GetBackgroundColour());
	}	

	SetTitle(wxT("Element"));
	SetSize(8,8,390,435-50);
	Center();

	std::string &primitive_path = ApplicationConfiguration::GetInstance()->GetPrimitiveDirectory();
	open_primitive =  new wxFileDialog(this, wxT("Choose a file"), wxString(primitive_path.c_str(),wxConvUTF8), wxT(""), wxT("Element Primitive(*.cel)|*.cel"), wxOPEN);

	//Choice
	primitiveChoice = new wxChoice(this, ID_PRIMITIVE_CHOICE, wxDefaultPosition, wxSize(270,19), arrayStringFor_formatChoice, 0, wxDefaultValidator, wxT("WxChoice1"));
	primitiveChoice->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
	primitiveChoice->SetSelection(-1);
	//browse
	primitiveBrowseButton = new wxButton(this, ID_BROWSEBUTTON, wxT("..."), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, wxT("BrowseButton"));
	primitiveBrowseButton->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	int col_= 20;int row_ = 20+24;

	Orientation_x_spin = new wxSpinButton(this, ID_SPIN_OX, wxDefaultPosition, wxSize(10,19), wxSP_VERTICAL);
	Orientation_y_spin = new wxSpinButton(this, ID_SPIN_OY, wxDefaultPosition, wxSize(10,19), wxSP_VERTICAL);
	Orientation_z_spin = new wxSpinButton(this, ID_SPIN_OZ, wxDefaultPosition, wxSize(10,19), wxSP_VERTICAL);
	Scale_x_spin = new wxSpinButton(this, ID_SPIN_SX, wxDefaultPosition, wxSize(10,19), wxSP_VERTICAL);
	Scale_y_spin = new wxSpinButton(this, ID_SPIN_SY, wxDefaultPosition, wxSize(10,19), wxSP_VERTICAL);
	Scale_z_spin = new wxSpinButton(this, ID_SPIN_SZ, wxDefaultPosition, wxSize(10,19), wxSP_VERTICAL);
	UTM_Lon_spin = new wxSpinButton(this, ID_SPIN_UTMLON, wxDefaultPosition, wxSize(10,19), wxSP_VERTICAL);
	UTM_Lat_spin = new wxSpinButton(this, ID_SPIN_UTMLAT, wxDefaultPosition, wxSize(10,19), wxSP_VERTICAL);
	UTM_Hei_spin = new wxSpinButton(this, ID_SPIN_UTMHEI, wxDefaultPosition, wxSize(10,19), wxSP_VERTICAL);


	label_1 = new wxStaticText(this, wxID_ANY, wxT("Name: "));
	label_3 = new wxStaticText(this, wxID_ANY, wxT("Description: "));
	label_4 = new wxStaticText(this, wxID_ANY, wxT("X"));
	label_5 = new wxStaticText(this, wxID_ANY, wxT("Y"));
	label_6 = new wxStaticText(this, wxID_ANY, wxT("Z"));
	label_7 = new wxStaticText(this, wxID_ANY, wxT("Lon"));
	label_8 = new wxStaticText(this, wxID_ANY, wxT("Lat"));
	label_9 = new wxStaticText(this, wxID_ANY, wxT("Hei"));
	label_10 = new wxStaticText(this, wxID_ANY, wxT("X"));
	label_11 = new wxStaticText(this, wxID_ANY, wxT("Y"));
	label_12 = new wxStaticText(this, wxID_ANY, wxT("Z"));
	label_blank = new wxStaticText(this, wxID_ANY, wxT(" "));
	label_13 = new wxStaticText(this, wxID_ANY, wxT("HTML:"));
	label_16 = new wxStaticText(this, wxID_ANY, wxT("Text:"));

	label_1->Show(false);
	label_3->Show(false);
	label_blank->Show(false);
	label_13->Show(false);
	label_16->Show(false);

	tc_1 = new wxTextCtrl(this, TEXT_NAME, wxT(""));
	tc_13 = new wxTextCtrl(this, wxID_ANY, wxT(""));
	tc_14 = new wxTextCtrl(this, wxID_ANY, wxT(""));
	button_html = new wxButton(this, BUTTON_SELECT_HTML, wxT("..."));
	tc_3 = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_RICH2);

	dynamic_check = new wxCheckBox(this, wxID_ANY, _T(""));
	dynamic_check->Show(false);
	link_height_check  = new wxCheckBox(this, ID_LINK_HEIGHT_CHK,_T(""),wxPoint(/*99*/20,178+23));
	over_terrain_check = new wxCheckBox(this, ID_OVER_TERRAIN_CHK,_T(""),wxPoint(/*99*/20+150,178+23));
	
	tc_4 = new wxTextCtrl(this, TEXT_COORDS, wxT("0.0"));
	tc_5 = new wxTextCtrl(this, TEXT_COORDS, wxT("0.0"));
	tc_6 = new wxTextCtrl(this, TEXT_COORDS, wxT("0.0"));

	tc_10 = new wxTextCtrl(this, TEXT_COORDS, wxT("1.0"));
	tc_11 = new wxTextCtrl(this, TEXT_COORDS, wxT("1.0"));
	tc_12 = new wxTextCtrl(this, TEXT_COORDS, wxT("1.0"));

	tc_7 = new wxTextCtrl(this, TEXT_COORDS, wxT("0"));
	tc_8 = new wxTextCtrl(this, TEXT_COORDS, wxT("0"));
	tc_9 = new wxTextCtrl(this, TEXT_COORDS, wxT("0"));


	button_attributes = new wxButton(this, BUTTON_EATTR, wxT("Attributes..."));
	button_back = new wxButton(this, BUTTON_BACK, wxT("< Back"));
	button_back->Show(false);
	button_ok = new wxButton(this, wxID_OK, wxT("Finish"));
	button_cancel = new wxButton(this, wxID_CANCEL, wxT("Cancel"));
	button_ok->Enable(false);

	do_layout();
	
	orientationx = orientationy = orientationz = 0.0f;
	utm1 = utm2 = utm3 = 0.0f;
	scalex = scaley = scalez = 1.0f;
	initialized = true;

}

void UIElement::SetPrimitivesUrl(const std::map<std::string, cpw::TypeId> &vec_url, const std::string &selected_primitive)
{
	v_url = vec_url;
	std::map<std::string, cpw::TypeId>::const_iterator i = vec_url.begin();
	primitiveChoice->Clear();
	for(i; i!=vec_url.end();i++)
	{
	  wxString* url = new wxString(wxString((i->first).c_str(),wxConvUTF8));
		primitiveChoice->Insert(*url,0);
		delete url;
	}

	if(selected_primitive != "") 
	  primitiveChoice->SetSelection(primitiveChoice->FindString(wxString(selected_primitive.c_str(),wxConvUTF8)));
}

void UIElement::OnChoiceChanged(wxCommandEvent& WXUNUSED(event))
{

	if (primitiveChoice->GetSelection() != wxNOT_FOUND)
	{		
	  std::string pname = std::string(primitiveChoice->GetString(primitiveChoice->GetSelection()).mb_str());
		std::map<std::string, cpw::TypeId>::iterator i = v_url.find(pname);
		if (i!= v_url.end())
		{
			element_controller->ChangePrimitive(i->second);
			button_ok->Enable(true);
		}
	}

}

void UIElement::OnBrowsePrimitivesButton(wxCommandEvent& event)
{
   
	if(open_primitive->ShowModal() == wxID_OK ) 
    {

      if(element_controller->ChangePrimitive(std::string( open_primitive->GetPath().mb_str())))

			button_ok->Enable(true);
	}
}

				

void UIElement::do_layout()
{
	wxBoxSizer *top_sizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(top_sizer);

	wxBoxSizer *vert0_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *vert1_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *vert3_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *vert4_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *vert5_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *vert6_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *vert7_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *vert8_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *vert9_sizer = new wxBoxSizer(wxHORIZONTAL);
	top_sizer->AddSpacer(20);

	primitiveBrowseButton->SetMinSize(wxSize(23, 22)); //name
	primitiveChoice->SetMinSize(wxSize(250, 21));
	vert0_sizer->Add(85,20,0);
	vert0_sizer->Add(primitiveChoice, 0, wxALIGN_RIGHT);
	vert0_sizer->Add(primitiveBrowseButton, 0, wxALIGN_RIGHT);
	
	label_1->SetMinSize(wxSize(65, 21)); //name
	tc_1->SetMinSize(wxSize(250, 21));
	vert1_sizer->Add(85,20,0);
	vert1_sizer->Add(tc_1, 0, wxALIGN_RIGHT);

	label_16->SetMinSize(wxSize(65, 21)); //text
	tc_13->SetMinSize(wxSize(250, 21));
	vert3_sizer->Add(85,20,0);
	vert3_sizer->Add(tc_13, 0, wxALIGN_RIGHT);

	label_13->SetMinSize(wxSize(65, 21)); //html
	tc_14->SetMinSize(wxSize(250, 21));
	button_html->SetMinSize(wxSize(25, 20));
	vert4_sizer->Add(85,20,0);
	vert4_sizer->Add(tc_14, 0, wxALIGN_RIGHT);
	vert4_sizer->AddSpacer(2);
	vert4_sizer->Add(button_html, 0, wxALIGN_RIGHT);

	label_3->SetMinSize(wxSize(65, 21)); //description
	vert5_sizer->AddSpacer(20);
	vert5_sizer->Add(label_3, 0, wxALIGN_LEFT);

	vert6_sizer->AddSpacer(20); //description textbox
	tc_3->SetMinSize(wxSize(340, 63));
	vert6_sizer->Add(tc_3, 0, wxALIGN_LEFT);
	if (!(cpw::ApplicationConfiguration::GetInstance()->IsThemed()))
	{
	  tc_3->SetBackgroundColour(wxString(cpw::ApplicationConfiguration::GetInstance()->GetPageColour().c_str(),wxConvUTF8));
	  tc_3->SetForegroundColour(wxString(cpw::ApplicationConfiguration::GetInstance()->GetPageFontColour().c_str(),wxConvUTF8));
	}

	//orientation
	vert7_sizer->AddSpacer(15);
	wxStaticBox *sb_orientation = new wxStaticBox(this, wxID_ANY, wxT("Orientation"), wxPoint(0, 0), wxSize(50, 128));
	wxStaticBoxSizer *sbs_orientation = new wxStaticBoxSizer(sb_orientation, wxHORIZONTAL);
	wxGridBagSizer *bs_orientation= new wxGridBagSizer(2, 2);
	sbs_orientation->Add(bs_orientation);
	sbs_orientation->SetMinSize(80, 85);
	vert7_sizer->Add(sbs_orientation);
	sbs_orientation->Show(false);

	bs_orientation->Add(label_4, wxGBPosition(0, 1));
	bs_orientation->Add(label_5, wxGBPosition(1, 1));
	bs_orientation->Add(label_6, wxGBPosition(2, 1));

	tc_4->SetMinSize(wxSize(49, 20));
	tc_5->SetMinSize(wxSize(49, 20));
	tc_6->SetMinSize(wxSize(49, 20));

	bs_orientation->Add(tc_4, wxGBPosition(0, 2));
	bs_orientation->Add(tc_5, wxGBPosition(1, 2));
	bs_orientation->Add(tc_6, wxGBPosition(2, 2));

	bs_orientation->Add(Orientation_x_spin, wxGBPosition(0, 3));
	bs_orientation->Add(Orientation_y_spin, wxGBPosition(1, 3));
	bs_orientation->Add(Orientation_z_spin, wxGBPosition(2, 3));

	vert7_sizer->AddSpacer(5);

	//scale
	wxStaticBox *sb_scale = new wxStaticBox(this, wxID_ANY, wxT("Scale"), wxPoint(0, 0), wxSize(175, 128));
	wxStaticBoxSizer *sbs_scale = new wxStaticBoxSizer(sb_scale, wxHORIZONTAL);
	wxGridBagSizer *bs_scale= new wxGridBagSizer(2, 2);
	sbs_scale->Add(bs_scale);
	sbs_scale->SetMinSize(80, 85);
	vert7_sizer->Add(sbs_scale);
	sb_scale->Show(false);

	bs_scale->Add(label_10, wxGBPosition(0, 1));
	bs_scale->Add(label_11, wxGBPosition(1, 1));
	bs_scale->Add(label_12, wxGBPosition(2, 1));

	tc_10->SetMinSize(wxSize(49, 20));
	tc_11->SetMinSize(wxSize(49, 20));
	tc_12->SetMinSize(wxSize(49, 20));
	
	bs_scale->Add(tc_10, wxGBPosition(0, 2));
	bs_scale->Add(tc_11, wxGBPosition(1, 2));
	bs_scale->Add(tc_12, wxGBPosition(2, 2));

	bs_scale->Add(Scale_x_spin, wxGBPosition(0, 3));
	bs_scale->Add(Scale_y_spin, wxGBPosition(1, 3));
	bs_scale->Add(Scale_z_spin, wxGBPosition(2, 3));

	vert7_sizer->AddSpacer(5);

	//utm
	wxStaticBox *sb_utm = new wxStaticBox(this, wxID_ANY, wxT("UTM"), wxPoint(0, 0), wxSize(75, 128));
	wxStaticBoxSizer *sbs_utm = new wxStaticBoxSizer(sb_utm, wxHORIZONTAL);
	wxGridBagSizer *bs_utm= new wxGridBagSizer(2, 2);
	sbs_utm->Add(bs_utm);
	sbs_utm->SetMinSize(148, 85);
	vert7_sizer->Add(sbs_utm);
	sbs_utm->Show(false);

	bs_utm->Add(label_7, wxGBPosition(0, 1));
	bs_utm->Add(label_8, wxGBPosition(1, 1));
	bs_utm->Add(label_9, wxGBPosition(2, 1));
	
	tc_7->SetMinSize(wxSize(100, 20));
	tc_8->SetMinSize(wxSize(100, 20));
	tc_9->SetMinSize(wxSize(100, 20));

	bs_utm->Add(tc_7, wxGBPosition(0, 2));
	bs_utm->Add(tc_8, wxGBPosition(1, 2));
	bs_utm->Add(tc_9, wxGBPosition(2, 2));

	bs_utm->Add(UTM_Lon_spin, wxGBPosition(0, 3));
	bs_utm->Add(UTM_Lat_spin, wxGBPosition(1, 3));
	bs_utm->Add(UTM_Hei_spin, wxGBPosition(2, 3));

	//buttons
	vert8_sizer->AddSpacer(20);
	vert8_sizer->Add(button_attributes, 0, wxALIGN_LEFT);
	vert8_sizer->AddSpacer(30);
	vert8_sizer->Add(button_back, 0, wxALIGN_RIGHT);
	vert8_sizer->AddSpacer(0);
	vert8_sizer->AddSpacer(75);
	vert8_sizer->Add(button_ok, 0, wxALIGN_RIGHT);
	vert8_sizer->AddSpacer(9);
	vert8_sizer->Add(button_cancel, 0, wxALIGN_RIGHT);

	//space
	vert9_sizer->AddSpacer(20);
	vert9_sizer->AddSpacer(20);

	
	top_sizer->Add(vert0_sizer);
	top_sizer->AddSpacer(2);	
	top_sizer->Add(vert1_sizer);
	top_sizer->AddSpacer(2);
	top_sizer->Add(vert3_sizer);
	top_sizer->AddSpacer(2);
	top_sizer->Add(vert4_sizer);
	top_sizer->AddSpacer(2);
	
	top_sizer->Add(vert5_sizer);
	top_sizer->Add(vert6_sizer);
	top_sizer->AddSpacer(5);
	top_sizer->Add(vert9_sizer);
	top_sizer->Add(vert7_sizer);
	top_sizer->AddSpacer(10);
	top_sizer->Add(vert8_sizer);

}


void UIElement::OnButtonOK(wxCommandEvent& WXUNUSED(event))
{
	ButtonOK();
}

void UIElement::ButtonOK()
{
  std::string p = std::string(tc_1->GetValue().mb_str());
	if(((primitiveChoice->GetSelection() == wxNOT_FOUND) && primitiveChoice->IsEnabled()) || (tc_1->IsEmpty()) || (p.at(0) == ' ' ) )
	{
		wxMessageDialog message(this,wxT("The element needs a template and a name."), wxT("Warning"),wxICON_EXCLAMATION |wxOK);
		message.ShowModal();
	}
	else
	{		
		float utm[3]={utm1, utm2, utm3};
		float orientation[3]={orientationx, orientationy, orientationz};
		float scale[3]={scalex, scaley, scalez};
		 		
		if(element_controller->CreateElement(std::string(tc_1->GetValue().mb_str()), // name 				      
						     std::string(tc_13->GetValue().mb_str()), // text
						     std::string(tc_14->GetValue().mb_str()), // html, 
						     std::string(tc_3->GetValue().mb_str()), // description,
						     utm, 
						     orientation, 
						     scale, 
						     dynamic_check->GetValue(), 
						     attributes, modify))
										
		{		
			EndDialog(wxID_OK);
		}
	}
}


void UIElement::OnButtonHtml(wxCommandEvent& WXUNUSED(event))
{
	std::string root_path = ApplicationConfiguration::GetInstance()->GetRootDirectory();
	wxFileDialog dialog(this,wxT("Open HTML file"),wxString(root_path.c_str(),wxConvUTF8),wxEmptyString,
						   wxT("File html(*.html)|*.html|All files(*.*)|*.*") );
	if(dialog.ShowModal() == wxID_OK)
	{		
		tc_14->SetValue(dialog.GetPath());
	}
}

bool UIElement::ChoiceIsReady()
{
	return !primitiveChoice->IsEmpty();
}

void UIElement::OnButtonCancel(wxCommandEvent& WXUNUSED(event))
{
	if((!tc_1->IsEmpty())||(!tc_13->IsEmpty())||(!tc_14->IsEmpty()))
	{
		wxMessageDialog message(this,wxT("Save changes before quit?"), wxT("Warning"),wxICON_EXCLAMATION |wxYES_NO |wxCANCEL);
		int modal = message.ShowModal();
		if(modal == wxID_YES)
		{		
			ButtonOK();
		}
		if(modal == wxID_NO )
		{
			if(!modify) 
				element_controller->Cancel();
			else
				element_controller->CancelModifications();

			this->EndDialog(wxID_CANCEL);			
		}		
	}
	else
	{
		if (!modify) element_controller->Cancel();
		
		this->EndDialog(wxID_CANCEL);		
	}
}

void UIElement::OnButtonAttributes(wxCommandEvent& WXUNUSED(event))
{
	UIEAttributes new_attr_dlg(1,this,1, wxT("Element attributes"),wxDefaultPosition);
	if(!attributes.empty())
		new_attr_dlg.SetAttributes(attributes);

	if(new_attr_dlg.ShowModal() == wxID_OK)
		attributes = new_attr_dlg.GetAttributes();
}

void UIElement::CheckName(wxCommandEvent& WXUNUSED(event))
{
  std::string aux_name = std::string(tc_1->GetValue().mb_str());
	element_controller->SetName(aux_name);
}

void UIElement::SetEntityName(const std::string &name) 
{
  tc_1->SetValue(wxString(name.c_str(),wxConvUTF8));
}

void UIElement::CheckCoords(wxCommandEvent& WXUNUSED(event))
{
	if ((!initialized) || (!check_coords))
		return;

	double aux;
	long int tc4_insertion_point, tc5_insertion_point, tc6_insertion_point,
		     tc7_insertion_point, tc8_insertion_point, tc9_insertion_point,
			 tc10_insertion_point, tc11_insertion_point, tc12_insertion_point;
	
	tc4_insertion_point = tc_4->GetInsertionPoint();
	tc5_insertion_point = tc_5->GetInsertionPoint();
	tc6_insertion_point = tc_6->GetInsertionPoint();
	tc7_insertion_point = tc_7->GetInsertionPoint();
	tc8_insertion_point = tc_8->GetInsertionPoint();
	tc9_insertion_point = tc_9->GetInsertionPoint();
	tc10_insertion_point = tc_10->GetInsertionPoint();
	tc11_insertion_point = tc_11->GetInsertionPoint();
	tc12_insertion_point = tc_12->GetInsertionPoint();

	if (tc_4->GetValue().Trim().ToDouble(&aux))
		orientationx = (float) aux;
	else 
	{

		std::stringstream aux2;
		aux2 << std::fixed << std::setprecision (5) << orientationx;
		tc_4->ChangeValue(wxString(aux2.str().c_str(),wxConvUTF8));
	}


	if (tc_5->GetValue().Trim().ToDouble(&aux))
		orientationy = (float) aux;
	else 
	{
		std::stringstream aux2;
		aux2 << std::fixed << std::setprecision (5) << orientationy;
		tc_5->ChangeValue(wxString(aux2.str().c_str(),wxConvUTF8));
	}


	if (tc_6->GetValue().Trim().ToDouble(&aux))
		orientationz = (float) aux;
	else 
	{
		std::stringstream aux2;
		aux2 << std::fixed << std::setprecision (5) << orientationz;
		tc_6->ChangeValue(wxString(aux2.str().c_str(),wxConvUTF8));
	}

	if (tc_7->GetValue().Trim().ToDouble(&aux))
		utm1 = aux;
	else 
	{
		std::stringstream aux2;
		aux2 << std::fixed << std::setprecision (5) << utm1;
		tc_7->ChangeValue(wxString(aux2.str().c_str(),wxConvUTF8));
	}

	if (tc_8->GetValue().Trim().ToDouble(&aux))
		utm2 = aux;
	else 
	{
		std::stringstream aux2;
		aux2 << std::fixed << std::setprecision (5) << utm2;
		tc_8->ChangeValue(wxString(aux2.str().c_str(),wxConvUTF8));
	}

	if (tc_9->GetValue().Trim().ToDouble(&aux))
		utm3 = aux;
	else 
	{
		std::stringstream aux2;
		aux2 << std::fixed << std::setprecision (5) << utm3;
		tc_9->ChangeValue(wxString(aux2.str().c_str(),wxConvUTF8));
	}

	if (tc_10->GetValue().Trim().ToDouble(&aux))
		scalex = (float) aux;
	else 
	{
		std::stringstream aux2;
		aux2 << std::fixed << std::setprecision (5) << scalex;
		tc_10->ChangeValue(wxString(aux2.str().c_str(),wxConvUTF8));
	}

	if (tc_11->GetValue().Trim().ToDouble(&aux))
		scaley = (float) aux;
	else 
	{
		std::stringstream aux2;
		aux2 << std::fixed << std::setprecision (5) << scaley;
		tc_11->ChangeValue(wxString(aux2.str().c_str(),wxConvUTF8));
	}

	if (tc_12->GetValue().Trim().ToDouble(&aux))
		scalez = (float) aux;
	else 
	{
		std::stringstream aux2;
		aux2 << std::fixed << std::setprecision (5) << scalez;
		tc_12->ChangeValue(wxString(aux2.str().c_str(),wxConvUTF8));
	}

	element_controller->SetPositionOrientationScale((float)utm1, (float)utm2, (float)utm3, 
		                                             orientationx, orientationy, orientationz,
													 scalex, scaley, scalez);

	tc_4->SetInsertionPoint(tc4_insertion_point);
	tc_5->SetInsertionPoint(tc5_insertion_point);
	tc_6->SetInsertionPoint(tc6_insertion_point);
	tc_7->SetInsertionPoint(tc7_insertion_point);
	tc_8->SetInsertionPoint(tc8_insertion_point);
	tc_9->SetInsertionPoint(tc9_insertion_point);
	tc_10->SetInsertionPoint(tc10_insertion_point);
	tc_11->SetInsertionPoint(tc11_insertion_point);
	tc_12->SetInsertionPoint(tc12_insertion_point);
}

void UIElement::SetPos(float x, float y, float z)
{
  tc_7->ChangeValue(wxString::Format(wxT("%f"),x));
  tc_8->ChangeValue(wxString::Format(wxT("%f"),y));
  tc_9->ChangeValue(wxString::Format(wxT("%f"),z));

	utm1 = x;
	utm2 = y;
	utm3 = z;
		 		
}

void UIElement::SetOrientation(float x, float y, float z)
{
  tc_4->ChangeValue(wxString::Format(wxT("%f"),x));
  tc_5->ChangeValue(wxString::Format(wxT("%f"),y));
  tc_6->ChangeValue(wxString::Format(wxT("%f"),z));
	
  orientationx = x;
  orientationy = y;
  orientationz = z;
}

void UIElement::SetScale (float x, float y, float z)
{
  tc_10->ChangeValue(wxString::Format(wxT("%f"),x));
  tc_11->ChangeValue(wxString::Format(wxT("%f"),y));
  tc_12->ChangeValue(wxString::Format(wxT("%f"),z));
  
  scalex = x;
  scaley = y;
  scalez = z;
}


void UIElement::OnOXSpinUp(wxSpinEvent& event)
{
	std::ostringstream wop; 
	double f; 
	tc_4->GetValue().ToDouble(&f);
	f+=5.0f;
	wop << std::fixed << std::setprecision(5) << f; 
	tc_4->SetValue(wxString(wop.str().c_str(),wxConvUTF8));
	orientationx = f;
}

void UIElement::OnOXSpinDown(wxSpinEvent& event)
{
	std::ostringstream wop; 
	double f; 
	tc_4->GetValue().ToDouble(&f);
	f-=5.0f;
	wop << std::fixed << std::setprecision(5) << f;  
	tc_4->SetValue(wxString(wop.str().c_str(),wxConvUTF8));
	orientationx = f;
}

void UIElement::OnOYSpinUp(wxSpinEvent& event)
{
	std::ostringstream wop; 
	double f; 
	tc_5->GetValue().ToDouble(&f);
	f+=5.0f;
	wop << std::fixed << std::setprecision(5) << f;  
	tc_5->SetValue(wxString(wop.str().c_str(),wxConvUTF8));
	orientationy = f;
}
void UIElement::OnOYSpinDown(wxSpinEvent& event)
{
	std::ostringstream wop; 
	double f; 
	tc_5->GetValue().ToDouble(&f);
	f-=5.0f;
	wop << std::fixed << std::setprecision(5) << f;  
	tc_5->SetValue(wxString(wop.str().c_str(),wxConvUTF8));
	orientationy = f;
}
void UIElement::OnOZSpinUp(wxSpinEvent& event)
{
	std::ostringstream wop; 
	double f; 
	tc_6->GetValue().ToDouble(&f);
	f+=5.0f;
	wop << std::fixed << std::setprecision(5) << f;  
	tc_6->SetValue(wxString(wop.str().c_str(),wxConvUTF8));
	orientationz = f;
}
void UIElement::OnOZSpinDown(wxSpinEvent& event)
{
	std::ostringstream wop; 
	double f; 
	tc_6->GetValue().ToDouble(&f);
	f-=5.0f;
	wop << std::fixed << std::setprecision(5) << f;  
	tc_6->SetValue(wxString(wop.str().c_str(),wxConvUTF8));
	orientationz = f;
}
void UIElement::OnSXSpinUp(wxSpinEvent& event)
{
	std::ostringstream wop; 
	double f; 
	tc_10->GetValue().ToDouble(&f);
	f+=0.5f;
	wop << std::fixed << std::setprecision(5) << f;  
	tc_10->SetValue(wxString(wop.str().c_str(),wxConvUTF8));
	scalex = f;
}
void UIElement::OnSXSpinDown(wxSpinEvent& event)
{
	std::ostringstream wop; 
	double f; 
	tc_10->GetValue().ToDouble(&f);
	f-=0.5f;
	wop << std::fixed << std::setprecision(5) << f;  
	tc_10->SetValue(wxString(wop.str().c_str(),wxConvUTF8));
	scalex = f;
}
void UIElement::OnSYSpinUp(wxSpinEvent& event)
{
	std::ostringstream wop; 
	double f; 
	tc_11->GetValue().ToDouble(&f);
	f+=0.5f;
	wop << std::fixed << std::setprecision(5) << f;  
	tc_11->SetValue(wxString(wop.str().c_str(),wxConvUTF8));
	scaley = f;
}
void UIElement::OnSYSpinDown(wxSpinEvent& event)
{
	std::ostringstream wop; 
	double f; 
	tc_11->GetValue().ToDouble(&f);
	f-=0.5f;
	wop << std::fixed << std::setprecision(5) << f;  
	tc_11->SetValue(wxString(wop.str().c_str(),wxConvUTF8));
	scaley = f;
}
void UIElement::OnSZSpinUp(wxSpinEvent& event)
{
	std::ostringstream wop; 
	double f; 
	tc_12->GetValue().ToDouble(&f);
	f+=0.5f;
	wop << std::fixed << std::setprecision(5) << f;  
	tc_12->SetValue(wxString(wop.str().c_str(),wxConvUTF8));
	scalez = f;
}
void UIElement::OnSZSpinDown(wxSpinEvent& event)
{
	std::ostringstream wop; 
	double f; 
	tc_12->GetValue().ToDouble(&f);
	f-=0.5f;
	wop << std::fixed << std::setprecision(5) << f;  
	tc_12->SetValue(wxString(wop.str().c_str(),wxConvUTF8));
	scalez = f;
}
void UIElement::OnUTMLonSpinUp(wxSpinEvent& event)
{
	std::ostringstream wop; 
	wop.precision(15);
	double f; 
	tc_7->GetValue().ToDouble(&f);
	f+=1.0f;
	wop << std::fixed << std::setprecision(5) << f;  
	tc_7->SetValue(wxString(wop.str().c_str(),wxConvUTF8));
	utm1 = f;
}
void UIElement::OnUTMLonSpinDown(wxSpinEvent& event)
{
	std::ostringstream wop; 
	wop.precision(15);
	double f; 
	tc_7->GetValue().ToDouble(&f);
	f-=1.0f;
	wop << std::fixed << std::setprecision(5) << f;  
	tc_7->SetValue(wxString(wop.str().c_str(),wxConvUTF8));
	utm1 = f;
}
void UIElement::OnUTMLatSpinUp(wxSpinEvent& event)
{
	std::ostringstream wop; 
	wop.precision(15);
	double f; 
	tc_8->GetValue().ToDouble(&f);
	f+=1.0f;
	wop << std::fixed << std::setprecision(5) << f;  
	tc_8->SetValue(wxString(wop.str().c_str(),wxConvUTF8));
	utm2 = f;
}
void UIElement::OnUTMLatSpinDown(wxSpinEvent& event)
{
	std::ostringstream wop; 
	wop.precision(15);
	double f; 
	tc_8->GetValue().ToDouble(&f);
	f-=1.0f;
	wop << std::fixed << std::setprecision(5) << f;  
	tc_8->SetValue(wxString(wop.str().c_str(),wxConvUTF8));
	utm2 = f;
}
void UIElement::OnUTMHeiSpinUp(wxSpinEvent& event)
{
	std::ostringstream wop; 
	wop.precision(15);
	double f; 
	tc_9->GetValue().ToDouble(&f);
	f+=1.0f;
	wop << std::fixed << std::setprecision(5) << f;  
	tc_9->SetValue(wxString(wop.str().c_str(),wxConvUTF8));
	utm3 = f;
}
void UIElement::OnUTMHeiSpinDown(wxSpinEvent& event)
{
	std::ostringstream wop; 
	wop.precision(15);
	double f; 
	tc_9->GetValue().ToDouble(&f);
	f-=1.0f;
	wop << std::fixed << std::setprecision(5) << f;  

	tc_9->SetValue(wxString(wop.str().c_str(),wxConvUTF8));
	utm3 = f;
}

void UIElement::OnLinkHeightCheck(wxCommandEvent& WXUNUSED(event))
{
	if (!link_height_check->IsChecked())
		over_terrain_check->SetValue(false);
	
	element_controller->UpdateHandlerProperties();
}

void UIElement::OnOverTerrainCheck(wxCommandEvent& WXUNUSED(event))
{
	if (over_terrain_check->IsChecked())
		link_height_check->SetValue(true);
	
	element_controller->UpdateHandlerProperties();
}

void UIElement::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	render(dc);
}


void UIElement::render(wxDC& dc)
{
	int client_h = 0;
	int client_w = 0;
	GetClientSize (&client_w, &client_h);

	if (!(cpw::ApplicationConfiguration::GetInstance()->IsThemed()))
	{
	  wxColour c_pen   = wxString(cpw::ApplicationConfiguration::GetInstance()->GetBackgroundGradient2Colour().c_str(),wxConvUTF8);
	  wxColour c_backg = wxString(cpw::ApplicationConfiguration::GetInstance()->GetBackgroundGradient1Colour().c_str(),wxConvUTF8);	
	  wxColour c_brush = wxString(cpw::ApplicationConfiguration::GetInstance()->GetBackgroundColour().c_str(),wxConvUTF8);
	  dc.SetTextForeground(wxString(cpw::ApplicationConfiguration::GetInstance()->GetFontLightColour().c_str(),wxConvUTF8));
		dc.SetPen(wxPen(c_pen));
		dc.SetBrush(wxBrush(c_brush));
		dc.GradientFillLinear( wxRect(0,0,client_w,client_h), c_backg, c_pen, wxSOUTH);
		wxPoint box1_first(15-4,198-2+20);
		wxPoint box1_last(215+154+2,198+86+4+20);
	}
	int row_ = 20;

	DrawStaticBox(dc,_T("Orientation"), wxPoint(20,row_+200), wxSize(100,90));
	DrawStaticBox(dc,_T("Scale"), wxPoint(123,row_+200), wxSize(100,90));
	DrawStaticBox(dc,_T("UTM"), wxPoint(226,row_+200), wxSize(145,90));

	dc.DrawRotatedText(_T("Template: "),20,row_, 0);
	dc.DrawRotatedText(_T("Name: "),20,row_+23, 0);
	dc.DrawRotatedText(_T("Text:"),20,row_+46, 0);
	dc.DrawRotatedText(_T("HTML"),20,row_+69, 0);
	dc.DrawRotatedText(_T("Description: "),20,row_+92, 0);
	dc.DrawRotatedText(_T("Do not go under terrain"),20+15,row_+181, 0);
	dc.DrawRotatedText(_T("Adjust to terrain height"),20+165,row_+181, 0);
}

void UIElement::DrawStaticBox(wxDC& dc,const wxString& label, const wxPoint& pos, const wxSize& size)
{
	wxColour c_pen = dc.GetPen().GetColour();
	dc.SetPen(wxPen(dc.GetTextForeground()));
	dc.DrawRoundedRectangle(pos.x, pos.y, size.x, size.y,3.1f);
	dc.SetPen(wxPen(c_pen));
	dc.DrawRotatedText(label,pos.x+10,pos.y,0);
}

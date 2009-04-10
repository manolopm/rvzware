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
#include <string>

#include "stdafx.h"

#include <gui/UICalculateDistance.h>

#include <cpw/ApplicationConfiguration.h>
#include <cpw/graphic/ApplicationScene.h>
#include <cpw/graphic/IScene.h>
#include <cpw/common/types.h>

using namespace cpw::gui;

#define IM_ROOT 0
#define IM_TRACKPOINT 1

#define BOX_HEIGHT 186-20
#define BOX_WIDTH 348

#define UNITS_KILOMETERS 0
#define UNITS_METERS 1
#define UNITS_CENTIMETERS 2
#define UNITS_INCHES 3
#define UNITS_YARDS 4
#define UNITS_MILLES 5
#define UNITS_NAUTICMILLES 6

#define FACTOR_KILOMETERS 0.001f
#define FACTOR_METERS 1.0f
#define FACTOR_CENTIMETERS 100.0f
#define FACTOR_INCHES 39.3700787f
#define FACTOR_YARDS 1.0936133f
#define FACTOR_MILLES 0.00164110913f
#define FACTOR_NAUTICMILLES 0.000539956803f

////Event Table Start
BEGIN_EVENT_TABLE(UICalculateDistance,wxDialog)  
	EVT_BUTTON    (ID_CANCEL_BUTTON,      UICalculateDistance::OnButtonCancel)
	EVT_BUTTON    (ID_RESET_BUTTON,       UICalculateDistance::OnButtonReset)

	EVT_CLOSE     (UICalculateDistance::OnClose)
	EVT_PAINT	  (UICalculateDistance::OnPaint )

	EVT_CHOICE		(ID_FORMAT_CHOICE, UICalculateDistance::OnChoiceChanged)
END_EVENT_TABLE()
////Event Table End

UICalculateDistance::UICalculateDistance(cpw::controllers::DistanceController *evc, wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style), paren(parent), ElementVectorial_controller(evc), 
vname("Distance"),distance(0.0f), factor(1.0f)
{
	if (!(ApplicationConfiguration::GetInstance()->IsThemed()))
	{
		SetFont(paren->GetFont());
		SetForegroundColour(paren->GetForegroundColour());
		SetBackgroundColour(paren->GetBackgroundColour());
	}	
	
	CreateGUIControls();
}

UICalculateDistance::~UICalculateDistance()
{
} 

void UICalculateDistance::CreateGUIControls()
{
	SetTitle(wxT(vname));
	SetIcon(wxNullIcon);
	SetSize(8,8,388,141);
	SetPosition(wxPoint(20,20));

	int _col = 465;
	int _row = 195;

	std::string &icon_path = ApplicationConfiguration::GetInstance()->GetUIIconDirectory();

	_col = 20;	_row = 10;

	//Choice
	wxArrayString arrayStringFor_formatChoice;
	arrayStringFor_formatChoice.Add("Kilometers");
	arrayStringFor_formatChoice.Add("Meters");
	arrayStringFor_formatChoice.Add("Centimeters");
	arrayStringFor_formatChoice.Add("Inches");
	arrayStringFor_formatChoice.Add("Yards");
	arrayStringFor_formatChoice.Add("Milles");
	arrayStringFor_formatChoice.Add("Nautic milles");
	formatChoice = new wxChoice(this, ID_FORMAT_CHOICE, wxPoint(_col+248,_row), wxSize(100,23), arrayStringFor_formatChoice, 0, wxDefaultValidator, wxT("WxChoice1"));
	formatChoice->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
	formatChoice->SetSelection(1);

	_col = 20;	_row = 45;


	_col = 79-30-15; 

	Position_x_Edit = new wxTextCtrl(this, ID_EDIT_FLAMES, wxT(""), wxPoint(_col,_row), wxSize(52+15+25,19), 0, wxDefaultValidator, wxT("Position_x_Edit"));
	Position_x_Edit->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
	Position_x_Edit->SetEditable(false);
	Position_x_Edit->Show(false);

	Position_x_StaticText = new wxStaticText(this, ID_Position_X_STATICTEXT, wxT("Long:"), wxPoint(_col,_row), wxDefaultSize, 0, wxT("Position_x_StaticText"));
	Position_x_StaticText->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	Position_y_Edit = new wxTextCtrl(this, ID_EDIT_FLAMES, wxT(""), wxPoint(_col+92+10+15-3,_row), wxSize(52+15+25,19), 0, wxDefaultValidator, wxT("Position_y_Edit"));
	Position_y_Edit->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
	Position_y_Edit->SetEditable(false);
	Position_y_Edit->Show(false);

	Position_y_StaticText = new wxStaticText(this, ID_POSITIONY_STATICTEXT, wxT("Lat:"), wxPoint(_col+92+10+15,_row), wxDefaultSize, 0, wxT("Position_y_StaticText"));
	Position_y_StaticText->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	Position_z_Edit = new wxTextCtrl(this, ID_EDIT_FLAMES, wxT(""), wxPoint(_col+184+20+15+10-2,_row), wxSize(52+15+25,19), 0, wxDefaultValidator, wxT("Position_z_Edit"));
	Position_z_Edit->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
	Position_z_Edit->SetEditable(false);
	Position_z_Edit->Show(false);

	Position_z_StaticText = new wxStaticText(this, ID_POSITIONZ_STATICTEXT, wxT("Hei:"), wxPoint(_col+184+20+15+10,_row), wxDefaultSize, 0, wxT("Position_z_StaticText"));
	Position_z_StaticText->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	_col = 201;	_row += 30;
	Reset_button = new wxButton(this, ID_RESET_BUTTON, wxT("Reset"), wxPoint(_col,_row), wxSize(75,23), 0, wxDefaultValidator, wxT("Finish_button"));
	Reset_button->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));


	Cancel_button = new wxButton(this, ID_CANCEL_BUTTON, wxT("OK"), wxPoint(_col+84,_row), wxSize(75,23), 0, wxDefaultValidator, wxT("Cancel_button"));
	Cancel_button->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

}


void UICalculateDistance::OnClose(wxCloseEvent& /*event*/)
{
	ElementVectorial_controller->Cancel();

	EndDialog(wxID_CANCEL);	
	ElementVectorial_controller->ReleaseMouseCallBack();
}

void UICalculateDistance::OnButtonCancel(wxCommandEvent& WXUNUSED(event))
{
	ElementVectorial_controller->Cancel();

	this->EndDialog(wxID_CANCEL);	
	ElementVectorial_controller->ReleaseMouseCallBack();
}


void UICalculateDistance::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	render(dc);
}


void UICalculateDistance::render(wxDC& dc)
{
	int client_h = 0;
	int client_w = 0;
	GetClientSize (&client_w, &client_h);

	if (!(ApplicationConfiguration::GetInstance()->IsThemed()))
	{
		wxColour c_pen   = cpw::ApplicationConfiguration::GetInstance()->GetBackgroundGradient2Colour();
		wxColour c_backg = cpw::ApplicationConfiguration::GetInstance()->GetBackgroundGradient1Colour();	
		wxColour c_brush = ApplicationConfiguration::GetInstance()->GetBackgroundColour();
		dc.SetTextForeground(wxColour(cpw::ApplicationConfiguration::GetInstance()->GetFontLightColour()));
		dc.SetPen(wxPen(c_pen));
		dc.SetBrush(wxBrush(c_brush));
		dc.GradientFillLinear( wxRect(0,0,client_w,client_h), c_backg, c_pen, wxSOUTH);
	}

	std::ostringstream wop,wop2,wop3,wop4; 
	wop << std::fixed << std::setprecision(3) << (distance*factor);  
	
	double aux = 0.000;
	Position_x_Edit->GetValue().Trim().ToDouble(&aux);
	wop2 << std::fixed << std::setprecision (3) << aux;
	Position_y_Edit->GetValue().Trim().ToDouble(&aux);
	wop3 << std::fixed << std::setprecision (3) << aux;
	Position_z_Edit->GetValue().Trim().ToDouble(&aux);
	wop4 << std::fixed << std::setprecision (3) << aux;

	
	wxPoint box1_first(20-2,45-9);
	wxPoint box1_last(368,45+22);
	dc.DrawRoundedRectangle(box1_first.x, box1_first.y, box1_last.x-box1_first.x, box1_last.y-box1_first.y,3.1f);
	dc.DrawRotatedText(_T(wop2.str().c_str()),89+10-30-5,45, 0);
	dc.DrawRotatedText(_T(wop3.str().c_str()),89+10-30-5+92+10+15-3,45, 0);
	dc.DrawRotatedText(_T(wop4.str().c_str()),89+10-30-5+184+20+15+10-2,45, 0);

	wxFont font(10, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma"));
	wxFont font_ = dc.GetFont();
	dc.SetFont(font);
	dc.DrawRotatedText(_T("Distance:"),20,12, 0);
	dc.DrawRotatedText(_T(wop.str().c_str()),20+65,12, 0);
	dc.SetFont(font_);
}

void UICalculateDistance::OnChoiceChanged(wxCommandEvent& WXUNUSED(event))
{
	int choice = formatChoice->GetSelection();
	float aux = 1.0f;
	switch(choice) 
	{
		case UNITS_KILOMETERS:
			aux = FACTOR_KILOMETERS;
			break;
		case UNITS_METERS:
			aux = FACTOR_METERS;
			break;
		case UNITS_CENTIMETERS:
			aux = FACTOR_CENTIMETERS;
			break;
		case UNITS_INCHES:
			aux = FACTOR_INCHES;
			break;
		case UNITS_YARDS:
			aux = FACTOR_YARDS;
			break;
		case UNITS_MILLES:
			aux = FACTOR_MILLES;
			break;
		case UNITS_NAUTICMILLES:
			aux = FACTOR_NAUTICMILLES;
			break;
		default:
			break;
	}
	factor = aux;
	Refresh();
	Update();
}

void UICalculateDistance::OnEraseBackground(wxEraseEvent& event)
{
	//don't remove this method
}

void UICalculateDistance::OnButtonReset(wxCommandEvent& WXUNUSED(event))
{
	ElementVectorial_controller->RemoveAll();
	Position_x_Edit->ChangeValue("0.000");
	Position_y_Edit->ChangeValue("0.000");
	Position_z_Edit->ChangeValue("0.000");
	Refresh();
	Update();
}
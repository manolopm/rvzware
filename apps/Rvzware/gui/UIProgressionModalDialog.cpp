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
#include <stdafx.h>

#include <gui/UIProgressionModalDialog.h>

using namespace cpw::gui;


BEGIN_EVENT_TABLE(UIProgressionModalDialog,wxDialog)	
	EVT_PAINT		(UIProgressionModalDialog::OnPaint)
END_EVENT_TABLE()


UIProgressionModalDialog::UIProgressionModalDialog(cpw::controllers::StatusController *status_controller, wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style), cpw::Observer(), paren(parent), sc(status_controller)
{
	InitGUI();
}

UIProgressionModalDialog::~UIProgressionModalDialog()
{
	((cpw::Subject *)sc)->Detach(this);
} 

void UIProgressionModalDialog::InitGUI()
{
	SetTitle(wxT("Progression"));
	SetIcon(wxNullIcon);
	SetSize(8,8,300,100);
	Center();

	statusLabel = new wxStaticText(this, wxID_ANY, wxT("Loading..."), wxPoint(44,10), wxSize(228,16),wxST_NO_AUTORESIZE);
	statusGauge = new wxGauge(this, wxID_ANY, 50, wxPoint(22,30), wxSize(250,20), wxGA_HORIZONTAL);

	std::string &icon_path = ApplicationConfiguration::GetInstance()->GetUIIconDirectory();
	animationGauge.LoadFile(wxString((icon_path + "loader.gif").c_str(),wxConvUTF8));
}

void UIProgressionModalDialog::Update(bool subject_deleted)
{
	if (subject_deleted)
	{
		Show(false);
		wxDialog::Refresh();
		wxDialog::Update();	
		return;
	}
	if (sc->GetModalTraceableItem().valid)
	{
	  statusLabel->SetLabel(wxString(sc->GetModalTraceableItem().label.c_str(),wxConvUTF8));
		statusGauge->SetRange(sc->GetModalTraceableItem().range);
		statusGauge->SetValue(sc->GetModalTraceableItem().value);

		float gifpercent = sc->GetModalTraceableItem().value / (float)(sc->GetModalTraceableItem().range);
		if (gifpercent > 1) gifpercent = 1.0f;
		int frame = (int)(gifpercent*(animationGauge.GetFrameCount()-1));
		frameImage = animationGauge.GetFrame(frame);

		if (!IsShown()) 
		{
			Center();
			Show(true);
		}
	}
	else
	{
		if (IsShown()) Show(false);
	}
	wxDialog::Refresh();
	wxDialog::Update();
}

void UIProgressionModalDialog::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	dc.SetFont(*wxNORMAL_FONT); 
	dc.SetBrush( *wxTRANSPARENT_BRUSH );
	dc.SetBackgroundMode( wxTRANSPARENT );
	dc.Clear();
	dc.DrawBitmap(frameImage, 22, 8, true);
}

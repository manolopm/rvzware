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

#include <gui/UISunConfiguration.h>

#include <cpw/entity/EntityRegistry.h>


using namespace cpw::gui;

BEGIN_EVENT_TABLE(UISunConfiguration, wxToolBar)
	EVT_TOOL (SUN_CONFIG,	        UISunConfiguration::OnButtonSun)
	EVT_COMMAND_SCROLL (SUN_SLIDER, UISunConfiguration::SliderChanges)
END_EVENT_TABLE()

UISunConfiguration::UISunConfiguration(wxAuiManager *m_mgr_, wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
: wxToolBar(parent, id, pos, size, style, name), parent_(parent), active(true), m_mgr(m_mgr_)
{
	std::string &icon_path = ApplicationConfiguration::GetInstance()->GetUIIconDirectory();

	AddTool(SUN_CONFIG, wxT("View Animations"), wxBitmap(wxImage(wxString((icon_path + "sun.png").c_str(),wxConvUTF8))), wxT("Sun"), wxITEM_CHECK);

	bitmap_active = wxBitmap(wxImage(wxString((icon_path + "sun.png").c_str(),wxConvUTF8)));
	bitmap_inactive = wxBitmap(wxImage(wxString((icon_path + "sun_cross.png").c_str(),wxConvUTF8)));
	slider = new wxSlider(this, SUN_SLIDER, 1000 , 0, 2000, wxDefaultPosition, wxSize(150,19), wxSL_HORIZONTAL);
	slider->SetLineSize(2);
	AddControl(slider);
}

UISunConfiguration::~UISunConfiguration()
{

} 

void UISunConfiguration::OnEraseBackground( wxEraseEvent& event )
{
    //event.Skip();
} 

void UISunConfiguration::Update_SunPos()
{
	CalculateSunPosition();

	if (((UIApplicationMainFrame *) parent_)->GetApplication() != NULL)
		((UIApplicationMainFrame *) parent_)->GetApplication()->SetSunPosition(pos);	
} 

void UISunConfiguration::CalculateSunPosition()
{
	pos = slider->GetValue() / (float) slider->GetMax();
}

void UISunConfiguration::SliderChanges(wxScrollEvent& WXUNUSED(event))
{
	Update_SunPos();
}

void UISunConfiguration::OnButtonSun(wxCommandEvent& WXUNUSED(event))
{
	active = !active;
	
	slider->Enable(active);


	if(active)
	{
		Hide();
		SetToolNormalBitmap(SUN_CONFIG, bitmap_active);
		Show();
	}
	else 
	{
		Hide();
		SetToolNormalBitmap(SUN_CONFIG, bitmap_inactive);
		Show();
	}
	m_mgr->Update();

	if (((UIApplicationMainFrame *) parent_)->GetApplication() != NULL)
		((UIApplicationMainFrame *) parent_)->GetApplication()->EnableSunConfiguration(active);	
	
}

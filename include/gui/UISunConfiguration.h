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
#ifndef __UISUNCONFIGURATION__
#define __UISUNCONFIGURATION__

#include <gui/GUItypes.h>

#include <wx/slider.h>
#include <wx/toolbar.h>
#include <wx/tbarbase.h>

#include "Application.h"
#include <cpw/ApplicationConfiguration.h>

#define sun_PI 3.14159265358979323846

namespace cpw 
{ 
	namespace gui
	{


		/** 
			\brief Scene Sun configuration
			\ingroup cpw
		*/
		class UISunConfiguration : public wxToolBar
		{
			private:

				DECLARE_EVENT_TABLE()

			public:

				UISunConfiguration(wxAuiManager *m_mgr_, wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTB_HORIZONTAL|wxTB_FLAT|wxTB_NODIVIDER, const wxString& name = wxPanelNameStr);
				~UISunConfiguration();

				virtual void Update_SunPos();

			private:
					
				enum{
					SUN_SLIDER = 5011,
					SUN_CONFIG = 5012,
					SUN_CONFIG2 = 5013,
					ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
				};
				
				wxSlider *slider;
				wxWindow *parent_;
				float pos;
				bool active;
				wxBitmap bitmap_active;
				wxBitmap bitmap_inactive;
				wxAuiManager *m_mgr;

				void CalculateSunPosition();
				void OnButtonSun(wxCommandEvent& WXUNUSED(event));
				void SliderChanges(wxScrollEvent& WXUNUSED(event));
				void OnEraseBackground( wxEraseEvent& event );

		};
	}
}
#endif



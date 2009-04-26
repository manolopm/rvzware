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
#ifndef __UIVIDEOCONTROLLER__
#define __UIVIDEOCONTROLLER__


#include <wx/slider.h>
#include <wx/spinctrl.h>
#include <wx/toolbar.h>
#include <wx/tbarbase.h>
#include <wx/datetime.h>
#include <wx/aui/aui.h>

#include <cpw/common/Observer.h>

#include <cpw/ApplicationConfiguration.h>
#include <ApplicationTime.h>
#include <gui/UIAnimableList.h>
#include <gui/GUItypes.h>


namespace cpw 
{ 
	
	namespace gui
	{
		
		/** 
			\brief Por comentar...
			\ingroup cpw::gui
		*/
		class UIVideoController : public wxToolBar, public Observer
		{
			private:

				DECLARE_EVENT_TABLE()

			public:

				UIVideoController(wxAuiManager *m_mgr_,wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTB_HORIZONTAL|wxTB_FLAT|wxTB_NODIVIDER, const wxString& name = wxPanelNameStr);
				~UIVideoController();

				virtual void Update(bool subject_deleted = false);
				void Stop();
				
					
				int  GetFireViewMode();

			private:
					
				enum{
					ID_MULTIPLIER_SPIN = 5015,
					ANIMATION_CURRENTTIME = 5013,
					APPLICATION_DATE = 5008,
					APPLICATION_HOUR = 5009,
					ANIMATION_START = 5004,
					ANIMATION_REWIND = 5003,
					ANIMATION_PAUSE = 5006,
					ANIMATION_FORWARD = 5002,
					ANIMATION_END = 5005,
					ANIMATION_REPEAT = 5007,
					ANIMATION_PLAY = 5000,
					ANIMATION_STOP = 5001,
					ANIMATION_MULTIPLIER = 5010,
					ANIMATION_SLIDER = 5011,
					ANIMATION_VIEW_ANIMABLE = 5012,
					ANIMATION_FIRE_VIEWMODE = 5014,
					ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
				};

				wxAuiManager *m_mgr;
				abstractTime app_Time;
				wxSlider *timeSlider;
				wxTextCtrl *multiplier;
				wxWindow *parent_;
				wxBitmap play_bitmap,pause_bitmap;
				bool repeat;
				bool playing;
				bool init;
				int mult;

				void OnButtonRewind(wxCommandEvent& WXUNUSED(event));
				void OnButtonForward(wxCommandEvent& WXUNUSED(event));
				void OnButtonPlay(wxCommandEvent& WXUNUSED(event));
				void OnButtonStart(wxCommandEvent& WXUNUSED(event));
				void OnButtonEnd(wxCommandEvent& WXUNUSED(event));
				void OnButtonRepeat(wxCommandEvent& WXUNUSED(event));
				void OnButtonStop(wxCommandEvent& WXUNUSED(event));
				void OnButtonCurrentTime(wxCommandEvent& WXUNUSED(event));
				void OnButtonViewAnimable(wxCommandEvent& WXUNUSED(event));
				void OnButtonFireViewMode(wxCommandEvent& WXUNUSED(event));
				void SetDateChanges(wxDateEvent& WXUNUSED(event));
				void SetHourChanges(wxCommandEvent& event);
				void SliderChanges(wxScrollEvent& WXUNUSED(event));
				void OnMultiplierChanged(wxCommandEvent& WXUNUSED(event));
				void OnMultiplierSpinUp(wxSpinEvent& event);
				void OnMultiplierSpinDown(wxSpinEvent& event);

				void SwitchPlayPauseButton();
				void SetPlayButton();
				void SetPauseButton();

		};
	}
}

#endif



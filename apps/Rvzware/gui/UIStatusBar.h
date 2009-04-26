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
#ifndef __UISTATUSBAR__
#define __UISTATUSBAR__

#include <wx/wx.h>
#include <wx/string.h>
#include <wx/image.h>
#include <wx/animate.h>
#include <wx/toolbar.h>
#include <wx/tbarbase.h>
#include <wx/control.h>
#include <wx/statbmp.h>

#include <cpw/common/Observer.h>
#include <OpenThreads/Mutex>


#include <cpw/ApplicationConfiguration.h>
#include <Application.h>
#include <ApplicationTime.h>

namespace cpw 
{ 
	namespace gui
	{
		/** 
			\brief Por comentar...
			\ingroup cpw
		*/
		class UIStatusBar: public wxToolBar, public cpw::Observer
		{
			public:

				UIStatusBar(Application *app, wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTB_HORIZONTAL|wxTB_FLAT|wxTB_NODIVIDER, const wxString& name = wxPanelNameStr);
				~UIStatusBar();

				virtual void Update(bool subject_deleted = false);

			private:

				OpenThreads::Mutex mutex;
				wxBitmap m_bmp;
				wxString m_label;

				DECLARE_EVENT_TABLE();

				enum 
				{
					ID_MEDIACTRL = 1001,
					ID_DUMMY_VALUE_
				};

				Application *application;
				wxStaticText *statusLabel;
				wxStaticBitmap *animGauge;
				wxAnimation animationGauge;
				wxImage frameImage;

				void InitGUI();
				void OnPaint(wxPaintEvent& event);
				void render(wxDC& dc);
				//void OnEraseBackground(wxEraseEvent& event);
		};
	}
}

#endif

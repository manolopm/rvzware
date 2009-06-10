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

#ifndef _UIPROGRESSIONMODALDIALOG_
#define _UIPROGRESSIONMODALDIALOG_

#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/image.h>
#include <wx/animate.h>
#include <wx/gauge.h>

#include <cpw/ApplicationConfiguration.h>
#include <controllers/StatusController.h>
#include <cpw/common/Observer.h>

namespace cpw { 
	
	namespace controllers { class StatusController; }

	namespace gui
	{

		/** 
			\brief Modal Dialog 
			\ingroup cpw
		*/
		class UIProgressionModalDialog : public wxDialog, public cpw::Observer
		{		
			public:
				UIProgressionModalDialog(controllers::StatusController *status_controller, wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("Progression"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION /*wxDEFAULT_DIALOG_STYLE*/);
				virtual ~UIProgressionModalDialog();

			private:

				enum
				{
					ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
				};

				wxStaticText *statusLabel;
				wxGauge *statusGauge;
				wxWindow *paren;
				controllers::StatusController *sc;
				wxAnimation animationGauge;
				wxImage frameImage;
			
				void InitGUI();
				void Update(bool subject_deleted = false);
				void OnPaint(wxPaintEvent& event);

				DECLARE_EVENT_TABLE();
				
		};
	}
}

#endif

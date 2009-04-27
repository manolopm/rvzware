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

#ifndef __UIHELP_h__
#define __UIHELP_h__

#include <wx/wx.h>
#include <wx/dialog.h>

#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/image.h>
#include <wx/string.h>

#include <iostream>
#include <iomanip>

#undef UIHelp_STYLE
#define UIHelp_STYLE wxCAPTION | wxSYSTEM_MENU | wxDIALOG_NO_PARENT | wxCLOSE_BOX

namespace cpw 
{ 
	
	namespace gui
	{
		/** 
			\brief Window Help panel
			\ingroup cpw
		*/
		class UIHelp : public wxDialog
		{
			private:
				DECLARE_EVENT_TABLE();
				
			public:
				UIHelp(wxWindow *parent, wxWindowID id, const wxString &app_title, const wxString &bk_image, 
					  const wxString &app_url, const wxString &app_name, const wxString &app_version,  
					  const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = UIHelp_STYLE);
				virtual ~UIHelp();

			private:

				wxButton *OK_button;
				
			private:
				enum
				{
					ID_OK_BUTTON = 1003,
					ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
				};
			
			private:
				
				void CreateGUIControls();

				wxWindow *paren;
				wxBitmap m_bmp;
				int hitx,hity;

				void OnClose(wxCloseEvent& event);
				void OnMove(wxMoveEvent& event);
				void OnButtonOK(wxCommandEvent& WXUNUSED(event));
				
				void OnPaint(wxPaintEvent& event);
				void OnEraseBackground(wxEraseEvent& event);

				wxString application_title;
				wxString background_image;
				wxString application_url; 
				wxString application_name;
				wxString application_version;

				void render(wxDC& dc);

		};
	}
}

#endif

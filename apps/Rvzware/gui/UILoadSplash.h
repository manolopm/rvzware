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

#ifndef __UILOADSPLASH_h__
#define __UILOADSPLASH_h__

#include <wx/wx.h>
#include <wx/dialog.h>

#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/image.h>
#include <wx/string.h>

#include <iostream>
#include <iomanip>

#undef UILoadSplash_STYLE
#define UILoadSplash_STYLE /*wxSTAY_ON_TOP |*/ wxDIALOG_NO_PARENT

namespace cpw 
{ 
	
	namespace gui
	{
		/** 
			\brief Window Help panel
			\ingroup cpw
		*/
		class UILoadSplash : public wxDialog
		{
			private:
				DECLARE_EVENT_TABLE();
				
			public:
				UILoadSplash(wxWindow *parent, wxWindowID id, const wxString &title, const wxString &bk_image, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = UILoadSplash_STYLE);
				virtual ~UILoadSplash();

			private:

			private:
				enum
				{
					ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
				};
			
			private:
				
				void CreateGUIControls();

				wxWindow *paren;
				wxBitmap m_bmp;
				int hitx,hity;

				wxString background_image;

				void OnClose(wxCloseEvent& event);
				void OnMove(wxMoveEvent& event);
				
				void OnPaint(wxPaintEvent& event);
				void OnEraseBackground(wxEraseEvent& event);

				void render(wxDC& dc);

		};
	}
}

#endif

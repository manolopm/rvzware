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

#ifndef __UISHAPED_FRAME__
#define __UISHAPED_FRAME__

#include <wx/wx.h>
#include <wx/dialog.h>

#include <wx/listbox.h>
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/stattext.h>
#include <wx/notebook.h>
#include <wx/image.h>
#include <wx/bmpbuttn.h>

#include <ApplicationConfiguration.h>
#include <cpw/graphic/INavigatorManager.h>
#include <cpw/graphic/INavigator.h>

namespace cpw 
{ 
	
	namespace gui
	{

		class UIShapedFrame : public wxFrame
		{
			private:
				DECLARE_EVENT_TABLE();

				//widgets
				wxBitmapButton *right_button;

				wxWindow *parent_;

				wxBitmap m_bmp;
				wxBitmap m_mask;
				wxPoint m_delta;
				wxPoint m_pos;
				wxPoint frame_pos;
				bool m_hasShape;
				cpw::INavigatorManager *navigator_manager_;

				enum
				{
					ID_RIGHT_BITMAPBUTTON = 1001,
					ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
				};

				
			public:
				UIShapedFrame( cpw::INavigatorManager *navigator_manager, wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("Fire"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxFRAME_SHAPED | wxSTAY_ON_TOP | wxFRAME_NO_TASKBAR);
				~UIShapedFrame();

				void UIShapedFrame::OnWindowCreate(wxWindowCreateEvent& WXUNUSED(event));
				void UIShapedFrame::SetWindowShape();
				void UIShapedFrame::OnMouseMove(wxMouseEvent& event);
				void UIShapedFrame::OnMouseLeftDown(wxMouseEvent& event);
				void UIShapedFrame::OnPaint(wxPaintEvent& event);
				void UIShapedFrame::OnFlechaParriba(wxCommandEvent& WXUNUSED(event));
				
				void UIShapedFrame::OnPaint();

				void InitGUI();
		};

	}
}

#endif 

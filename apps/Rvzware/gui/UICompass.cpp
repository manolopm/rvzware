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


#include <gui/UIShapedFrame.h>
#include <UIApplicationMainFrame.h>

#include <wx/region.h>
#include <wx/bitmap.h>
#include <cpw/debugger.h>

using namespace cpw::gui;

////Event Table Start
BEGIN_EVENT_TABLE(UIShapedFrame, wxFrame)
	EVT_BUTTON			( ID_RIGHT_BITMAPBUTTON,      UIShapedFrame::OnFlechaParriba)	
	EVT_LEFT_DOWN       ( UIShapedFrame::OnMouseLeftDown )	
	EVT_MOTION			( UIShapedFrame::OnMouseMove )
	EVT_PAINT			( UIShapedFrame::OnPaint )
	#ifdef __WXGTK__
	EVT_WINDOW_CREATE(UIShapedFrame::OnWindowCreate)
	#endif
END_EVENT_TABLE()

UIShapedFrame::UIShapedFrame(cpw::INavigatorManager *navigator_manager, wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxFrame(parent, id, title, position, size, style), parent_(parent),navigator_manager_(navigator_manager)
{
	m_hasShape = false;
	std::string &icon_path = ApplicationConfiguration::GetInstance()->GetIconDirectory();	
	m_bmp = wxBitmap(wxT(icon_path + "compass0003_transp.bmp"), wxBITMAP_TYPE_BMP);
	m_mask = wxBitmap(wxT(icon_path + "compass0003_mask.bmp"), wxBITMAP_TYPE_BMP);
	SetSize(wxSize(m_bmp.GetWidth(), m_bmp.GetHeight()));
	#ifndef __WXGTK__
	// On wxGTK we can’t do this yet because the window hasn’t
	// been created yet so we wait until the EVT_WINDOW_CREATE
	// event happens. On wxMSW and wxMac the window has been created
	// at this point so we go ahead and set the shape now.
	SetWindowShape();
	#endif	

	//SetTransparent(200);
	InitGUI();
	Show();
}

UIShapedFrame::~UIShapedFrame()
{
} 

// Used on GTK+ only
void UIShapedFrame::OnWindowCreate(wxWindowCreateEvent& WXUNUSED(event))
{
	SetWindowShape();
}

void UIShapedFrame::SetWindowShape()
{
	wxRegion region(m_mask, wxColour(0,0,0),0);
	m_hasShape = SetShape(region);
}

void UIShapedFrame::OnMouseLeftDown(wxMouseEvent& event)
{
	m_pos = ClientToScreen(event.GetPosition());	
	frame_pos = GetPosition();
}
void UIShapedFrame::OnMouseMove(wxMouseEvent& event)
{
	if (event.LeftIsDown())
	{
		if (event.Dragging())
		{
			wxPoint pt = event.GetPosition();
			wxPoint pos = ClientToScreen(pt);
			m_delta.x =  m_pos.x - pos.x;
			m_delta.y =  m_pos.y - pos.y;
			Move(wxPoint(frame_pos.x - m_delta.x, frame_pos.y - m_delta.y));
		}
	}
}

void UIShapedFrame::OnPaint()
{

}
void UIShapedFrame::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	dc.DrawBitmap(m_bmp, 0, 0, true);
}

void UIShapedFrame::InitGUI()
{
	std::string &icon_path = ApplicationConfiguration::GetInstance()->GetIconDirectory();	

	wxBitmap m_BitmapButton( icon_path + "compass_north_label.bmp", wxBITMAP_TYPE_BMP );
    right_button = new wxBitmapButton(this, ID_RIGHT_BITMAPBUTTON, m_BitmapButton,  wxPoint(74,15), wxDefaultSize, NULL); 
	right_button->SetBitmapHover(wxBitmap(icon_path + "compass_north_hover.bmp", wxBITMAP_TYPE_BMP));
	right_button->SetBitmapSelected(wxBitmap(icon_path + "compass_north_pressed.bmp", wxBITMAP_TYPE_BMP));
	right_button->SetBitmapFocus(wxBitmap(icon_path + "compass_north_label.bmp", wxBITMAP_TYPE_BMP));
	right_button->SetBitmapLabel(wxBitmap(icon_path + "compass_north_label.bmp", wxBITMAP_TYPE_BMP));
}

void UIShapedFrame::OnFlechaParriba(wxCommandEvent& WXUNUSED(event))
{
	int peo = 1;
	//INavigator *navigator = navigator_manager_->GetFocusedOrFirstNavigator();
	//if (navigator != NULL)
	//{
	//	navigator->GetCamera()->GainHeight();
	//}

}

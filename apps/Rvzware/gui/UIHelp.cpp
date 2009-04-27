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

#include <sstream>
#include <string>

#include <stdafx.h>

#include <gui/UIHelp.h>

#include <wx/hyperlink.h>

#include <cpw/ApplicationConfiguration.h>

using namespace cpw::gui;

#define TIMELINE_CORNERX 10
#define TIMELINE_CORNERY 10+20
#define TIMELINE_HEIGHT 400
#define TIMELINE_WIDTH 200

BEGIN_EVENT_TABLE(UIHelp,wxDialog)  
	EVT_PAINT		(UIHelp::OnPaint)
	 EVT_BUTTON    (ID_OK_BUTTON,      UIHelp::OnButtonOK)
	 EVT_CLOSE     (UIHelp::OnClose)
	 EVT_MOVE      (UIHelp::OnMove)
 END_EVENT_TABLE()

 UIHelp::UIHelp(wxWindow *parent, wxWindowID id, const wxString &app_title, const wxString &bk_image, 
			    const wxString &app_url, const wxString &app_name, const wxString &app_version,  
			    const wxPoint &position, const wxSize& size, long style)
 : wxDialog(parent, id, app_title, position, size, style), paren(parent), application_title (app_title), background_image(bk_image), 
   application_url(app_url), application_name(app_name), application_version(app_version)
 {
	 if (!(ApplicationConfiguration::GetInstance()->IsThemed()))
	 {
		 SetFont(paren->GetFont());
		 SetForegroundColour(paren->GetForegroundColour());
		 SetBackgroundColour(paren->GetBackgroundColour());
	 }

	 CreateGUIControls();
 }

 UIHelp::~UIHelp()
 {
 } 

 void UIHelp::CreateGUIControls()
 {
   SetTitle(wxString(application_title.c_str(),wxConvUTF8));
	 SetIcon(wxNullIcon);
	 SetSize(8,8,463,TIMELINE_CORNERY+TIMELINE_HEIGHT+45);
	 Center();

	 int _col = 10;
	 int _row = 10;

	 m_bmp = wxBitmap(wxImage(wxString(background_image.c_str(),wxConvUTF8)));

	wxHyperlinkCtrl *link = new wxHyperlinkCtrl(this, wxID_ANY, wxString(application_url.c_str(),wxConvUTF8), wxString(application_url.c_str(),wxConvUTF8), wxPoint(TIMELINE_CORNERX+TIMELINE_WIDTH+40,TIMELINE_CORNERY+50+10+20));
	if (!(ApplicationConfiguration::GetInstance()->IsThemed()))
	{
	  wxColour c_pen   = wxString(cpw::ApplicationConfiguration::GetInstance()->GetPageColour().c_str(),wxConvUTF8);
		link->SetBackgroundColour(c_pen);
	}

	_row = TIMELINE_CORNERY+TIMELINE_HEIGHT-23;

	OK_button = new wxButton(this, ID_OK_BUTTON, wxT("OK"), wxPoint(TIMELINE_CORNERX+TIMELINE_WIDTH+235-75,_row), wxSize(75,23), 0, wxDefaultValidator, wxT("Cancel_button"));
	OK_button->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

}

void UIHelp::OnClose(wxCloseEvent& /*event*/)
{
	this->EndDialog(wxID_CANCEL);	
}

void UIHelp::OnButtonOK(wxCommandEvent& WXUNUSED(event))
{
	this->EndDialog(wxID_CANCEL);	
}

void UIHelp::OnMove(wxMoveEvent& event)
{
}

void UIHelp::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	render(dc);
}

void UIHelp::render(wxDC& dc)
{
	int client_h = 0;
	int client_w = 0;
	GetClientSize (&client_w, &client_h);

	if (!(ApplicationConfiguration::GetInstance()->IsThemed()))
	{
	  wxColour c_pen   = wxString(cpw::ApplicationConfiguration::GetInstance()->GetBackgroundGradient2Colour().c_str(),wxConvUTF8);
	  wxColour c_backg = wxString(cpw::ApplicationConfiguration::GetInstance()->GetBackgroundGradient1Colour().c_str(),wxConvUTF8);	
	  wxColour c_brush = wxString(ApplicationConfiguration::GetInstance()->GetBackgroundColour().c_str(),wxConvUTF8);
	  wxColour c_page = wxString(ApplicationConfiguration::GetInstance()->GetPageColour().c_str(),wxConvUTF8);
	  wxColour c_page_font = wxString(ApplicationConfiguration::GetInstance()->GetPageFontColour().c_str(),wxConvUTF8);
		
		//dc.SetTextForeground(wxColour(cpw::ApplicationConfiguration::GetInstance()->GetFontLightColour());
		dc.SetTextForeground(c_page_font);
		dc.SetPen(wxPen(c_page_font));
		dc.SetBrush(wxBrush(c_page));
		dc.GradientFillLinear( wxRect(0,0,client_w,client_h), c_backg, c_pen, wxSOUTH);
	}
	//dc.DrawBitmap(m_bmp, TIMELINE_CORNERX, TIMELINE_CORNERY, false);
	dc.DrawBitmap(m_bmp, 0, 0, false);
	wxPoint box1_first(TIMELINE_CORNERX+TIMELINE_WIDTH+15,TIMELINE_CORNERY);
	wxPoint box1_last(TIMELINE_CORNERX+TIMELINE_WIDTH+235,TIMELINE_CORNERY+TIMELINE_HEIGHT-30);
	//wxPoint box1_first(0,0);
	//wxPoint box1_last(463,455);
	dc.DrawRoundedRectangle(box1_first.x, box1_first.y, box1_last.x-box1_first.x, box1_last.y-box1_first.y,3.1f);

	dc.DrawRotatedText(wxString(application_version.c_str(),wxConvUTF8), TIMELINE_CORNERX+TIMELINE_WIDTH+40,TIMELINE_CORNERY+45+15, 0);
	dc.SetFont(wxFont(12,wxDEFAULT,wxFONTSTYLE_NORMAL,wxBOLD,true));
	dc.DrawRotatedText(wxString(application_name.c_str(),wxConvUTF8),TIMELINE_CORNERX+TIMELINE_WIDTH+40,TIMELINE_CORNERY+30, 0);
}


void UIHelp::OnEraseBackground(wxEraseEvent& event)
{
}

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

#include <stdafx.h>

#include <gui/UIStatusBar.h>


using namespace cpw::gui;

BEGIN_EVENT_TABLE(UIStatusBar,wxToolBar)
	EVT_PAINT		(UIStatusBar::OnPaint)
END_EVENT_TABLE()


UIStatusBar::UIStatusBar(Application *app, wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
: wxToolBar(parent, id, pos, size, style, name), application(app), cpw::Observer()
{
	InitGUI();
}

UIStatusBar::~UIStatusBar()
{
	cpw::Subject * sc;
	sc = (cpw::Subject *) (application->GetStatusController());
	if (sc != NULL)
		sc->Detach((cpw::Observer*) this);
}

void UIStatusBar::InitGUI()
{
	std::string &icon_path = ApplicationConfiguration::GetInstance()->GetUIIconDirectory();

	statusLabel = new wxStaticText(this,ID_MEDIACTRL, wxT("Ready"), wxPoint(200,0), wxSize(150,16),wxST_NO_AUTORESIZE);

	animationGauge.LoadFile(wxString(( icon_path + "loader.gif" ).c_str(),wxConvUTF8));
	frameImage = animationGauge.GetFrame(0);
	animGauge = new wxStaticBitmap(this, wxID_ANY, wxBitmap(frameImage));
	m_bmp = wxBitmap(frameImage);
	m_label = _T("");
	animGauge->Show(false);
	statusLabel->Show(false);

	this->AddControl(animGauge);
	this->AddSeparator();
	this->AddControl(statusLabel);	
	this->AddSeparator();
}

void UIStatusBar::Update(bool subject_deleted)
{
	mutex.lock();

	if (subject_deleted)
		return;
	cpw::controllers::StatusController * sc;
	sc = application->GetStatusController();
	if (application->GetStatusController() != NULL)
	{
			int statusrange = sc->GetRange();
			int statusvalue = sc->GetValue();
			if (statusrange>0)
			{
				float gifpercent = statusvalue / (float)statusrange;
				if (gifpercent > 1) gifpercent =1;
				int gifcount = (animationGauge.GetFrameCount())-1;
				int frame =(int)( gifpercent*(gifcount));
				//frameImage = animationGauge.GetFrame(frame);
				/*animGauge->SetBitmap(frameImage);
				*/
				m_bmp = wxBitmap(animationGauge.GetFrame(frame));
				m_label = wxString(sc->GetLabel().c_str(),wxConvUTF8);
			wxPaintEvent tmp = wxPaintEvent();
			OnPaint((wxPaintEvent &)tmp);
			}
		//statusLabel->SetLabel(_T(sc->GetLabel()));
	}
	mutex.unlock();

}

void UIStatusBar::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	render(dc);
}

void UIStatusBar::render(wxDC& dc)
{
	dc.Clear();
	dc.DrawBitmap(m_bmp, 0, 5, true);
	if (!(ApplicationConfiguration::GetInstance()->IsThemed()))
	{
	  wxColour c_page_font = wxString(ApplicationConfiguration::GetInstance()->GetPageFontColour().c_str(),wxConvUTF8);
		dc.SetTextForeground(c_page_font);
	}
	dc.DrawRotatedText(m_label,25,5, 0);
}



//void UIStatusBar::OnEraseBackground(wxEraseEvent& event)
//{
//}

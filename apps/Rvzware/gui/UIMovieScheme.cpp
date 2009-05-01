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

#include <gui/UIMovieScheme.h>

using namespace cpw::gui;

BEGIN_EVENT_TABLE(UIMovieScheme, wxWindow)
	EVT_PAINT		(							UIMovieScheme::OnPaint)
	EVT_LEFT_DOWN	(							UIMovieScheme::OnMouseLeftDown)
	EVT_MOTION		(							UIMovieScheme::OnMouseMove)
	EVT_SIZE		(							UIMovieScheme::OnSize)
	EVT_COMMAND_SCROLL_LINEUP(ID_SBH,			UIMovieScheme::OnHScrollLineUp)
	EVT_COMMAND_SCROLL_LINEDOWN(ID_SBH,			UIMovieScheme::OnHScrollLineDown)
	EVT_COMMAND_SCROLL (ID_SBH,					UIMovieScheme::OnHScrollMove)
	EVT_COMMAND_SCROLL (ID_SBV,					UIMovieScheme::OnVScrollMove)
	EVT_BUTTON		(ID_MAGNIFIER_ZOOM_IN,		UIMovieScheme::OnZoomIn)
	EVT_BUTTON		(ID_MAGNIFIER_ZOOM_OUT,		UIMovieScheme::OnZoomOut)
	EVT_BUTTON		(ID_ADJUST_TIME_TO_WIDTH,	UIMovieScheme::OnAdjust)
	EVT_BUTTON		(ID_GOTO_CURRENT,			UIMovieScheme::OnGoToCurrent)
	EVT_TEXT		(ID_ZOOM_EDIT,				UIMovieScheme::OnZoomEditChanges)
	EVT_SPIN_UP		(ID_SPIN_ZOOM,				UIMovieScheme::OnZoomSpinUp)
	EVT_SPIN_DOWN   (ID_SPIN_ZOOM,				UIMovieScheme::OnZoomSpinDown)
	EVT_ERASE_BACKGROUND(						UIMovieScheme::OnEraseBackground) 
END_EVENT_TABLE()

UIMovieScheme::UIMovieScheme(wxWindow *parent, wxWindowID id, const wxPoint& pos, 
							 const wxSize& size, long style)
							 : cpw::Observer(), wxWindow(parent,id,pos,size,style), zoomEdit(NULL)
{
	zoom = 3;
	InitGUI();
	Init();
}

UIMovieScheme::~UIMovieScheme()
{
	ApplicationTime *time = ApplicationTime::GetInstance();
	time->Detach(this);
	Clear();
}

void UIMovieScheme::OnEraseBackground( wxEraseEvent& event )
{
    //event.Skip();
} 

void UIMovieScheme::InitGUI()
{
	int w,h;
	GetClientSize(&w,&h);

	if (!(ApplicationConfiguration::GetInstance()->IsThemed()))
	{
	  wxColour c_backg   = wxString(ApplicationConfiguration::GetInstance()->GetBackgroundColour().c_str(),wxConvUTF8);
	  wxColour c_foreg   = wxString(ApplicationConfiguration::GetInstance()->GetForegroundColour().c_str(),wxConvUTF8);
		SetBackgroundColour(c_backg);
		SetForegroundColour(c_foreg);
	}
	else
		SetBackgroundColour(WX_COLOUR_SILK_BLUE);


	std::string &icon_path = ApplicationConfiguration::GetInstance()->GetUIIconDirectory();
	
	//Zoom buttons
	zoomIn_Button  = new wxBitmapButton(this, ID_MAGNIFIER_ZOOM_IN, wxBitmap(wxImage(wxString((icon_path + "magnifier_zoom_in_normal.png").c_str(),wxConvUTF8))), wxPoint(2,1), wxSize(22,21), 0, wxDefaultValidator, wxT("Quit_Flame_Button"));
	zoomIn_Button->SetBackgroundColour(WX_COLOUR_SILK_BLUE);
	zoomIn_Button->SetBitmapFocus(wxBitmap(wxImage(wxString((icon_path + "magnifier_zoom_in_pressed.png").c_str(),wxConvUTF8))));
	zoomIn_Button->SetBitmapHover(wxBitmap(wxImage(wxString((icon_path + "magnifier_zoom_in_hover.png").c_str(),wxConvUTF8))));
	zoomIn_Button->SetBitmapSelected(wxBitmap(wxImage(wxString((icon_path + "magnifier_zoom_in_pressed.png").c_str(),wxConvUTF8))));
	zoomOut_Button = new wxBitmapButton(this, ID_MAGNIFIER_ZOOM_OUT, wxBitmap(wxImage(wxString((icon_path + "magnifier_zoom_out_normal.png").c_str(),wxConvUTF8))), wxPoint(26,1), wxSize(22,21), 0, wxDefaultValidator, wxString(((std::string)("Quit_Flame_Button")).c_str(),wxConvUTF8));
	zoomOut_Button->SetBackgroundColour(WX_COLOUR_SILK_BLUE);
	zoomOut_Button->SetBitmapFocus(wxBitmap(wxImage(wxString((icon_path + "magnifier_zoom_out_pressed.png").c_str(),wxConvUTF8))));
	zoomOut_Button->SetBitmapHover(wxBitmap(wxImage(wxString((icon_path + "magnifier_zoom_out_hover.png").c_str(),wxConvUTF8))));
	zoomOut_Button->SetBitmapSelected(wxBitmap(wxImage(wxString((icon_path + "magnifier_zoom_out_pressed.png").c_str(),wxConvUTF8))));
		
	zoomEdit = new wxTextCtrl(this, ID_ZOOM_EDIT, _T("1"), wxPoint(50,2), wxSize(64,19));
	zoomEdit->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
	wxSpinButton *zoom_spin = new wxSpinButton(this, ID_SPIN_ZOOM, wxPoint(50+64,2), wxSize(10,19), wxSP_VERTICAL);

	adjust_Button = new wxBitmapButton(this, ID_ADJUST_TIME_TO_WIDTH, wxBitmap(wxImage(wxString((icon_path + "adjust_normal.png").c_str(),wxConvUTF8))), wxPoint(126,1), wxSize(22,21), 0, wxDefaultValidator, wxT("Adjust to scheme width"));	
	adjust_Button->SetBackgroundColour(WX_COLOUR_SILK_BLUE);
	adjust_Button->SetBitmapFocus(wxBitmap(wxImage(wxString((icon_path + "adjust_pressed.png").c_str(),wxConvUTF8))));
	adjust_Button->SetBitmapHover(wxBitmap(wxImage(wxString((icon_path + "adjust_hover.png").c_str(),wxConvUTF8))));
	adjust_Button->SetBitmapSelected(wxBitmap(wxImage(wxString((icon_path + "adjust_pressed.png").c_str(),wxConvUTF8))));

	gotocurrent_Button = new wxBitmapButton(this, ID_GOTO_CURRENT, wxBitmap(wxImage(wxString((icon_path + "gotocurrent_normal.png").c_str(),wxConvUTF8))), wxPoint(150,1), wxSize(22,21), 0, wxDefaultValidator, wxT("Adjust to scheme width"));	
	gotocurrent_Button->SetBackgroundColour(WX_COLOUR_SILK_BLUE);
	gotocurrent_Button->SetBitmapFocus(wxBitmap(wxImage(wxString((icon_path + "gotocurrent_pressed.png").c_str(),wxConvUTF8))));
	gotocurrent_Button->SetBitmapHover(wxBitmap(wxImage(wxString((icon_path + "gotocurrent_hover.png").c_str(),wxConvUTF8))));
	gotocurrent_Button->SetBitmapSelected(wxBitmap(wxImage(wxString((icon_path + "gotocurrent_pressed.png").c_str(),wxConvUTF8))));

	//SetScrollbar(position of thumb,size of thumb:how many units can be seen at a time, total size in units, size(for Avgpág movement))
	scrollBarH = new wxScrollBar(this, ID_SBH, wxPoint(0,h-(wxDefaultSize.y)), wxSize(w,wxDefaultSize.y), wxSB_HORIZONTAL);
	scrollBarV = new wxScrollBar(this, ID_SBV, wxPoint(w-(wxDefaultSize.x),0), wxSize(wxDefaultSize.x,h), wxSB_VERTICAL);

}

void UIMovieScheme::OnZoomEditChanges(wxCommandEvent& WXUNUSED(event))
{
	double f;
	if (zoomEdit != NULL)
	{
		if (zoomEdit->GetValue().Trim().ToDouble(&f))
		{
			if (f < 1)
			{
			  zoomEdit->ChangeValue(wxT("1"));
				zoom = 1;
				return;
			}
			std::stringstream aux2;
			int aux = atoi(((std::string)zoomEdit->GetValue().mb_str()).c_str());
			zoom = aux;
			aux2 << aux;
			zoomEdit->ChangeValue(wxString(aux2.str().c_str(),wxConvUTF8));					
		}
		else
		{
			std::stringstream aux;
			aux << zoom;
			zoomEdit->ChangeValue(wxString(aux.str().c_str(),wxConvUTF8));
		}
		UpdateScrollBars();
		Update();
	}
}

void UIMovieScheme::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	dc.SetFont(*wxNORMAL_FONT); 
	render(dc);
	Flush();
}

void UIMovieScheme::Flush()
{
	int w,h,w2,h2;
	wxClientDC dc(this);
	GetClientSize(&w,&h);
	w2 = w - scrollBarV->GetSize().x;
	h2 = h - scrollBarH->GetSize().y;
	wxColour c_backg = this->GetBackgroundColour();
	dc.GradientFillLinear( wxRect(w2,h2,w,h), c_backg, c_backg, wxSOUTH);
	if (virtualCanvas_memoryDC.IsOk())	
		dc.Blit(0, 0, w2, h2, &virtualCanvas_memoryDC, 0, 0);
}

void UIMovieScheme::render(wxDC &dc)
{
	int w,h;
	GetClientSize(&w,&h);
	w-=scrollBarV->GetSize().x;
	h-=scrollBarH->GetSize().y;
	wxBitmap bitmap(w, h);
	virtualCanvas_memoryDC.SetFont(*wxNORMAL_FONT); 
	virtualCanvas_memoryDC.SelectObject(bitmap);

	if (!(ApplicationConfiguration::GetInstance()->IsThemed()))
	{
	  wxColour c_pen   = wxString(ApplicationConfiguration::GetInstance()->GetBackgroundGradient2Colour().c_str(),wxConvUTF8);
	  wxColour c_brush = wxString(ApplicationConfiguration::GetInstance()->GetForegroundColour().c_str(),wxConvUTF8);
	  wxColour c_backg = wxString(ApplicationConfiguration::GetInstance()->GetBackgroundGradient1Colour().c_str(),wxConvUTF8);
		virtualCanvas_memoryDC.SetPen(wxPen(c_pen));
		virtualCanvas_memoryDC.SetBrush(wxBrush(c_brush));
		virtualCanvas_memoryDC.SetBackground(wxBrush(c_backg));
		virtualCanvas_memoryDC.GradientFillLinear( wxRect(0,0,w,h), c_backg, c_pen, wxSOUTH);
	}
	else
	{
		virtualCanvas_memoryDC.SetPen(wxPen(WX_COLOUR_SILK_DARKBLUE));
		virtualCanvas_memoryDC.SetBrush(wxBrush(WX_COLOUR_SILK_PALEBLUE));
		virtualCanvas_memoryDC.SetBackground(wxBrush(WX_COLOUR_SILK_BLUE));
		virtualCanvas_memoryDC.Clear();
	}

	if (!AnimatedBoxes.empty())
	{
		int offset = virtualCanvas_pos.x;
		int canvas_size = virtualCanvas_size.x;
		int default_width  = 800;
		int default_height = 200;
		float delta = (default_width/(float)zoom < 10) ? 10 : default_width/(float)zoom;
		for (float i = 0; (int)i-(offset/zoom) < virtualCanvas_size.x; i += delta )
		{
			int col_ = (((int)i-(offset/zoom)) % virtualCanvas_size.x);
			virtualCanvas_memoryDC.DrawLine(col_,0,col_,h); 
		}	
		offset = virtualCanvas_pos.y;
		for (float i = 0; ((int)i - offset) < virtualCanvas_size.y; i += 30 )
		{
			int row_ = (((int)i - offset) % virtualCanvas_size.y);
			virtualCanvas_memoryDC.DrawLine(0,row_,w,row_); 
		}

		std::vector<cpw::gui::AnimatedBox*>::const_iterator iter_b = AnimatedBoxes.begin();
		for(;iter_b != AnimatedBoxes.end(); iter_b++)
			(*iter_b)->Draw(virtualCanvas_memoryDC);
	}
}
void UIMovieScheme::OnZoomIn(wxCommandEvent& WXUNUSED(event))
{
	if (zoom > 1 ) 
	{
		zoom -= 1; 
		std::ostringstream wop;
		wop << std::fixed << zoom;	
		zoomEdit->ChangeValue(wxString(wop.str().c_str(),wxConvUTF8));
		UpdateZoom();
	}
}

void UIMovieScheme::OnZoomOut(wxCommandEvent& WXUNUSED(event))
{
	zoom+=1; 
	std::ostringstream wop;
	wop << std::fixed << zoom;	
	zoomEdit->ChangeValue(wxString(wop.str().c_str(),wxConvUTF8));
	UpdateZoom();
}

void UIMovieScheme::OnAdjust(wxCommandEvent& WXUNUSED(event))
{
	AdjustTimeToWidth();
}

void UIMovieScheme::OnGoToCurrent(wxCommandEvent& WXUNUSED(event))
{
	GoToCurrent();
}

void UIMovieScheme::UpdateScrollBars()
{
	int w,h;
	GetClientSize(&w,&h);
	h-=scrollBarH->GetSize().y;

	//H
	int position = scrollBarH->GetThumbPosition();
	int thumbSize = scrollBarH->GetThumbSize();
	int range = scrollBarH->GetRange();
	int pageSize = scrollBarH->GetPageSize();

	scrollBarH->SetScrollbar(position, thumbSize, range, pageSize,true);
	thumbSize = scrollBarH->GetThumbSize();

	//V
	position = scrollBarV->GetThumbPosition();
	range = BOX_SCHEME_VALIGNMENT + AnimatedBoxes.size() * BOX_LINE_HEIGHT;
	thumbSize = h;
	pageSize = h;
	scrollBarV->SetScrollbar(position, thumbSize, range, pageSize,true);
	thumbSize = scrollBarV->GetThumbSize();
}

void UIMovieScheme::OnMouseLeftDown(wxMouseEvent &event)
{
	int w,h;
	GetClientSize(&w,&h);
	w-=scrollBarV->GetSize().x;
	h-=scrollBarH->GetSize().y;
	m_pos = wxPoint(event.m_x,event.m_y);
	slider_relative_pos = event.m_x;

	SetTimeFromSliderPos();
	Update();

	event.StopPropagation();
}

void UIMovieScheme::OnMouseMove(wxMouseEvent &event)
{
	if (event.LeftIsDown())
	{
		if (event.Dragging())
		{
			int w,h;
			GetClientSize(&w,&h);
			w-=scrollBarV->GetSize().x;
			h-=scrollBarH->GetSize().y;
			m_pos = wxPoint(event.m_x,event.m_y);
			slider_relative_pos = event.m_x;

			SetTimeFromSliderPos();
			Update();
		}
	}
}

void UIMovieScheme::SetTimeFromSliderPos()
{
	ApplicationTime *time = ApplicationTime::GetInstance();
	if (time->GetPeriodEstablished())
	{
		int endTime = time->GetEndDate().GetTicks();
		int startTime = time->GetInitDate().GetTicks();
		if (endTime > startTime)
		{
			wxDateTime new_time( (time_t) (zoom*slider_relative_pos + virtualSpace_pos.x + virtualCanvas_pos.x) );
			if (new_time > end_time) new_time = end_time;
			time->SetTransformedTime(new_time);
		}
	}
}

void UIMovieScheme::OnSize(wxSizeEvent& event)
{
	int w,h;
	this->wxWindow::GetClientSize(&w,&h);
	w-=scrollBarV->GetSize().x;
	h-=scrollBarH->GetSize().y;
	
	scrollBarV->SetSize(wxDefaultSize.x, h);
	scrollBarH->SetSize(w ,wxDefaultSize.y);
	scrollBarH->SetPosition(wxPoint( 0 , h ));
	scrollBarV->SetPosition(wxPoint( w , 0 ));

	UpdateZoom();
	Update();
}


void UIMovieScheme::GoToCurrent()
{
	ApplicationTime *time = ApplicationTime::GetInstance();
	if (time->GetPeriodEstablished())
	{
		int endTime = time->GetEndDate().GetTicks();
		int startTime = time->GetInitDate().GetTicks();
		if (endTime > startTime)
		{

			int refpos  = (virtualSpace_pos.x /*+ virtualCanvas_pos.x*/);
			int refsize =  virtualSpace_size.x;
			float s_pos = time->GetTransformedTime().GetTicks() - refpos;
			float s_pos2 = s_pos / refsize;
			int scroll_current = scrollBarH->GetRange();
			scroll_current *= (int) s_pos2;
			scrollBarH->SetThumbPosition(scroll_current);
			Update();
		}
	}
	UpdateScrollBars();
}

void UIMovieScheme::AdjustTimeToWidth()
{
	ApplicationTime *time = ApplicationTime::GetInstance();
	if (time->GetPeriodEstablished())
	{
		int w,h;
		this->wxWindow::GetClientSize(&w,&h);
		w-=scrollBarV->GetSize().x;
		h-=scrollBarH->GetSize().y;

		scrollBarV->SetSize(wxDefaultSize.x, h);
		scrollBarH->SetSize(w ,wxDefaultSize.y);
		scrollBarH->SetPosition(wxPoint( 0 , h ));
		scrollBarV->SetPosition(wxPoint( w , 0 ));

		int endTime = time->GetEndDate().GetTicks();
		int startTime = time->GetInitDate().GetTicks();
		if (endTime > startTime)
		{
			int length = endTime - startTime;
			zoom = (int)((length)/(float)w)+1;
			UpdateZoom();
			Update();
		}
	}
	UpdateScrollBars();
}
void UIMovieScheme::UpdateZoom()
{
	//long int insertion_point;
	//insertion_point = zoomEdit->GetInsertionPoint();
	std::ostringstream wop;
	wop << zoom;	
	zoomEdit->SetValue(wxString(wop.str().c_str(),wxConvUTF8));
	//zoomEdit->SetInsertionPoint(insertion_point);
}

void UIMovieScheme::OnVScrollMove(wxScrollEvent& event)
{
	virtualCanvas_pos.y = scrollBarV->GetThumbPosition();
	Update();
}

void UIMovieScheme::OnHScrollLineUp(wxScrollEvent& event)
{
	scrollBarH->SetThumbPosition(scrollBarH->GetThumbPosition()-zoom);
	virtualCanvas_pos.x = scrollBarH->GetThumbPosition();
	Update();
}
void UIMovieScheme::OnHScrollLineDown(wxScrollEvent& event)
{
	scrollBarH->SetThumbPosition(scrollBarH->GetThumbPosition()+zoom);
	virtualCanvas_pos.x = scrollBarH->GetThumbPosition();
	Update();
}
void UIMovieScheme::OnHScrollMove(wxScrollEvent& event)
{
	wxClassInfo *ci = event.GetClassInfo();
	wxEventType er = event.GetEventType();
	wxString str = event.GetString();

	virtualCanvas_pos.x = scrollBarH->GetThumbPosition();
	Update();
}

void UIMovieScheme::Init()
{
	virtualCanvas_pos.y = scrollBarV->GetThumbPosition();
	virtualCanvas_pos.x = scrollBarH->GetThumbPosition();
	virtualCanvas_size.x = 1000;
	virtualCanvas_size.y = 200;
	virtualSpace_pos = wxPoint(0,0);
	virtualSpace_size = wxSize(0,0);
	
	Clear();
	Fill();

	UpdatevirtualCanvas();
}

void UIMovieScheme::UpdatevirtualCanvas()
{
}

void UIMovieScheme::Fill()
{
	int w,h;
	GetClientSize(&w,&h);
	w-=scrollBarV->GetSize().x;
	h-=scrollBarH->GetSize().y;
	
	ApplicationTime *time = ApplicationTime::GetInstance();
	//slider
	if (time->GetPeriodEstablished())
	{
		start_time = time->GetInitDate();
		end_time = time->GetEndDate();
		if (end_time > start_time)
		{
			std::vector<cpw::Entity *> animatedEntities = cpw::EntityRegistry::GetInstance()->GetAnimatedEntities();
			if (!animatedEntities.empty())
			{
				std::vector<cpw::Entity*>::const_iterator iter = animatedEntities.begin();
				n_entities = 0;
				for(;iter != animatedEntities.end(); iter++)
				{
					if ((*iter)->isAnimate())
					{

						wxDateTime startt_((time_t)((*iter)->GetStartTime().seconds));
						wxDateTime endt_ ((time_t)((*iter)->GetEndTime().seconds));
						if ((startt_.GetTicks() > 1) && (endt_.GetTicks() > 1))
						{
						  AnimatedBox * newbox = new AnimatedBox( n_entities, wxString((*iter)->GetName().c_str(),wxConvUTF8), startt_, endt_ );
							AnimatedBoxes.push_back( newbox );
							n_entities++;
						}
					}
				}
				scrollBarH->SetScrollbar(scrollBarH->GetThumbPosition(), w*zoom, virtualSpace_size.x, w*zoom);
			}
		}
	}
}

void UIMovieScheme::UpdateList()
{
	Clear();
	Fill();
	Update();
}
void UIMovieScheme::Update(bool subject_deleted)
 {
	if (subject_deleted)
	{
		wxWindow::Update();
		return;
	}
	if (this->GetParent()->IsShown())
	{
		int w,h;
		GetClientSize(&w,&h);
		w-=scrollBarV->GetSize().x;
		h-=scrollBarH->GetSize().y;
		ApplicationTime *time = ApplicationTime::GetInstance();
		UpdateScrollBars();

		if (time->GetPeriodEstablished())
		{
			int endTime = time->GetEndDate().GetTicks();
			int startTime = time->GetInitDate().GetTicks();
			if (endTime > startTime)
			{
				Clear();
				Fill(); 
				int w,h;
				GetClientSize(&w,&h);
				w-=scrollBarV->GetSize().x;
				h-=scrollBarH->GetSize().y;
				
				virtualCanvas_pos.y = scrollBarV->GetThumbPosition();
				virtualCanvas_pos.x = scrollBarH->GetThumbPosition();
				virtualCanvas_size.x = w;
				virtualCanvas_size.y = h;

				virtualSpace_pos.x = start_time.GetTicks();
				virtualSpace_pos.y = 20;
				virtualSpace_size.x = end_time.GetTicks() - start_time.GetTicks();
				virtualSpace_size.y = virtualSpace_pos.y + n_entities * ROW_SIZE;

				std::vector<cpw::gui::AnimatedBox*>::const_iterator iter_b = AnimatedBoxes.begin();
				for(;iter_b != AnimatedBoxes.end(); iter_b++)
				{
					(*iter_b)->CalculateRelativeProperties(virtualCanvas_pos, zoom, start_time, end_time);
				}
				
				slider_relative_pos = time->GetTransformedTime().GetTicks() - (virtualSpace_pos.x + virtualCanvas_pos.x);
				wxClientDC client_dc(this);
				client_dc.SetFont(*wxNORMAL_FONT); 
				render(client_dc); //Repaint
				wxPen temp_pen = virtualCanvas_memoryDC.GetPen();
				virtualCanvas_memoryDC.SetPen(wxPen(WX_COLOUR_GREEN));
				virtualCanvas_memoryDC.DrawLine((wxCoord)slider_relative_pos/zoom,0,(wxCoord)slider_relative_pos/zoom,h);//slider		
				virtualCanvas_memoryDC.SetPen(temp_pen);
				
				Flush();
			}
		}
		else
		{
			Clear();
			wxClientDC client_dc(this);
			client_dc.SetFont(*wxNORMAL_FONT); 
			render(client_dc); //Repaint
			Flush();
		}
	}
	wxWindow::Update();
}
void UIMovieScheme::Clear()
{
	if (!AnimatedBoxes.empty()) 
	{
		std::vector<cpw::gui::AnimatedBox*>::const_iterator iter_b = AnimatedBoxes.begin();
		for(;iter_b != AnimatedBoxes.end(); iter_b++)
			delete (*iter_b);
		AnimatedBoxes.clear();
	}
}

void UIMovieScheme::OnZoomSpinDown(wxSpinEvent& event)
{
	std::ostringstream wop; 
	double f; 
	zoomEdit->GetValue().ToDouble(&f);
	f-=1.0f;
	wop << f; 
	zoomEdit->SetValue(wxString(wop.str().c_str(),wxConvUTF8)); 
}
void UIMovieScheme::OnZoomSpinUp(wxSpinEvent& event)
{
	std::ostringstream wop; 
	double f; 
	zoomEdit->GetValue().ToDouble(&f);
	f+=1.0f;
	wop << f; 
	zoomEdit->SetValue(wxString(wop.str().c_str(),wxConvUTF8)); 
}

//Animated box
AnimatedBox::AnimatedBox(int row, const wxString &label, const wxDateTime &startt_local, const wxDateTime &endt_local)
			: label_(label),startt_(startt_local),endt_(endt_local), row_(row)
{
	virtual_space_pos_ = startt_local.GetTicks();
	virtual_space_size = endt_local.GetTicks() - startt_local.GetTicks();
	vertical_offset = BOX_LINE_HEIGHT;
}

AnimatedBox::AnimatedBox(const wxString &label, const wxDateTime &startt_local, const wxDateTime &endt_local, const wxDateTime &startt_global, const wxDateTime &endt_global, int zoom)
			: label_(label),startt_(startt_local),endt_(endt_local)
{
}

AnimatedBox::AnimatedBox(const wxPoint &pos, const wxSize &size, const wxString &label, const wxDateTime &startt, const wxDateTime &endt, int zoom)
			: pos_(pos),size_(size),label_(label),startt_(startt),endt_(endt)
{
}

AnimatedBox::~AnimatedBox()
{
}

void AnimatedBox::CalculateRelativeProperties(wxPoint virtualCanvas_pos, int zoom, wxDateTime global_start_time, wxDateTime global_end_time)
{
  pos_.x  = (int)( (virtual_space_pos_ - global_start_time.GetTicks() - virtualCanvas_pos.x) / (float)zoom);
  pos_.y  = (int)( BOX_SCHEME_VALIGNMENT + row_ * vertical_offset - virtualCanvas_pos.y);
  size_.x = (int) (virtual_space_size / (float)zoom);
  size_.y = 15;
}

void AnimatedBox::Draw(wxDC &dc)
{
	wxFont font = dc.GetFont();
	int fontSize = font.GetPointSize(); 

	font.SetPointSize(6);
	dc.SetFont(font);
	wxSize refSize = dc.GetTextExtent(startt_.Format());
	font.SetPointSize(fontSize);
	dc.SetFont(font);
	wxSize refSize2 = dc.GetTextExtent(label_);
	font.SetPointSize(6);
	dc.SetFont(font);

	if ((size_.GetWidth() > 2*refSize.GetWidth()) && (size_.GetWidth() > refSize2.GetWidth()))
	{
		dc.DrawRoundedRectangle(pos_.x,pos_.y,size_.GetWidth(),size_.GetHeight(),3.1f);
		wxSize string_size = dc.GetTextExtent(endt_.Format()); 
		dc.SetTextForeground(WX_COLOUR_SILK_ORANGE);
		dc.DrawRotatedText(startt_.Format(),pos_.x,pos_.y-10, 0);
		dc.DrawRotatedText(endt_.Format(),pos_.x + size_.GetWidth()-string_size.x,pos_.y-10, 0);
		dc.SetTextForeground(wxColour(0,0,0));
		font.SetPointSize(fontSize);
		dc.SetFont(font);
		dc.DrawRotatedText(label_,pos_.x+((size_.GetWidth()-refSize2.GetWidth())/2),pos_.y+((size_.GetHeight()/2)-7), 0);
	}
	else
	{
		font.SetPointSize(fontSize);
		dc.SetFont(font);		
		//dc.DrawCircle(pos_.x,pos_.y,5);
		dc.DrawRoundedRectangle(pos_.x,pos_.y+7,size_.GetWidth(),8,3.1f);
		dc.SetTextForeground(WX_COLOUR_SILK_ORANGE);
		dc.DrawRotatedText(label_,pos_.x,pos_.y-7, 0);
		wxSize string_size = dc.GetTextExtent(label_); 
		dc.DrawRotatedText(startt_.Format(),pos_.x+string_size.x+7,pos_.y-7, 0);
		wxSize string_size2 = dc.GetTextExtent(startt_.Format()); 
		dc.DrawRotatedText(_T("-"),pos_.x + string_size.x+string_size2.x+13,pos_.y-7, 0);
		dc.DrawRotatedText(endt_.Format(),pos_.x + string_size.x+string_size2.x+23,pos_.y-7, 0);
		dc.SetTextForeground(wxColour(0,0,0));
	}
}

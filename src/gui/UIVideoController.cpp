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

#include <sstream>

#include "stdafx.h"

#include "gui/UIVideoController.h"

#include <cpw/entity/EntityRegistry.h>


using namespace cpw::gui;

#define SPINMIN -1000000
#define SPINMAX 999999

BEGIN_EVENT_TABLE(UIVideoController, wxToolBar)
EVT_TOOL (ANIMATION_START,		UIVideoController::OnButtonStart)
EVT_TOOL (ANIMATION_REWIND,		UIVideoController::OnButtonRewind)
EVT_TOOL (ANIMATION_FORWARD,	UIVideoController::OnButtonForward)
EVT_TOOL (ANIMATION_PLAY,		UIVideoController::OnButtonPlay)
EVT_TOOL (ANIMATION_END,		UIVideoController::OnButtonEnd)
EVT_TOOL (ANIMATION_REPEAT,		UIVideoController::OnButtonRepeat)
EVT_TOOL (ANIMATION_STOP,		UIVideoController::OnButtonStop)
EVT_TOOL (ANIMATION_CURRENTTIME,	UIVideoController::OnButtonCurrentTime)
EVT_TOOL (ANIMATION_VIEW_ANIMABLE,	UIVideoController::OnButtonViewAnimable)
EVT_TOOL (ANIMATION_FIRE_VIEWMODE,	UIVideoController::OnButtonFireViewMode)
EVT_DATE_CHANGED (APPLICATION_DATE, UIVideoController::SetDateChanges)
EVT_COMMAND_SCROLL  (ANIMATION_SLIDER, UIVideoController::SliderChanges)
EVT_TEXT		    (ANIMATION_MULTIPLIER, UIVideoController::OnMultiplierChanged)
EVT_SPIN_UP			(ID_MULTIPLIER_SPIN,  UIVideoController::OnMultiplierSpinUp)
EVT_SPIN_DOWN		(ID_MULTIPLIER_SPIN,  UIVideoController::OnMultiplierSpinDown)
END_EVENT_TABLE()

UIVideoController::UIVideoController(wxAuiManager *m_mgr_,wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
: wxToolBar(parent, id, pos, size, style, name), cpw::Observer(), parent_(parent), mult(1), multiplier(NULL), init(false), m_mgr(m_mgr_)
{
  
  std::string icon_path = ApplicationConfiguration::GetInstance()->GetUIIconDirectory();
  
  AddTool(ANIMATION_CURRENTTIME, wxT("CURRET DATE & TIME"),
	  wxBitmap(wxString((icon_path + "date_next.png").c_str(),wxConvUTF8)),
	  wxT("CURRENTTIME"));
	
  app_Time.Date = new wxDatePickerCtrl(this, APPLICATION_DATE, wxDefaultDateTime,
				       wxDefaultPosition, wxSize(80,19),
				       wxDP_DEFAULT | wxDP_SHOWCENTURY | wxDP_DROPDOWN);

  AddControl(app_Time.Date);
  wxString dt = MyTimeCtrl::GetCurrentTime();

  app_Time.Hour = new MyTimeCtrl(this, APPLICATION_HOUR,
				 dt, wxDefaultPosition,
				 wxSize(50,19));

  AddControl(app_Time.Hour);
  AddSeparator();

  AddTool(ANIMATION_START, wxT("START"),
	  wxBitmap(wxString((icon_path + "control_start_blue.png").c_str(),wxConvUTF8)),
	  wxT("START"));
  AddTool(ANIMATION_REWIND, wxT("REWIND"), wxBitmap(wxImage(wxString((icon_path + "control_rewind_blue.png").c_str(),wxConvUTF8))), wxT("REWIND"));
  AddTool(ANIMATION_PLAY, wxT("PLAY"), wxBitmap(wxImage(wxString((icon_path + "control_play_blue.png").c_str(),wxConvUTF8))), wxT("PLAY"));
  AddTool(ANIMATION_STOP, wxT("STOP"), wxBitmap(wxImage(wxString((icon_path + "control_stop_blue.png").c_str(),wxConvUTF8))), wxT("STOP"));
  AddTool(ANIMATION_FORWARD, wxT("FORWARD"), wxBitmap(wxImage(wxString((icon_path + "control_fastforward_blue.png").c_str(),wxConvUTF8))), wxT("FORWARD"));
  AddTool(ANIMATION_END, wxT("END"), wxBitmap(wxImage(wxString((icon_path + "control_end_blue.png").c_str(),wxConvUTF8))), wxT("END"));
  AddTool(ANIMATION_REPEAT, wxT("REPEAT"), wxBitmap(wxImage(wxString((icon_path + "control_repeat_blue.png").c_str(),wxConvUTF8))), wxT("REPEAT"),wxITEM_CHECK);
  AddSeparator();

  multiplier = new wxTextCtrl(this, ANIMATION_MULTIPLIER,
			      wxT("1"), wxDefaultPosition, wxSize(70,19)); // min = invalid
  AddControl(multiplier);
  wxSpinButton *multiplier_spin = new wxSpinButton(this, ID_MULTIPLIER_SPIN,
						   wxDefaultPosition, wxSize(10,19), wxSP_VERTICAL);
  AddControl(multiplier_spin);
  timeSlider = new wxSlider(this, ANIMATION_SLIDER, 0 , 0, 200,
			    wxDefaultPosition, wxSize(150,19), wxSL_HORIZONTAL);
  timeSlider->SetLineSize(2);
  AddControl(timeSlider);
  AddSeparator();
  AddTool(ANIMATION_VIEW_ANIMABLE, wxT("View Animations"),
	  wxBitmap(wxImage(wxString((icon_path + "view_animable.png").c_str(),wxConvUTF8))),
	  wxT("view Animation"));
  AddTool(ANIMATION_FIRE_VIEWMODE, wxT("Fire View Mode"),
	  wxBitmap(wxImage(wxString((icon_path + "fire_view_mode.png").c_str(),wxConvUTF8))),
	  wxT("Fire view mode"),wxITEM_CHECK);

  this->Connect(APPLICATION_HOUR, MyEVT_TIMECHANGE,
		(wxObjectEventFunction)(wxEventFunction)
		(wxCommandEventFunction) &UIVideoController::SetHourChanges);

  playing = false;

  play_bitmap  = wxBitmap(wxImage(wxString((icon_path + "control_play_blue.png").c_str(),wxConvUTF8)));
  pause_bitmap = wxBitmap(wxImage(wxString((icon_path + "control_pause_blue.png").c_str(),wxConvUTF8)));

}

UIVideoController::~UIVideoController()
{
  ApplicationTime *time = ApplicationTime::GetInstance();
  time->Detach(this);
} 

void UIVideoController::Update(bool subject_deleted)
{

  if (subject_deleted)
    return;
  ApplicationTime *time = ApplicationTime::GetInstance();
  int imultiplier = atoi(((std::string)multiplier->GetValue().mb_str()).c_str());
  if ((imultiplier <= SPINMAX) && (imultiplier >= SPINMIN+1))
    time->SetMultiplier(imultiplier);
  else 
    time->SetMultiplier(0);

  //repeat
  if (time->GetPeriodEstablished() && GetToolState(ANIMATION_REPEAT))
    {
      wxDateTime new_time = time->GetTransformedTime();
      if (new_time >= time->GetEndDate())
	{
	  wxDateTime startTime = time->GetInitDate();
	  time->GetTransformedTime().SetDay(startTime.GetDay());
	  time->GetTransformedTime().SetMonth(startTime.GetMonth());
	  time->GetTransformedTime().SetYear(startTime.GetYear());

	  time->GetTransformedTime().SetHour(startTime.GetHour());
	  time->GetTransformedTime().SetMinute(startTime.GetMinute());
	  time->GetTransformedTime().SetSecond(startTime.GetSecond());
	}
    }

  //slider
  if (time->GetPeriodEstablished())
    {
      int endTime = time->GetEndDate().GetTicks();
      int startTime = time->GetInitDate().GetTicks();
      if (endTime > startTime)
	{
	  double percent = (time->GetTransformedTime().GetTicks() - startTime) /(double)(endTime - startTime);
	  timeSlider->SetValue((int)(percent * timeSlider->GetMax()));
	}
      else
	timeSlider->SetValue(0);
    }

  wxString time_time = time->GetTransformedTime().FormatTime();
  app_Time.Date->SetValue(time->GetTransformedTime());
  app_Time.Hour->SetValue(time_time);

} 

void UIVideoController::OnButtonRewind(wxCommandEvent& WXUNUSED(event))
{
  int value = atoi(((std::string)multiplier->GetValue().mb_str()).c_str());
  value-=20;
  std::stringstream aux;
  if (value <= SPINMAX && (value >= SPINMIN+1))
    {
      aux << value;
      multiplier->SetValue(wxString(aux.str().c_str(),wxConvUTF8));
    }
  else
    {
      aux << SPINMIN+1;
      multiplier->SetValue(wxString(aux.str().c_str(),wxConvUTF8));
    }
}

void UIVideoController::OnButtonForward(wxCommandEvent& WXUNUSED(event))
{
  int value = atoi(((std::string)multiplier->GetValue().mb_str()).c_str());
  value+=20;
  std::stringstream aux;
  if (value <= SPINMAX && (value >= SPINMIN+1))
    {
      aux << value;
      multiplier->SetValue(wxString(aux.str().c_str(),wxConvUTF8));
    }
  else
    aux << SPINMAX;
  multiplier->SetValue(wxString(aux.str().c_str(),wxConvUTF8));
}

void UIVideoController::OnButtonPlay(wxCommandEvent& WXUNUSED(event))
{
  SwitchPlayPauseButton();
}

void UIVideoController::SetPlayButton()
{
  std::string &icon_path = ApplicationConfiguration::GetInstance()->GetUIIconDirectory();
  Hide();
  SetToolNormalBitmap(ANIMATION_PLAY,play_bitmap);
  Show();
  m_mgr->Update();
  playing = false;
}
void UIVideoController::SetPauseButton()
{
  std::string &icon_path = ApplicationConfiguration::GetInstance()->GetUIIconDirectory();
  Hide();
  SetToolNormalBitmap(ANIMATION_PLAY,pause_bitmap);
  Show();
  m_mgr->Update();
  playing = true;
}


void UIVideoController::SwitchPlayPauseButton()
{
  std::string &icon_path = ApplicationConfiguration::GetInstance()->GetUIIconDirectory();
  if (playing)
    {
      ApplicationTime::GetInstance()->Pause();
      Hide();
      SetToolNormalBitmap(ANIMATION_PLAY,play_bitmap);
      Show();
    }
  else
    {
      ApplicationTime::GetInstance()->On();	
      Hide();
      SetToolNormalBitmap(ANIMATION_PLAY,pause_bitmap);
      Show();
    }
  m_mgr->Update();
  playing=!playing;
}

void UIVideoController::OnButtonStart(wxCommandEvent& WXUNUSED(event))
{
  ApplicationTime *time = ApplicationTime::GetInstance();
  if (time->GetPeriodEstablished())
    time->SetTransformedTime(time->GetInitDate());
  time->On();
  time->Off();
  SetPlayButton();
  Update();
}

void UIVideoController::OnButtonEnd(wxCommandEvent& WXUNUSED(event))
{
  ApplicationTime *time = ApplicationTime::GetInstance();
  if (time->GetPeriodEstablished())
    time->SetTransformedTime(time->GetEndDate());
  time->On();
  time->Off();
  SetPlayButton();
  Update();
}

void UIVideoController::OnButtonRepeat(wxCommandEvent& WXUNUSED(event))
{
}
	
void UIVideoController::Stop()
{
  ApplicationTime *time = ApplicationTime::GetInstance();
  if (time->GetPeriodEstablished())
    time->SetTransformedTime(time->GetInitDate());
  time->On();
  time->Off();
  SetPlayButton();
  Update();
}


void UIVideoController::OnButtonStop(wxCommandEvent& WXUNUSED(event))
{
  Stop();
}

void UIVideoController::SetDateChanges(wxDateEvent& WXUNUSED(event))
{
  ApplicationTime *time = ApplicationTime::GetInstance();
  wxDateTime new_time = app_Time.Date->GetValue();
  time->GetTransformedTime().SetDay(new_time.GetDay());
  time->GetTransformedTime().SetMonth(new_time.GetMonth());
  time->GetTransformedTime().SetYear(new_time.GetYear());
  Update();
}

void UIVideoController::SetHourChanges(wxCommandEvent& event)
{
  ApplicationTime *time = ApplicationTime::GetInstance();
  wxString time_str = app_Time.Hour->GetValue();
  long hour_t, minute_t, second_t;
  time_str(0,2).ToLong(&hour_t);
  time_str(3,2).ToLong(&minute_t);
  time_str(6,2).ToLong(&second_t);
  wxDateTime new_time = wxDateTime(hour_t, minute_t, second_t);
  time->GetTransformedTime().SetHour(new_time.GetHour());
  time->GetTransformedTime().SetMinute(new_time.GetMinute());
  time->GetTransformedTime().SetSecond(new_time.GetSecond());
  Update();
}

void UIVideoController::OnButtonCurrentTime(wxCommandEvent& WXUNUSED(event))
{
  wxDateTime current;
  ApplicationTime *time = ApplicationTime::GetInstance();
  time->SetTransformedTime(current.Now());
  Update();
}

void UIVideoController::SliderChanges(wxScrollEvent& WXUNUSED(event))
{
  ApplicationTime *time = ApplicationTime::GetInstance();

  if (time->GetPeriodEstablished())
    {
      int endTime = time->GetEndDate().GetTicks();
      int startTime = time->GetInitDate().GetTicks();
      if (endTime > startTime)
	{
	  double percent = timeSlider->GetValue()/(double)timeSlider->GetMax();
	  wxDateTime new_time( (time_t) (startTime+(endTime - startTime)*percent) );
	  time->SetTransformedTime(new_time);
	}
      else
	timeSlider->SetValue(0);
    }
  Update();
}

void UIVideoController::OnMultiplierChanged(wxCommandEvent& WXUNUSED(event))
{
  double f;
  //check if every edit text is a number
  if (multiplier != NULL)
    {
      if (multiplier->GetValue().Trim().ToDouble(&f) || f<0)
	{
	  std::stringstream aux2;
	  if (f < SPINMIN)
	    {
	      mult = SPINMIN;
	      aux2 << mult;
	      multiplier->ChangeValue(wxString(aux2.str().c_str(),wxConvUTF8));
	      return;
	    }
	  if (f > SPINMAX)
	    {
	      mult = SPINMAX;
	      aux2 << mult;
	      multiplier->ChangeValue(wxString(aux2.str().c_str(),wxConvUTF8));
	      return;
	    }
	  int aux = atoi(((std::string)multiplier->GetValue().mb_str()).c_str());
	  aux2 << aux;
	  multiplier->ChangeValue(wxString(aux2.str().c_str(),wxConvUTF8));
	  mult = atoi(((std::string)multiplier->GetValue().mb_str()).c_str());
	}
      else
	{
	  std::stringstream aux;
	  aux << mult;
	  multiplier->ChangeValue(wxString(aux.str().c_str(),wxConvUTF8));
	}
    }
}

void UIVideoController::OnButtonViewAnimable(wxCommandEvent& WXUNUSED(event))
{
  UIAnimableList *animableDialog = new UIAnimableList(parent_, wxID_ANY, wxT("Animable List"), wxDefaultPosition, wxDefaultSize);
  animableDialog->Append(cpw::EntityRegistry::GetInstance()->GetAnimatedEntities());

  animableDialog->ShowModal();
}

void UIVideoController::OnButtonFireViewMode(wxCommandEvent& WXUNUSED(event))
{
}

int UIVideoController::GetFireViewMode()
{
  int value;
  value = (GetToolState(ANIMATION_FIRE_VIEWMODE))? 1 : 0;
  return value;
}

void UIVideoController::OnMultiplierSpinUp(wxSpinEvent& event)
{
  std::ostringstream wop; 
  double f; 
  multiplier->GetValue().ToDouble(&f);
  int value = f+1;
  wop << value;  
  multiplier->SetValue(wxString(wop.str().c_str(),wxConvUTF8)); 
}
void UIVideoController::OnMultiplierSpinDown(wxSpinEvent& event)
{
  std::ostringstream wop; 
  double f; 
  multiplier->GetValue().ToDouble(&f);
  int value = f-1;
  wop << value;  
  multiplier->SetValue(wxString(wop.str().c_str(),wxConvUTF8)); 
}

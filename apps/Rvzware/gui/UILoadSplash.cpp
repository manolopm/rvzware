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
#include <string>

#include <stdafx.h>

#include <gui/UILoadSplash.h>

#include <cpw/ApplicationConfiguration.h>

using namespace cpw::gui;

BEGIN_EVENT_TABLE(UILoadSplash,wxDialog)  
EVT_PAINT     (UILoadSplash::OnPaint)
EVT_CLOSE     (UILoadSplash::OnClose)
EVT_MOVE      (UILoadSplash::OnMove)
END_EVENT_TABLE()

UILoadSplash::UILoadSplash(wxWindow *parent, wxWindowID id,
			   const wxString &title, const wxString &bk_image,
			   const wxPoint &position, const wxSize& size,
			   long style)
: wxDialog(parent, id, title,
	   position, size, style),
  paren(parent),
  background_image(bk_image)
{
  
  if (!(ApplicationConfiguration::GetInstance()->IsThemed()))
    {
      SetFont(paren->GetFont());
      SetForegroundColour(paren->GetForegroundColour());
      SetBackgroundColour(paren->GetBackgroundColour());
    }
  
  CreateGUIControls();
}

UILoadSplash::~UILoadSplash()
{
} 

void UILoadSplash::CreateGUIControls()
{
  SetTitle(wxT("CAPAWARE"));
  SetIcon(wxNullIcon);
  
  m_bmp = wxBitmap(wxImage(background_image));
  SetSize(8,8,m_bmp.GetWidth(),m_bmp.GetHeight());
  Center();
}

void UILoadSplash::OnClose(wxCloseEvent& /*event*/)
{
  this->EndDialog(wxID_CANCEL);	
}

void UILoadSplash::OnMove(wxMoveEvent& event)
{
}

void UILoadSplash::OnPaint(wxPaintEvent& event)
{
  wxPaintDC dc(this);
  render(dc);
}

void UILoadSplash::render(wxDC& dc)
{
  dc.DrawBitmap(m_bmp, 0, 0, false);
}


void UILoadSplash::OnEraseBackground(wxEraseEvent& event)
{
}

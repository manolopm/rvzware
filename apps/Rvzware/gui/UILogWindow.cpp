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

#include <wx/textctrl.h>

#include <stdafx.h>

#include <gui/UILogWindow.h>

using namespace cpw::gui;



UILogWindow::UILogWindow(wxWindow *parent,
			 wxWindowID id, const wxString &name,
			 const int &sizex, const int &sizey,
			 const int &posx, const int &posy)
  : wxPanel(parent, wxID_ANY, wxPoint(posx, posy), wxSize(sizex, sizey), wxTAB_TRAVERSAL, name)
{
  
  wxBoxSizer *top_sizer = new wxBoxSizer(wxVERTICAL);
  SetSizer(top_sizer);
  
  log_text = new wxTextCtrl(this, wxID_ANY, wxString(std::string("").c_str(),wxConvUTF8),
			    wxDefaultPosition, wxDefaultSize,
			    wxTE_MULTILINE | wxTE_READONLY | wxHSCROLL);
  
  if (top_sizer)
    top_sizer->Add(log_text,1,wxGROW);

}

UILogWindow::~UILogWindow(void)
{
}

void UILogWindow::NewLogMessage(const std::string &new_message)
{
  if ((SHOW_ADDITIONAL_WINDOWS)&&(log_text))
    {
      log_text->AppendText(wxT("\n"));
      log_text->AppendText(wxString(new_message.c_str(),wxConvUTF8));
    }
}



void UILogWindow::ClearLog()
{
  if ((SHOW_ADDITIONAL_WINDOWS) && (log_text))
    log_text->Clear();
}

void UILogWindow::printf (const char *format, ...)
{
  va_list params;
  char linea[300];
  
  // creamos la string
  va_start(params, format);
  vsprintf (linea, format, params);
  va_end(params);
  
  // lanzamos el mensaje
  NewLogMessage (linea);
}

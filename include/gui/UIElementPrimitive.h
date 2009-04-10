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
#ifndef _UIELEMENTPRIMITIVE_
#define _UIELEMENTPRIMITIVE_

#include <vector>
#include <wx/wx.h>
#include <wx/image.h>
#include <wx/statline.h>
#include <wx/fontdlg.h>

#include <cpw/common/Attribute.h>
#include <gui/UIAttributes.h>


namespace cpw 
{
	namespace gui	
	{

		/** 
			\brief Window panel for creating Element Templates
			\ingroup cwcontent
		*/
		class UIElementPrimitive: public wxDialog
		{
		DECLARE_EVENT_TABLE()
		public:
			UIElementPrimitive(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);//(wxMDIParentFrame* parent, int id, const wxChar *titulo);//(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);
			virtual ~UIElementPrimitive();

			std::string GetPrimitiveName(){return std::string(text_name->GetValue().mb_str());}
			std::string GetModelPath(){return std::string(text_model->GetValue().mb_str());}
			std::string GetIconPath(){return std::string(text_icon->GetValue().mb_str());}
			std::vector<cpw::Attribute> GetAttributes(){ return v_attr;}
			std::string GetFontType(){return std::string(text_font->GetValue().mb_str());}
			std::string GetDescription(){return std::string(text_description->GetValue().mb_str());}

			void SetElementPrimitiveName(const std::string &name) {
			  wxString wxstr(name.c_str(), wxConvUTF8);
			  text_name->SetValue(wxstr);}


		private:		
			void set_properties();
			void do_layout();
			void OnButtonExplorer1(wxCommandEvent& WXUNUSED(event));
			void OnButtonExplorer2(wxCommandEvent& WXUNUSED(event));
			void OnButtonFont(wxCommandEvent& WXUNUSED(event));
			void OnButtonAttr(wxCommandEvent& WXUNUSED(event));
			void OnButtonCancel(wxCommandEvent& WXUNUSED(event));
			void OnButtonOK(wxCommandEvent& WXUNUSED(event));
			void OnPaint(wxPaintEvent& event);
			void render(wxDC& dc);

			wxStaticText* label_name;
			wxStaticText* label_model;
			wxStaticText* label_icon;
			wxStaticText* label_font;
			wxStaticText* label_description;

			wxTextCtrl* text_name;
			wxTextCtrl* text_model;
			wxTextCtrl* text_icon;
			wxTextCtrl* text_font;
			wxTextCtrl* text_description;
			
			wxButton* button_model;
			wxButton* button_icon;
			wxButton* button_font;		
			wxButton* button_attributes;
			wxButton* button_ok;
			wxButton* button_cancel;

			wxPanel* panel_name;
			wxPanel* panel_desc1;
			wxPanel* panel_desc2;
			wxPanel* panel_attr;
			
			std::vector<cpw::Attribute> v_attr;
			wxWindow *paren;

		};
	}
}

#endif

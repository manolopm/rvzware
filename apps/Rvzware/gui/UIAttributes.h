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

#ifndef _UIATTRIBUTES_
#define _UIATTRIBUTES_

#include <vector>
#include <wx/wx.h>
#include <wx/image.h>
#include <wx/grid.h>

#include <cpw/common/Attribute.h>


namespace cpw 
{ 
	
	namespace gui
	{

		/** 
			\brief Attributes Window
			\ingroup cwcontent
		*/
		class UIAttributes: public wxDialog
		{
		DECLARE_EVENT_TABLE()
		public:
			UIAttributes(int flag,wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);//(wxMDIParentFrame* parent, int id, const wxChar *titulo);//(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);
			virtual ~UIAttributes();
		
			//Get data method
			std::vector<cpw::Attribute> GetAttributes();

			void SetAttributes(const std::vector<cpw::Attribute> &attr);

		private:		
			void set_properties();
			void do_layout();
			void OnCharHook(wxKeyEvent& event);
			void OnButtonAdd(wxCommandEvent& WXUNUSED(event));
			void OnButtonDelete(wxCommandEvent& WXUNUSED(event));
			void OnButtonCancel(wxCommandEvent& WXUNUSED(event));
			void OnPaint(wxPaintEvent& event);
			void render(wxDC& dc);

			wxStaticText* label_name;
			wxStaticText* label_type;
			wxStaticText* label_value;

			wxTextCtrl* text_name;
			wxTextCtrl* text_value;
			
			wxButton* button_add;
			wxButton* button_delete;
			wxButton* button_ok;
			wxButton* button_cancel;

			wxPanel* panel_name;
			wxPanel* panel_desc1;
			wxPanel* panel_desc2;
			
			wxComboBox* combo_box_type;
			wxGrid* grid_1;

			bool grid_modify;

			wxWindow *paren;

			int parent_flag; // 0 - layer primitive, 1 - element primitive

			std::string aux_value;
		};

	}
}

#endif

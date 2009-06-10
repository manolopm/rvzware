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

#ifndef _UILAYER_
#define _UILAYER_

#include <vector>

#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/stattext.h>


#include <cpw/common/Attribute.h>
#include <cpw/entity/Entity.h>
#include <controllers/LayerController.h>
#include <gui/UIEAttributes.h>


namespace cpw 
{ 
	namespace controllers { class LayerController; }

	namespace gui
	{
		
		/** 
			\brief Window for Adding a new Layer
			\ingroup cwcontent
		*/
		class UILayer : public wxDialog
		{
			private:
				DECLARE_EVENT_TABLE();
				
			public:
				UILayer(controllers::LayerController *lc_, wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("Layer"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);
				virtual ~UILayer();

				void SetAttributes(std::vector<cpw::Attribute> &attr){attributes = attr;}
				std::string GetName(){return std::string(tc_name->GetValue().ToAscii());}
				std::string GetHTML(){return std::string(tc_html->GetValue().ToAscii());}
				std::vector<cpw::Attribute> &GetAttributes(){ return attributes;}		
				std::string GetDescription(){return std::string(tc_description->GetValue().ToAscii());}
				bool IsDynamic() {return false;}
				void RePaint(){wxMoveEvent tmp = wxMoveEvent(); OnMove(tmp);}
				void SetLayerName(const std::string &name) {tc_name->SetValue((wxString &)name);}
				void SetHtml(const std::string &html) {tc_html->SetValue((wxString &)html);}
				void SetDescription(const std::string &desc) {tc_description->SetValue((wxString &)desc);}
				void Modify(const bool &value) {modify = value; tc_name->Enable(false); primitiveChoice->Enable(false); primitiveBrowseButton->Enable(false); button_ok->Enable(true);}
				void SetPrimitivesUrl(const std::map<std::string, cpw::TypeId> &vec_url);
				void DisableNonEditable() {};
				std::string &GetUrl() {return primitive_url;}
				cpw::TypeId &GetId() {return primitive_id;}

			private:

				wxArrayString arrayStringFor_formatChoice;
				
				wxButton *button_html;		
				wxButton *button_cancel;
				wxButton *button_ok;
				wxButton *button_attributes;
				wxButton *primitiveBrowseButton;
				wxTextCtrl *tc_description;
				wxTextCtrl *tc_html;
				wxTextCtrl *tc_name;
				wxChoice *primitiveChoice;

				wxFileDialog *open_primitive;
				controllers::LayerController	*lc;

				std::vector<cpw::Attribute> attributes;
				std::map<std::string, cpw::TypeId> v_url;
				cpw::TypeId primitive_id;
				std::string primitive_url;
				wxWindow *paren;
				bool modify;

				void OnClose(wxCloseEvent& event);
				void CreateGUIControls();
				
				void ButtonOK();

				void OnButtonOK(wxCommandEvent& WXUNUSED(event));
				void OnButtonCancel(wxCommandEvent& WXUNUSED(event));
				void OnButtonAttributes(wxCommandEvent& WXUNUSED(event));
				void OnButtonHtml(wxCommandEvent& WXUNUSED(event));
				void OnMove(wxMoveEvent& event);
				void OnChoiceChanged(wxCommandEvent& WXUNUSED(event));
				void OnBrowsePrimitivesButton(wxCommandEvent& event);

				void OnPaint(wxPaintEvent& event);
				void render(wxDC& dc);

		};
	}
}

#endif

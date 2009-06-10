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

#ifndef _UIWCSSERVERCONNECTION_
#define _UIWCSSERVERCONNECTION_

#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/spinbutt.h>
#include <wx/treectrl.h>
#include <string>
#include <vector>

#include <cpw/common/Attribute.h>
#include <cpw/entity/Entity.h>

#include <controllers/WCSServerConnectionController.h>
#include <gui/UIEAttributes.h>

#include <ogcwcs/WcsLayerController.h>


namespace cpw 
{ 
	namespace controllers {class WCSServerConnectionController; }
	namespace gui
	{

		/** 
			\brief User Interface for WCS server connections
			\ingroup cwcontent
		*/
		class UIWCSServerConnection : public wxDialog
		{		
			public:
				UIWCSServerConnection(controllers::WCSServerConnectionController* scc,wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("Server Connection"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE);
				virtual ~UIWCSServerConnection();

				void SetServerList(std::vector<std::string> &vector_server);
				std::vector<std::string> GetServerList();

				void ShowSRS(const bool vis) {view_srs = vis; srsChoice->Show(vis);}

			private:

				wxWindow *paren;
				wxStaticText *WxStaticText2;
				wxTextCtrl *description_memo;
				wxTextCtrl *Layer_name_Ed;
				wxStaticText *WxStaticText1;
				wxComboBox *server_combo_box;
				
				wxTreeCtrl *Layer_tree_TreeCtrl;
				wxImageList *img_list;
				wxChoice *formatChoice;
				wxChoice *srsChoice;
				
				wxButton *connect_button;
				wxButton *cancel_button;
				wxButton *next_button;
			
				void OnClose(wxCloseEvent& event);
				void CreateGUIControls();

				void OnButtonCancel(wxCommandEvent& WXUNUSED(event));
				void OnButtonNext(wxCommandEvent& WXUNUSED(event));
				void OnButtonConnect(wxCommandEvent& WXUNUSED(event));
				void OnConnect(wxCommandEvent& WXUNUSED(event));
				void OnMove(wxMoveEvent& event);

				void OnPaint(wxPaintEvent& event);
				void render(wxDC& dc);

				controllers::WCSServerConnectionController* server_connection_controller;
				int connection_state;

				DECLARE_EVENT_TABLE();

			public:

				void                     SetLayers(ogcwcs::Node &root_node);
				std::vector<std::string> GetSelectedLayers();
				std::string              GetName(){return std::string(Layer_name_Ed->GetValue().mb_str());}
				void                     SetName(const std::string &name){Layer_name_Ed->SetValue(wxString(name.c_str(),wxConvUTF8));}
				std::string              GetSelectedFormat() {return std::string(formatChoice->GetString(formatChoice->GetSelection()).mb_str());}
				std::string              GetSelectedSRS() {return std::string(srsChoice->GetString(srsChoice->GetSelection()).mb_str());}
				bool					 Builder() {return view_srs;}

				void Modify (const bool &value) {modify=value; Layer_name_Ed->Enable(false);/* Back_button->Enable(false);*/}
				void SetConnectionMemo(std::string &server_url, std::string &abstr, int &c_state);

			private:		
				bool changes;
				bool modify;
				void EvtTreeItemSelChanged(wxTreeEvent& WXUNUSED(event));
				void FillTree(std::vector<ogcwcs::Node*> &nodes, wxTreeItemId &parent_id);
				std::map<std::string, std::string> node_title_name;			
				void AddIcon(const std::string url, wxTreeItemId id);
				void Close();
				bool view_srs;
		};
	}
}

#endif

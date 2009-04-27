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

#ifndef _UIREMOTEPUBLISHED_
#define _UIREMOTEPUBLISHED_

#include <map>
#include <string>

#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/spinbutt.h>
#include <wx/treectrl.h>
#include <string>
#include <vector>
 
#include <remote/PublishedTree.h>

/*!
 *  \file UIRemotePublished.h
 */

namespace cpw 
{ 
	namespace gui
	{
		/*!
		 *  \class UIRemotePublished UIRemotePublished.h "cwcontent/gui/UIRemotePublished.h"
		 *  \ingroup cwcontent
		 *  \brief This is a dialog showing the tree of published entities on the remote host
		 *
		 *  This is a dialog showing the tree of published entities on the remote host.
		 */
		class UIRemotePublished : public wxDialog
		{
			public:
				UIRemotePublished(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("Connection"),
					const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
					long style = wxDEFAULT_DIALOG_STYLE);
				virtual ~UIRemotePublished();

				bool IsBack() {return flag_back;}
				
				void SetEntities(const cpw::remote::PublishedNode *root_node);
				std::vector<cpw::TypeId> GetSelectedEntities();
			
			private:
				void OnButtonAddEntity(wxCommandEvent& WXUNUSED(event));
				void OnButtonRemoveEntity(wxCommandEvent& WXUNUSED(event));
				void OnButtonBack(wxCommandEvent& WXUNUSED(event));
				void OnButtonCancel(wxCommandEvent& WXUNUSED(event));
				void OnButtonFinish(wxCommandEvent& WXUNUSED(event));
				void OnSelectItem(wxTreeEvent& WXUNUSED(event));
				void OnClose(wxCloseEvent& event);

				void FillTree(std::vector<cpw::remote::PublishedNode*> &nodes, wxTreeItemId &parent_id);
				
				void CreateGUIControls();
				void AddIcon(const std::string url, wxTreeItemId id);

				void OnPaint(wxPaintEvent& event);
				void render(wxDC& dc);



				wxStaticText *Layer_tree_StaticText;
				wxButton *Quit_layer_Button;
				wxButton *Add_layer_Button;
				wxTreeCtrl *entities_tree;
				wxButton *Back_button;
				wxStaticText *Layer_composition_StaticText;
				wxListBox *WxListBox1;
				wxButton *Cancel_button;
				wxButton *Finish_button;
					
				wxImageList *img_list;
				bool flag_back;
				
				std::map<std::string, cpw::TypeId> node_id;

				DECLARE_EVENT_TABLE();
		};
	}
}

#endif

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

#ifndef _CONNECTIONTREEPANEL_
#define _CONNECTIONTREEPANEL_

#include <map>
#include <string>
#include <sstream>
#include <wx/wx.h>
#include <wx/menu.h>
#include <wx/treectrl.h>
#include <wx/imaglist.h>
#include <wx/bitmap.h>
#include <wx/minifram.h>
#include <wx/colour.h>
#include <wx/image.h>

#include <cpw/common/Observer.h>
#include <cpw/common/RemoteNode.h>
#include <remote/ConnectionManager.h>
#include <gui/GUItypes.h>

/*!
 *  \file UIConnectionTreePanel.h
 */

namespace cpw 
{ 
	
	namespace gui
	{
		/*!
		 *  \class UIConnectionTreePanel UIConnectionTreePanel.h "cwcontent/gui/UIConnectionTreePanel.h"
		 *  \ingroup cwcontent
		 *  \brief This is a panel showing the connections available
		 *
		 *  This is a panel showing the connections available.
		 */
		class UIConnectionTreePanel : public wxPanel, cpw::Observer
		{
		public:
			UIConnectionTreePanel(cpw::remote::RemoteProtocol *protocol, wxWindow *parent,
					      wxWindowID id, const wxString& name,
					      int xpos, int ypos, int width, int height, wxString &title,
					      long style = wxTAB_TRAVERSAL);
			~UIConnectionTreePanel(void);

			void InitializeTree();

			void Update(bool subject_deleted = false);

			void ExploreNode(const cpw::RemoteNode &node);
			void ReconnectNode(const cpw::RemoteNode &node);
			void DisconnectNode(const cpw::RemoteNode &node);
			void DeleteNode(const cpw::RemoteNode &node);
			void DisconnectEntity(const cpw::RemoteNode &node, const cpw::TypeId &entity_id);


		private:
			void AddIcon(const std::string &primitive_url, wxTreeItemId id);
			void AddIcon(cpw::remote::ConnectionState state, wxTreeItemId id);

			void EvtTreeItemExpanded(wxTreeEvent& wxevent);
			void EvtTreeItemCollapsed(wxTreeEvent& wxevent);
			
			void OnRightClick(wxTreeEvent& exevent);
			void OnMove(wxMoveEvent& event);

			
			void DoLayOut();
			void FillTree(cpw::RemoteNode node, wxTreeItemId parent_id); //show the layer tree on the interface
			cpw::RemoteNode GetItemId(wxTreeItemId item_id);


			wxWindow *parent_window;
			wxTreeCtrl *tree_ctrl;
			wxTreeItemId root_id;
			wxImageList *img_list;
			wxString name_;
			
			cpw::LayerTree *layer_tree;

			std::map<cpw::RemoteNode, wxTreeItemId> connection_tree_relation;
			std::multimap<cpw::TypeId, wxTreeItemId> entity_tree_relation;
			std::map<cpw::RemoteNode, bool> items_expanded;

			wxBitmap frame_shape;

			cpw::remote::ConnectionManager *connection_manager;
			cpw::remote::RemoteProtocol *protocol;
			
			DECLARE_EVENT_TABLE()
		};


		class UIConnectionTreePanelPopUpMenu : public wxMenu
		{
		public:
			UIConnectionTreePanelPopUpMenu(wxWindow* parent, UIConnectionTreePanel *ltp, 
				bool connection, const cpw::RemoteNode &node, cpw::remote::ConnectionState state,
				const cpw::TypeId &entity_id);
			~UIConnectionTreePanelPopUpMenu();
				
		private:
			void PopupAdd(wxCommandEvent& WXUNUSED(event));
			void Explore(wxCommandEvent& WXUNUSED(event));
			void Reconnect(wxCommandEvent& WXUNUSED(event));
			void Disconnect(wxCommandEvent& WXUNUSED(event));
			void Delete(wxCommandEvent& WXUNUSED(event));
			void DisconnectEntity(wxCommandEvent& WXUNUSED(event));

			wxWindow *parent_window;
			UIConnectionTreePanel *connection_tree_panel;
			cpw::RemoteNode node;
			cpw::TypeId entity_id;

			DECLARE_EVENT_TABLE()
		};
	}
}

#endif



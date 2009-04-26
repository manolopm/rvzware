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

#include <algorithm>
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

#include <stdafx.h>

#include <cpw/common/Observer.h>
#include <cpw/common/RemoteNode.h>
#include <cpw/entity/EntityRegistry.h>
#include <remote/Connection.h>
#include <remote/ConnectionManager.h>
#include <remote/GetPublishedRequestData.h>

#include <cpw/ApplicationConfiguration.h>

#include <gui/UIConnectionTreePanel.h>
#include <gui/GUItypes.h>
#include <gui/UIRemoteConnection.h>


#define CONNECTION_TREE_PANEL 1001
#define RECONNECT_MENU_ITEM 2001
#define DISCONNECT_MENU_ITEM 2002
#define DELETE_MENU_ITEM 2003
#define EXPLORE_MENU_ITEM 2004
#define DISCONNECT_ENTITY_MENU_ITEM 2005


using namespace cpw::gui;

BEGIN_EVENT_TABLE(UIConnectionTreePanel, wxPanel)

EVT_TREE_ITEM_EXPANDED(CONNECTION_TREE_PANEL, UIConnectionTreePanel::EvtTreeItemExpanded)
	EVT_TREE_ITEM_COLLAPSED(CONNECTION_TREE_PANEL, UIConnectionTreePanel::EvtTreeItemCollapsed)
	
	EVT_TREE_ITEM_RIGHT_CLICK(CONNECTION_TREE_PANEL, UIConnectionTreePanel::OnRightClick)

END_EVENT_TABLE()


BEGIN_EVENT_TABLE(UIConnectionTreePanelPopUpMenu, wxMenu)

	EVT_MENU	 ( RECONNECT_MENU_ITEM, UIConnectionTreePanelPopUpMenu::Reconnect	)
	EVT_MENU	 ( EXPLORE_MENU_ITEM, UIConnectionTreePanelPopUpMenu::Explore	)
	EVT_MENU	 ( DISCONNECT_MENU_ITEM, UIConnectionTreePanelPopUpMenu::Disconnect	)
	EVT_MENU	 ( DELETE_MENU_ITEM, UIConnectionTreePanelPopUpMenu::Delete	)

	EVT_MENU	 ( DISCONNECT_ENTITY_MENU_ITEM, UIConnectionTreePanelPopUpMenu::DisconnectEntity	)

END_EVENT_TABLE()


/*!
 *  \param connection_manager A pointer to the connection manager
 *  \param parent A pointer to the main panel
 *  \param xpos X component of the position
 *  \param ypos Y component of the position
 *  \param width Width of the widget
 *  \param height Height of the widget
 *  \param title Title of window
 *  \param style Style of panel
 */
UIConnectionTreePanel::UIConnectionTreePanel(cpw::remote::RemoteProtocol *protocol,
	wxWindow *parent, int xpos, int ypos, int width, int height, wxString &title, long style)
	: wxPanel(parent, wxID_ANY), parent_window(parent), connection_manager(protocol->GetConnectionManager()),
	protocol(protocol)
{
	SetMinSize(wxSize(128, 50));
	wxBoxSizer *top_sizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(top_sizer);

	tree_ctrl = new wxTreeCtrl(this, CONNECTION_TREE_PANEL, wxDefaultPosition, wxDefaultSize,
		wxTR_HIDE_ROOT | wxTR_DEFAULT_STYLE | wxTR_HAS_BUTTONS | wxTR_HAS_VARIABLE_ROW_HEIGHT | wxTR_SINGLE);
		//wxTR_FULL_ROW_HIGHLIGHT);
	
	if (!(cpw::ApplicationConfiguration::GetInstance()->IsThemed()))
	{
	  wxColour c_foreg = wxString(cpw::ApplicationConfiguration::GetInstance()->GetPageFontColour().c_str(),wxConvUTF8);
	  wxColour c_backg = wxString(cpw::ApplicationConfiguration::GetInstance()->GetPageColour().c_str(),wxConvUTF8);	
		tree_ctrl->SetBackgroundColour(c_backg);
		tree_ctrl->SetForegroundColour(c_foreg);
	}

	tree_ctrl->SetSize(this->GetSize());
	
	root_id = tree_ctrl->AddRoot(wxT("-Root item (it is hidden)-"));
		
	img_list = new wxImageList(16, 16);
	tree_ctrl->AssignImageList(img_list);
	
	top_sizer->Add(tree_ctrl,1,wxGROW);
}


void UIConnectionTreePanel::DoLayOut()
{
	wxBoxSizer *top_sizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(top_sizer);

	top_sizer->Add(tree_ctrl, 0, wxALIGN_LEFT);
}


UIConnectionTreePanel::~UIConnectionTreePanel(void)
{
	if (connection_manager != NULL)
		connection_manager->Detach(this);
}


void UIConnectionTreePanel::InitializeTree()
{
	//tree_ctrl->DeleteChildren(root_id);
	//img_list->RemoveAll();
	entity_tree_relation.clear();

	std::vector<cpw::RemoteNode> nodes;
	nodes = connection_manager->GetNodes();
	std::vector<cpw::RemoteNode>::iterator it2;

	std::map<cpw::RemoteNode, wxTreeItemId>::iterator it;

	//Connections we already have on the tree, just change the icon
	for (it = connection_tree_relation.begin(); it != connection_tree_relation.end(); it++)
	{
		AddIcon(connection_manager->GetConnectionState(it->first), it->second);

		it2 = find(nodes.begin(), nodes.end(), it->first);

		if (it2 != nodes.end())
			nodes.erase(it2);

		FillTree(it->first, it->second);
	}

	for (it2 = nodes.begin(); it2 != nodes.end(); it2++)
	{
		std::string name;
		std::stringstream ss;
		ss << it2->GetPort();
		name = it2->GetIPAddress() + ":" + ss.str();
		wxTreeItemId id = tree_ctrl->AppendItem(root_id, wxString(name.c_str(),wxConvUTF8));
		AddIcon(connection_manager->GetConnectionState(*it2), id);

		connection_tree_relation[*it2] = id;
	}


	//tree_ctrl->Expand(root_id);
	std::map<cpw::RemoteNode, bool>::iterator i;
		
	for( i = items_expanded.begin(); i!= items_expanded.end(); i++)
	{
		if ((*i).second == true)
		{
			//expand all the items
			std::map<cpw::RemoteNode, wxTreeItemId>::iterator j;

			j = connection_tree_relation.find(i->first);

			if (j != connection_tree_relation.end())
				tree_ctrl->Expand(j->second);
		}
	}
}


void UIConnectionTreePanel::FillTree(cpw::RemoteNode node, wxTreeItemId parent_id)
{
	tree_ctrl->DeleteChildren(parent_id);

	std::vector<cpw::TypeId> entities = connection_manager->GetSubscribed(node);

	std::vector<cpw::TypeId>::iterator it;
	for (it = entities.begin(); it != entities.end(); it++)
	{
		cpw::Entity *entity = cpw::EntityRegistry::GetInstance()->GetEntity(*it);
		if (entity != NULL)
		{
			wxTreeItemId id = tree_ctrl->AppendItem(parent_id, wxString(entity->GetName().c_str(),wxConvUTF8));
			AddIcon(entity->GetIcon(), id);
			entity_tree_relation.insert(std::pair<cpw::TypeId, wxTreeItemId>(entity->GetId(), id));
		}
	}
}


void UIConnectionTreePanel::Update(bool subject_deleted)
{
	if (!subject_deleted)
		InitializeTree();
	else
		connection_manager = NULL;
	//paren->Update();
	wxPanel::Update();
}


void UIConnectionTreePanel::ReconnectNode(const cpw::RemoteNode &node)
{
	if ((connection_manager->GetConnectionState(node) != cpw::remote::cstConnected) &&
		(connection_manager->GetConnectionState(node) != cpw::remote::cstConnecting))
	{
		UIRemoteConnection ui_connect(parent_window);
		ui_connect.SetHostname(node.GetIPAddress());
		ui_connect.SetPort(node.GetPort());

		if (ui_connect.ShowModal() == wxID_OK)
		{
			DeleteNode(node);

			cpw::remote::Connection *connection =
				connection_manager->Connect(cpw::RemoteNode(ui_connect.GetHostname(), ui_connect.GetPort()));

			if (connection == NULL)
				wxMessageBox(_("Invalid address"), _("Information"), wxICON_INFORMATION);
		}
	}
}


void UIConnectionTreePanel::ExploreNode(const cpw::RemoteNode &node)
{
	cpw::remote::Connection *connection = connection_manager->GetConnection(node);
	cpw::remote::GetPublishedRequestData *message = new cpw::remote::GetPublishedRequestData();

	connection->Send(message);

	delete message;
}


void UIConnectionTreePanel::DisconnectNode(const cpw::RemoteNode &node)
{
	cpw::remote::Connection *connection = connection_manager->GetConnection(node);
	if (connection != NULL)
		connection->Disconnect();
}


void UIConnectionTreePanel::DeleteNode(const cpw::RemoteNode &node)
{
	DisconnectNode(node);
	if (connection_tree_relation.find(node) != connection_tree_relation.end())
	{
		tree_ctrl->Delete(connection_tree_relation[node]);
		connection_tree_relation.erase(connection_tree_relation.find(node));
	}
}


void UIConnectionTreePanel::DisconnectEntity(const cpw::RemoteNode &node, const cpw::TypeId &entity_id)
{
	cpw::remote::Connection *connection = connection_manager->GetConnection(node);
	cpw::remote::DisconnectEntityData *message = new cpw::remote::DisconnectEntityData();
	message->SetEntityId(entity_id);

	connection->Send(message);

	delete message;

	connection_manager->Unsubscribe(connection, (cpw::TypeId &) entity_id);

	protocol->CleanUpModifiers();
}


/*!
 *  \fn void UIConnectionTreePanel::EvtTreeItemExpanded(wxTreeEvent& wxevent)
 *  \brief 
 */
void UIConnectionTreePanel::EvtTreeItemExpanded(wxTreeEvent& wxevent)
{
	wxTreeItemId expanded_item_id = wxevent.GetItem();
	
	if (expanded_item_id.IsOk())
	{
		//Search for the URL of this item
		cpw::RemoteNode node = GetItemId(expanded_item_id);

		if (node != cpw::RemoteNode())
			items_expanded[node] = true;
	}
}


void UIConnectionTreePanel::EvtTreeItemCollapsed(wxTreeEvent& wxevent) 
{
	wxTreeItemId collapsed_item_id = wxevent.GetItem();
	
	if (collapsed_item_id.IsOk())
	{
		//Search for the URL of this item
		cpw::RemoteNode node = GetItemId(collapsed_item_id);

	if (node != cpw::RemoteNode())
		items_expanded[node] = false;
	}
}


cpw::RemoteNode UIConnectionTreePanel::GetItemId(wxTreeItemId item_id)
{
	//Search for the URL of this item
	std::map<cpw::RemoteNode, wxTreeItemId>::iterator i;
	cpw::RemoteNode node = cpw::RemoteNode();

	if (item_id.IsOk())
	{
		//search for the tree item with the wanted ID
		for( i = connection_tree_relation.begin(); i!= connection_tree_relation.end(); i++)
		{
			if ((*i).second == item_id)
			{
				node = (*i).first;
				return node;
			}
		}
	}

	return node;
}


void UIConnectionTreePanel::AddIcon(const std::string &icon_filename, wxTreeItemId id)
{
	//add the icon
		//cpw::PrimitiveRegistry::GetInstance()->GetPrimitiveFromUrl(primitive_url)->GetIcon();
	if (id.IsOk())
	{
		wxIcon icon(wxString(icon_filename.c_str(),wxConvUTF8), wxBITMAP_TYPE_ANY);
		int img_index = img_list->Add(icon);
		tree_ctrl->SetItemImage(id, img_index);
	}

}


void UIConnectionTreePanel::AddIcon(cpw::remote::ConnectionState state, wxTreeItemId id)
{
	
	std::string file_name = cpw::ApplicationConfiguration::GetInstance()->GetUIIconDirectory();
	switch(state)
	{
	case cpw::remote::cstConnecting:
		AddIcon(file_name+"connecting.png", id);
		break;
	case cpw::remote::cstConnected:
		AddIcon(file_name+"connected.png", id);
		break;
	case cpw::remote::cstDisconnecting:
	case cpw::remote::cstDisconnected:
		AddIcon(file_name+"disconnected.png", id);
		break;
	case cpw::remote::cstUnknown:
	default:
		AddIcon(file_name+"unknown.png", id);
		break;
	}
}


void UIConnectionTreePanel::OnRightClick(wxTreeEvent& exevent)
{
	//UIConnectionTreePanelPopUpMenu pop_up_menu(this);// = new UIConnectionTreePanelPopUpMenu();
	//PopupMenu(&pop_up_menu);

	wxTreeItemId item_selected = exevent.GetItem();
	tree_ctrl->SelectItem(item_selected, true);

	if (item_selected.IsOk())
	{
		bool isconnection;
		cpw::TypeId entity_id;
		cpw::RemoteNode node;

		if (tree_ctrl->GetItemParent(item_selected) != root_id)
		{
			isconnection = false;

			std::multimap<cpw::TypeId, wxTreeItemId>::iterator it;
			for (it = entity_tree_relation.begin(); it != entity_tree_relation.end(); it++)
			{
				if (it->second == item_selected)
				{
					entity_id = it->first;
					item_selected = tree_ctrl->GetItemParent(it->second);
					break;
				}
			}
		}
		else
			isconnection = true;
			
		std::map<cpw::RemoteNode, wxTreeItemId>::iterator it;
		for (it = connection_tree_relation.begin(); it != connection_tree_relation.end(); it++)
		{
			if (it->second == item_selected)
			{
				node = it->first;
				break;
			}
		}

		UIConnectionTreePanelPopUpMenu pop_up_menu(parent_window, this, isconnection, node,
			connection_manager->GetConnectionState(node), entity_id);
		PopupMenu(&pop_up_menu);
	}
	//exevent.StopPropagation();
}


void UIConnectionTreePanel::OnMove(wxMoveEvent& event)
{
//	((cpw::gui::UIApplicationMainFrame *)parent_window)->RePaint();	
}



///////////////////////////////////////////////////////////
///////////////////POPUP-MENU//////////////////////////////
///////////////////////////////////////////////////////////



UIConnectionTreePanelPopUpMenu::UIConnectionTreePanelPopUpMenu(wxWindow* parent, UIConnectionTreePanel *ltp,
	bool isconnection, const cpw::RemoteNode &node, cpw::remote::ConnectionState state, const cpw::TypeId &entity_id)
	: wxMenu(), parent_window(parent), connection_tree_panel(ltp), node(node), entity_id(entity_id)
{
	if (isconnection)
	{
		//show menu for connections
		if (state == cpw::remote::cstConnected)
		{
			Append(EXPLORE_MENU_ITEM, _("Explore..."));
			Append(DISCONNECT_MENU_ITEM, _("Disconnect"));
		}
		else if (state == cpw::remote::cstDisconnected)
		{
			Append(RECONNECT_MENU_ITEM, _("Connect again..."));
			Append(DELETE_MENU_ITEM, _("Delete"));
		}
		else
		{
		}
	}
	else
	{
		//show menu for entities
		Append(DISCONNECT_ENTITY_MENU_ITEM, _("Disconnect"));
	}
}


UIConnectionTreePanelPopUpMenu::~UIConnectionTreePanelPopUpMenu()
{
}


void UIConnectionTreePanelPopUpMenu::Explore(wxCommandEvent& WXUNUSED(event))
{
	connection_tree_panel->ExploreNode(node);
}


void UIConnectionTreePanelPopUpMenu::Reconnect(wxCommandEvent& WXUNUSED(event))
{
	connection_tree_panel->ReconnectNode(node);
}


void UIConnectionTreePanelPopUpMenu::Disconnect(wxCommandEvent& WXUNUSED(event))
{
	connection_tree_panel->DisconnectNode(node);
}


void UIConnectionTreePanelPopUpMenu::Delete(wxCommandEvent& WXUNUSED(event))
{
	connection_tree_panel->DeleteNode(node);
}


void UIConnectionTreePanelPopUpMenu::DisconnectEntity(wxCommandEvent& WXUNUSED(event))
{
	connection_tree_panel->DisconnectEntity(node, entity_id);
}

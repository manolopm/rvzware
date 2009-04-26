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
#include <string>
#include <wx/wx.h>
#include <wx/imaglist.h>

#include <stdafx.h>

#include <gui/UIRemotePublished.h>

#include <remote/PublishedTree.h>
#include <cpw/ApplicationConfiguration.h>


using namespace cpw::gui;


enum
{
	ID_FINISH_BUTTON = 1018,
	ID_REMOVEENTITY_BUTTON = 1016,
	ID_ADDENTITY_BUTTON = 1015,
	ID_BACK_BUTTON = 1009,
	ID_WXLISTBOX1 = 1006,
	ID_CANCEL_BUTTON = 1002,
	ID_TREE_CTRL = 1017, 
	ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
};


BEGIN_EVENT_TABLE(UIRemotePublished,wxDialog)
	EVT_CLOSE     (UIRemotePublished::OnClose)
	EVT_BUTTON    (ID_CANCEL_BUTTON,    UIRemotePublished::OnButtonCancel)
	EVT_BUTTON    (ID_BACK_BUTTON,      UIRemotePublished::OnButtonBack)
	EVT_BUTTON    (ID_FINISH_BUTTON,    UIRemotePublished::OnButtonFinish)
	EVT_BUTTON    (ID_ADDENTITY_BUTTON,  UIRemotePublished::OnButtonAddEntity)
	EVT_BUTTON    (ID_REMOVEENTITY_BUTTON,  UIRemotePublished::OnButtonRemoveEntity)
	EVT_PAINT	    (				 UIRemotePublished::OnPaint )
	EVT_TREE_ITEM_ACTIVATED (ID_TREE_CTRL, UIRemotePublished::OnSelectItem )
END_EVENT_TABLE()


/*!
 *  \param parent A pointer to the main panel
 *  \param id Id of the window
 *  \param title Title of window
 *  \param position Position of the window
 *  \param size Width of the widget
 *  \param style Style of panel
 */
UIRemotePublished::UIRemotePublished(wxWindow *parent, wxWindowID id, const wxString &title,
	const wxPoint &position, const wxSize& size, long style)
	: wxDialog(parent, id, title, position, size, style)
{
	flag_back = false;
	CreateGUIControls();
}

UIRemotePublished::~UIRemotePublished()
{
} 

void UIRemotePublished::CreateGUIControls()
{
	SetTitle(wxT("Published Entities"));
	SetIcon(wxNullIcon);
	SetSize(8,8,390,600);
	Center();

	//Layer_tree_StaticText = new wxStaticText(this, wxID_ANY, wxT("Entities tree:"), wxPoint(20,10), wxDefaultSize, 0, wxT("Layer_tree_StaticText"));
	//Layer_tree_StaticText->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
	
	entities_tree = new wxTreeCtrl(this, ID_TREE_CTRL, wxPoint(20,25), wxSize(314,338), wxTR_HIDE_ROOT|wxTR_DEFAULT_STYLE);
	entities_tree->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
	img_list = new wxImageList(16, 16);
	entities_tree->AssignImageList(img_list);

	Add_layer_Button = new wxButton(this, ID_ADDENTITY_BUTTON, wxT("+"), wxPoint(340,342), wxSize(22,22), 0, wxDefaultValidator, wxT("Add_Layer_Button"));
	Add_layer_Button->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	//Layer_composition_StaticText = new wxStaticText(this, wxID_ANY, wxT("Added entities:"), wxPoint(20,170/*218*/), wxDefaultSize, 0, wxT("Layer_composition_StaticText"));
	//Layer_composition_StaticText->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
		
	wxArrayString arrayStringFor_WxListBox1;
	WxListBox1 = new wxListBox(this, ID_WXLISTBOX1, wxPoint(20,385), wxSize(314,132), arrayStringFor_WxListBox1, wxLB_SINGLE);
	WxListBox1->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	Quit_layer_Button = new wxButton(this, ID_REMOVEENTITY_BUTTON, wxT("-"), wxPoint(340,385), wxSize(22,22), 0, wxDefaultValidator, wxT("Quit_Layer_Button"));
	Quit_layer_Button->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));


	Back_button = new wxButton(this, ID_BACK_BUTTON, wxT("< Back"), wxPoint(128,530), wxSize(75,23), 0, wxDefaultValidator, wxT("back_button"));
	Back_button->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	Finish_button = new wxButton(this, ID_FINISH_BUTTON, wxT("Finish"), wxPoint(203,530), wxSize(75,23), 0, wxDefaultValidator, wxT("Finish_button"));
	Finish_button->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	Cancel_button = new wxButton(this, ID_CANCEL_BUTTON, wxT("Cancel"), wxPoint(287,530), wxSize(75,23), 0, wxDefaultValidator, wxT("Cancel_button"));
	Cancel_button->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

}

void UIRemotePublished::OnClose(wxCloseEvent& /*event*/)
{
	flag_back = false;		
	this->EndModal(wxID_CANCEL);
	Destroy();
}

void UIRemotePublished::OnButtonCancel(wxCommandEvent& WXUNUSED(event))
{
	this->EndModal(wxID_CANCEL);	
	flag_back = false;		
}

void UIRemotePublished::OnButtonBack(wxCommandEvent& WXUNUSED(event))
{
	entities_tree->DeleteAllItems();
	WxListBox1->Clear();
	this->EndModal(wxID_OK);
	flag_back = true;
}

void UIRemotePublished::OnButtonFinish(wxCommandEvent& WXUNUSED(event))
{
	if (WxListBox1->GetCount() == 0)
	{
		wxMessageDialog message(this, _("You have to select entities to continue. Add some entities to download or click cancel."), _("Warning"),wxICON_EXCLAMATION |wxOK);
		message.ShowModal();
	}
	else
		this->EndModal(wxID_OK);
	flag_back = false;
}

void UIRemotePublished::FillTree(std::vector<cpw::remote::PublishedNode*> &nodes, wxTreeItemId &parent_id)
{
	wxTreeItemId id;
   
	for(std::vector<cpw::remote::PublishedNode*>::iterator i=nodes.begin();i!=nodes.end();i++)
	{
		id = entities_tree->AppendItem(parent_id,wxString::From8BitData((*i)->GetName().c_str()));
		if(!(*i)->IsLeaf())	
		{
			node_id[(*i)->GetName()] = (*i)->GetId();
			FillTree(((cpw::remote::PublishedFolder*) *i)->GetNodes(),id);
			std::string &icon_path = ApplicationConfiguration::GetInstance()->GetIconDirectory();
			AddIcon(icon_path + "/folder.png", id);
			entities_tree->Expand(id);
		}
		else
		{
			node_id[(*i)->GetName()] = (*i)->GetId();
			std::string &icon_path = ApplicationConfiguration::GetInstance()->GetIconDirectory();
			AddIcon(icon_path + "/bullet_black.png", id);
		}
	}	
}


void UIRemotePublished::SetEntities(const cpw::remote::PublishedNode *root_node)
{	
	wxTreeItemId root_id = entities_tree->AddRoot(wxString::From8BitData(root_node->GetName().c_str()));
	FillTree(((cpw::remote::PublishedFolder*) root_node)->GetNodes(), root_id);
	std::string &icon_path = ApplicationConfiguration::GetInstance()->GetIconDirectory();
	AddIcon(icon_path + "/folder.png", root_id);
	if (entities_tree->GetChildrenCount(root_id)>0)
	{
		wxTreeItemIdValue child;
		entities_tree->SelectItem(entities_tree->GetFirstChild(root_id, child), true);
	}
}

void UIRemotePublished::OnButtonAddEntity(wxCommandEvent& WXUNUSED(event))
{
	wxTreeItemId TreeElementId = entities_tree->GetSelection();
	wxTreeItemId root_node = entities_tree->GetRootItem();
	if (TreeElementId != root_node)
	{
		wxString TreeElementString = entities_tree->GetItemText(TreeElementId);
		if (WxListBox1->FindString(TreeElementString) == wxNOT_FOUND)
		{
			wxTreeItemId TreeElementId_aux = TreeElementId;
			while (TreeElementId_aux != root_node)
			{
				if (WxListBox1->FindString(TreeElementString) != wxNOT_FOUND)
					return;
				TreeElementId_aux = entities_tree->GetItemParent(TreeElementId_aux);
				if (TreeElementId_aux != root_node)
					TreeElementString = entities_tree->GetItemText(TreeElementId_aux);
			}
			WxListBox1->AppendAndEnsureVisible(entities_tree->GetItemText(TreeElementId));
		}
	}
}

void UIRemotePublished::OnSelectItem(wxTreeEvent& WXUNUSED(event))
{
	wxTreeItemId TreeElementId = entities_tree->GetSelection();
	wxTreeItemId root_node = entities_tree->GetRootItem();
	if (TreeElementId != root_node)
	{
		wxString TreeElementString = entities_tree->GetItemText(TreeElementId);
		if (WxListBox1->FindString(TreeElementString) == wxNOT_FOUND)
		{
			wxTreeItemId TreeElementId_aux = TreeElementId;
			while (TreeElementId_aux != root_node)
			{
				if (WxListBox1->FindString(TreeElementString) != wxNOT_FOUND)
					return;
				TreeElementId_aux = entities_tree->GetItemParent(TreeElementId_aux);
				if (TreeElementId_aux != root_node)
					TreeElementString = entities_tree->GetItemText(TreeElementId_aux);
			}
			WxListBox1->AppendAndEnsureVisible(entities_tree->GetItemText(TreeElementId));
		}
	}
}

void UIRemotePublished::OnButtonRemoveEntity(wxCommandEvent& WXUNUSED(event))
{
	int selectionId = WxListBox1->GetSelection();
	if (selectionId != wxNOT_FOUND)
		WxListBox1->Delete(selectionId);
}

std::vector<cpw::TypeId> UIRemotePublished::GetSelectedEntities()
{	
	std::vector<cpw::TypeId> v;
	for (unsigned int i=0; i<WxListBox1->GetCount(); i++)
	{
	  v.push_back(node_id[std::string(WxListBox1->GetString(i).mb_str())]);
	}
	return v;
}

void UIRemotePublished::AddIcon(const std::string url, wxTreeItemId id)
{
	wxBitmap icon(wxString::From8BitData(url.c_str()), wxBITMAP_TYPE_ANY);
 	int img_index = img_list->Add(icon);
	entities_tree->SetItemImage(id, img_index);
}


void UIRemotePublished::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	render(dc);
}


void UIRemotePublished::render(wxDC& dc)
{
	int client_h = 0;
	int client_w = 0;
	GetClientSize (&client_w, &client_h);

	if (!(cpw::ApplicationConfiguration::GetInstance()->IsThemed()))
	{
	  wxColour c_pen   = wxString(cpw::ApplicationConfiguration::GetInstance()->GetBackgroundGradient2Colour().c_str(),wxConvUTF8);
	  wxColour c_backg = wxString(cpw::ApplicationConfiguration::GetInstance()->GetBackgroundGradient1Colour().c_str(),wxConvUTF8);	
	  wxColour c_brush = wxString(cpw::ApplicationConfiguration::GetInstance()->GetBackgroundColour().c_str(),wxConvUTF8);
	  dc.SetTextForeground(wxColour(wxString(cpw::ApplicationConfiguration::GetInstance()->GetFontLightColour().c_str(),wxConvUTF8)));
		dc.SetPen(wxPen(c_pen));
		dc.SetBrush(wxBrush(c_brush));
		dc.GradientFillLinear( wxRect(0,0,client_w,client_h), c_backg, c_pen, wxSOUTH);
	}

	dc.DrawRotatedText(_T("Entities tree:"),20,10, 0);
	dc.DrawRotatedText(_T("Added entities:"),20,370, 0);
}

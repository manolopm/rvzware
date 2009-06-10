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

#include <stdafx.h>

#include <gui/UIAnimableList.h>

#include <cpw/ApplicationConfiguration.h>

using namespace cpw::gui;


#define IM_TREE_ROOT 0
#define IM_TREE_SELECTED 1
#define IM_TREE_DESELECTED 2


BEGIN_EVENT_TABLE(UIAnimableList,wxDialog)
	EVT_BUTTON    (ID_DESELECTALL_BUTTON,      UIAnimableList::OnButtonDeselectAll)
	EVT_BUTTON    (ID_SELECTALL_BUTTON,      UIAnimableList::OnButtonSelectAll)
	EVT_BUTTON    (ID_CANCEL_BUTTON,      UIAnimableList::OnButtonCancel)
	EVT_BUTTON    (ID_FINISH_BUTTON,      UIAnimableList::OnButtonFinish)
	EVT_CLOSE     (UIAnimableList::OnClose)
	EVT_MOVE      (UIAnimableList::OnMove)
	EVT_PAINT	    (				  UIAnimableList::OnPaint )
END_EVENT_TABLE()

UIAnimableList::UIAnimableList(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style), parent_(parent), treeitem_entity_map()
{
	if (!(ApplicationConfiguration::GetInstance()->IsThemed()))
	{
		SetFont(parent_->GetFont());
		SetForegroundColour(parent_->GetForegroundColour());
		SetBackgroundColour(parent_->GetBackgroundColour());
	}	

	CreateGUIControls();
}

UIAnimableList::~UIAnimableList()
{
} 

void UIAnimableList::CreateGUIControls()
{
	SetTitle(wxT("Animable"));
	SetIcon(wxNullIcon);
	SetSize(8,8,390,360);
	Center();

	std::string &icon_path = ApplicationConfiguration::GetInstance()->GetUIIconDirectory();

	selectAll_button = new wxButton(this, ID_SELECTALL_BUTTON, wxT("Select All"), wxPoint(25,294), wxSize(75,23), 0, wxDefaultValidator, wxT("Load_fire_texture_button"));
	selectAll_button->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	deselectAll_button = new wxButton(this, ID_DESELECTALL_BUTTON, wxT("Deselect All"), wxPoint(109,294), wxSize(75,23), 0, wxDefaultValidator, wxT("Load_fire_texture_button"));
	deselectAll_button->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	//AnimableTree
	img_list = new wxImageList(16, 16);
	img_list->Add(wxBitmap(wxImage(wxString((icon_path + "folder.png").c_str(),wxConvUTF8))));
	img_list->Add(wxBitmap(wxImage(wxString((icon_path + "selected.png").c_str(),wxConvUTF8))));
                                      
	animableTree = new wxTreeListCtrl(this, ID_ANIMABLE_TREE, wxPoint(20,20), wxSize(345,265), wxTR_HAS_BUTTONS | wxTR_HAS_VARIABLE_ROW_HEIGHT | wxTR_FULL_ROW_HIGHLIGHT | wxTR_EDIT_LABELS| wxTR_HIDE_ROOT);//wxTR_SINGLE| wxTR_HAS_BUTTONS /**/ );
	animableTree->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxNORMAL, false, wxT("Tahoma")));
	animableTree->AssignImageList(img_list);
	animableTree->AddColumn(wxT("Animable List"),345 ,wxLIST_FORMAT_LEFT);
	
	if (!(ApplicationConfiguration::GetInstance()->IsThemed()))
	{
	  animableTree->SetBackgroundColour(wxColour(wxString(cpw::ApplicationConfiguration::GetInstance()->GetPageColour().c_str(),wxConvUTF8)));
	  animableTree->SetForegroundColour(wxColour(wxString(cpw::ApplicationConfiguration::GetInstance()->GetPageFontColour().c_str(),wxConvUTF8)));
	}
	
	wxTreeItemId t_root  = animableTree->AddRoot(wxT("Top Layer"));
	animableTree->SetItemImage(t_root, IM_TREE_ROOT);
	animableTree->SetItemType(t_root,0,wxCheckboxItemType);

	animableTree->Expand(t_root);
	animableTree->Refresh();
	

	Finish_button = new wxButton(this, ID_FINISH_BUTTON, wxT("Finish"), wxPoint(203,294), wxSize(75,23), 0, wxDefaultValidator, wxT("Finish_button"));
	Finish_button->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	Cancel_button = new wxButton(this, ID_CANCEL_BUTTON, wxT("Cancel"), wxPoint(287,294), wxSize(75,23), 0, wxDefaultValidator, wxT("Cancel_button"));
	Cancel_button->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	this->Connect(ID_ANIMABLE_TREE, MyEVT_CHECKBOXCHANGE, (wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction) &UIAnimableList::OnCheckBoxChange);
}

void UIAnimableList::OnClose(wxCloseEvent& /*event*/)
{
	Destroy();
}

void UIAnimableList::OnButtonCancel(wxCommandEvent& WXUNUSED(event))
{
	EndDialog(wxID_CANCEL);	
}


void UIAnimableList::OnButtonFinish(wxCommandEvent& WXUNUSED(event))
{
	std::map<cpw::Entity*, wxTreeItemId>::iterator iter_map = treeitem_entity_map.begin();
	for(; iter_map != treeitem_entity_map.end(); iter_map++)
	{
		if(animableTree->GetItemChecked((*iter_map).second,0))
			(*iter_map).first->SetAnimate(true);
		else
			(*iter_map).first->SetAnimate(false);
	}
	EndDialog(wxID_OK);	
}


void UIAnimableList::OnMove(wxMoveEvent& event)
{

}

void UIAnimableList::ToggleSelection(wxTreeEvent& event)
{

}

void UIAnimableList::Clear()
{

}

void UIAnimableList::AddIcon(const std::string &icon_filename, wxTreeItemId id)
{
	wxIcon icon(wxString(icon_filename.c_str(),wxConvUTF8), wxBITMAP_TYPE_ANY);
	int img_index = img_list->Add(icon);
	animableTree->SetItemImage(id, img_index);
}

void UIAnimableList::Append(const std::vector<cpw::Entity*> &animated_entities)
{
	std::vector<cpw::Entity*>::const_iterator iter = animated_entities.begin();
	wxTreeItemId t_root = animableTree->GetRootItem();
	wxTreeItemId t_id1;
	for(;iter != animated_entities.end(); iter++)
	{
	  t_id1 = animableTree->AppendItem(t_root,wxString((*iter)->GetName().c_str(),wxConvUTF8));
		AddIcon((*iter)->GetIcon(), t_id1);
		animableTree->SetItemType(t_id1,0,wxCheckboxItemType);
		if ((*iter)->isAnimate())
			animableTree->SetItemChecked(t_id1,0);

		treeitem_entity_map[(*iter)] = t_id1;

	}

	animableTree->Expand(t_root);
	animableTree->Refresh();

}

void UIAnimableList::Quit(const wxTreeItemId& id)
{
}

void UIAnimableList::OnButtonSelectAll(wxCommandEvent& WXUNUSED(event))
{
	wxTreeItemId t_root  = animableTree->GetRootItem();
	SetRecursively(t_root,true);
}

void UIAnimableList::SetRecursively(const wxTreeItemId& id, bool value)
{
	if(id.IsOk())
	{
		wxTreeListItemType item_type = animableTree->GetItemType(id, 0); 
		if (item_type == wxCheckboxItemType) 
			animableTree->SetItemChecked(id,0,value);
		if (animableTree->HasChildren(id))
		{
			wxTreeItemIdValue cookie;
			wxTreeItemId child = animableTree->GetFirstChild(id, cookie);
			if (child.IsOk()) 
			{
				SetRecursively(child,value);
				for (int i = 1; i < (signed)animableTree->GetChildrenCount(id); i++)
				{		
					if(child.IsOk()) child = animableTree->GetNextSibling(child);
					if(child.IsOk()) SetRecursively(child,value);
				}
			}
		}
	}
}

void UIAnimableList::OnButtonDeselectAll(wxCommandEvent& WXUNUSED(event))
{
	wxTreeItemId t_root  = animableTree->GetRootItem();
	SetRecursively(t_root,false);
}

void UIAnimableList::OnCheck(wxTreeEvent& WXUNUSED(event))
{
}

void UIAnimableList::OnCheckBoxChange(wxCommandEvent& event)
{
}

void UIAnimableList::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	render(dc);
}


void UIAnimableList::render(wxDC& dc)
{
	int client_h = 0;
	int client_w = 0;
	GetClientSize (&client_w, &client_h);

	if (!(ApplicationConfiguration::GetInstance()->IsThemed()))
	{
	  wxColour c_pen   = wxString(cpw::ApplicationConfiguration::GetInstance()->GetBackgroundGradient2Colour().c_str(),wxConvUTF8);
	  wxColour c_backg = wxString(cpw::ApplicationConfiguration::GetInstance()->GetBackgroundGradient1Colour().c_str(),wxConvUTF8);	
	  wxColour c_brush = wxString(ApplicationConfiguration::GetInstance()->GetBackgroundColour().c_str(),wxConvUTF8);
	  dc.SetTextForeground(wxColour(wxString(cpw::ApplicationConfiguration::GetInstance()->GetFontLightColour().c_str(),wxConvUTF8)));
		dc.SetPen(wxPen(c_pen));
		dc.SetBrush(wxBrush(c_brush));
		dc.GradientFillLinear( wxRect(0,0,client_w,client_h), c_backg, c_pen, wxSOUTH);
	}

}

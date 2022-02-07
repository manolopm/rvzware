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

#include <gui/UILayerTreePanel.h>

#include <UIApplicationMainFrame.h>
#include <cpw/ApplicationConfiguration.h>

#include <cpw/entity/ContainerLayer.h>
#include <cpw/entity/Entity.h>
#include <cpw/graphic/ApplicationScene.h>
#include <cpw/common/ApplicationLog.h>

#include <cpwutils/Fire.h>



#define LAYER_TREE_PANEL 1001
#define WX_GRID 1002

using namespace cpw::gui;

BEGIN_EVENT_TABLE(UILayerTreePanel, wxPanel)
EVT_MENU	 ( VISUALIZE, UILayerTreePanel::VisualizeMenu )
EVT_MENU	 ( ANIMATE, UILayerTreePanel::AnimateMenu )
EVT_MENU	 ( PUBLISH, UILayerTreePanel::PublishMenu )
EVT_MENU	 ( SENDFRONT, UILayerTreePanel::SendFront )
EVT_MENU	 ( SENDBACK, UILayerTreePanel::SendBack )
EVT_MENU	 ( EDIT_COPY, UILayerTreePanel::Copy )
EVT_MENU	 ( EDIT_DUPLICATE, UILayerTreePanel::Duplicate )
EVT_MENU	 ( EDIT_CUT, UILayerTreePanel::Cut )
EVT_MENU	 ( EDIT_PASTE, UILayerTreePanel::Paste )
EVT_MENU	 ( EDIT_DELETE, UILayerTreePanel::Delete )
EVT_MENU	 ( EDIT_MODIFY_PROPERTIES, UILayerTreePanel::ModifyProperties )
EVT_MENU	 ( GOTO, UILayerTreePanel::GoTo )

EVT_TREE_SEL_CHANGED(LAYER_TREE_PANEL, UILayerTreePanel::EvtTreeItemSelChanged)
EVT_TREE_ITEM_EXPANDED(LAYER_TREE_PANEL, UILayerTreePanel::EvtTreeItemExpanded)
EVT_TREE_ITEM_COLLAPSED(LAYER_TREE_PANEL, UILayerTreePanel::EvtTreeItemCollapsed)
	
EVT_TREE_ITEM_RIGHT_CLICK(LAYER_TREE_PANEL, UILayerTreePanel::OnRightClick)
EVT_TREE_BEGIN_DRAG(LAYER_TREE_PANEL, UILayerTreePanel::OnStartDragAndDrop)
EVT_TREE_END_DRAG(LAYER_TREE_PANEL, UILayerTreePanel::OnEndDragAndDrop)
EVT_TREE_KEY_DOWN(LAYER_TREE_PANEL, UILayerTreePanel::OnTreeKeyDown)

EVT_MOUSE_CAPTURE_LOST(UILayerTreePanel::OnMouseCaptureLost)
EVT_MOTION(UILayerTreePanel::OnMouseMotion)
END_EVENT_TABLE()


UILayerTreePanel::UILayerTreePanel(UIApplicationMainFrame* main_frame,
				   wxWindowID id,
				   const wxString& name,
				   cpw::INavigatorManager *navigator_manager,
				   cpw::LayerTree &_layer_tree,
				   wxWindow *parent,
				   int xpos, int ypos, int width, int height,
				   wxString &title, long style)
: wxPanel(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, name),
  mainframe(main_frame),nm(navigator_manager),
  layer_tree(&_layer_tree), paren(parent), dragging(false), escape(false),
  cut(false), copy(false), duplicate(false)
{
  entity_properties_grid = NULL;
  SetMinSize(wxSize(128, 50));
  wxBoxSizer *top_sizer = new wxBoxSizer(wxVERTICAL);
  SetSizer(top_sizer);

  tree_ctrl = new wxTreeListCtrl(this, LAYER_TREE_PANEL, wxDefaultPosition, wxDefaultSize, wxTR_HAS_BUTTONS | wxTR_HAS_VARIABLE_ROW_HEIGHT | wxTR_FULL_ROW_HIGHLIGHT | wxTR_EDIT_LABELS);
  
  icondefault = new wxIcon(wxString((cpw::ApplicationConfiguration::GetInstance()->GetUIIconDirectory()+"default.png").c_str(),wxConvUTF8), wxBITMAP_TYPE_ANY);
	
  if (!(cpw::ApplicationConfiguration::GetInstance()->IsThemed()))
    {
      wxColour c_foreg = wxString(cpw::ApplicationConfiguration::GetInstance()->GetPageFontColour().c_str(),wxConvUTF8);
      wxColour c_backg = wxString(cpw::ApplicationConfiguration::GetInstance()->GetPageColour().c_str(),wxConvUTF8);	
      tree_ctrl->SetBackgroundColour(c_backg);
      tree_ctrl->SetForegroundColour(c_foreg);
    }
  tree_ctrl->AppendColumn(wxT("Layers"),600 ,wxALIGN_LEFT);
  tree_ctrl->SetSize(this->GetSize());
  //  this->Connect(LAYER_TREE_PANEL, MyEVT_CHECKBOXCHANGE, (wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction) &UILayerTreePanel::OnCheckBoxEvent);

  //	root_id = tree_ctrl->AddRoot(wxT("root node"));
  last_selected = cpw::TypeId();
  img_list = new wxImageList(16, 16);
  tree_ctrl->AssignImageList(img_list);
  top_sizer->Add(tree_ctrl,1,wxGROW);
}

void UILayerTreePanel::DoLayOut()
{
  wxBoxSizer *top_sizer = new wxBoxSizer(wxVERTICAL);
  SetSizer(top_sizer);
  top_sizer->Add(tree_ctrl, 0, wxALIGN_LEFT);
}

UILayerTreePanel::~UILayerTreePanel(void)
{
  layer_tree->Detach(this);
  if (icondefault)
    {
      delete icondefault;
      icondefault = NULL;
    }
}

void UILayerTreePanel::Clear()
{
  if (img_list)
    img_list->RemoveAll();
  entity_tree_relation.clear();
  //tree_ctrl->DeleteChildren(root_id);
  if (tree_ctrl)
    tree_ctrl->DeleteAllItems();
  CleanEntityPropertiesGrid();
}

void UILayerTreePanel::FillTree()
{
  Clear();
  if (layer_tree->GetTopLayer() == NULL)
      return;
	
  if(!tree_ctrl->GetRootItem().IsOk())
          tree_ctrl->AppendItem(tree_ctrl->GetRootItem(),wxT("root node"));

  root_id = tree_ctrl->GetRootItem();

	
  tree_ctrl->SetItemText(root_id, wxString(layer_tree->GetTopLayer()->GetName().c_str(),wxConvUTF8));
  entity_tree_relation.insert(std::make_pair(layer_tree->GetTopLayer()->GetId(), root_id));

  //  AddIcon(layer_tree->GetTopLayer()->GetIcon(), root_id); //MPM
  //  tree_ctrl->SetItemType(root_id,0,wxCheckboxItemType); MPM
  //  tree_ctrl->SetItemChecked(root_id,0,layer_tree->GetTopLayer()->isVisible()); MPM

  for (int i = 0; i < layer_tree->GetTopLayer()->GetNumChildren(); i++)
    FillTree(layer_tree->GetTopLayer()->GetChild(i), root_id);

  tree_ctrl->Expand(root_id);
  std::map<cpw::TypeId, bool>::iterator i;
		
  for( i = items_expanded.begin(); i!= items_expanded.end(); i++)
    {
      if ((*i).second == true)
	{
	  //expand all the items
	  std::multimap<cpw::TypeId, wxTreeListItem>::iterator j;
	  j = entity_tree_relation.find((*i).first);
	  if (j != entity_tree_relation.end())
	    {
	      std::multimap<cpw::TypeId, wxTreeListItem>::iterator last_element;
	      last_element = entity_tree_relation.upper_bound((*i).first);

              //	      for (; j != last_element; j++) MPM
              //		tree_ctrl->Expand(j->second); MPM
	    }
	}
    }

  //select the last added entity
  if ((layer_tree->GetLastEntity() != NULL) && (last_selected != cpw::TypeId()))
    {
      std::multimap<cpw::TypeId, wxTreeListItem>::iterator j;
      j = entity_tree_relation.find(layer_tree->GetLastEntity()->GetId());
      if (j != entity_tree_relation.end())
                tree_ctrl->Select(j->second);
		
    }
}

void UILayerTreePanel::FillTree(cpw::Entity *entity, wxTreeListItem parent_id)
{
  std::string debug = entity->GetName();
  wxTreeListItem id = tree_ctrl->AppendItem(parent_id, wxString(entity->GetName().c_str(),wxConvUTF8));
  //  tree_ctrl->SetItemType(id,0,wxCheckboxItemType); MPM
  entity_tree_relation.insert(std::make_pair(entity->GetId(), id));
  if (entity->isVisible()) {
          //    tree_ctrl->SetItemChecked(id,0,true); MPM
  }
  
  std::string url = entity->GetIcon();
  if (url == "")
    url = cpw::ApplicationConfiguration::GetInstance()->GetUIIconDirectory()+"default.png";
  AddIcon( url, id);

  if (entity->isContainer())
    {
      for (int i = 0; i < ((cpw::ContainerLayer *)entity)->GetNumChildren(); i++)
	FillTree(((cpw::ContainerLayer *)entity)->GetChild(i), id);
    }		
}

void UILayerTreePanel::Update(bool subject_deleted)
{
  if (subject_deleted)
    return;
  FillTree();
  wxPanel::Update();
}


void UILayerTreePanel::EvtTreeItemExpanded(wxTreeEvent& wxevent)
{ 
  //find the url of the item
  wxTreeItemId expanded_item_id = wxevent.GetItem();
	
  if (expanded_item_id.IsOk())
    {
      //Search for the URL of this item
      cpw::TypeId id =  GetItemId(expanded_item_id.GetID());

      if (id != cpw::TypeId())
	items_expanded[id] = true;
    }
  Repaint();
}

void UILayerTreePanel::EvtTreeItemCollapsed(wxTreeEvent& wxevent) 
{ 
  //find the url of the item
  wxTreeItemId collapsed_item_id = wxevent.GetItem();
	
  if (collapsed_item_id.IsOk())
    {
      //Search for the URL of this item
      cpw::TypeId id = GetItemId(collapsed_item_id.GetID());

      if (id != cpw::TypeId())
	items_expanded[id] = false;
    }
  Repaint();
}

cpw::TypeId UILayerTreePanel::GetItemId(wxTreeItemId item_id)
{
  //Search for the URL of this item
  std::multimap<cpw::TypeId, wxTreeListItem>::iterator i;
  cpw::TypeId id = cpw::TypeId();

  if (item_id.IsOk())
    {
      //search for the tree item with the wanted ID
      for( i = entity_tree_relation.begin(); i!= entity_tree_relation.end(); i++)
	{
	  if ((*i).second == item_id)
	    {
	      id = (*i).first;
	      return id;
	    }
	}
    }

  return id;
}



void UILayerTreePanel::AddIcon(const std::string &icon_filename, wxTreeListItem id)
{
  if (id.IsOk())
    {
      wxIcon icon(wxString(icon_filename.c_str(),wxConvUTF8), wxBITMAP_TYPE_ANY);
      int img_index;
      if (icon.IsOk())
	img_index = img_list->Add(icon);
      else
	img_index = img_list->Add((*icondefault));
      tree_ctrl->SetItemImage(id, img_index);
    }
}


void UILayerTreePanel::EvtTreeItemSelChanged(wxTreeEvent& WXUNUSED(event))
{
  wxTreeListItem item_selected = tree_ctrl->GetSelection();
  bool search_for_parent = true;
  std::multimap<cpw::TypeId, wxTreeListItem>::iterator i;

  if ((item_selected.IsOk()) && (!dragging) )
    {
      //search for the tree item with the wanted ID
      for( i = entity_tree_relation.begin(); i!= entity_tree_relation.end(); i++)
	{
	  if ((*i).second == item_selected)
	    {
	      cpw::TypeId id = (*i).first;
	      cpw::Entity *entity = layer_tree->MakeActive(id);
				
	      if (entity->isContainer())
		{
		  search_for_parent = false;
		  layer_tree->MakeParentActive(id);
		}
				
	      if (entity != NULL)
		{
		  entity_properties_grid->ShowEntity(entity);				
		}
	    }
	}

      if (search_for_parent)
	{
	  wxTreeListItem parent_item_selected = tree_ctrl->GetItemParent(item_selected);

	  if (parent_item_selected.IsOk())
	    {
	      for( i = entity_tree_relation.begin(); i!= entity_tree_relation.end(); i++)
		{
		  if ((*i).second == parent_item_selected)
		    {
		      cpw::TypeId id = (*i).first;
		      layer_tree->MakeParentActive(id);
		    }
		}
	    }
	}
      last_selected = GetItemId(item_selected.GetID());
    }
  Repaint();
}

void UILayerTreePanel::OnRightClick(wxTreeEvent& exevent)
{
  wxTreeListItem item_selected = tree_ctrl->GetSelection();
  if (item_selected.IsOk())
    {
      bool _visible = tree_ctrl->GetCheckedState(item_selected);
      bool _animable = false;
      bool _publish = false;
      bool _container = false;
      cpw::Entity* ent = layer_tree->GetEntity(GetItemId(item_selected.GetID()));
      if (ent!=NULL)
	{
	  _visible  = ent->isVisible();
	  _animable = ent->isAnimate();
	  _publish  = ent->isPublished();
	  _container = ent->isContainer();
	}
      UILayerTreePanelPopUpMenu pop_up_menu(paren,this,_animable,_visible,_publish,_container);
      PopupMenu(&pop_up_menu);
    }
}



UILayerTreePanelPopUpMenu::UILayerTreePanelPopUpMenu(wxWindow* parent, UILayerTreePanel *ltp, bool animable, bool visible, bool publish, bool container) : wxMenu()
{
  paren = parent;
  layer_tree_panel = ltp;
#ifndef _CAPAVIEWER
  wxMenu *submenu_add_new = new wxMenu();

  submenu_add_new->Append(UIApplicationMainFrame::GetMenuItem(NEW_LAYER, submenu_add_new));
  submenu_add_new->Append(UIApplicationMainFrame::GetMenuItem(NEW_ELEMENT, submenu_add_new));
#endif
  AppendCheckItem(VISUALIZE, _T("Visualize"))->Check(visible);
  AppendCheckItem(ANIMATE, _T("Animate"))->Check(animable);
#ifndef _CAPAVIEWER
  AppendCheckItem(PUBLISH, _T("Publish"))->Check(publish);
#endif
  Append(UIApplicationMainFrame::GetMenuItem(GOTO, this));
  Enable(GOTO,!container);

#ifndef _CAPAVIEWER
  AppendSeparator();
  AppendCheckItem(SENDFRONT, _T("Send Front"));
  AppendCheckItem(SENDBACK, _T("Send Back"));
  AppendSeparator();
  Append(UIApplicationMainFrame::GetMenuItem(ADD_ENTITY, this));
  AppendSubMenu(submenu_add_new, wxString(_T("Add new")));
  AppendSeparator();
#endif
  //Append(UIApplicationMainFrame::GetMenuItem(VIEW_HTML, this));
  //AppendSeparator();
#ifndef _CAPAVIEWER
  Append(UIApplicationMainFrame::GetMenuItem(EDIT_CUT, this));
  Append(UIApplicationMainFrame::GetMenuItem(EDIT_COPY, this));
  Append(UIApplicationMainFrame::GetMenuItem(EDIT_PASTE, this));
  Append(UIApplicationMainFrame::GetMenuItem(EDIT_DELETE, this));
  AppendSeparator();
  Append(UIApplicationMainFrame::GetMenuItem(EDIT_MODIFY_PROPERTIES, this));
#else
  Append(UIApplicationMainFrame::GetMenuItem(SHOW_PROPERTIES, this));
#endif
	
}
UILayerTreePanelPopUpMenu::~UILayerTreePanelPopUpMenu()
{

}


void UILayerTreePanel::OnStartDragAndDrop(wxTreeEvent& wxevent)
{	
#ifndef _CAPAVIEWER
  ItemID = tree_ctrl->GetSelection();
  dragging = true; 
  escape = false;
  if(ItemID.IsOk())   
    {
      //      tree_ctrl->SetDragItem(ItemID);
      mainframe->Cut(GetItemId(ItemID.GetID()));
    }
#endif
}


void UILayerTreePanel::OnMouseCaptureLost(wxMouseCaptureLostEvent& WXUNUSED(event))
{
	
}


wxTreeListItem UILayerTreePanel::GetItem(wxTreeItemId itemId)
{
  wxTreeListItem actual = tree_ctrl->GetFirstItem();
  while (actual=tree_ctrl->GetNextItem(actual)) {
    if (actual.GetID()==itemId) return actual;
  }
  return NULL;
}

void UILayerTreePanel::OnEndDragAndDrop(wxTreeEvent& wxevent)
{
#ifndef _CAPAVIEWER
  if(escape) return;

  wxCursor cursorArrow(wxCURSOR_ARROW);
  SetCursor(cursorArrow);
  //  tree_ctrl->SetDragItem();
  dragging = false;
  wxTreeListItem item = GetItem(wxevent.GetItem());

  if(!item.IsOk()) return;

  // COMPROBAMOS QUE EL DESTINO NO SEA UN HIJO DEL ORIGEN
  wxTreeListItem aux = item;
  while((aux != tree_ctrl->GetRootItem()) && (aux != ItemID))
    aux = tree_ctrl->GetItemParent(aux);
  if(aux != tree_ctrl->GetRootItem()) return;

  // BUSCAMOS EL PADRE DEL ORIGEN 
  wxTreeListItem parent_orig = tree_ctrl->GetItemParent(ItemID);
  if(!parent_orig.IsOk()) return;

  // BUSCAMOS EL PADRE DE DESTINO 
  wxTreeListItem parent_dest = tree_ctrl->GetItemParent(item);
  if((item != tree_ctrl->GetRootItem()) && (!parent_dest.IsOk())) return;
  mainframe->Paste(GetItemId(ItemID.GetID()),
                   GetItemId(item.GetID()),
                   GetItemId(parent_orig.GetID()),
                   GetItemId(parent_dest.GetID()), true, false, false);
  Update();
#endif
	
}


void UILayerTreePanel::OnTreeKeyDown(wxTreeEvent& event)
{
  if(event.GetKeyCode() == WXK_ESCAPE)
    {
      wxCursor cursorArrow(wxCURSOR_ARROW);
      SetCursor(cursorArrow);
      //      tree_ctrl->SetDragItem();

      dragging = false;
      cut = false;
      copy = false;
      duplicate = false;
      escape = true;
    }
  event.Skip();
}

void UILayerTreePanel::OnMouseMotion(wxMouseEvent& wxevent)
{
}


void UILayerTreePanel::OnMove(wxMoveEvent& event)
{
}

void UILayerTreePanel::VisualizeRecursively(const wxTreeListItem& id, bool value)
{
  if(id.IsOk())
    {
      //      wxTreeListItemType item_type = tree_ctrl->GetItemType(id, 0);
      //      if (item_type == wxCheckboxItemType)
	{
                //	  tree_ctrl->SetItemChecked(id,0,value); MPM
	  CheckAllInstancesOf(id,value);
	}
  if (tree_ctrl->GetFirstChild(id).IsOk())
	{
    //	  wxTreeListItemValue cookie;
	  wxTreeListItem child = tree_ctrl->GetFirstChild(id/*, cookie*/);
	  if (child.IsOk())
	    {
	      VisualizeRecursively(child,value);
        int childrenCount = 1; //(signed)tree_ctrl->GetChildrenCount(id)
	      for (int i = 1; i < childrenCount; i++)
          {
            if(child.IsOk()) child = tree_ctrl->GetNextSibling(child);
            if(child.IsOk()) VisualizeRecursively(child,value);
          }
	    }
	}
    }
}


void UILayerTreePanel::VisualizeMenu(wxCommandEvent& event)
{
  wxTreeListItem item_selected = tree_ctrl->GetSelection();
  if (item_selected.IsOk())
    {
      //      wxTreeListItemType item_type = tree_ctrl->GetItemType(item_selected, 0);
      //      if (item_type == wxCheckboxItemType)
	{
	  bool value = tree_ctrl->GetCheckedState(item_selected);
	  VisualizeRecursively(item_selected, !value);
	  mainframe->Visualize(GetItemId(item_selected.GetID()), !value);
	}
    }
}

void UILayerTreePanel::PublishMenu(wxCommandEvent& event)
{
#ifndef _CAPAVIEWER
  wxTreeListItem item_selected = tree_ctrl->GetSelection();
  if (item_selected.IsOk())
    {
      mainframe->Publish(GetItemId(item_selected.GetID()));
    }
#endif
}

void UILayerTreePanel::AnimateMenu(wxCommandEvent& WXUNUSED(event))
{
  wxTreeListItem item_selected = tree_ctrl->GetSelection();
  if (item_selected.IsOk())
    {
      mainframe->Animate(GetItemId(item_selected.GetID()));
    }
}

void UILayerTreePanel::OnCheckBoxEvent(wxCommandEvent& event)
{
  wxTreeListItem item_selected = tree_ctrl->GetSelection();
  if (item_selected.IsOk())
    {
      //      wxTreeListItemType item_type = tree_ctrl->GetItemType(item_selected, 0);
      //      if (item_type == wxCheckboxItemType)
	{
	  bool value = tree_ctrl->GetCheckedState(item_selected);
	  VisualizeRecursively(item_selected, value);
	  mainframe->Visualize(GetItemId(item_selected.GetID()), value);
	}
    }
  event.StopPropagation();
  Repaint();
}

void UILayerTreePanel::CheckAllInstancesOf(const wxTreeListItem& id, const bool &value)
{
  cpw::TypeId eid = GetItemId(id.GetID());
  std::multimap<cpw::TypeId, wxTreeListItem>::iterator iter;
  for(iter = entity_tree_relation.begin(); iter != entity_tree_relation.end(); iter++)
    {
            if(((*iter).first == eid) && (((*iter).second).IsOk()))
                    {
                            //	tree_ctrl->SetItemChecked((*iter).second,0,value);MPM
                    }
    }
}


void UILayerTreePanel::SendFront(wxCommandEvent& event)
{
#ifndef _CAPAVIEWER
  wxTreeListItem item_selected = tree_ctrl->GetSelection();
  if (item_selected.IsOk())
    {
      mainframe->SendFront(GetItemId(item_selected.GetID()));
    }
#endif
}
void UILayerTreePanel::SendBack(wxCommandEvent& event)
{
#ifndef _CAPAVIEWER
  wxTreeListItem item_selected = tree_ctrl->GetSelection();
  if (item_selected.IsOk())
    {
      mainframe->SendBack(GetItemId(item_selected.GetID()));
    }
#endif
}

void UILayerTreePanel::DeleteAllChildren(const wxTreeListItem& id)
{
#ifndef _CAPAVIEWER
  if(id.IsOk())
   {
      wxTreeListItem child = tree_ctrl->GetFirstChild(id);
      if(child.IsOk())
	{
	  wxTreeItemIdValue cookie;
	  wxTreeListItem child = tree_ctrl->GetFirstChild(id/*, cookie*/);
	  wxTreeListItem next;
	  if (child.IsOk())
	    {
	      DeleteAllChildren(child);
        int childrenCount = 1; //(signed)tree_ctrl->GetChildrenCount(id)
	      for (int i = 1; i < childrenCount; i++)
		{
		  if(child.IsOk()) next = tree_ctrl->GetNextSibling(child);
		  if(next.IsOk()) DeleteAllChildren(next);
		  if(next.IsOk())
		    {
		      std::multimap<cpw::TypeId, wxTreeListItem>::iterator iter = entity_tree_relation.begin();
		      while((iter != entity_tree_relation.end()) && ((*iter).second != next)) 
			iter++;
		      entity_tree_relation.erase(iter);
		      tree_ctrl->DeleteItem(next);
		    }

		}
	      if(child.IsOk())
		{
		  std::multimap<cpw::TypeId, wxTreeListItem>::iterator iter = entity_tree_relation.begin();
		  while((iter != entity_tree_relation.end()) && ((*iter).second != child)) 
		    iter++;
		  entity_tree_relation.erase(iter);				
		  tree_ctrl->DeleteItem(child);
		}
	    }
	}		
    }	
#endif
}


void UILayerTreePanel::Delete(wxCommandEvent& event)
{
#ifndef _CAPAVIEWER
  wxTreeListItem item_selected = tree_ctrl->GetSelection();
  if ((item_selected.IsOk()) && (item_selected != tree_ctrl->GetRootItem()))
    {		
      wxTreeListItem aux = tree_ctrl->GetItemParent(item_selected);
      if(aux.IsOk())
	{

	  wxMessageDialog message1(NULL,wxT("Are you sure you want to remove this item?"), wxT("Warning"),wxICON_EXCLAMATION |wxYES |wxNO);
	  if(message1.ShowModal() == wxID_YES)
	    {
	      mainframe->Delete(GetItemId(item_selected.GetID()),
                          GetItemId(aux.GetID()));

              wxTreeListItem child = tree_ctrl->GetFirstChild(item_selected);
	      if(child.IsOk())
		DeleteAllChildren(item_selected);

	      std::multimap<cpw::TypeId, wxTreeListItem>::iterator itermm = entity_tree_relation.begin();
	      while((itermm != entity_tree_relation.end()) && ((*itermm).second != (item_selected))) 
		itermm++;

	      if(itermm != entity_tree_relation.end())
		{
		  entity_tree_relation.erase(itermm);
		  tree_ctrl->DeleteItem(item_selected);
		}
	    }
	}
    }
  else
    {
      if(!item_selected.IsOk()) return;

      wxMessageDialog message1(NULL,wxT("Are you sure you want to remove all items?"), wxT("Warning"),wxICON_EXCLAMATION |wxYES |wxNO);
      if(message1.ShowModal() == wxID_YES)
	{
          tree_ctrl->DeleteAllItems();
          mainframe->Delete(GetItemId(tree_ctrl->GetRootItem().GetID()),
                            GetItemId(tree_ctrl->GetRootItem().GetID()));
	  entity_tree_relation.clear();
	}
    }

  Update();
#endif
}

void UILayerTreePanel::Copy(wxCommandEvent& event)
{
#ifndef _CAPAVIEWER
  ItemID = tree_ctrl->GetSelection();
  escape = false;
  copy = true;	
  if(ItemID.IsOk()) 
    {
            //      tree_ctrl->SetDragItem(ItemID);
      mainframe->Copy(GetItemId(ItemID.GetID()));
    }
#endif
}


void UILayerTreePanel::Duplicate(wxCommandEvent& event)
{
#ifndef _CAPAVIEWER
  ItemID = tree_ctrl->GetSelection();
  escape = false;
  duplicate = true;	
  if(ItemID.IsOk())   
    {
            //      tree_ctrl->SetDragItem(ItemID);
      mainframe->Duplicate(GetItemId(ItemID.GetID()));
    }
#endif
}


void UILayerTreePanel::Cut(wxCommandEvent& event)
{
#ifndef _CAPAVIEWER
  ItemID = tree_ctrl->GetSelection();
  escape = false;
  cut = true;
  if(ItemID.IsOk())   
    {
            //      tree_ctrl->SetDragItem(ItemID);
      mainframe->Cut(GetItemId(ItemID.GetID()));
    }
#endif
}

void UILayerTreePanel::Paste(wxCommandEvent& event)
{
#ifndef _CAPAVIEWER

  if(escape) return;

  //  tree_ctrl->SetDragItem(); MPM

  if(!ItemID.IsOk()) return;

  wxTreeListItem item = tree_ctrl->GetSelection();

  if(!item.IsOk()) return;

  //// COMPROBAMOS QUE EL DESTINO NO SEA UN HIJO DEL ORIGEN
  wxTreeListItem aux = item;
  while((aux != tree_ctrl->GetRootItem()) && (aux != ItemID) && (GetItemId(aux.GetID()) != GetItemId(ItemID.GetID())))
    aux = tree_ctrl->GetItemParent(aux);
  if(aux != tree_ctrl->GetRootItem()) return;


  //// BUSCAMOS EL PADRE DEL ORIGEN
  wxTreeListItem parent_orig = tree_ctrl->GetItemParent(ItemID);
  if(!parent_orig.IsOk()) return;

  //// BUSCAMOS EL PADRE DE DESTINO, SI NO HAY COGEMOS AL ROOT
  wxTreeListItem parent_dest = tree_ctrl->GetItemParent(item);
  if(!parent_dest.IsOk()) parent_dest = tree_ctrl->GetRootItem();
  mainframe->Paste(GetItemId(ItemID.GetID()), GetItemId(item.GetID()),
                   GetItemId(parent_orig.GetID()),
                   GetItemId(parent_dest.GetID()), cut, copy, duplicate);
  copy = false;
  cut = false;
  duplicate = false;
  Update();
#endif
}


void UILayerTreePanel::GoTo(wxCommandEvent& event)
{
  wxTreeListItem item_selected = tree_ctrl->GetSelection();
  if (item_selected.IsOk())
    mainframe->GoTo(GetItemId(item_selected.GetID()));
}


void UILayerTreePanel::ModifyProperties(wxCommandEvent& event)
{
#ifndef _CAPAVIEWER
  wxTreeListItem item_selected = tree_ctrl->GetSelection();
  if (item_selected.IsOk())	
    {
      mainframe->ModifyProperties(GetItemId(item_selected.GetID()));
      tree_ctrl->Select(item_selected);
    }

#endif
}

void UILayerTreePanel::ModifySelection() 
{
  wxCommandEvent tmp = wxCommandEvent();
  ModifyProperties((wxCommandEvent &) tmp);
}

void UILayerTreePanel::CutSelection() 
{
  wxCommandEvent tmp = wxCommandEvent();
  Cut((wxCommandEvent &) tmp);
}

void UILayerTreePanel::CopySelection() 
{
  wxCommandEvent tmp = wxCommandEvent();
  Copy((wxCommandEvent &) tmp);
}

void UILayerTreePanel::PasteSelection() 
{
  wxCommandEvent tmp = wxCommandEvent();
  Paste((wxCommandEvent &) tmp);
}

void UILayerTreePanel::DeleteSelection() 
{
  wxCommandEvent tmp = wxCommandEvent();
  Delete((wxCommandEvent &) tmp);
}

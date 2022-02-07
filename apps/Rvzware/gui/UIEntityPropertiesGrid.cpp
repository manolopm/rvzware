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

#include <vector>

#include <stdafx.h>

#include <gui/UIEntityPropertiesGrid.h>
#include <cpw/common/Composite.h>
#include <cpw/ApplicationConfiguration.h>

using namespace cpw::gui;


#define ROW_LABEL_SIZE 80

BEGIN_EVENT_TABLE(UIEntityPropertiesGrid, wxPanel)
EVT_SIZE(UIEntityPropertiesGrid::OnSize)
END_EVENT_TABLE()

UIEntityPropertiesGrid::UIEntityPropertiesGrid(wxWindow *parent, wxWindowID id, const wxString& name) :
wxPanel(parent, id, wxDefaultPosition, wxDefaultSize,wxTAB_TRAVERSAL, name)
{

  this->SetMinSize(wxSize(128, 128));
  properties_tree = new wxTreeListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_ROW_LINES | wxTR_HAS_BUTTONS | wxTR_HAS_VARIABLE_ROW_HEIGHT | wxTR_FULL_ROW_HIGHLIGHT | wxTR_EDIT_LABELS| wxTR_TWIST_BUTTONS| wxTR_HIDE_ROOT | wxTR_NO_LINES );//wxTR_SINGLE| wxTR_HAS_BUTTONS /*| wxTR_HIDE_ROOT*/ ););
  //  properties_tree->SetIndent(0);
  if (!(cpw::ApplicationConfiguration::GetInstance()->IsThemed()))
    {
      wxColour c_foreg = wxString(ApplicationConfiguration::GetInstance()->GetPageFontColour().c_str(),wxConvUTF8);
      wxColour c_backg = wxString(ApplicationConfiguration::GetInstance()->GetPageColour().c_str(),wxConvUTF8);	
      properties_tree->SetBackgroundColour(c_backg);
      properties_tree->SetForegroundColour(c_foreg);
    }

  properties_tree->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxNORMAL, false, wxT("Tahoma")));                                 
  properties_tree->AppendColumn(wxT("Property"),110 ,wxALIGN_LEFT); 
  properties_tree->AppendColumn(wxT("Value"),500 ,wxALIGN_LEFT); 
  //  properties_tree->SetColumnPickType(1,wxTR_COLUMN_FLOAT_TEXT);

}

UIEntityPropertiesGrid::~UIEntityPropertiesGrid(void)
{
}


void UIEntityPropertiesGrid::OnSize(wxSizeEvent& event)
{
  int width, height;

  //EPYME caso extraño con size cuando esta desacoplado
  if (GetParent())
    GetParent()->GetSize(&width, &height);
  
  if (properties_tree)
    properties_tree->SetSize(width, height);
	
  Refresh();
}

void UIEntityPropertiesGrid::ShowEntity(cpw::Node *root_node, cpw::Node *node, wxTreeItemId t_parent)
{
  wxTreeItemId t_root = properties_tree->GetRootItem().GetID();
  wxTreeItemId t_id1;
  if (node != root_node)
    t_id1 = properties_tree->AppendItem(GetItem(t_parent),wxString(node->GetName().c_str(),wxConvUTF8)).GetID();
  else
    t_id1 = t_root;


  wxTreeItemId t_aux;
  if(node->GetNumAttributes() != 0)
    {
      //      properties_tree->SetItemBold(t_id1,true);	
      std::map<std::string, std::string> *attr = node->GetAttributes();
      std::map<std::string, std::string>::iterator attr_iter = attr->begin();
      for( ; attr_iter != attr->end(); attr_iter++)
	{
	  t_aux = properties_tree->AppendItem(GetItem(t_id1),wxString(attr_iter->first.c_str(),wxConvUTF8)).GetID();
	  properties_tree->SetItemText(GetItem(t_aux),1,wxString(attr_iter->second.c_str(),wxConvUTF8));
	}
    }
  else
    {
      properties_tree->SetItemText(GetItem(t_id1),1,wxString(node->GetValue().c_str(),wxConvUTF8));
    }
  properties_tree->Expand(GetItem(t_root));

  if (node->isContainer())
    {
      //      properties_tree->SetItemBold(t_id1,true);
      std::vector<cpw::Node *> children_vec = node->GetChildren();
      std::vector<cpw::Node *>::iterator i = children_vec.begin();

      for (;i != children_vec.end(); i++)
	{
	  ShowEntity(root_node,*i,t_id1);
	}
    }	
}

void UIEntityPropertiesGrid::ShowEntity(cpw::Entity *entity)
{
  if (entity == NULL)
    {
      wxSizeEvent tmp = wxSizeEvent();
      this->OnSize((wxSizeEvent &)tmp);	
      return;
    }

  cpw::Node *root = entity->GetProperties();

  properties_tree->DeleteAllItems();
	
  wxTreeItemId t_root  = properties_tree->AppendItem(properties_tree->GetRootItem(),wxString(root->GetName().c_str(),wxConvUTF8)).GetID();
  properties_tree->Expand(GetItem(t_root));
  ShowEntity(root, root, t_root);
  //  properties_tree->ExpandAll(t_root);
	
  wxSizeEvent tmp = wxSizeEvent();
  this->OnSize((wxSizeEvent &)tmp);
  if (root)
    {
      delete root;
      root = NULL;
    }
}

void UIEntityPropertiesGrid::Clear()
{
  properties_tree->DeleteAllItems();
}

wxTreeListItem UIEntityPropertiesGrid::GetItem(wxTreeItemId itemId)
{
  wxTreeListItem actual = properties_tree->GetFirstItem();
  while (actual=properties_tree->GetNextItem(actual)) {
    if (actual.GetID()==itemId) return actual;
  }
  return NULL;
}

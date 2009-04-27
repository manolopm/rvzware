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


#include <wx/button.h>
#include <osg/Geode>
#include <sstream>

#include <stdafx.h>


#include <gui/UISceneTree.h>

using namespace cpw::gui; 


#include <cpw/graphic/ApplicationScene.h>
#include <cpw/entity/Entity.h>
#include <cpw/common/TypeId.h>


BEGIN_EVENT_TABLE(UISceneTree, wxPanel)
	EVT_BUTTON   (REFRESH_BUTTON,   UISceneTree::Refresh)
END_EVENT_TABLE()

UISceneTree::UISceneTree(wxWindow *parent) : wxPanel(parent, wxID_ANY), osg_scene(NULL)
{
	SetMinSize(wxSize(128, 50));
	wxBoxSizer *top_sizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(top_sizer);

	wxBoxSizer *sub_sizer = new wxBoxSizer(wxHORIZONTAL);

	tree_ctrl = new wxTreeCtrl(this, -1);
	tree_ctrl->SetSize(this->GetSize());

	wxButton *button = new wxButton(this, REFRESH_BUTTON, wxT("Refresh"));
	button->SetMaxSize(wxSize(55, 35));
	button->SetMinSize(wxSize(55, 35));

	node_count = new wxStaticText(this, wxID_ANY, wxT(" Node count 0"));
		
	top_sizer->Add(tree_ctrl,1,wxGROW);
	top_sizer->Add(sub_sizer, 0);//, wxALIGN_CENTER);

	sub_sizer->Add(button, 0, wxALIGN_CENTER);
	sub_sizer->Add(node_count, 0, wxALIGN_CENTER);


	entity_registry = cpw::EntityRegistry::GetInstance();


	
}

UISceneTree::~UISceneTree()
{
}

void UISceneTree::Refresh(wxCommandEvent &event)
{
	osg_scene = (iosg::OsgScene *) cpw::ApplicationScene::GetInstance()->GetScene();
	tree_ctrl->DeleteAllItems();

	if (osg_scene == NULL)
		return;

	osg::Group *root_group = (osg::Group *) osg_scene->GetRootNode();

	if (root_group == NULL)
		return;
	
	total_nodes = 1;

	wxTreeItemId root_id = tree_ctrl->AddRoot(wxString(GetTreeText((osg::Node *) root_group).c_str(),wxConvUTF8));

	for (unsigned int i=0; i<root_group->getNumChildren(); i++)
		AddNode(root_id, root_group->getChild(i));

	//update the node count
	std::ostringstream node_count_ss;
	
	node_count_ss << total_nodes;
	std::string node_count_str = " Node count " + node_count_ss.str();
	node_count->SetLabel(wxString(node_count_str.c_str(),wxConvUTF8));
	
}


void UISceneTree::AddNode(wxTreeItemId &parent_id, osg::Node *osg_node)
{
	total_nodes++;
	wxTreeItemId id = tree_ctrl->AppendItem(parent_id, wxString(GetTreeText(osg_node).c_str(),wxConvUTF8));

	std::string class_name = osg_node->className();

	if (class_name == "Geode")
	{
		osg::Geode *geode = (osg::Geode *) osg_node;
		unsigned int drawable_count = geode->getNumDrawables();

		for (unsigned int i=0; i<drawable_count; i++)
		{
			std::string text;
			
			osg::Drawable *drawable = geode->getDrawable(i);
			text = drawable->className();
			text += ", ";
			text += drawable->getName();
			tree_ctrl->AppendItem(id, wxString(text.c_str(),wxConvUTF8));
			total_nodes++;
		}
	}

	
	osg::Group *group = dynamic_cast<osg::Group *>(osg_node);

	if (group != NULL)
	{
		for (unsigned int i=0; i<group->getNumChildren(); i++)
			AddNode(id, group->getChild(i));
	}
	
}

std::string UISceneTree::GetTreeText(osg::Node *node)
{
	std::string out;
	
	out = node->className();
	out += ", " + node->getName();
	
	wxString str = wxString(node->getName().c_str(),wxConvUTF8);
	if (!str.empty())
	{
		if (entity_registry != NULL)
		{
		  cpw::TypeId id(std::string(str.mb_str()));
			cpw::Entity *entity = entity_registry->GetEntity(id);
		
			if (entity != NULL)
				out += ", " + entity->GetName();
		}
	}
	
	return out;
}

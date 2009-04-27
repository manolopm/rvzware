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

#ifndef _UISceneTree_
#define _UISceneTree_

#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/stattext.h>

#include <cpw/entity/EntityRegistry.h>

#include <osg/Node>
#include <iosg/OsgScene.h>

#define REFRESH_BUTTON 7600

namespace cpw 
{ 
	namespace gui
	{

		/** 
			\brief OSG Tree Debug Panel...
			\ingroup cpw
		*/
		class UISceneTree : public wxPanel
		{
			public:
				
				UISceneTree(wxWindow *parent);
				~UISceneTree(void);

				void Refresh(wxCommandEvent &event);

			private:


				void AddNode(wxTreeItemId &parent_id, osg::Node *osg_node);
				std::string GetTreeText(osg::Node *node);

				wxTreeCtrl *tree_ctrl;
				wxStaticText *node_count;
				iosg::OsgScene *osg_scene;
				cpw::EntityRegistry *entity_registry;
				unsigned int total_nodes;


				DECLARE_EVENT_TABLE()
		};
	}
}

#endif

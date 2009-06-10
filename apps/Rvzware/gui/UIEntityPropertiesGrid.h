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

#ifndef _UIENTITYPROPERTIESGRID_
#define _UIENTITYPROPERTIESGRID_

#include <wx/wx.h>
#include <wx/minifram.h>
#include <wx/grid.h>
#include <wx/colour.h>

#include <cpw/entity/Entity.h>

#include <gui/GUItypes.h>

#include <gui/treelistctrl.h>


namespace cpw
{
	namespace gui
	{

		/** 
			\brief Window panel that shows the entity properties
			\ingroup cpw
		*/
		class UIEntityPropertiesGrid : public wxPanel
		{
			public:

				UIEntityPropertiesGrid(wxWindow *parent, wxWindowID id);
				~UIEntityPropertiesGrid(void);
				void ShowEntity(cpw::Entity *entity = NULL);
				void Clear();

			private:
				
				wxGrid *grid;
				wxTreeListCtrl *properties_tree;
				
				void ShowEntity(cpw::Node *root_node, cpw::Node *node, wxTreeItemId t_parent);
				void OnSize(wxSizeEvent& event);

				DECLARE_EVENT_TABLE()
		};
	}
}

#endif



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

#ifndef _LAYERTREEPANEL_
#define _LAYERTREEPANEL_

#include <map>
#include <string>
#include <sstream>
#include <wx/wx.h>
#include <wx/menu.h>
#include <gui/treelistctrl.h>
#include <wx/imaglist.h>
#include <wx/bitmap.h>

#include <wx/dragimag.h>
#include <wx/generic/dragimgg.h>

#include <wx/minifram.h>
#include <wx/colour.h>
#include <wx/image.h>

#include <cpw/graphic/INavigatorManager.h>
#include <cpw/common/Observer.h>
#include <cpw/entity/LayerTree.h>

#include <gui/UIEntityPropertiesGrid.h>
#include <gui/GUItypes.h>

#include <UIApplicationMainFrame.h>

namespace cpw
{
	namespace gui
	{
		class UIApplicationMainFrame;

		/** 
			\brief This window panel shows info about the layers and objects contained in scene
			\ingroup cwcontent
		*/
		class UILayerTreePanel : public wxPanel, cpw::Observer
		{
			public:
		  UILayerTreePanel(UIApplicationMainFrame* main_frame,
				   cpw::INavigatorManager *navigator_manager,
				   cpw::LayerTree &_layer_tree, wxWindow *parent,
				   int xpos, int ypos, int width, int height,
				   wxString &title, long style = wxTAB_TRAVERSAL );
				~UILayerTreePanel(void);

				void Clear();

				void FillTree();

				void Update(bool subject_deleted = false);

				void Repaint(){	tree_ctrl->Refresh(); tree_ctrl->UpdateWindowUI(); }

				void SetEntityPropertiesGrid(UIEntityPropertiesGrid *epg) {entity_properties_grid = epg;}
				void CleanEntityPropertiesGrid() {if (entity_properties_grid!=NULL) entity_properties_grid->Clear();}
					
				void CutSelection();
				void CopySelection();
				void PasteSelection();
				void DeleteSelection();
				void ModifySelection(); 

			private:

				wxGenericDragImage *img;
				wxTreeItemId ItemID;
				wxIcon *icondefault;
				void OnMouseMotion(wxMouseEvent& wxevent);
				void OnMouseCaptureLost(wxMouseCaptureLostEvent& WXUNUSED(event));
				void OnTreeKeyDown(wxTreeEvent& event);
				bool dragging;
				bool escape;
				bool cut;
				bool copy;
				bool duplicate;
				cpw::INavigatorManager *nm;
				UIApplicationMainFrame* mainframe;

				void AddIcon(const std::string &primitive_url, wxTreeItemId id);

				void EvtTreeItemSelChanged(wxTreeEvent& WXUNUSED(event));
				void EvtTreeItemExpanded(wxTreeEvent& wxevent);
				void EvtTreeItemCollapsed(wxTreeEvent& wxevent);
				
				void OnRightClick(wxTreeEvent& exevent);
				void OnStartDragAndDrop(wxTreeEvent& wxevent);
				void OnEndDragAndDrop(wxTreeEvent& wxevent);
				void OnMove(wxMoveEvent& event);

				void OnCheckBoxEvent(wxCommandEvent& event);
				void VisualizeMenu (wxCommandEvent& event);
				void AnimateMenu   (wxCommandEvent& WXUNUSED(event));
				void PublishMenu   (wxCommandEvent& WXUNUSED(event));
				void VisualizeRecursively (const wxTreeItemId& id, bool value);
				void SendBack(wxCommandEvent& event);
				void SendFront(wxCommandEvent& event);
				void Copy(wxCommandEvent& event);  // not a real copy, it's a link
				void Duplicate(wxCommandEvent& event);
				void Cut(wxCommandEvent& event);
				void Paste(wxCommandEvent& event);
				void Delete(wxCommandEvent& event);
				void GoTo(wxCommandEvent& event);
				void ModifyProperties(wxCommandEvent& event);

				void CheckAllInstancesOf(const wxTreeItemId& id, const bool &value);
				void DeleteAllChildren(const wxTreeItemId& id);


				wxWindow *paren;
				wxTreeListCtrl *tree_ctrl;
				wxTreeItemId root_id;
				cpw::TypeId last_selected;
				wxImageList *img_list;
				UIEntityPropertiesGrid *entity_properties_grid;
				
				cpw::LayerTree *layer_tree;

				std::multimap<cpw::TypeId, wxTreeItemId> entity_tree_relation;
				std::map<cpw::TypeId, bool> items_expanded;

				wxBitmap frame_shape;
				
				void DoLayOut();
				void FillTree(cpw::Entity *entity, wxTreeItemId parent_id); //show the layer tree on the interface
				cpw::TypeId GetItemId(wxTreeItemId item_id);
				
				DECLARE_EVENT_TABLE()
		};

		/** 
			\brief Tree Panel Pop-up menu
			\ingroup cwcontent
		*/
		class UILayerTreePanelPopUpMenu : public wxMenu
		{
			public:
				UILayerTreePanelPopUpMenu(wxWindow* parent, UILayerTreePanel *ltp, bool animable = false, bool visible = false, bool publish = false, bool container = false);
				~UILayerTreePanelPopUpMenu();
				
			private:			
				wxWindow *paren;
				UILayerTreePanel *layer_tree_panel;
				void PopupAdd(wxCommandEvent& WXUNUSED(event));

		};
	}
}

#endif

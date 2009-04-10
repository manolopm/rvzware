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
#ifndef __UIANIMABLELIST__
#define __UIANIMABLELIST__

#include <vector>
#include <map>
#include <cpw/entity/Entity.h>

#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/listbox.h>
#include <wx/button.h>
#include <wx/image.h>

#include "treelistctrl.h"

#undef UIAnimableList_STYLE
#define UIAnimableList_STYLE wxCAPTION | wxSYSTEM_MENU | wxDIALOG_NO_PARENT | wxCLOSE_BOX

namespace cpw 
{ 
	
	namespace gui
	{

		/** 
			\brief List of Animable Elements
			\ingroup cpw
		*/
		class UIAnimableList : public wxDialog
		{
			private:
				DECLARE_EVENT_TABLE();
				
			public:
				UIAnimableList(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("Fire"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = UIAnimableList_STYLE);
				virtual ~UIAnimableList();
				wxListBox * GetAnimablesListBox(){ return WxListBox1; }
				
				void Clear();
				void Append(const std::vector<cpw::Entity*> &animated_entities);
				void Quit(const wxTreeItemId& id);

			private:
				wxListBox *WxListBox1;
				wxButton *Finish_button;
				wxButton *Cancel_button;

				wxStaticText *animableList_StaticText;
				wxButton *selectAll_button;
				wxButton *deselectAll_button;
				wxImageList *img_list;
				wxTreeListCtrl *animableTree;
				
				enum
				{
					ID_ANIMABLE_TREE = 1007,
					ID_DESELECTALL_BUTTON = 1006,
					ID_SELECTALL_BUTTON = 1005,
					ID_ANIMABLELIST_STATICTEXT = 1004,
					ID_WXLISTBOX1 = 1003,
					ID_FINISH_BUTTON = 1002,
					ID_CANCEL_BUTTON = 1001,
					ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
				};
			
			private:
				
				void CreateGUIControls();

				wxWindow *parent_;

				void OnClose(wxCloseEvent& event);
				void OnMove(wxMoveEvent& event);
				void OnButtonSelectAll(wxCommandEvent& WXUNUSED(event));
				void OnButtonDeselectAll(wxCommandEvent& WXUNUSED(event));
				void OnButtonCancel(wxCommandEvent& WXUNUSED(event));
				void OnButtonFinish(wxCommandEvent& WXUNUSED(event));
				void OnListBoxSelectItem(wxCommandEvent& WXUNUSED(event));
				void ToggleSelection(wxTreeEvent& WXUNUSED(event));
				void OnCheck(wxTreeEvent& WXUNUSED(event));
				void OnCheckBoxChange(wxCommandEvent& event);
				
				void OnPaint(wxPaintEvent& event);
				void render(wxDC& dc);

				void SetRecursively(const wxTreeItemId& id, bool value);

				void AddIcon(const std::string &icon_filename, wxTreeItemId id);

				std::map<cpw::Entity*, wxTreeItemId> treeitem_entity_map;

		};
	}

}

#endif

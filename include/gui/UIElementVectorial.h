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
#ifndef __UIELEMENTVECTORIAL__
#define __UIELEMENTVECTORIAL__

#include <wx/wx.h>
#include <wx/dialog.h>

#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/notebook.h>
#include <wx/image.h>
#include <wx/treectrl.h>
#include <wx/spinbutt.h>
#include <wx/calctrl.h>
#include <wx/string.h>
#include <wx/clrpicker.h>

#include <iostream>
#include <iomanip>

#include <cpw/common/Attribute.h>
#include <cpw/common/types.h>
#include <cpw/entity/Entity.h>
#include <controllers/ElementVectorialController.h>

#include <gui/UIEAttributes.h>
#include <gui/MyTimeCtrl.h>

#undef UIElementVectorial_STYLE
#define UIElementVectorial_STYLE wxCAPTION | wxSYSTEM_MENU | wxDIALOG_NO_PARENT | wxCLOSE_BOX

namespace cpw 
{ 
	namespace controllers { class ElementVectorialController; }

	namespace gui
	{


		/** 
			\brief Window base panel for creating Vectorial Elements (pe. firewalls)
			\ingroup cwcontent
		*/
		class UIElementVectorial : public wxDialog
		{
			private:
				DECLARE_EVENT_TABLE();
				
			public:
				UIElementVectorial(controllers::ElementVectorialController *evc,wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("ElementVectorial"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = UIElementVectorial_STYLE);
				
				virtual ~UIElementVectorial();

				void AddPoint();
				void RemovePoint();

				bool GetCoordsFromEditControls(cpw::Point3d<float> &pos);

				void SetPositionX(const std::string &position_x) { update_on_changes = false; Position_x_Edit->SetValue(position_x); update_on_changes = true;}
				void SetPositionY(const std::string &position_y) { update_on_changes = false; Position_y_Edit->SetValue(position_y); update_on_changes = true;}
				void SetPositionZ(const std::string &position_z) { update_on_changes = false; Position_z_Edit->SetValue(position_z); update_on_changes = true;}

				void SetElementVectorialName(std::string &name)  { ElementVectorial_name_Ed->SetLabel(_T(name));  }
				void SetElementVectorialType(std::string &name)  { vname = name; SetTitle(wxT(vname));}
				void SetElementVectorialIcon(std::string &parenticon, std::string &childicon);

				void Modify(const bool &value) {modify = value; ElementVectorial_name_Ed->Enable(false); }
				void FillUI(const int &nump, const std::string &color, const int &width);


			private:

				bool modify;
				
				wxSpinButton *Spin_Button;
				wxImageList *img_list;
				wxNotebook *note_panel;
				wxTreeCtrl *Points_TreeCtrl;
				wxBitmapButton *Remove_Button; 
				wxStaticBox *Position_StaticBox;
				wxTextCtrl *Position_z_Edit;
				wxStaticText *Position_z_StaticText;
				wxTextCtrl *Position_y_Edit;
				wxStaticText *Position_y_StaticText;
				wxStaticText *Position_x_StaticText;
				wxTextCtrl *Position_x_Edit;
				wxStaticText *WxStaticText2;
				wxColourPickerCtrl *colour_ctrl;
				wxTextCtrl *border_Edit;
				wxStaticText *border_StaticText;
				wxStaticText *WxStaticText1;
				wxButton *Finish_button;
				wxButton *Cancel_button;
				wxTextCtrl *ElementVectorial_name_Ed;
				wxStaticText *ElementVectorial_name_StaticText;
				
			private:
				enum
				{
					ID_SPIN_POS_X = 1094,
					ID_SPIN_POS_Y = 1093,
					ID_SPIN_POS_Z = 1092,
					ID_POINTS_TREECTRL = 1078,
					ID_EDIT_FLAMES = 1076,
					ID_AUTOADDBUTTON = 1074,
					ID_REMOVE_BUTTON = 1072,
					ID_POSITION_STATICBOX = 1065,
					ID_POSITIONZ_EDIT = 1020,
					ID_POSITIONZ_STATICTEXT = 1019,
					ID_POSITIONY_EDIT = 1017,
					ID_POSITIONY_STATICTEXT = 1016,
					ID_Position_X_STATICTEXT = 1015,
					ID_POSITIONX_EDIT = 1014,
					ID_POSITION_STATICTEXT = 1013,
					ID_COLOUR_CTRL = 1011,
					ID_EDIT_BORDER = 1010,
					ID_SPIN_BORDER = 1009,
					ID_BORDER_STATICTEXT = 1008,
					ID_LOADElementVectorialTEXURE_BUTTON = 1007,
					ID_ElementVectorialTEXTURE_EDIT = 1006,
					ID_ElementVectorialTEXTURE_STATICTEXT = 1005,
					ID_FINISH_BUTTON = 1004,
					ID_CANCEL_BUTTON = 1003,
					ID_ELEMENTVECTORIALNAME_EDIT = 1002,
					ID_ELEMENTVECTORIALNAME_STATICTEXT = 1001,
					ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
				};
			
			private:
				
				bool polygon_flag;
				bool update_on_changes;
				bool m_isDragging;
				bool changes;

				float posx, posy, posz;

				wxWindow *paren;
				wxBitmap m_bmp;

				controllers::ElementVectorialController *ElementVectorial_controller;
				std::string vname;
				std::string vicon, vicon_child;

				void ButtonOK();
				void Exit();
				void CreateGUIControls();
				void OnClose(wxCloseEvent& event);
				void OnButtonCancel(wxCommandEvent& WXUNUSED(event));
				void OnButtonFinish(wxCommandEvent& WXUNUSED(event));
				void CheckPointsValues(wxCommandEvent& WXUNUSED(event));

				void OnButtonRemovePoint(wxCommandEvent& WXUNUSED(event));
				void OnTreeSelectionChanged(wxTreeEvent& wxevent);

				void OnBorderSpinUp(wxSpinEvent& event);
				void OnPositionXSpinUp(wxSpinEvent& event);
				void OnPositionYSpinUp(wxSpinEvent& event);
				void OnPositionZSpinUp(wxSpinEvent& event);

				void OnBorderSpinDown(wxSpinEvent& event);
				void OnPositionXSpinDown(wxSpinEvent& event);
				void OnPositionYSpinDown(wxSpinEvent& event);
				void OnPositionZSpinDown(wxSpinEvent& event);

				void OnColourPickerChange(wxColourPickerEvent& event);

				void OnPaint(wxPaintEvent& event);
				void render(wxDC& dc);
				void DrawStaticBox(wxDC& dc,const wxString& label, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);

				void OnEraseBackground(wxEraseEvent& event); //don't remove this method
		};
	}
}

#endif

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
#ifndef __UICALCULATEDISTANCE__
#define __UICALCULATEDISTANCE__

#include <wx/wx.h>
#include <wx/dialog.h>

#include <wx/listbox.h>
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/stattext.h>
#include <wx/image.h>
#include <wx/string.h>
#include <wx/choice.h>

#include <iostream>
#include <iomanip>

#include <cpw/common/Attribute.h>
#include <cpw/common/types.h>
#include <cpw/entity/Entity.h>
#include "../controllers/DistanceController.h"

#include <gui/MyTimeCtrl.h>

#undef UICalculateDistance_STYLE
#define UICalculateDistance_STYLE wxCAPTION | wxSYSTEM_MENU | wxDIALOG_NO_PARENT | wxCLOSE_BOX

namespace cpw { 
	
	namespace controllers { class DistanceController; }

	namespace gui
	{

		/** 
			\brief User interface for distance calculation
			\ingroup cpw
		*/
		class UICalculateDistance : public wxDialog
		{
			private:
				DECLARE_EVENT_TABLE();
				
			public:
				UICalculateDistance(controllers::DistanceController *evc,wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("ElementVectorial"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = UICalculateDistance_STYLE);
				
				virtual ~UICalculateDistance();

				void SetPositionX(const std::string &position_x) { Position_x_Edit->SetValue((wxString&)position_x); Refresh(); Update();}
				void SetPositionY(const std::string &position_y) { Position_y_Edit->SetValue((wxString&)position_y); Refresh(); Update();}
				void SetPositionZ(const std::string &position_z) { Position_z_Edit->SetValue((wxString&)position_z); Refresh(); Update();}

				void SetDistanceName(std::string &name)  { ElementVectorial_name_Ed->SetLabel((wxString&)name);  }
				void SetDistanceType(std::string &name)  { vname = name; SetTitle((wxString&)vname);}

				float GetDistance() {return distance;}
				void SetDistance(const float &d) {distance = d;}


			private:

				float distance, factor;
				
				wxStaticBox *Position_StaticBox;
				wxTextCtrl *Position_z_Edit;
				wxStaticText *Position_z_StaticText;
				wxTextCtrl *Position_y_Edit;
				wxStaticText *Position_y_StaticText;
				wxStaticText *Position_x_StaticText;
				wxTextCtrl *Position_x_Edit;
				wxStaticText *WxStaticText2;
				wxStaticText *WxStaticText1;
				wxButton *Cancel_button;
				wxButton *Reset_button;
				wxTextCtrl *ElementVectorial_name_Ed;
				wxStaticText *ElementVectorial_name_StaticText;
				wxChoice *formatChoice;
				
			private:
				enum
				{
					ID_RESET_BUTTON = 1096,
					ID_FORMAT_CHOICE = 1095,
					ID_EDIT_FLAMES = 1076,				
					ID_POSITION_STATICBOX = 1065,
					ID_POSITIONZ_EDIT = 1020,
					ID_POSITIONZ_STATICTEXT = 1019,
					ID_POSITIONY_EDIT = 1017,
					ID_POSITIONY_STATICTEXT = 1016,
					ID_Position_X_STATICTEXT = 1015,
					ID_POSITIONX_EDIT = 1014,
					ID_POSITION_STATICTEXT = 1013,
					ID_LOADElementVectorialTEXURE_BUTTON = 1007,
					ID_ElementVectorialTEXTURE_EDIT = 1006,
					ID_ElementVectorialTEXTURE_STATICTEXT = 1005,
					ID_CANCEL_BUTTON = 1003,
					ID_ELEMENTVECTORIALNAME_EDIT = 1002,
					ID_ELEMENTVECTORIALNAME_STATICTEXT = 1001,
					ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
				};
			
			private:
				
				wxWindow *paren;

				controllers::DistanceController *ElementVectorial_controller;
				std::string vname;

				void CreateGUIControls();
				void OnClose(wxCloseEvent& event);
				void OnButtonCancel(wxCommandEvent& WXUNUSED(event));
				void OnButtonReset(wxCommandEvent& WXUNUSED(event));

				void OnChoiceChanged(wxCommandEvent& WXUNUSED(event));

				void OnPaint(wxPaintEvent& event);
				void render(wxDC& dc);
				void OnEraseBackground(wxEraseEvent& event);  //don't remove this method


		};
	}
}

#endif



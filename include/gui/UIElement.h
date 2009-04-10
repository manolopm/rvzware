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
#ifndef _UIELEMENT_
#define _UIELEMENT_

#include <wx/wx.h>
#include <wx/image.h>
#include <wx/statline.h>
#include <wx/grid.h>
#include <wx/gbsizer.h>
#include <wx/checkbox.h>
#include <wx/spinbutt.h>

#include <vector>

#include <controllers/ElementController.h>
#include <cpw/common/Attribute.h>


namespace cpw 
{ 
	namespace controllers{ class ElementController;}

	namespace gui
	{

		/** 
			\brief Window panel for creating new Elements
			\ingroup cpw::controllers
		*/
		class UIElement : public wxDialog
		{

			public:

				/** 
					\brief constructor
					\param ec element controller
					\param parent parent window
					\param id ID for the new window
					\param title window title
					\param pos window position
					\param size window size
					\param style window style
				*/
				UIElement(cpw::controllers::ElementController *ec, wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);
				/** 
					\brief destructor
				*/
				virtual ~UIElement();
				
				/** 
					\brief Gets whether the link height checkbox is checked or not.
				*/
				bool GetLinkHeightCheck() { return over_terrain_check->IsChecked(); }
				
				/** 
					\brief Gets whether the over terrain checkbox is checked or not.
				*/
				bool GetOverTerrainCheck() { return link_height_check->IsChecked(); }

				void SetLinkHeightCheck(const bool &_check) { over_terrain_check->SetValue(_check);}
				void SetOverTerrainCheck(const bool &_check) { link_height_check->SetValue(_check);}

				/** 
					\brief Sets x,y,z positioning values in the window
				*/
				void SetPos(float x, float y, float z);
				/** 
					\brief Sets x,y,z rotation values in the window
				*/
				void SetOrientation(float x, float y, float z);
				/** 
					\brief Sets x,y,z scale values in the window
				*/
				void SetScale (float x, float y, float z);
				/** 
					\brief Sets the Text value in the window
				*/
				void SetText(const std::string &text) {tc_13->SetValue((const wxString &)text);}
				/** 
					\brief Sets the HTML value in the window
				*/
				void SetHtml(const std::string &html) {tc_14->SetValue((const wxString &)html);}
				/** 
					\brief Sets Description content in the window
				*/
				void SetDescription(const std::string &desc) {tc_3->SetValue((const wxString &)desc);}
				/** 
					\brief Sets the entity name value in the window
				*/
				void SetEntityName(const std::string &name);
				/** 
					\brief Gets the entity name value in the window
				*/
				std::string GetEntityName() {return std::string( tc_1->GetValue().ToAscii());};
				
				//
				
				/** 
					\brief Sets the Attributes inherited by the primitive template used
				*/
				void SetAttributes(const std::vector<cpw::Attribute> &attr){attributes = attr;}
				/** 
					\brief Sets the window interface as 'Edition', it disables non-editable controls
				*/
				void Modify(bool value){modify=value; 	tc_1->Enable(false); button_back->Enable(false);; primitiveChoice->Enable(false); primitiveBrowseButton->Enable(false); button_ok->Enable(true);}
				/** 
					\brief LockTestCoords
				*/
				void LockTestCoords(){initialized = !initialized; }
				/** 
					\brief Gets linked-height flag
				*/
				bool GetLinkedHeight() {return height_linked;}
				/** 
					\brief Gets over-terrain flag
				*/
				bool GetOverTerrain() {return over_terrained;}
				/** 
					\brief Sets the URL list for the Choice control
				*/
				void SetPrimitivesUrl(const std::map<std::string, cpw::TypeId> &vec_url,  const std::string &selected_primitive = "");
				/** 
					\brief returns whether the choice control is ready or not.
				*/
				bool ChoiceIsReady();
				/** 
					\brief sets whether the values of the coordinates controls should be checked or not
				*/
				void SetCheckCoords(const bool &_check_coords) { check_coords = _check_coords; }
				
				enum
				{
					ID_OVER_TERRAIN_CHK = 1013,
					ID_BROWSEBUTTON = 1012,
					ID_PRIMITIVE_CHOICE = 1011,
					ID_LINK_HEIGHT_CHK = 1010,
					ID_SPIN_OX = 1009,
					ID_SPIN_OY = 1008,
					ID_SPIN_OZ = 1007,
					ID_SPIN_SX = 1006,
					ID_SPIN_SY = 1005,
					ID_SPIN_SZ = 1004,
					ID_SPIN_UTMLON = 1003,
					ID_SPIN_UTMLAT = 1002,
					ID_SPIN_UTMHEI = 1001,
					ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
				};


			private:

				wxStaticText *label_1, *label_3, *label_4, *label_5, *label_6;
				wxStaticText *label_7, *label_8, *label_9, *label_10, *label_11, *label_12;
				wxStaticText *label_13, *label_14, *label_15, *label_16, *label_17, *label_blank;
				wxGridBagSizer *top_sizer;
				wxTextCtrl *tc_1, *tc_3, *tc_4, *tc_5, *tc_6, *tc_7, *tc_8, *tc_9, *tc_10, *tc_11, *tc_12, *tc_13, *tc_14;
				wxStaticLine *line_1;
				wxCheckBox *dynamic_check, *link_height_check, *over_terrain_check;
				wxButton *button_ok, *button_cancel, *button_back, *button_attributes, *button_html;

				wxSpinButton *Orientation_x_spin;
				wxSpinButton *Orientation_y_spin;
				wxSpinButton *Orientation_z_spin;
				wxSpinButton *Scale_x_spin;
				wxSpinButton *Scale_y_spin;
				wxSpinButton *Scale_z_spin;
				wxSpinButton *UTM_Lon_spin;
				wxSpinButton *UTM_Lat_spin;
				wxSpinButton *UTM_Hei_spin;
				wxArrayString arrayStringFor_formatChoice;
				wxChoice *primitiveChoice;
				wxButton *primitiveBrowseButton;

				wxFileDialog *open_primitive;
				
				cpw::controllers::ElementController *element_controller;
				
				bool initialized, height_linked, over_terrained, check_coords;
				bool modify;
				float orientationx, orientationy, orientationz;
				float utm1, utm2, utm3;
				float scalex, scaley, scalez;
				wxWindow *paren;

				std::vector<cpw::Attribute> attributes;
				std::map<std::string, cpw::TypeId> v_url;


				void do_layout();

				//events
				/** 
					\brief Checks and creates the element in the scene
				*/
				void ButtonOK();
				/** 
					\brief Handles OK Button event
				*/
				void OnButtonOK(wxCommandEvent& WXUNUSED(event));
				/** 
					\brief Handles Cancel Button event
				*/
				void OnButtonCancel(wxCommandEvent& WXUNUSED(event));
				/** 
					\brief Handles Attributes Button event
				*/
				void OnButtonAttributes(wxCommandEvent& WXUNUSED(event));
				/** 
					\brief Handles HTML Button event
				*/
				void OnButtonHtml(wxCommandEvent& WXUNUSED(event));
				/** 
					\brief Checks that every coord value is correct
				*/
				void CheckCoords(wxCommandEvent& WXUNUSED(event));
				/** 
					\brief Checks that the Element name is correct
				*/
				void CheckName(wxCommandEvent& WXUNUSED(event));
				/** 
					\brief Handles link-height toggle event
				*/
				void OnLinkHeightCheck(wxCommandEvent& WXUNUSED(event));
				/** 
					\brief Handles over-terrain toggle event
				*/
				void OnOverTerrainCheck(wxCommandEvent& WXUNUSED(event));
				/** 
					\brief Handles Move event
				*/
				void OnMove(wxMoveEvent& event);

				/** 
					\brief Handles spin event
				*/
				void OnOXSpinUp(wxSpinEvent& event);
				/** 
					\brief Handles spin event
				*/
				void OnOXSpinDown(wxSpinEvent& event);
				/** 
					\brief Handles spin event
				*/			
				void OnOYSpinUp(wxSpinEvent& event);
				/** 
					\brief Handles spin event
				*/
				void OnOYSpinDown(wxSpinEvent& event);
				/** 
					\brief Handles spin event
				*/
				void OnOZSpinUp(wxSpinEvent& event);
				/** 
					\brief Handles spin event
				*/
				void OnOZSpinDown(wxSpinEvent& event);
				/** 
					\brief Handles spin event
				*/
				void OnSXSpinUp(wxSpinEvent& event);
				/** 
					\brief Handles spin event
				*/			
				void OnSXSpinDown(wxSpinEvent& event);
				/** 
					\brief Handles spin event
				*/
				void OnSYSpinUp(wxSpinEvent& event);
				/** 
					\brief Handles spin event
				*/
				void OnSYSpinDown(wxSpinEvent& event);
				/** 
					\brief Handles spin event
				*/
				void OnSZSpinUp(wxSpinEvent& event);
				/** 
					\brief Handles spin event
				*/
				void OnSZSpinDown(wxSpinEvent& event);
				/** 
					\brief Handles spin event
				*/			
				void OnUTMLonSpinUp(wxSpinEvent& event);
				/** 
					\brief Handles spin event
				*/
				void OnUTMLonSpinDown(wxSpinEvent& event);
				/** 
					\brief Handles spin event
				*/
				void OnUTMLatSpinUp(wxSpinEvent& event);
				/** 
					\brief Handles spin event
				*/
				void OnUTMLatSpinDown(wxSpinEvent& event);
				/** 
					\brief Handles spin event
				*/
				void OnUTMHeiSpinUp(wxSpinEvent& event);
				/** 
					\brief Handles spin event
				*/			
				void OnUTMHeiSpinDown(wxSpinEvent& event);
				
				/** 
					\brief Handles change selection event for the primitive's choice list
				*/void OnChoiceChanged(wxCommandEvent& WXUNUSED(event));
				/** 
					\brief Browses for primitives in the system
				*/
				void OnBrowsePrimitivesButton(wxCommandEvent& event);

				/** 
					\brief Handles Paint event
				*/
				void OnPaint(wxPaintEvent& event);
				/** 
					\brief Renders the window
				*/
				void render(wxDC& dc);
				/**
					\brief Renders a labeled Box
					*/
				void DrawStaticBox(wxDC& dc,const wxString& label, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);

				DECLARE_EVENT_TABLE()


		};

	}
}

#endif 
